//-----------------------------------------------------------------
// EJEMPLO2
// Descripción:
//     Colaborador john lennon
//     Contribución: Espectro Armónico/Inarmónico, implementa distintos Onsets
//-----------------------------------------------------------------

// SynthDefs

(
SynthDef.new(\granular, {
	arg freq=800, amp=0.3, gate = 1, bend=0, attack = 0.01, sustain=0,release = 0.01 ;
	var env,sig;

	sig = GrainFM.ar(2, Impulse.kr(10), 0.1, 440, 200, LFNoise1.kr.range(1, 10));
	env = EnvGen.kr(Env.linen(attack, sustain, release, 0.7),doneAction:2);
	sig = sig * env;
	Out.ar(0,sig);
}).add;

SynthDef.new(\saturated, {
	arg freq=200, amp=0.6, gate = 1, bend=0, attack = 0.01, sustain=0,release = 0.01 ;
	var env,sig;
	sig = PinkNoise.ar!2;
	env = EnvGen.kr(Env.linen(attack, sustain, release, 0.7),doneAction:2);
	sig = sig * env * amp;
	Out.ar(0,sig);
}).add;


// this way you define and register a particular sound unit fase

~opensemble.register(\saturated_granular,  [\saturated, \granular],
	{ arg suf;
	var att, occ, den, func, spectrum;

	//simple example

	func = switch (suf.function)
	{\departure} {5}
	{\emergence} {4}
	{\anacrusis} {3}
	{\attack} {2}
	{\upbeat} {1}
	{\downbeat} {-1};

	den = switch (suf.spectrum_density)
	{\filled} {1}
	{\packed} {0.8}
	{\opaque} {0.6}
	{\translucent} {0.4}
	{\transparent} {0.2}
	{\empty} {0.1};


	x = Synth(suf.spectrum_type.asSymbol, [\freq, occ, \attack, suf.duration, \curve, func] );
});


)