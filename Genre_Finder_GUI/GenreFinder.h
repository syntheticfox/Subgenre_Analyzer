#ifndef GENREFINDER_H
#define GENREFINDER_H


#include <QObject>
#include <QDebug>
#include "Genre.h"
#include "AudioData.h"

using namespace std;


string loadAudio(string path);
string analyzeGenre();
bool loadGenre();
QString getGenre(int k);
bool setGenre(int k);
AudioData* getAudioData();
int getMaxTempo();
void saveAudioData();
bool genreLoaded(string genre);
string showSubgenres(int k);
string getCurrentPath();
bool createImage();
QSize getImageSize();

#endif // GENREFINDER_H
