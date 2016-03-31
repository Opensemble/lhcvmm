(
//-----------------------------------
// FRAMEWORK: INIT STAGE
//-----------------------------------

~interpreter = (
	table: List.new,
	register:  {
		|self, descriptor, sound_unit_fase|
		self.table.add([descriptor,sound_unit_fase]);
	},
	pick:  {
		|self, event|
		var bestIndex;

		// pick best matching descriptor
		// wich will be by now the one
		// that matches more descriptive words
		bestIndex = self.table.maxIndex({ | item, i |
			item[0].count({ arg word; event.includes(word) });
		});

		self.table[bestIndex][1];
	},
	playEvent:  {
		|self, event|
		var sof = self.pick(event);
		// fill raw osc data
		sof.fill(event);
		// now play the event
		sof.playEvent();
	},
	clearTable: {
		| self|
		self.table.clear();
	}

);

//-----------------------------------
// BASE SOUND UNIT FASE TO EXTEND
//-----------------------------------
~sound_unit_fase = (
	fill:	{
		|self, data|
		data[1].postln;
		self.function = data[0];
		self.duration = data[1];
		self.unidirectional_motion = data[2];
		self.reciprocal_motion = data[3];
		self.cyclic_motion = data[4];
		self.characteristic_motion = data[5];
		self.texture_motion_relationship = data[6];
		self.texture_motion_continuity = data[7];
		self.texture_motion_periodic = data[8];
		self.texture_motion_acceleration = data[9];
		self.spectrum_type = data[10];
		self.spectrum_occupation = data[11];
		self.spectrum_plenitude = data[12];
		self.spectrum_concentration = data[13];
		self.spectrum_interstices = data[14];
		self.spectrum_crossover = data[15];
		self.spectrum_density = data[16];
	},
	register:  {
		|self, descriptor|
		~interpreter.register(descriptor,self);
	},
	playEvent:  {
		|self|
		"play function not implemented!".postln;
	}

);

)

(

~interpreter.clearTable;

SynthDef(\smooth, { |freq = 440, sustain = 1, amp = 0.5|
    var sig;
    sig = SinOsc.ar(freq, 0, amp) * EnvGen.kr(Env.linen(0.05, sustain, 0.1), doneAction: 2);
    Out.ar(0, sig ! 2)
}).add;


// this way you define and register a particular sound unit fase
(
	parent: ~sound_unit_fase,
	playEvent:  {
		|self|

		Synth(\smooth, [freq: 330, sustain: self.duration]);
	}

).register( [\harmonic, \inharmonic, \canopy, \centre] );

(
	parent: ~sound_unit_fase,
	playEvent:  {
		|self|
		Synth(\smooth, [freq: 440, sustain: self.duration]);
	}

).register( [\root] );



)

(
// test area: make the interpreter play some events
//~interpreter.playEvent(['root', 'harmonic']);
~interpreter.playEvent(['harmonic',0.8]);

)




