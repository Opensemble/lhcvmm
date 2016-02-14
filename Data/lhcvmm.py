import time
import sys
from thread import start_new_thread
from OSC import  OSCClient, OSCClientError, OSCBundle, OSCMessage
from random import random

data_file = None
data_file_path = "data_files/data_Egamma_A.root"
osc_port = 57120 #default SuperCollider port (must be open before executing this program)
interval = 0.12 # time between events, in seconds
# just some easy ansi colors printing: 'o' for ok, 'w' for warning, 'e' for error.
def printc(t, c='o'):
    print '\033[9' + {'o': '2m','w': '3m','e': '1m'}[c] + t + '\033[0m'

def get_ttree():
    global data_file    # Need a reference to file to close it

    try:
        from ROOT import TFile
        # open test data file
        data_file = TFile.Open(data_file_path, "read")

        #gathers mini ttree
        tree = data_file.Get("mini")
        entries_count = tree.GetEntriesFast()

        printc( "TTree 'mini' loaded from %s containing %s entries." % (data_file_path, entries_count),'o')
        #tree.Scan("lep_eta")
        return tree

    except ImportError:
        printc( "\nError: Unable to load ROOT.\n",'e')
        printc( "Data will be simulated.\n")
        return None
    except ReferenceError:
        printc( "\nError: %s file not found. Data will be simulated." % data_file_path, 'e')
        printc( "Download it from https://tripiana.web.cern.ch/tripiana/openensemble/ATLAS_data/data_Egamma_A.root\n", 'w')
        printc( "Data will be simulated.\n")
        return None
    except AttributeError:
        printc( "\nError: Expected TTree 'mini' in %s file.\n" % data_file_path, 'e')
        printc( "Data will be simulated.\n")
        return None


def send_event(tree):
    try:
        bundle = OSCBundle()
        msg = OSCMessage("/entry")
        if tree is None:
            msg.append(random())
            msg.append(random())
            msg.append(random(), 'b')
        else:
            msg.append(tree.lbNumber)
            msg.append(tree.mu)
            msg.append(tree.lep_eta[0], 'b')
        bundle.append(msg)
        client.send(bundle)

    except OSCClientError, e:
        printc( "\OSCClientError: Connection refused on port %s." % osc_port, 'e')

# Define a function for the thread
def send_events_periodically():
    print "Start Sending events every %s seconds." % interval
    print "Each * printed represents an event sent."
    print "Press Ctrl + C to finalize."

    entries_index = 0
    entries_count = 0
    if tree is not None:
        entries_count = tree.GetEntriesFast()

    starttime=time.time()

    while 1:
        # restart * printing every 10 events
        sys.stdout.write('\r----------\r')
        for i in range(0, 10):
            if entries_count is 0:
                send_event()
            else:
                # copy next entry into memory
                tree.GetEntry(entries_index)
                send_event(tree)
                entries_index += 1
                entries_index %= entries_count

            time_to_sleep = interval - ((time.time() - starttime) % interval)
            sys.stdout.write('*')
            sys.stdout.flush()
            time.sleep(time_to_sleep)


try:
    #initialize osc client
    client = OSCClient()

    client.connect(('127.0.0.1', osc_port))   # connect to SuperCollider

    tree = get_ttree()

    start_new_thread( send_events_periodically, ())
    while 1:
        pass

except OSCClientError:
    printc( "\OSCClientError: Connection refused on port %s." % osc_port, 'e')

except KeyboardInterrupt:
    if data_file is not None:
        data_file.Close()
    print "\nProgram terminated."






'''
From varlist.pdf sent by Martin Tripiana.

Proposed content of slimmed down version of the a data sample for educational purposes.
The content presented is a further tuple produced by AnalysisTop.


+==========================+===============+====================================================================================+
| BRANCH NAME              | TYPE          | DESCRIPTION                                                                        |
+==========================+===============+====================================================================================+
| runNumber                | int           | runNumber                                                                          |
+==========================+===============+====================================================================================+
| eventNumber              | int           | eventNumber                                                                        |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| channelNumber            | int           | channelNumber                                                                      |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| lbNumber                 | int           | lbNumber                                                                           |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| rndRunNumber             | int           | randomized run number mimicking run number distribution in data                    |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| mu                       | float         | average interactions per bunch crossing                                            |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| mcWeight                 | float         | weight of an MC event                                                              |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| pvxp_n                   | int           | number of primary vertices                                                         |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| isGoodEvent              | int           | summary of diverse quality flags like hfor                                         |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| scaleFactor              | float         | overall scale factor for the preselected event                                     |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| trigE                    | bool          | boolean whether a standard trigger has fired in the egamma stream                  |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| trigM                    | bool          | boolean whether a standard trigger has fired in the muon stream                    |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| passGRL                  | bool          | signifies whether event passes the GRL may be put in isGoodEvent                   |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| hasGoodVertex            | bool          | signifies whether the event has at least one good vertex                           |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| lep_n                    | int           | number of preselected leptons                                                      |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| lep_truthMatched         | vector<bool>  | boolean indicating whether the lepton is matched to a truth lepton                 |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| lep_trigMatched          | vector<bool>  | boolean signifying whether the lepton is the one triggering the event              |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| lep_pt                   | vector<float> | transverse momentum of the lepton                                                  |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| lep_eta                  | vector<float> | pseudo-rapidity of the lepton                                                      |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| lep_phi                  | vector<float> | azimuthal angle of the lepton                                                      |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| lep_E                    | vector<float> | energy of the lepton                                                               |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| lep_z0                   | vector<float> | z-coordinate of the track associated to the lepton wrt.                            |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| lep_charge               | vector<float> | the primary vertex charge of the lepton                                            |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| lep_isTight              | vector<bool>  | boolean indicating whether the lepton is of tight quality                          |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| lep_flag                 | vector<int>   | bitmask implementing object cuts of the top group                                  |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| lep_type                 | vector<int>   | number signifying the lepton type (e, mu, tau) of the lepton                       |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| lep_ptcone30             | vector<float> | ptcone30 isolation for the lepton                                                  |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| lep_etcone20             | vector<float> | etcone20 isolation for the lepton                                                  |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| lep_trackd0pvunbiased    | vector<float> | d0 of the track associated to the lepton at the point of closest approach (p.o.a.) |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| lep_tracksigd0pvunbiased | vector<float> | d0 signifcance of the track associated to the lepton at the p.o.a.                 |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| met_et                   | float         | Transverse energy of the missing momentum vector                                   |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| met_phi                  | float         | Azimuthal angle of the missing momentum vector                                     |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| jet_n                    | int           | number of selected jets                                                            |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| jet_pt                   | vector<float> | transverse momentum of the jet                                                     |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| jet_eta                  | vector<float> | pseudorapidity of the jet                                                          |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| jet_phi                  | vector<float> | azimuthal angle of the jet                                                         |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| jet_E                    | vector<float> | energy of the jet                                                                  |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| jet_m                    | vector<float> | invariant mass of the jet                                                          |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| jet_jvf                  | vector<float> | JetVertexFraction of the jet                                                       |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| jet_flag                 | vector<int>   | bitmask implementing object cuts of the top group                                  |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| jet_trueflav             | vector<int>   | true flavor of the jet                                                             |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| jet_truthMatched         | vector<int>   | information whether the jet matches a jet on truth level                           |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| jet_SV0                  | vector<float> | SV0 weight of the jet                                                              |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| jet_MV1                  | vector<float> | MV1 weight of the jet                                                              |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| scaleFactor_BTAG         | float         | scalefactor for btagging                                                           |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| scaleFactor_ELE          | float         | scalefactor for electron eciency                                                   |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| scaleFactor_JVFSF        | float         | scalefactor for jet vertex fraction                                                |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| scaleFactor_MUON         | float         | scalefactor for muon eciency                                                       |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| scaleFactor_PILEUP       | float         | scalefactor for pileup reweighting                                                 |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| scaleFactor_TRIGGER      | float         | scalefactor for trigger                                                            |
+--------------------------+---------------+------------------------------------------------------------------------------------+
| scaleFactor_ZVERTEX      | float         | scalefactor for z-vertex reweighting                                               |
+==========================+===============+====================================================================================+


'''
