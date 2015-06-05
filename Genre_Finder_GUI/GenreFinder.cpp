#include "GenreFinder.h"

#include <QDir>

#include "dirent.h"
#include "GUI_Object.h"
#include <fstream>
#include <QImage>
//#include <QSize>

#define MAX_TEMPO 300

vector<string> genres;
vector<Genre*> subgenres;//sub-genres from the loaded genre file
string currentPath = "";
string currentGenre = "";
AudioData *audioData = new AudioData;
vector<int> featureVector[MAX_TEMPO][MAX_TEMPO]; //[peak][ac]
vector<DatabaseItem>database; //database of all previously added tempo data and new data being analyzed
int numberOfClusters;


string getCurrentPath(){
    return currentPath;
}
bool createDirectory(){
    QString databaseDir = QString::fromStdString(currentPath)+"Database";

    if(!QDir(QString::fromStdString(currentPath)+"/Database").exists()){
        qDebug() << "Creating Database directory in: " << databaseDir;
        QDir().mkdir(databaseDir);
        return true;
    }
    qDebug() << "Database directory located in: " << databaseDir;
    return false;
}

bool createImage(){
    int featureSize = 5;
    QImage image(MAX_TEMPO*featureSize,MAX_TEMPO*featureSize,QImage::Format_RGB32);

    //white, red, blue, green, orange, grey, turquoise, brown,gold, blue/grey, purple, pink
    QRgb colors[12] = {qRgb(255,255,255),qRgb(204,0,0),qRgb(0,0,255),
                       qRgb(0,153,0),qRgb(255,128,0),qRgb(128,128,128),
                       qRgb(51,255,153),qRgb(102,51,0),qRgb(204,204,0),
                       qRgb(0,153,153),qRgb(153,0,153),qRgb(255,0,127)};



    for(int x = 0; x < MAX_TEMPO; x++)
        for(int y = 0; y < MAX_TEMPO; y++)
               for(int j = 0; j < featureSize; j++)
                   for(int k = 0; k < featureSize; k++){
                        if(featureVector[x][y].size() == 0)
                            image.setPixel(x*featureSize+j,y*featureSize+k,colors[0]);
                        else
                            image.setPixel(x*featureSize+j,y*featureSize+k,colors[featureVector[x][y][0]]);
        }
            //image.setPixel(x,y,colors[9]);
    QString imgName = QString::fromStdString(currentPath)+"Database/Feature_Vector.tif";

    return image.save(imgName,"TIFF", 100);
}



int getMaxTempo(){
    return MAX_TEMPO;
}

string showSubgenres(int k){
    if(k == subgenres.size())return "";
    return subgenres[k]->getName();
}

string AudioData::getGenre(){
    return subgenres[featureVector[peakTempo-1][acTempo-1][location]]->getName();
}

bool genreLoaded(string genre){
    return genre == currentGenre;
}

string analyzeGenre(){

    //add current audio file to feature vector
    numberOfClusters++;
    featureVector[audioData->getPeakTempo()-1][audioData->getACTempo()-1].push_back(numberOfClusters);
    database.push_back(DatabaseItem(audioData->getPeakTempo(),audioData->getACTempo(),featureVector[audioData->getPeakTempo()-1][audioData->getACTempo()-1].size()-1));
    audioData->location = featureVector[audioData->getPeakTempo()-1][audioData->getACTempo()-1].size()-1;

    for(int k = 0; k < database.size(); k++){

        bool closestClusterFound = false;
        int x = database[k].peakTempo-1, y = database[k].acTempo-1, stepSize = 1;


        while(!closestClusterFound){

            vector<int> nearClusters;//all clusters that are the same distance away

            if(x+stepSize < MAX_TEMPO-1){

                //check nearest to the right
                if(featureVector[x+stepSize][y].size() != 0){
                    int cluster = featureVector[x+stepSize][y][0];
                    nearClusters.push_back(cluster);
                }
            }
            if(y+stepSize < MAX_TEMPO-1){
                //check nearest to the top
                if(featureVector[x][y+stepSize].size() != 0){
                    int cluster = featureVector[x][y+stepSize][0];
                    nearClusters.push_back(cluster);
                }
            }
            if(x-stepSize < 0){
                //check nearest to the left
                if(featureVector[x-stepSize][y].size() != 0){
                    int cluster = featureVector[x-stepSize][y][0];
                    nearClusters.push_back(cluster);
                }

            }
            if(y-stepSize < 0){
                //check nearest to bottom
                if(featureVector[x][y-stepSize].size() != 0){
                    int cluster = featureVector[x][y-stepSize][0];
                    nearClusters.push_back(cluster);
                }

            }
            if(nearClusters.size() != 0){//if there was a near cluster, merge the clusters and break out of loop
                featureVector[x][y][database[k].location] = nearClusters[0]; //unhappy when loads database file
                closestClusterFound = true;
            }
            else stepSize++;
        }
    }
    return audioData->getGenre();
}



void createGenreFeatureVector(){
    //create Feature Vector with subgenre data
    for(int k = 0; k < subgenres.size(); k++){ //covers all the subgenres
        int numBins = subgenres[k]->getMaxTempo() - subgenres[k]->getMinTempo();
        for(int n = 0; n < numBins; n++){
            for(int m = 0; m < numBins; m++){
                featureVector[subgenres[k]->getMinTempo()+n][subgenres[k]->getMinTempo()+m].push_back(k+1);
            }
        }
    }
    numberOfClusters = subgenres.size();
    //load DatabaseItems from file and add to Feature Vector
    string filePath = currentPath + "Database/" + currentGenre + "_Database.txt";
    ifstream databaseFile(filePath.c_str());

    if(databaseFile.is_open()){ //read database items and add to feature vector
        while(databaseFile.good()){
            int peakTempo, acTempo;

            databaseFile >> peakTempo;
            databaseFile >> acTempo;

            if(databaseFile.eof()) break; //break if end of file, prevents adding last line of file twice

            numberOfClusters++;
            featureVector[peakTempo-1][acTempo-1].push_back(numberOfClusters); //add to feature vector as a new cluster
            database.push_back(DatabaseItem(peakTempo, acTempo, featureVector[peakTempo-1][acTempo-1].size()-1));
        }
        databaseFile.close();
    }
}


AudioData* getAudioData(){
    return audioData;
}

//implement a loading bar for this later
string loadAudio(string path){

    ZPlay *player = CreateZPlay(); //audio file handler, used to determine audio information such as tempo
    path = path.substr(8,path.size());

    if(player->OpenFile(path.c_str(), sfAutodetect) == 0)
        return "";
    else{
        int peakTempo = player->DetectBPM(dmPeaks), acTempo = player->DetectBPM(dmAutoCorrelation);
        audioData->setPeakTempo(peakTempo); //dmPeaks is faster
        audioData->setACTempo(acTempo); //dmAutoCorrelation is more accurate
        audioData->setName(path.substr(path.find_last_of("/\\")+1));

        TStreamInfo streamInfo;
        player->GetStreamInfo(&streamInfo);
        audioData->setSongLen(streamInfo.Length.sec);

        return  audioData->getName();//return loaded file name
    }
   // player->Close();
   // delete player;

}

QString getGenre(int k){
    if(k == genres.size())return QString("");
    return QString::fromStdString(genres[k]);

}

bool loadGenre(){ //load genre file and add subgenres to vector
    genres.clear();
    currentPath = QDir::currentPath().QString::toStdString();
    currentPath = currentPath.substr(0,currentPath.find_last_of("/")) + "/Genre_Finder_GUI/";

    DIR *directory = opendir((currentPath+"Genres/").c_str());
    struct dirent *item = NULL;

    int g = 1;//directory number of genre

    if(directory == NULL)return false; //missing genre folder
    else {
        while(item = readdir(directory)){//display files in the 'Genres' directory
            string gName = item->d_name;
            if(gName != "." && gName!= ".."){
                gName = gName.substr(0,gName.length()-4); //remove file path (.txt)
                genres.push_back(gName);
                g++;
            }
        }
        closedir(directory);
        return true; //genres loaded correctly
    }
}
bool setGenre(int k){
    string filePath = currentPath + "Genres/" + genres[k] + ".txt";
    ifstream genreFile(filePath.c_str());
    bool convResult;//used to trap string to int conversion errors
    currentGenre = genres[k];

    //
    subgenres.clear();
    if(genreFile.is_open()){

        while(genreFile.good()){
            string name, tempo;//, space;
            int minTempo, maxTempo;


            getline(genreFile, name, ',');

            getline(genreFile, tempo, ',');
            minTempo = QString::fromStdString(tempo).toInt(&convResult,10);
            if(!convResult)return false; //error converting string to int

            getline(genreFile, tempo);
            maxTempo =  QString::fromStdString(tempo).toInt(&convResult,10);
            if(!convResult)return false; //error converting string to int

            subgenres.push_back(new Genre(name, minTempo, maxTempo));
        }
        genreFile.close();
        createGenreFeatureVector();//create initial feature vector from file data
        return true; //genre data read correctly
    }
    else{
        return false; //error reading genre data
    }

}
void saveAudioData(){//save audio data when user selected to quit program
    if(currentGenre != ""){
        string filePath = currentPath + "Database/" + currentGenre + "_Database.txt";//sp.substr(0,sp.length()-22) + "Genre Finder\\Database\\" + currentGenre + "_Database.txt";
        ofstream databaseFile(filePath.c_str());

        if(databaseFile.is_open()){
            for(int k = 0; k < database.size(); k++)//save all database items
                if(databaseFile.good())databaseFile << database[k].peakTempo << ' ' << database[k].acTempo << endl;
        }
    }
}
