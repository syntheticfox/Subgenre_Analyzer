#ifndef AUDIODATA_H
#define AUDIODATA_H

#include "libzplay.h"

using namespace libZPlay;

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
    void setGenre(string g){genre = g;}

    int getSongLen(){return songLen;}
    int getPeakTempo(){return peakTempo;}
    int getACTempo(){return acTempo;}
    string getName(){return name;}
    string getGenre();
};

class DatabaseItem {
public:
    int peakTempo;
    int acTempo;
    int location;
     DatabaseItem(){
         peakTempo = 0;
         acTempo = 0;
     }
     DatabaseItem(int peak, int ac, int loc){
         peakTempo = peak;
         acTempo = ac;
         location = loc;
     }
     ~DatabaseItem(){}
};
void createGenreFeatureVector();
//vector<DatabaseItem>database; //database of all previously added tempo data and new data being analyzed



#endif // AUDIODATA_H
