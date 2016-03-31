(
//clear registered functions
~interpreter.clearTable();
)

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
// this way you define and register a particular sound unit fase
(

(
	parent: ~sound_unit_fase,
	playEvent:  { arg self;

	var att, occ, den, func;

	//simple example

	func = switch (self.function)
	{\departure} {5}
	{\emergence} {4}
	{\anacrusis} {3}
	{\attack} {2}
	{\upbeat} {1}
	{\downbeat} {-1};

	occ = switch (self.spectrum_occupation)
	{\root} {100}
	{\centre} {500}
	{\canopy} {2000};

	den = switch (self.spectrum_density)
	{\filled} {1}
	{\packed} {0.8}
	{\opaque} {0.6}
	{\translucent} {0.4}
	{\transparent} {0.2}
	{\empty} {0.1};

x = Synth(\harmonic2, [\freq, occ, \attack, self.duration, \curve, func] );
}

).register( [\harmonic, \inharmonic, \root, \canopy, \centre] );


)





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




// this way you define and register a particular sound unit fase
(

(
	parent: ~sound_unit_fase,
	playEvent:  { arg self;
	var att, occ, den, func, spectrum;

	//simple example

	func = switch (self.function)
	{\departure} {5}
	{\emergence} {4}
	{\anacrusis} {3}
	{\attack} {2}
	{\upbeat} {1}
	{\downbeat} {-1};

	den = switch (self.spectrum_density)
	{\filled} {1}
	{\packed} {0.8}
	{\opaque} {0.6}
	{\translucent} {0.4}
	{\transparent} {0.2}
	{\empty} {0.1};


	x = Synth(self.spectrum_type.asSymbol, [\freq, occ, \attack, self.duration, \curve, func] );
}

).register( [\saturated, \granular] );


)



