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


//y=Pmono(\main,\dur,Pxrand((1/8!8)++(1/4!8)++Pseq([1/16,1/16],Prand([1,2],1)),inf),\trig,1,\p,{rrand(-1,1)},\i,Pwrand([0,1,3,4],[0.946,0.03,0.02,0.004],inf),\d,Pwrand([1,30],[0.98,0.02],inf),\f,Pwrand([35,40,6000,20000],[0.30,0.65,0.03,0.02],inf),\l,{rrand(0.1,0.5)},\n,{[1,2,3,30].wchoose([0.8,0.1,0.05,0.05])});
//z=Pmono(\hat,\dur,Pxrand((1/8!8)++(1/4!8)++Pseq([1/32,1/32,1/32,1/32],Prand([1,2],1)),inf),\trig,1,\p,{rrand(-1,1)},\a,0.1);
//w=Ppar([y,z],inf).play;


SynthDef(\bass,{arg out=0, freq=120,amp=1, noise=0, pan=0;
	var sig = (
		SinOsc.ar(freq*(1+noise))
		+ SinOsc.ar(if((noise>0),Pulse.ar(2,0.5),0)*freq/2)
	);


  Out.ar(out,Pan2.ar((sig*amp),pan));

}).add;



~opensemble.register(\attack_opaque, [\attack, \opaque],
	{ arg suf;

		Pmono(\main,
			\dur,Pxrand((1/8!8)++(1/4!8)++Pseq([1/16,1/16],Prand([1,2],1)),suf.duration.round*2),
			\trig,1,
			\p,{rrand(-1,1)},
			\i,Pwrand([0,1,3,4],[0.946,0.03,0.02,0.004],inf),
			\d,Pwrand([1,30],[0.98,0.02],inf),
			\f,Pwrand([35,40,6000,20000],[0.30,0.65,0.03,0.02],inf),
			\l,{rrand(0.1,0.5)},
			\n,{[1,2,3,30].wchoose([0.8,0.1,0.05,0.05])},
			\a, {rrand(0.1,0.5)},
		).play;
} );



~opensemble.register(\attack_translucent, [\attack, \translucent],
	{ arg suf;
		var y=Pmono(\main,\dur,Pxrand((1/4!8),suf.duration.round*2),\trig,1,\p,{rrand(-1,1)},\i,Pwrand([0,1,3,4],[0.946,0.03,0.02,0.004],inf),\d,Pwrand([1,30],[0.98,0.02],inf),\f,Pwrand([35,40,6000,20000],[0.30,0.65,0.03,0.02],inf),\l,{rrand(0.1,0.5)},\n,{[1,2,3,30].wchoose([0.8,0.1,0.05,0.05])});
		var z=Pmono(\hat,\dur,Pxrand((1/4!8),suf.duration.round*2),\trig,1,\p,{rrand(-1,1)},\a,0.1);
		Ppar([y,z],1).play;

});



~opensemble.register(\attack_packed, [\attack, \packed],
	{ arg suf;
		Pmono(\bass,
			\dur,Pxrand((1/4!8)++(1/8!4),suf.duration.round*2),
			\freq, 120,
			\noise, {rrand(-1.0,1.0)},
			\pan,{rrand(-1,1)},
		).play;

});

~opensemble.register(\attack_empty, [\attack, \empty],
	{ arg suf;

		Pmono(\hat,
			\dur,Pxrand((1/8!8)++(1/16!4),suf.duration.round*2),
			//\dur,Pxrand((1/6!8)++(1/16!4)++(1/3!2),suf.duration.round*2),
			\trig,1,
			\p,{rrand(-1,1)},
			\a,0.1

	).play;

});


//~opensemble.unregister(\attack_opaque);
//~opensemble.unregister(\attack_translucent);
//~opensemble.unregister(\attack_packed);
//~opensemble.unregister(\attack_empty);

)




(
y=Pmono(\main,\dur,Pxrand((1/8!8)++(1/4!8)++Pseq([1/16,1/16],Prand([1,2],1)),inf),\trig,1,\p,{rrand(-1,1)},\i,Pwrand([0,1,3,4],[0.946,0.03,0.02,0.004],inf),\d,Pwrand([1,30],[0.98,0.02],inf),\f,Pwrand([35,40,6000,20000],[0.30,0.65,0.03,0.02],inf),\l,{rrand(0.1,0.5)},\n,{[1,2,3,30].wchoose([0.8,0.1,0.05,0.05])});
z=Pmono(\hat,\dur,Pxrand((1/8!8)++(1/4!8)++Pseq([1/32,1/32,1/32,1/32],Prand([1,2],1)),inf),\trig,1,\p,{rrand(-1,1)},\a,0.1);
w=Ppar([y,z],inf).play;

)

TempoClock.tempo = 1