// --------------------------------------------------
// Audio test for Buenos Aires Planetarium 5.1 setup
// --------------------------------------------------

// boot internal server
(
        s = Server.internal;
        Server.default = s;
        s.boot;
)

// --------------------------------------------------
// Bus allocation
a = Bus.audio(s, 4); // allocate four channels for B-Format routing

// --------------------------------------------------
// Transform Function (example)
//(user must define as many of these as needed)
(
~transform = {arg sig;

	var transformer1, transformer2, transformern;
    var angle, azim;            // angle and azimuth control
    var freq;
    // ir...
    var gain = -18;
    var freqs = [50.0, 7000.0], gains = [-24, 0], rtimes = [0.1, 2.0];
    var frequencies, amplitudes, ringTimes;
    var numModes = 20;
    var density = 20;                  // grain/klank density
    var rttFreq = 10 * density;

	transformer1 = 'rtt';
	transformer2 = 'push';

	// display transformers
    "Ambisonic transforming via % transformer".format(transformer1).postln;
    "Ambisonic transforming via % transformer".format(transformer2).postln;

    // calculate klank args
    frequencies = Array.rand(numModes, freqs.at(0), freqs.at(1)).sort;
    amplitudes = Array.rand(numModes, gains.at(0), gains.at(1)).sort.reverse.dbamp;
    ringTimes = Array.rand(numModes, rtimes.at(0), rtimes.at(1)).sort.reverse;

    // angle ---> top         = push to plane wave
    //          bottom    = omni-directional
    angle = MouseY.kr(pi/2, 0);

    // azimuth -> hard left     = back
    //          centre     = centre
    //          hard right     = back
    azim = MouseX.kr(pi, -pi);

    // ------------------------------------------------------------
    // test sig
    //sig = Dust.ar(Array.fill(4, density / 4));

    // ------------------------------------------------------------
    // transform 1 (rtt)
    sig = FoaTransform.ar(
        sig,
        transformer1,
        LFSaw.ar(rttFreq, pi, add: pi),
        LFSaw.ar(rttFreq**(1/3), pi, add: pi),
        LFSaw.ar(rttFreq**(2/3), pi, add: pi)
    );

    // ------------------------------------------------------------
    // Klank
    sig = gain.dbamp * Klank.ar(
        `[ frequencies, amplitudes, ringTimes ],
        sig
    );

    // ------------------------------------------------------------
    // transform 2 (push)
    sig = FoaTransform.ar(sig, transformer2, angle, azim);
}
)
// --------------------------------------------------


// Encoder UGens
// Mono encoder (omni)
(
SynthDef(\foaEncodeMono, {arg outBus, duration = 0.05, theta, phi;
    var src, foa, encoderMatrix;
    src = PinkNoise.ar(-6.dbamp) *
        EnvGen.kr(
            Env([0, 1, 0], [0.5, 0.5], \sin),
            timeScale: duration,
            doneAction: 2);

    // Encode into our foa signal
	encoderMatrix = FoaEncoderMatrix.newOmni;
    "Ambisonic encoding via % encoder".format(encoderMatrix.kind).postln;
    foa = FoaEncode.ar(src, encoderMatrix);

	// Ambisonic Transforming
	foa = ~transform.value(foa);

	// Write to Decoder bus
    Out.ar(outBus, foa);
}).add;
)

// Stereo encoder (newStereo)
(
SynthDef(\foaEncodeStereo, {arg outBus, duration = 0.05, theta, phi;
    var sig, foa, encoderMatrix;

	// mono pink noise
    sig = PinkNoise.ar  *
        EnvGen.kr(
            Env([0, 1, 0], [0.5, 0.5], \sin),
            timeScale: duration,
            doneAction: 2);

	// pan (encode) to stereo
    sig = Pan2.ar(sig, 0); // center

    // encode
	encoderMatrix = FoaEncoderMatrix.newStereo;
    "Ambisonic encoding via % encoder".format(encoderMatrix.kind).postln;
	foa = FoaEncode.ar(sig, encoderMatrix);

	// Ambisonic Transforming
	foa = ~transform.value(foa);

	Out.ar(outBus, foa);
}).add;
)



// ----- DECODER KIND, FUNCTION AND UGEN ------
// choose a decoder (just one)
// stereophonic / binaural
~decoder = FoaDecoderMatrix.newStereo(131/2 * pi/180, 0.5) // Cardioids at 131 deg
~decoder = FoaDecoderKernel.newUHJ                         // UHJ (kernel)
~decoder = FoaDecoderKernel.newSpherical                   // synthetic binaural (kernel)
~decoder = FoaDecoderKernel.newCIPIC                       // KEMAR binaural (kernel)

// pantophonic (2D)
~decoder = FoaDecoderMatrix.newQuad(k: 'dual')             // psycho optimised quad
~decoder = FoaDecoderMatrix.newQuad(pi/6, 'dual')          // psycho optimised narrow quad
~decoder = FoaDecoderMatrix.new5_0                         // 5.0

// inspect you choice
~decoder.kind

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

// ----- Run the example ------

// start the decoder, reading bus 'a' at the \tail
b = Synth(\foaDecode, [\inBus, a], 1, \addToTail);

// test mono encoding
(
Routine.run({
    200.do({
        Synth(\foaEncodeMono, [\outBus, a, \theta, pi.rand2, \phi, 0]);
        0.1.wait;
    })
});
)

// test stereo encoding
(
Routine.run({
    200.do({
        Synth(\foaEncodeStereo, [\outBus, a,  \duration, 0.05]);
        0.1.wait;
    })
});
)

// ------ Free Resources -----

b.free; // free the decoder
a.free; // free the audio bus

