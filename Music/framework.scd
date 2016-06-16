(
// todo: make some ~opensemble methods private

// taken from: Faking Objected-Oriented Programing in Supercollider with Events
// http://www.tmroyal.com/supercollider-pseudoclasses-with-events.html


//-----------------------------------
// FRAMEWORK: INIT STAGE
//-----------------------------------
Server.default.boot;

~opensemble = {
	var window;
	var oscIndicator, onsetIndicator, continuantIndicator,terminationIndicator;
	var functionList;

	//-----------------------------------
	// SOUND UNIT PHASE
	//-----------------------------------
	var sound_unit_phase = (
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
		}

	);

	(
	initListener:  {
		|self|
		self.osc = OSCdef(\sound_unit, { |msg, time, addr, recvPort|
			var onset, continuant, termination;
			onset = msg[1..17] ;
			continuant = msg[18..34] ;
			termination = msg[35..51] ;
			fork{
				{oscIndicator.background_(Color.green);}.defer;
				0.2.wait;
				{oscIndicator.background_(Color.grey);}.defer;
			};

			fork{
				if(not(onset[0].asString.isEmpty),{
					self.playEvent(onset);
					{onsetIndicator.background_(Color.rand);}.defer;
					onset[1].wait;
					{onsetIndicator.background_(Color.grey);}.defer;
				});
				if(not(continuant[0].asString.isEmpty),{
					self.playEvent(continuant);
					{continuantIndicator.background_(Color.rand);}.defer;
					continuant[1].wait;
					{continuantIndicator.background_(Color.grey);}.defer;
				});
				if(not(termination[0].asString.isEmpty),{
					self.playEvent(termination);
					{terminationIndicator.background_(Color.rand);}.defer;
					termination[1].wait;
					{terminationIndicator.background_(Color.grey);}.defer;
				});
			};
		}, '/sound_unit').enable;
	},
	startListener:  {
		|self|
		self.osc.enable;
		"Opensemble is listening OSC messages.".postln;
	},
	stopListener:  {
		|self|
		self.osc.disable;
		"Opensemble stopped listening OSC messages.".postln;
	},
	register:  {
		|self, key, keywords, func|
		var sup = ();
		sup.parent = sound_unit_phase;
		sup.playEvent = func;
		sup.register(key, keywords );
		//todo: validate words
		self.dict.add(key -> [keywords,sup]);
		functionList.items = self.dict.keys.collect({|k| k.asString;}).asArray;
		format("Function % registered\n", key);

	},
	unregister:  {
		|self, key|
		self.dict.removeAt(key);
		functionList.items = self.dict.keys.collect({|k| k.asString;}).asArray;
		format("Function % unregistered\n", key);
	},
	pick:  {
		|self, event|
		var wordsCount,best, sof=nil;

		if(self.dict.isEmpty,{"No functions registered!".postln;},{
			// pick best matching descriptor
			// wich will be by now the one
			// that matches more descriptive words
			wordsCount = self.dict.collect({ | item, key |
				item[0].count({ arg word; event.includes(word) });
			});

			//cant find a better way...
			best = wordsCount.findKeyForValue(wordsCount.maxItem);
			if(wordsCount[best]>0,{sof = self.dict[best][1];});
			if(sof.isNil,{"No function matches ".post;event.postln;});
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
	showGui:{
		|self|
		window.visible=true;
	},
	hideGui:{
		|self|
		window.visible=false;
	},
	createGui:{
		|self|
		var r;
		//ensure opensemble window is not already opened
		Window.allWindows.do({|ww| if(ww.name=="Opensemble",ww.close);});
		r = Window.availableBounds;
		// set up a window on bottom right corner, right above server status
		//window = Window(name:"Opensemble" ,border:true,bounds:Rect(r.width-200-50, r.top+50, 200, 80));
		window = Window(name:"Opensemble", bounds:Rect(r.width-400-50, r.top+80, 400, 400));
		//window.alwaysOnTop = true;

		window.background = Color.black.alpha = 0.6;


		oscIndicator  = StaticText(window,20@20).background_(Color.grey);
		onsetIndicator  = StaticText(window,20@20).background_(Color.grey);
		continuantIndicator  = StaticText(window,20@20).background_(Color.grey);
		terminationIndicator  = StaticText(window,20@20).background_(Color.grey);


		functionList = ListView()
		//.items_([ "SinOsc", "Saw", "LFSaw", "WhiteNoise", "PinkNoise" ]);
		//.selection = 4;
			.selectionMode_(\none);
		//.colors = [Color.red, Color.blue];

		window.layout = VLayout(
				StaticText().string_("Registered Functions:").stringColor_(Color.white),
				functionList,
				HLayout(oscIndicator, onsetIndicator, continuantIndicator, terminationIndicator),
				Button()
				.states_([
					["Listening", Color.white, Color.green.alpha = 0.6;],
					["Idle...", Color.white, Color.red.alpha = 0.6;]
				])
				.action_({ |b|
					if(b.value==0, {self.startListener}, {self.stopListener});
				});

		);





	},
	init:{
		|self|
		self.dict = Dictionary.new;
		self.createGui;
		self.showGui;
		self.initListener;
		self; // needed for method chaining
	}

).init; // call init upon creation
}.value;

"~opensemble has been defined.".underlined.postln;
"Loading contributions.".underlined.postln;
(thisProcess.nowExecutingPath.dirname +/+ "/contributions/*.scd").loadPaths(warn:false);


)
