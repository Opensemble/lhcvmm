# lhcvmm
Large Hadron Collider Visual Music Machine - Data layer

This python program drives ATLAS collisions data through OSC to both openFrameworks and SuperCollider.
It could also be posible to have other "interpreters" in the future listening to the same messages (like some arduino based machine)

## Directory Structure

* data_files: Here is where you download ROOT data files. They are excluded from repository to keep it slim.


## Getting started

To get this working you should have ROOT framework with pyROOT binding enabled on your machine.
Although if you don't, the program will simulate some random data to let you have a quick and dirty test.


### Installing ROOT

You can, of course, build it yourself. But CERN provides binaries here: https://root.cern.ch/downloading-root
Once installed, source it in your terminal to have ROOT executables available on your PATH.

```bash

cd /path/to/ROOT/ && source bin/thisroot.sh

```

### Check Installation

Check root is available by typing:

```bash

root
.q #to quit

```

Check python binding (pyROOT):

```bash

python -c 'import ROOT'

```
If you receive a message like `ImportError: No module named ROOT` you have not pyROOT properly installed.

### Starting the OSC listener

`receiveOsc.sde` file will serve you as an example of getting OSC messages from this program.
Open it in SuperCollider before running this program.

### Starting the program

Simply execute

```bash

python lhcvmm.py

```


## Troubleshooting

You may have `libPyROOT.so` compiled with a different version of python that the one you use to run `lhcvmm.py`.
In that case, find out which version of python you should be using to match `libPyROOT.so` one.

OSX:
$ otool -L /path/to/ROOT/lib/libPyROOT.so

Linux:
$ ldd /path/to/ROOT/lib/libPyROOT.so


### To Do

Instead of sending messages to a single server, will work on a subscrition/unsubscription pattern.

the OSCMultiClient supports:

* Sending a specific OSC-message to multiple remote servers
* Remote server subscription / unsubscription (through OSC-messages, of course)
* Message-address filtering.

-------------------

The OSCMultiClient supports 'multiple unicasting' of OSC-messages. This means that a given OSC-message can be sent to multiple servers, each with their own IP-address. This is different from sending OSC-messages to a specific multicast or broadcast IP-address (which is also supported, of course). If you want to have multiple clients controlling, and receiving replies from, one OSC-server, it's difficult to say which approach, multiple-unicast, multicast or broadcast, is 'best'. This depends mostly on your network-topology. multicasting is not supported by all routers, and broadcasting generally only works within your local subnet, but both of these methods produce less network-traffic than multiple-unicasting.

To allow for dynamic configuration of which remote clients will receive reply-messages from the OSCMultiClient, an OSC-server that is bound to an OSCMultiClient supports client subscription. By sending messages to the server's '/subscribe' and '/unsubscribe' OSC-addresses, clients can (un)subscribe themselves (or any other client) to receive reply-messages from the OSCMultiClient.

More info: https://trac.v2.nl/wiki/pyOSC
