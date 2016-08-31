(


o = OSCdef(\raw_data, { |msg, time, addr, recvPort|
	var data = msg[1].asString.parseYAML; //parsed data
	data.postln;
}, '/raw_data').enable;

)






(

NetAddr("127.0.0.1", 50000).sendMsg("/subscribe");

)