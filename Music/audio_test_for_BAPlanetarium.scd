// --------------------------------------------------
// Audio test for Buenos Aires Planetarium 5.1 setup
// --------------------------------------------------

// boot internal server
(
        s = Server.internal;
        Server.default = s;
        s.boot;
)


// Bus allocation
a = Bus.audio(s, 4); // allocate four channels for routing

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
    foa = FoaEncode.ar(src, encoderMatrix);
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
    sig = Pan2.ar(sig, 1.0);

    // encode
	encoderMatrix = FoaEncoderMatrix.newStereo;
    foa = FoaEncode.ar(sig, encoderMatrix);
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
    20.do({
        Synth(\foaEncodeMono, [\outBus, a, \theta, pi.rand2, \phi, 0]);
        0.1.wait;
    })
});
)

// test stereo encoding
(
Routine.run({
    20.do({
        Synth(\foaEncodeStereo, [\outBus, a,  \duration, 0.05]);
        0.1.wait;
    })
});
)

// ------ Free Resources -----

b.free; // free the decoder
a.free; // free the audio bus

