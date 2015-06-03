#include "libzplay.h"
#include <iostream>
#include "Genre.h"


using namespace libZPlay;

ZPlay *player = CreateZPlay(); //audio file handler, used to determine audio information such as tempo

void analyzeAudio();

class AudioData {
private:
	string genre; //the song's genre
	int peakTempo; //dmPeaks 
	int acTempo;//autocorrection, dmAutoCorrelation 
	string name; //song name
	int songLen; //in seconds
	
public:
	AudioData(){genre = "";}
	~AudioData(){}

	int location; //in feature vector

	void setSongLen(int len){songLen = len;}
	void setPeakTempo(int bpm){peakTempo = bpm;}
	void setACTempo(int bpm){acTempo = bpm;}
	void setName(string n){name = n;}

	int getSongLen(){return songLen;}
	int getPeakTempo(){return peakTempo;}
	int getACTempo(){return acTempo;}
	string getName(){return name;}
	string getGenre(){
		return subgenres[featureVector[peakTempo-1][acTempo-1][location]]->getName();
	}
};