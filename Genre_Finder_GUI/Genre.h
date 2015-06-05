#ifndef GENRE_H
#define GENRE_H

#include <string>
#include <vector>

using namespace std;

struct range {//used in Genre class for the tempo range
    int min;
    int max;
};

class Genre {
private:
    string name; //sub-genre name
    range tempo;
public:
    Genre(string genreName, int minTempo, int maxTempo){
        name = genreName;
        tempo.min = minTempo;
        tempo.max = maxTempo;
    }
    Genre(){
        name = "";
        tempo.min = 0;
        tempo.max = 0;
    }
    ~Genre(){}

    string getName(){return name;}
    int getMinTempo(){return tempo.min;}
    int getMaxTempo(){return tempo.max;}
};



#endif // GENRE_H
