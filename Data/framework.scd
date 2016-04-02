(
// todo: make some ~opensemble methods private

// taken from: Faking Objected-Oriented Programing in Supercollider with Events
// http://www.tmroyal.com/supercollider-pseudoclasses-with-events.html


//-----------------------------------
// FRAMEWORK: INIT STAGE
//-----------------------------------

~opensemble = {
	var window;
	var oscIndicator, onsetIndicator, continuantIndicator,terminationIndicator;

	//-----------------------------------
	// SOUND UNIT FASE
	//-----------------------------------
	var sound_unit_fase = (
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
				self.playEvent(onset);
				{onsetIndicator.background_(Color.rand);}.defer;
				onset[1].wait;
				{onsetIndicator.background_(Color.grey);}.defer;
				self.playEvent(continuant);
				{continuantIndicator.background_(Color.rand);}.defer;
				continuant[1].wait;
				{continuantIndicator.background_(Color.grey);}.defer;
				self.playEvent(termination);
				{terminationIndicator.background_(Color.rand);}.defer;
				termination[1].wait;
				{terminationIndicator.background_(Color.grey);}.defer;
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
		var sof = ();
		sof.parent = sound_unit_fase;
		sof.playEvent = func;
		sof.register(key, keywords );
		//todo: validate words
		self.dict.add(key -> [keywords,sof]);
		format("Function % registered\n", key);

	},
	unregister:  {
		|self, key|
		//todo: validate words
		self.dict.removeAt(key);
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
		window = Window(name:"Opensemble" ,border:false,bounds:Rect(r.width-200-18, r.top+3, 200, 80));
		window.alwaysOnTop = true;
		window.background = Color.black.alpha = 0.6;
		t = StaticText.new(window,Rect(10,10,85,26)).string_("Opensemble:");
		t.stringColor = Color.white;
		oscIndicator  = StaticText.new(window,Rect(10,50,20,20));
		onsetIndicator  = StaticText.new(window,Rect(110,50,20,20));
		continuantIndicator  = StaticText.new(window,Rect(140,50,20,20));
		terminationIndicator  = StaticText.new(window,Rect(170,50,20,20));
		Button(window, Rect(100, 10, 90, 26))
		.states_([
            ["Listening", Color.white, Color.green.alpha = 0.6;],
            ["Idle...", Color.white, Color.red.alpha = 0.6;]
        ])
        .action_({ |b|
			if(b.value==0, {self.startListener}, {self.stopListener});
        });
	},
	init:{
		|self|
		self.dict = Dictionary.new;
		self.createGui;
		self.showGui;
		self.initListener;
		self; // method chaining

	}

).init; // call init upon creation
}.value;


//print this message instead of the ~opensemble object to the console
"Opensemble env loaded.".underlined;

)



