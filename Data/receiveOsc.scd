(
//creates a simple synth and attachs an OSC listener

SynthDef.new(\tone, {
  arg freq=400, amp=0.3, gate = 1, bend=0;
    var sig, env;
    sig = LFTri.ar(freq * bend.midiratio)!2;
    env = EnvGen.kr(Env.perc(0.05, 1, 1, -4), gate, doneAction:2);
    sig = sig * env * amp;
  Out.ar(0,sig);
}).add;

o = OSCresponderNode(nil, '/entry', {
  |t, r, msg|
  msg.postln ;
  Synth(\tone,[
      \freq, msg[1]*10,
    ]);
}).add;


)


//to remove listener
o.remove;