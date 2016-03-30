
//-----------------------------------------------------------------
// EJEMPLO
// Descripción:
//     Colaborador kurt kobain
//     Contribución: Espectro Armónico/Inarmónico, implementa distintos Onsets
//-----------------------------------------------------------------

// SynthDefs

(
SynthDef.new(\harmonic2, {
  arg freq=400, amp=0.3, gate = 1, bend=0, attack = 0.01, sustain = 0,release = 0.01, curve = 0,
	spectrum_plenitude = 1,
	spectrum_concentration = 1,
	spectrum_interstices = 1,
	spectrum_crossover = 1,
	spectrum_density = 1;

    var sig, env, freq_base, partials;

	sig = Mix.fill(8, { SinOsc.ar(freq + 500.0.rand, 0, 0.05) });

	//sig = LFTri.ar(spectrum_occupation);
	env = EnvGen.kr(Env.linen(attack, sustain, release, 0.7, curve),doneAction:2);
    sig = sig * env * amp;
  Out.ar(0,sig);
}).add;
)


// Funcion - Interface

(
~f1 = {|function, duration, spectrum_type, spectrum_occupation, spectrum_plenitude, spectrum_concentration, spectrum_interstices, spectrum_crossover, spectrum_density|

	var att, occ, den, func;

	//simple example

	func = switch (function)
	{\departure} {5}
	{\emergence} {4}
	{\anacrusis} {3}
	{\attack} {2}
	{\upbeat} {1}
	{\downbeat} {-1};

	occ = switch (spectrum_occupation)
	{\root} {100}
	{\centre} {500}
	{\canopy} {2000};

	den = switch (spectrum_density)
	{\filled} {1}
	{\packed} {0.8}
	{\opaque} {0.6}
	{\translucent} {0.4}
	{\transparent} {0.2}
	{\empty} {0.1};

x = Synth(\harmonic2, [\freq, occ, \attack, duration, \curve, func] );

};
)

//test
//f.value(\attack, 0.2, \canopy, 0.5, 0.6, 0.7, 0.2, \opaque);


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
)

// Funcion - Interface

(
~f2 = {|function, duration, spectrum_type, spectrum_occupation, spectrum_plenitude, spectrum_concentration, spectrum_interstices, spectrum_crossover, spectrum_density|

	var att, occ, den, func, spectrum;

	//simple example
==
	func = switch (function)
	{\departure} {5}
	{\emergence} {4}
	{\anacrusis} {3}
	{\attack} {2}
	{\upbeat} {1}
	{\downbeat} {-1};

	den = switch (spectrum_density)
	{\filled} {1}
	{\packed} {0.8}
	{\opaque} {0.6}
	{\translucent} {0.4}
	{\transparent} {0.2}
	{\empty} {0.1};


	// occ.postln;
	// duration.postln;
	// func.postln;

x = Synth(spectrum_type.asSymbol, [\freq, occ, \attack, duration, \curve, func] );

};
)

//test
//f.value(\attack, 0.2, \canopy, 0.5, 0.6, 0.7, 0.2, \opaque);
