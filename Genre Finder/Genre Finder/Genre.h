#include "dirent.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#define MAX_TEMPO 300 //will the the size of the feature vector

string currentGenre = ""; //current genre category

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

vector<Genre*> subgenres;//sub-genres from the loaded genre file

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
vector<DatabaseItem>database; //database of all previously added tempo data and new data being analyzed
//first two dimensions is the graph with x and y two different methods of calculating tempo
//the third dimension is a vector of ints, where if there is a point there, it will display what cluster it belongs to

vector<int> featureVector[MAX_TEMPO][MAX_TEMPO]; //[peak][ac]
int numberOfClusters;

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
	string filePath = "Database/" + currentGenre + "_Database.txt";
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

void loadGenre(){ //load genre file and add subgenres to vector
	char path[_MAX_PATH+1];
	GetModuleFileName(NULL,path,_MAX_PATH);
	string sp = path;
	sp = sp.substr(0,sp.length()-22) + "Genre Finder\\Genres"; //set path for genre folder (root is changed due to file selection window)

	DIR *directory = opendir(sp.c_str());
	struct dirent *item = NULL;
	vector<string> genres;
	int g = 1;//directory number of genre

	
	if(directory == NULL)cout << "\nMissing Genre Folder.\n";
	else {
		cout << "\nChoose a Genre:\n";

		while(item = readdir(directory)){//display files in the 'Genres' directory
			string gName = item->d_name;
			if(gName != "." && gName!= ".."){
				gName = gName.substr(0,gName.length()-4); //remove file path (.txt)
				genres.push_back(gName);
				cout << g << ". " << gName << endl;//genre select menu
				g++;
			}
		}
		closedir(directory);	 
	
		cin >> g;
		while(g < 0 || g > genres.size()){//genre select menu
			cout << "\nChoose a Genre:\n";
			for(int k = 0; k < genres.size(); k++)
				cout << k+1 << ". " << genres[k] << endl;
			cin >> g;
		}
		currentGenre = genres[g-1];

		//load subgenres from file and create Genre objects
		cout << "\nLoading Genre...";
		string filePath = "Genres/" + currentGenre + ".txt";
		ifstream genreFile(filePath.c_str());

		if(genreFile.is_open()){
			while(genreFile.good()){
				string name, tempo, space;
				int minTempo, maxTempo;
		
				getline(genreFile, name, ',');

				getline(genreFile, tempo, ',');
				minTempo = stoi(tempo);
				
				getline(genreFile, tempo);
				maxTempo = stoi(tempo); 

				subgenres.push_back(new Genre(name, minTempo, maxTempo));
			}
			genreFile.close();
			createGenreFeatureVector();//create initial feature vector from file data
			cout << "Done\n";
		}
		else{
			cout << "\nCould not open file containing the " << currentGenre << " genre's info.\n";
			loadGenre();//ask to select something else
		}
	}
}

