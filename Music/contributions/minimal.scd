(
// taken from: http://www.kimri.org/blog/?tag=alva-noto
SynthDef(\main,{arg out=0, f=50,t_trig=0,p=0,i=0,d=10,l=0.1,a=0.5,n=1;
  var fmod=SinOsc.ar(f/60,0,f/60);
  var sig=[SinOsc.ar([f,f+600]+fmod,0,[d,0.005]).mean.tanh,HPF.ar(WhiteNoise.ar(1),8000),SinOsc.ar(30,0,1),VarSaw.ar(f/40,mul:d*10000)];
  var env=EnvGen.ar(Env([0.0,1.0,0.0],[0.0,l]),gate:t_trig);
  Out.ar(out,Pan2.ar(((Select.ar(i,sig)*env).fold(-1,1)*0.7+SinOsc.ar(40,0,0.3))*(1+HPF.ar(WhiteNoise.ar(0.02*n),8000)),p)*a);
}).add;
SynthDef(\hat,{arg out=0, f=50,t_trig=0,p=0,a=1;
  var sig=HPF.ar(WhiteNoise.ar(1),6000);
  var env=EnvGen.ar(Env([0.0,1.0,0.0],[0.0,0.01]),gate:t_trig);
  Out.ar(out,Pan2.ar((sig*env*a),p));
}).add;
SynthDef(\bass,{arg out=0, freq=120,amp=1, noise=0, pan=0;
	var sig = (
		SinOsc.ar(freq*(1+noise))
		+ SinOsc.ar(if((noise>0),Pulse.ar(2,0.5),0)*freq/2)
	);
	Out.ar(out,Pan2.ar((sig*amp),pan));
}).add;


//y=Pmono(\main,\dur,Pxrand((1/8!8)++(1/4!8)++Pseq([1/16,1/16],Prand([1,2],1)),inf),\trig,1,\p,{rrand(-1,1)},\i,Pwrand([0,1,3,4],[0.946,0.03,0.02,0.004],inf),\d,Pwrand([1,30],[0.98,0.02],inf),\f,Pwrand([35,40,6000,20000],[0.30,0.65,0.03,0.02],inf),\l,{rrand(0.1,0.5)},\n,{[1,2,3,30].wchoose([0.8,0.1,0.05,0.05])});
//z=Pmono(\hat,\dur,Pxrand((1/8!8)++(1/4!8)++Pseq([1/32,1/32,1/32,1/32],Prand([1,2],1)),inf),\trig,1,\p,{rrand(-1,1)},\a,0.1);
//w=Ppar([y,z],inf).play;



//only dumps raw data received... no
~opensemble.registerRaw(\raw_dump,
	{ arg dict;
		dict.at("pvxp_n").postln;
		dict.at("mu").postln;
		dict.at("met_phi").postln;
		dict.at("met_et").postln;
		dict.at("leps").do({ arg lep;
			lep.at("lep_pt").postln;
			lep.at("lep_eta").postln;
			lep.at("lep_phi").postln;
			lep.at("lep_E").postln;
		});
		dict.at("jets").do({ arg jet;
			jet.at("jet_phi").postln;
			jet.at("jet_eta").postln;
			jet.at("jet_pt").postln;
			jet.at("jet_m").postln;
			jet.at("jet_E").postln;
		});
});

//comment the following line to dump rad data to the post window
~opensemble.unregisterRaw(\raw_dump);




~opensemble.registerRaw(\raw_test,
	{ arg dict;

	if((dict.at("leps").size > 0),{
		dict.at("leps").do({ arg lep;

			Pmono(\hat,
				\dur,Pxrand((1/8!8)++(1/16!4),5),
				\p,{rrand(-1,1)},
				\a,lep.at("lep_E").asFloat.linlin(10000, 300000, 0.05, 0.6)
			).play;


			Pmono(\main,
				\dur,Pxrand((1/8!8)++(1/4!8)++Pseq([1/16,1/16],Prand([1,2],1)),3),
				\trig,1,
				\p,{rrand(-1,1)},
				\i,Pwrand([0,1,3,4],[0.946,0.03,0.02,0.004],inf),
				\d,Pwrand([1,30],[0.98,0.02],inf),
				\f,Pwrand([35,40,6000,20000],[0.30,0.65,0.03,0.02],inf),
				\l,{rrand(0.1,0.5)},
				\n,{[1,2,3,30].wchoose([0.8,0.1,0.05,0.05])},
				\a, {rrand(0.1,0.5)},
			).play;

		});

	});


	if((dict.at("jets").size > 0),{
		dict.at("jets").do({ arg jet;
			Pmono(\bass,
				\dur,Pxrand((1/4!8)++(1/8!4),4),
				\freq, 150,
				\noise, {[-1,-0.5,0,0.25,0.5,0.75,1].choose},
				\pan,{rrand(-1,1)},
				\amp,jet.at("jet_E").asFloat.linlin(50000, 300000, 0.1, 1)

			).play;
			//jet.keys.postln;
			//jet.at("jet_phi").postln;
			//jet.at("jet_eta").postln;
			//jet.at("jet_pt").postln;
			//jet.at("jet_m").postln;
		});

	});




});


)

(

~f = Bus.control;
~f.set(50);
~p = Bus.control;
~p.set(0);
~i = Bus.control;
~i.set(0);
~d = Bus.control;
~d.set(10);
~l = Bus.control;
~l.set(0.1);
~a = Bus.control;
~a.set(0.5);
~n = Bus.control;
~n.set(1);

SynthDef(\main0,{arg out=0, f=50,t_trig=0,p=0,i=0,d=10,l=0.1,a=0.5,n=1;
  var fmod=SinOsc.ar(f/60,0,f/60);
  var sig=[SinOsc.ar([f,f+600]+fmod,0,[d,0.005]).mean.tanh,HPF.ar(WhiteNoise.ar(1),8000),SinOsc.ar(30,0,1),VarSaw.ar(f/40,mul:d*10000)];
  var env=EnvGen.ar(Env([0.0,1.0,0.0],[0.0,l]),gate:t_trig);
  Out.ar(out,Pan2.ar(((Select.ar(i,sig)*env).fold(-1,1)*0.7+SinOsc.ar(40,0,0.3))*(1+HPF.ar(WhiteNoise.ar(0.02*n),8000)),p)*a);
}).add;



w = Window("main0", Rect(20, 400, 440, 380));
w.front;
w.view.decorator = FlowLayout(w.view.bounds);

EZSlider(w, 400@24, "~f", ControlSpec(30, 20000, \exp), {|ez| ~f.set(ez.value);},50);
EZSlider(w, 400@24, "~p", ControlSpec(-1, 1, \lin), {|ez| ~p.set(ez.value);},0);
EZSlider(w, 400@24, "~i", ControlSpec(0, 4, \lin).step_(1), {|ez| ~i.set(ez.value);},0);
EZSlider(w, 400@24, "~d", ControlSpec(1, 30, \lin), {|ez| ~d.set(ez.value);},10);
EZSlider(w, 400@24, "~l", ControlSpec(0.1, 0.5, \lin), {|ez| ~l.set(ez.value);},0.1);
EZSlider(w, 400@24, "~a", ControlSpec(0.01, 1, \lin), {|ez| ~a.set(ez.value);},0.5);
EZSlider(w, 400@24, "~n", ControlSpec(1, 30, \lin), {|ez| ~n.set(ez.value);},1);


Pmono(\main0,
	\dur,Pxrand((1/2!8),inf),
	\f, ~f.asMap,
	\p, ~p.asMap,
	\i, ~i.asMap,
	\d, ~d.asMap,
	\l, ~l.asMap,
	\a, ~a.asMap,
	\n, ~n.asMap,
).play;

)


(
c = Bus.control(s, 7);
Pbind(\instrument, \default, \freq, c[0].asMap,).play;
)

b.set(440);
b.set(660);

c.get(3).asMap
c.set(1,1).asMap
~f.asMap

