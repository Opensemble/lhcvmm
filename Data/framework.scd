(
//-----------------------------------
// FRAMEWORK: INIT STAGE
//-----------------------------------

// taken from: Faking Objected-Oriented Programing in Supercollider with Events
// http://www.tmroyal.com/supercollider-pseudoclasses-with-events.html

~interpreter = (
	osc: OSCdef(\sound_unit, { |msg, time, addr, recvPort|
		var onset, continuant, termination;
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
		};
	}, '/sound_unit').disable, //starts disabled
	dict: Dictionary.new,
	startListener:  {
		|self|
		self.osc.enable;
	},
	stopListener:  {
		|self|
		self.osc.disable;
	},
	register:  {
		|self, key, keywords, sof|
		//todo: validate words
		self.dict.add(key -> [keywords,sof]);
	},
	unregister:  {
		|self, key|
		//todo: validate words
		self.dict.removeAt(key);
	},
	pick:  {
		|self, event|
		var wordsCount,best, sof=nil;

		if(self.dict.isEmpty,{"No function has been registered yet!".postln;},{
			// pick best matching descriptor
			// wich will be by now the one
			// that matches more descriptive words
			wordsCount = self.dict.collect({ | item, key |
				item[0].count({ arg word; event.includes(word) });
			});
			//cant find a better way...
			best = wordsCount.findKeyForValue(wordsCount.maxItem);
			if(wordsCount[best]>0,{sof = self.dict[best][1];});
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
			sof.playEvent;
		});
	},
	clearTable: {
		| self|
		self.dict.clear();
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
		|self, key, keywords|
		~interpreter.register(key, keywords,self);
	},
	playEvent:  {
		|self|
		"Play function not implemented!".postln;
	}

);

)