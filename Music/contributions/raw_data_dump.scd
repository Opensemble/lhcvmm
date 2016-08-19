(
// test function to dump raw data received

var raw_dump = 	{ arg dict;
		"-".dup(20).join.postln;
		dict.keysValuesDo({|key, val|
			if(val.isKindOf(Array),{
				"%: ".format(key).postln;
				val.do({ arg item;
					item.keysValuesDo({|k,v|
						" ----> %: %".format(k, v).postln;
					});
				});

			},{
				"%: %".format(key, val).postln;
			});
		});
		"-".dup(20).join.postln;
};

// uncomment to inspect the received data
//~opensemble.registerRaw(\raw_dump,raw_dump);

// annoyed by that Opensemble window?? uncomment the following line
//~opensemble.hideUI;



)
