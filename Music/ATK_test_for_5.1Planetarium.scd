Quarks.gui

// Allocate four channels for B-Format routing
~decoderBus = Bus.audio(s, 4);
~transformBus = Bus.audio(s, 4);

// ----- DECODER KIND, FUNCTION AND UGEN ------
// choose a decoder (just one)

// stereophonic / binaural
~decoder = FoaDecoderMatrix.newStereo(131/2 * pi/180, 0.5); // Cardioids at 131 deg
~decoder = FoaDecoderKernel.newUHJ;                         // UHJ (kernel)
~decoder = FoaDecoderKernel.newSpherical;                   // synthetic binaural (kernel)
//~decoder = FoaDecoderKernel.newCIPIC                       // KEMAR binaural (kernel)

// pantophonic (2D)
~decoder = FoaDecoderMatrix.newQuad(k: 'dual');             // psycho optimised quad
~decoder = FoaDecoderMatrix.newQuad(pi/6, 'dual');          // psycho optimised narrow quad
~decoder = FoaDecoderMatrix.new5_0;                         // 5.0

// inspect you choice
~decoder.kind;

// ------------------------------------------------------------
// define ~renderDecode function for generic audio output code
(
~renderDecode = { arg in, decoder;
    var kind;
    var fl, bl, br, fr;
    var fc, lo;

    kind = decoder.kind;
    "Ambisonic decoding via % decoder".format(kind).postln;

    case
        { decoder.numChannels == 2 }
            {
                // decode to stereo (or binaural)
                FoaDecode.ar(in, decoder)
            }
        { kind == 'quad' }
            {
                // decode (to quad)
                #fl, bl, br, fr = FoaDecode.ar(in, decoder);

                // reorder output to match speaker arrangement
                [fl, fr, bl, br]
            }
        { kind == '5.0' }
            {
                // decode (to 5.0)
                #fc, fl, bl, br, fr = FoaDecode.ar(in, decoder);
                lo = Silent.ar;

                // reorder output to match speaker arrangement
                [fl, fr, fc, lo, bl, br]
            }
}
)

// Decoder UGen
(
SynthDef(\foaDecode, {arg inBus;
    var foa, decoderMatrix, out;
    foa = In.ar(inBus, 4);
	out = ~renderDecode.value(foa, ~decoder);
    Out.ar(0, out);
}).add;
)

// Encoder UGens
// Mono encoder (omni)
(
SynthDef(\foaEncodeMono2, {arg outBus, duration = 0.05, an = 0, phi;
    var src, foa, encoderMatrix;
	src = Dust.ar(20,0.5);

    // Encode into our foa signal
	encoderMatrix = FoaEncoderMatrix.newDirection(pi);
    "Ambisonic encoding via % encoder".format(encoderMatrix.kind).postln;
    foa = FoaEncode.ar(src, encoderMatrix);

	// Ambisonic Transforming
//	foa = ~transform.value(foa);

	// Write to output bus
    Out.ar(outBus, foa);
}).add;
)

// Stereo encoder (newStereo)
(
SynthDef(\foaEncodeStereo, {arg outBus;
    var sig, foa, encoderMatrix;

	// Based on sc140 example
	sig = AllpassC.ar(SinOsc.ar(55).tanh, 0.4, TExpRand.ar(2e-4, 0.4, Impulse.ar(8)).round([2e-3,4e-3]), 2);

	sig = sig * 0.3;
    // encode
	encoderMatrix = FoaEncoderMatrix.newStereo;
    "Ambisonic encoding via % encoder".format(encoderMatrix.kind).postln;
	foa = FoaEncode.ar(sig, encoderMatrix);

	// Ambisonic Transforming
	//foa = ~transform.value(foa);

	Out.ar(outBus, foa);
}).add;
)

(
SynthDef(\foaEncodeStereo2, {arg outBus;
    var sig, foa, encoderMatrix;

	// Based on Rumush example
    var buf1 = Buffer.alloc(s, 4e5, 2);
	var buf2 = Buffer.alloc(s, 2e5, 2);

	var tim = 3;
	var freq1 = [1, 5, 9, 20, 50].choose * [0.5, 2, 6, 4].choose;
	var freq2 = [1, 5, 9, 20, 50].choose * [0.5, 2, 6, 4].choose;
	// ("loop time" + tim).postln;
	// ("freq1" + freq1).postln;
	// ("freq2" + freq2).postln;

		var mainEnv = EnvGen.ar(Env.linen(tim*0.5, tim*8, tim), doneAction:2);
		var timA = Array.geom(3, tim, 0.5);
		var trig = Impulse.ar(timA);
			var pos = Demand.ar(trig[1], 0, Dxrand([0, 4e5*0.25, 4e5*0.4, 4e5*0.75], inf));
		var pos2 = Demand.ar(trig[2], 0, Dxrand([0, 2e5*0.25, 2e5*0.4, 2e5*0.75], inf));
		var src1 = BufRd.ar(2, buf1, Phasor.ar(trig[0], 1, 0, 4e5, pos))*0.5;
		var src2 = PinkNoise.ar(Decay.ar(trig[0]!2, 0.1, 250)).fold(-0.5, 0.5);
		var src3 = BufRd.ar(2, buf2, Phasor.ar(trig[0], 1, 0, 2e5, pos2))*0.75;
		var src4 = Mix.fill(8, {
			SinOsc.ar([freq1*100.rand, freq2*100.rand], 0, LFSaw.ar([Rand(0.25, 1), Rand(0.25, 1)])+1*0.5)*0.125
		});
		BufWr.ar(Blip.ar([freq1, freq2], LFNoise1.ar(freq1)+1*50)+src1, buf1, Phasor.ar(0, 1, 0, 2e5)+(LFNoise1.ar(freq1*0.1)+1*2e5));
		BufWr.ar(Saw.ar([freq1, freq2],0.25)+src3, buf2, LFNoise1.ar(freq2*0.1)+1*2e4);

	    src2 = src2*0.125;

		sig = (((src1+src2+src3+src4)*0.25).tanh)*mainEnv;

	// encode
	encoderMatrix = FoaEncoderMatrix.newStereo;
    "Ambisonic encoding via % encoder".format(encoderMatrix.kind).postln;
	foa = FoaEncode.ar(sig, encoderMatrix);

	// Ambisonic Transforming
	//foa = ~transform.value(foa);

	Out.ar(outBus, foa);
}).add;
)

// Transform UGen // just rotate for 15 secs
(
SynthDef(\foaTransform, {arg inBus;
    var foa, decoderMatrix, out;
    foa = In.ar(inBus, 4);
	out = FoaTransform.ar(foa, 'rotate', XLine.kr(0.1, 8*pi, 10)); //15 sec //may be a gated Env ?
    Out.ar(~decoderBus, out);
}).add;
)

// ----- Run the example ------

// start transform and decoder UGens

(
// start the transform, reading bus 'a' at the \tail
t = Synth(\foaTransform, [\inBus, ~transformBus], 1, \addToTail); //add after source and before decoder


// start the decoder, reading bus 'a' at the \tail
Synth(\foaDecode, [\inBus, ~decoderBus],  target: t, addAction: 'addAfter');
)


// verify execution order; foaDecode node should go after foaTransform node
s.plotTree

// -- This UGens send B-Format to transform UGen via ~transformBus

// test mono encoding

r = Synth(\foaEncodeMono2, [\outBus, ~transformBus, \dir, pi.rand2, \phi, 0]);
r.free

// f = Synth(\foaEncodeMono3, [\outBus, ~transformBus, \theta, pi.rand2, \phi, 0]);
// f.free

// test stereo encoding
v = Synth(\foaEncodeStereo, [\outBus, ~transformBus]);
v.free

q = Synth(\foaEncodeStereo2, [\outBus, ~transformBus]);
q.free

// This UGen implements its own transform function ---------------------------

// Start the experiment -- 15 secs
(
{
    var sig;                            // audio signal
    var angle;                          // angle control
    var freq;
    var density = 20;                   // grain density
	var encoder;
	// define encoder matricies, for each cardinal point

	// encoder = [
	// 	FoaEncoderMatrix.newDirection,
	// 	FoaEncoderMatrix.newDirection(pi/2),
	// 	FoaEncoderMatrix.newDirection(pi),
	// 	FoaEncoderMatrix.newDirection(pi.neg/2)
	// ];

// or this one for 1 direction only
//
	encoder = [
		FoaEncoderMatrix.newDirection,
		FoaEncoderMatrix.newDirection,
		FoaEncoderMatrix.newDirection,
		FoaEncoderMatrix.newDirection
	];

    // display encoder and decoder
    "Ambisonic encoding via % encoder".format(encoder.at(0).kind).postln;
    "Ambisonic decoding via % decoder".format(~decoder.kind).postln;

	// SOURCE
    // frequencies
    freq = 220 * [ 6.1234, 7.678, 4.4, 5.3];

//	sig = Dust.ar(Array.fill(4, density / 4));
	sig = Pulse.ar([2, 4, 3, 6] * 1.5, 0.01, 0.05);

    // ------------------------------------------------------------
    // comb filter
    sig = BPF.ar(sig, freq, 0.5,  mul: 18.dbamp, );
    sig = CombL.ar(sig, freq.reciprocal, freq.reciprocal*2, mul: 9.neg.dbamp);
	//sig = MoogFF.ar(sig, LFNoise1.kr([1,2,3,4]).range(1300, 1700), MouseX.kr(0, 4));
	sig = sig * SinOsc.ar(XLine.kr(2000, 3000, 15));
	sig = sig * EnvGen.ar(Env.linen(0.1, 5, 0.7, 0.7), doneAction:2);

    // ------------------------------------------------------------
    // encode
    sig = Mix.fill(sig.numChannels, { arg i;  FoaEncode.ar(sig.at(i), encoder.at(i)) });
    // Look at Multichannel Expansion help

	// What is the best way to handle acceleration ?
	sig = FoaTransform.ar(sig, 'rotate', EnvGen.kr(Env([0.1, 20*pi], [15], \lin), doneAction:2)); //push, mirror, focus, rotate
    // ------------------------------------------------------------

    // decode via ~decoderBus
	Out.ar(~decoderBus, sig);
}.play;
)