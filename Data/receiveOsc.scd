(
//creates a simple synth and attachs an OSC listener
o.remove;

SynthDef.new(\harmonic, {
  arg freq=400, amp=0.3, gate = 1, bend=0, attack = 0.01, sustain=0,release = 0.01 ;
    var sig, env;
    sig = LFTri.ar(freq * bend.midiratio)!2;
	env = EnvGen.kr(Env.linen(attack, sustain, release, 0.7),doneAction:2);
    sig = sig * env * amp;
  Out.ar(0,sig);
}).add;


SynthDef.new(\inharmonic, {
  arg freq=600, amp=0.3, gate = 1, bend=0, attack = 0.01, sustain=0,release = 0.01 ;
    var sig, env;
    sig = GrainSin.ar(2, Impulse.kr(10), 0.1, 440);
    env = EnvGen.kr(Env.linen(attack, sustain, release, 0.7),doneAction:2);
    sig = sig * env * amp;
  Out.ar(0,sig);
}).add;

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

o = OSCresponderNode(nil, '/entry', {
  |t, r, msg|
	var onset, continuant, termination;
	onset = msg[1..3] ;
	continuant = msg[4..6] ;
	termination = msg[7..9] ;

	fork{
		Synth(onset[2], [\attack, onset[1]]);
		onset[1].wait;
		Synth(continuant[2], [\sustain, continuant[1]]);
		continuant[1].wait;
		Synth(termination[2],[\release, termination[1]]);
	}


}).add;


)

s.freeAll;
//to remove listener
o.remove;





