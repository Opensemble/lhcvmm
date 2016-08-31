
#ifndef __EventStream__
#define __EventStream__

#define MAX_EVENT_ARRAY 32

#include "ofMain.h"
#include "ofxGui.h"
#include "ofMath.h"
#include <TFile.h>
#include <TTree.h>
#include "OscMultiClient.h"


struct Datafile {
  string name;
  string path;
};

struct RootEvent{
  int pvxp_n;
  float mu;
  float met_phi;
  float met_et;
  unsigned int lep_n;
    float lep_pt[MAX_EVENT_ARRAY];
    float lep_phi[MAX_EVENT_ARRAY];
    float lep_eta[MAX_EVENT_ARRAY];
    float lep_E[MAX_EVENT_ARRAY];
    unsigned int jet_n;
    float jet_pt[MAX_EVENT_ARRAY];
    float jet_phi[MAX_EVENT_ARRAY];
    float jet_eta[MAX_EVENT_ARRAY];
    float jet_E[MAX_EVENT_ARRAY];
    float jet_m[MAX_EVENT_ARRAY];

};


class EventStream : public ofThread {
  ofParameter<float> radius;
  ofParameter<int> detectorsCount;

  TFile* data_file;
  TTree* tree;
  RootEvent data;

  int events_count;

	// data file browser
	vector<Datafile> *dataFiles;
	ofParameter<int> dataFileIndex;
  ofParameter<string> fileToAdd;

  ofParameter<float> rate;
  ofParameter<float> randomness;
  ofParameter<bool> loop;
  ofParameter<int> offset;
  ofParameter<int> count;
  void dataFileIndexChanged(int &);
  void enabledChanged(bool &);
  void loadTTree();

    OscMultiClient *oscMultiClient;

public:
  EventStream();
	~EventStream();
  void threadedFunction();

  void setup(vector<Datafile>*,  OscMultiClient *oscMultiClient, int channel);
  void update();
  ofParameterGroup parameters;
  ofParameter<bool> enabled;
    float currentEventTime;
    float nextEventTime;
    int currentEventIndex;
    int channel;


};
#endif /* defined(__EventStream__) */
