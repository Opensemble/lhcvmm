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
	var oscIndicator;
	var rawFunctionIndicator;
	var rawFunc;

	(
	initListener:  {
		|self|
		self.osc = OSCdef(\raw_data, { |msg, time, addr, recvPort|
				var data = msg[1].asString.parseYAML; //parsed data
				var d = Dictionary.new;  //new dictionary

				// iterate through data to do proper casting
				data.keysValuesDo({|key, val|
					if(val.isKindOf(Array),{
						var items = Array.new;
						val.do({ arg item;
							// we only have one nested level
							var d2 = Dictionary.new;
							item.keysValuesDo({|k2,v2|
								//cast keys as symbols and values as floats
								d2.add(k2.asSymbol -> v2.asFloat);
							});
							items.add(d2);
						});
						d.add(key.asSymbol -> items);
					},{
						//cast keys as symbols and values as floats
						d.add(key.asSymbol -> val.asFloat);
					});
				});

				rawFunc.value(d);
				fork{
					{oscIndicator.background_(Color.green);}.defer;
					0.2.wait;
					{oscIndicator.background_(Color.grey);}.defer;
				};
		}, '/raw_data').enable;

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
	registerRaw:  {
		|self, key, func|
		rawFunc = func;
		rawFunctionIndicator.string_("Registered: " + key);
		format("Raw function % registered\n", key);
	},
	unregisterRaw:  {
		|self, key|
		rawFunc = nil;
		rawFunctionIndicator.string_("No Function reg.");
		format("Raw function % unregistered\n", key);
	},
	showUI:{
		|self|
		window.visible=true;
	},
	hideUI:{
		|self|
		window.visible=false;
	},
	createUI:{
		|self|
		var r;
		//ensure opensemble window is not already opened
		Window.allWindows.do({|ww| if(ww.name=="Opensemble",ww.close);});
		r = Window.availableBounds;
		// set up a window on bottom right corner, right above server status
		window = Window(name:"Opensemble", bounds:Rect(r.width-400-50, r.top+80, 400, 100));
		window.alwaysOnTop = true;

		window.background = Color.black.alpha = 0.6;


		oscIndicator  = StaticText(window).background_(Color.grey);
		rawFunctionIndicator  = StaticText(window).stringColor_(Color.white).string_("No Function reg.");

		window.layout = VLayout(
				HLayout(oscIndicator, rawFunctionIndicator),
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
		self.createUI;
		self.showUI;
		self.initListener;
		self; // needed for method chaining
	}

).init; // call init upon creation
}.value;

"~opensemble has been defined.".underlined.postln;
"Loading contributions.".underlined.postln;
(thisProcess.nowExecutingPath.dirname +/+ "/contributions/*.scd").loadPaths(warn:false);


)
