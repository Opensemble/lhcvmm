
// Experiment: Encode signals at cardinal points and rotate the soundfield for 15 seconds
// --------------------------------------------------------------------------------------


// DECODER
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


// ENCODER --------


// define encoder matricies, for each cardinal point
(
~encoder = [
    FoaEncoderMatrix.newDirection,
    FoaEncoderMatrix.newDirection(pi/2),
    FoaEncoderMatrix.newDirection(pi),
    FoaEncoderMatrix.newDirection(pi.neg/2)
]
)

// or this one for 1 direction only
(
~encoder = [
    FoaEncoderMatrix.newDirection,
    FoaEncoderMatrix.newDirection,
    FoaEncoderMatrix.newDirection,
    FoaEncoderMatrix.newDirection
]
)

// how can we encode this example from sc140?
b = {f=LocalIn.ar(2).tanh;k=Latch.kr(f[0].abs,Impulse.kr(1/4));LocalOut.ar(f+CombC.ar(Blip.ar([4,6],100*k+50,0.9),1,k*0.3,50*f));f}//44.1kHz


// Start the experiment -- 15 secs
(
{
    var sig;                            // audio signal
    var angle;                          // angle control
    var freq;
    var density = 20;                   // grain density


    // display encoder and decoder
    "Ambisonic encoding via % encoder".format(~encoder.at(0).kind).postln;
    "Ambisonic decoding via % decoder".format(~decoder.kind).postln;

	// SOURCE
    // frequencies
    freq = 220 * [ 4, 5, 7, 6 ] / 1;

	sig = Dust.ar(Array.fill(4, density / 4));
	//sig = Pulse.ar([6, 4, 3, 5] * 2, 0.05, 0.08);

    // ------------------------------------------------------------
    // comb filter
    sig = BPF.ar(sig, freq, mul: 18.dbamp);
    sig = CombL.ar(sig, freq.reciprocal, freq.reciprocal, mul: 9.neg.dbamp);
	sig = sig * EnvGen.ar(Env.linen(0.1, 14, 0.7, 0.7), doneAction:2);


    // ------------------------------------------------------------
    // encode
    sig = Mix.fill(sig.numChannels, { arg i;  FoaEncode.ar(sig.at(i), ~encoder.at(i)) });
    // Look at Multichannel Expansion help

    // ------------------------------------------------------------
    // transform -- FoaTransform.ar(sig, ~transformer, angle);

	// What is the best way to handle angular acceleration ?
	sig = FoaTransform.ar(sig, 'rotate', EnvGen.kr(Env([0.1, 8*pi], [15], \lin)));
    // ------------------------------------------------------------

    // decode (via ~renderDecode)
    ~renderDecode.value(sig, ~decoder)
}.play;
)
