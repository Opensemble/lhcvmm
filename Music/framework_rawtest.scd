(
t = Task({
	"Starting to stream random events in 1 second.".underlined.postln;
	1.wait;
    loop {
		[[1.24,2,3],[4,"qweq"]].do({ |d|
 			NetAddr.localAddr.sendMsg("/raw_data", *d);
			1.wait;
        });
    }
}).play;
)