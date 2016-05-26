/*
This piece of code gathers events from a public read only spreadsheet found at

https://docs.google.com/spreadsheets/d/16FjhAmzO4CZyGDEJfdMguKVnG9FgMQe8M04X5MLigR4

and once donwloaded, streams osc messages to NetAddr.localAddr.

Its purpose is to play with opensemble framework.
To do so, go ahead and
  1) make yourself a copy of this doc
  2) change docId var to point to your doc copy
  3) start the framework ( hit cmd/ctrl + enter on framework.scd)
  4) play your music hitting cmd/ctrl + enter on this document

You can iterate by editing your doc copy and hitting cmd/ctrl + enter
on this document over and over.

*/
(
var docId = "16FjhAmzO4CZyGDEJfdMguKVnG9FgMQe8M04X5MLigR4";
var csv = "https://docs.google.com/spreadsheets/d/"++docId++"/export?format=csv";
var tempFile = "tmp.file";
var events;

Download(csv, tempFile , {
	"Reading googlespreadsheet CSV downloaded file.".postln;
	events = CSVFileReader.read(tempFile,true,startRow:2);
	"Removing empty entries.".postln;
	events.removeAllSuchThat({ arg c, i; c[0].isEmpty });
	"Converting numberlike data to float.".underlined.postln;
	events = events.collect({arg e;
		e.collect({arg d;
			if(d.notNil and: {d.notEmpty} and: {d.asString[0].isDecDigit},
				{d.asFloat;},
				{d.asString;}
			);
		});
	});

	{
		"Starting to stream csv events in 1 second.".underlined.postln;
		1.wait;

		events.do({arg e;
			NetAddr.localAddr.sendMsg("/sound_unit", *e[1..]);
			e[0].wait;
		});

		"Ended csv events.".underlined.postln;

	}.fork;


}, {\error.error}, {});
)

