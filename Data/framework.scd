(
//-----------------------------------
// FRAMEWORK: INIT STAGE
//-----------------------------------

// taken from: Faking Objected-Oriented Programing in Supercollider with Events
// http://www.tmroyal.com/supercollider-pseudoclasses-with-events.html

~interpreter = (
	table: List.new,
	register:  {
		|self, descriptor, sound_unit_fase|
		//todo: validate words
		self.table.add([descriptor,sound_unit_fase]);
	},
	pick:  {
		|self, event|
		var wordsCount,bestIndex, sof=nil;

		if(self.table.isEmpty,{"No function has been registered yet!".postln;},{
			// pick best matching descriptor
			// wich will be by now the one
			// that matches more descriptive words
			wordsCount = self.table.collect({ | item, i |
				item[0].count({ arg word; event.includes(word) });
			});
			bestIndex = wordsCount.maxIndex;
			if(wordsCount[bestIndex]>0,{sof = self.table[bestIndex][1];});
			if(sof.isNil,{"No function matched the requirements!".postln;});
		});

		sof; //can return nil
	},
	playEvent:  {
		|self, event|
		var sof = self.pick(event);
		if(sof.notNil,{
			// fill raw osc data
			sof.fill(event);
			// now play the event
			sof.playEvent( sof.function,
				sof.duration,
				sof.unidirectional_motion,
				sof.reciprocal_motion,
				sof.cyclic_motion,
				sof.characteristic_motion,
				sof.texture_motion_relationship,
				sof.texture_motion_continuity,
				sof.texture_motion_periodic,
				sof.texture_motion_acceleration,
				sof.spectrum_type,
				sof.spectrum_occupation,
				sof.spectrum_plenitude,
				sof.spectrum_concentration,
				sof.spectrum_interstices,
				sof.spectrum_crossover,
				sof.spectrum_density);
		});
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
	isOnset:	{
		|self|
		['departure','emergence','anacrusis','attack','upbeat','downbeat'].includes(self.function)
	},
	isContinuant:	{
		|self|
		['passage','transition','prolongation','maintenance','statement'].includes(self.function)
	},
	isTermination:	{
		|self|
		['arrival','disappearence','closure','release','resolution','plane'].includes(self.function)
	},
	// fills object with incoming OSC data
	fill:	{
		|self, data|
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
		"Play function not implemented!".postln;
	}

);


// remove listener, just in case
o.remove;

o = OSCresponderNode(nil, '/sound_unit', {
	|x, r, msg|
	var onset, continuant, termination, event;
	onset = msg[1..17] ;
	continuant = msg[18..34] ;
	termination = msg[35..51] ;
	fork{
		~interpreter.playEvent(onset);
  		onset[1].wait;
		~interpreter.playEvent(continuant);
  		continuant[1].wait;
		~interpreter.playEvent(termination);
		//termination[1].wait;
  	}


}).add;

)


(
// remove listener
o.remove;
)