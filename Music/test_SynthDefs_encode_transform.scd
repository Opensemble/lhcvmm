


// ------------------------------------------------------------
// omni encoder
// mono pink noise source

(
// define encoder matrix
~encoder = FoaEncoderMatrix.newOmni;

// inspect


d = {arg sig;                            // audio signal
    var angle, azim;                    // angle and azimuth control

    // angle ---> top           = push to plane wave
    //            bottom        = omni-directional
    angle = MouseY.kr(0, pi/2);

    // azimuth -> hard left     = back
    //            centre        = centre
    //            hard right    = back
    azim = MouseX.kr(-pi, pi);

    // ------------------------------------------------------------
    // transform
    sig = FoaTransform.ar(sig, 'push', angle, azim);

};

SynthDef(\foaEncodeMono, {arg outBus, duration = 3, theta, phi;
    var src, foa, encoderMatrix;
    src = PinkNoise.ar(-6.dbamp) *
        EnvGen.kr(
            Env([0, 1, 0], [0.2, 5], \sin),
            timeScale: duration,
            doneAction: 2);

    // Encode into our foa signal
	encoderMatrix = FoaEncoderMatrix.newOmni;
    "Ambisonic encoding via % encoder".format(encoderMatrix.kind).postln;
    foa = FoaEncode.ar(src, encoderMatrix);

	// Ambisonic Transforming
	foa = d.value(foa);

	// Write to Decoder bus
    Out.ar(outBus, foa);
}).play;
)



// Experiment: Encode 4 signals in cardinal directions and rotate
// ------------------------------------------------------------
// x-axis transformers
// comb filtered dust noise source, at cardinal points


// define encoder matricies, for each cardinal point
(
~encoder = [
    FoaEncoderMatrix.newDirection,
    FoaEncoderMatrix.newDirection(pi/2),
    FoaEncoderMatrix.newDirection(pi),
    FoaEncoderMatrix.newDirection(pi.neg/2)
]
)

(
~encoder = [
    FoaEncoderMatrix.newDirection,
    FoaEncoderMatrix.newDirection,
    FoaEncoderMatrix.newDirection,
    FoaEncoderMatrix.newDirection
]
)

// choose transformer
~transformer = 'focusX'
~transformer = 'zoomX'
~transformer = 'pushX'

(
{
    var sig;                            // audio signal
    var angle;                          // angle control
    var freq;
    var density = 10;                   // grain density


    // display encoder and decoder
    "Ambisonic encoding via % encoder".format(~encoder.at(0).kind).postln;
    "Ambisonic transforming via % transformer".format(~transformer).postln;
    "Ambisonic decoding via % decoder".format(~decoder.kind).postln;

    // frequencies
    freq = 220 * [ 4, 5, 7, 6 ] / 1;

    // angle ---> top           = pi/2
    //            bottom        = -pi/2
    angle = MouseY.kr(pi/2, pi.neg/2);

    // ------------------------------------------------------------
    // test sig
	    // sig = [Dust, Dust, Dust, Dust]
	//sig = Dust.ar(Array.fill(4, density / 4));

	sig = Pulse.ar([6, 4, 3, 5] * 2, 0.05, 0.08);

    // ------------------------------------------------------------
    // comb filter
    sig = BPF.ar(sig, freq, mul: 18.dbamp);
    sig = CombL.ar(sig, freq.reciprocal, freq.reciprocal, mul: 9.neg.dbamp);
	sig = sig * EnvGen.ar(Env.linen(0.1, 14, 0.7, 0.7), doneAction:2);


	//sig = FoaEncode.ar(sig, ~encoder.at(0));
	//sig = FoaEncode.ar(Synth("wow", [\freq, rrand(40, 70).midicps, \amp, rrand(0.1, 0.5)]), ~encoder.at(0));

		// LA DUDA ES: COMO REEMPLAZAR UNA UGEN POR UN SYNTH Y QUE FUNCIONE CON ATK
	// Y LUEGO PREGUNTARME, COMO USAR PATTERNS CON ATK ?
	//sig = SinOsc.ar([440, 550, 660, 770],0 ,0.3);
	//sig = Synth(\glitchbuff);
	//sig = Array.fill(4, {Synth("wow", [\freq, rrand(40, 70).midicps, \amp, rrand(0.1, 0.5)])});


    // ------------------------------------------------------------
    // encode
    sig = Mix.fill(sig.numChannels, { arg i;  FoaEncode.ar(sig.at(i), ~encoder.at(i)) });
    // Look at Multichannel Expansion help

    // ------------------------------------------------------------
    // transform
    //sig = FoaTransform.ar(sig, ~transformer, angle);

	sig = FoaTransform.ar(sig, 'rotate', XLine.kr(1, 12 * pi, 15));
    // ------------------------------------------------------------
    // decode (via ~renderDecode)
    ~renderDecode.value(sig, ~decoder)

}.scope;
)
// ------------------------------------------------------------

s.freeAll
