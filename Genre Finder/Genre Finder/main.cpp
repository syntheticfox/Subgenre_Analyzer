#include "AudioData.h"
#include <cstring>
#include <algorithm>
#include <Windows.h>

string currentFile = ""; //currently loaded audio file
HWND win = NULL; //window to open file selector
AudioData *audioData = new AudioData; //audio data for currently loaded audio file

void printHeader(){
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "CSCI 4478: Artificial Intelligence" << endl; 
	cout << "Christian Ruiz, Spring 2014" << endl;
	cout << "Term Project: Sub-Genre Finder" << endl;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
}
void loadAudio(){

	//dialog box to obtain file path of audio
	char file[MAX_PATH]="";
	OPENFILENAME ofn={0};
	ofn.lStructSize=sizeof(ofn);
	ofn.hwndOwner=win;
	ofn.nMaxFile=MAX_PATH;
	ofn.lpstrFile=file;
	ofn.Flags=OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_EXPLORER;
	ofn.lpstrTitle="Select a file to load";
	ofn.lpstrFilter="playable files\0*.mo3;*.xm;*.mod;*.s3m;*.it;*.mtm;*.umx;*.mp3;*.mp2;*.mp1;*.ogg;*.wav;*.aif\0All files\0*.*\0\0";
	if (!GetOpenFileName(&ofn)) cout << "Cannot get file";
		
	if(player->OpenFile(file, sfAutodetect) == 0)
		cout << "Load Failed.\n" ;
	else cout << "Audio Loaded.\n";

	string str(file);
	currentFile =  str.substr(str.find_last_of("/\\")+1); //update loaded file name

	analyzeAudio();	
}
void analyzeGenre(){
	cout << "\nDetermining Genre...";

	for(int k = 0; k < database.size(); k++){

		//cout << k << endl;
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
	cout << "Done\n";
}
void analyzeAudio(){
	cout << "Analyzing " << currentFile << "...\n";

	int peakTempo = player->DetectBPM(dmPeaks), acTempo = player->DetectBPM(dmAutoCorrelation);
	audioData->setPeakTempo(peakTempo); //dmPeaks is faster
	audioData->setACTempo(acTempo); //dmAutoCorrelation is more accurate
	audioData->setName(currentFile);
	
	TStreamInfo streamInfo;//find the length of the song
	player->GetStreamInfo(&streamInfo);
	audioData->setSongLen(streamInfo.Length.sec);

	numberOfClusters++;//add loaded audio data to feature vector
	featureVector[peakTempo-1][acTempo-1].push_back(numberOfClusters);
	database.push_back(DatabaseItem(audioData->getPeakTempo(),audioData->getACTempo(),featureVector[peakTempo-1][acTempo-1].size()-1));
	audioData->location = featureVector[peakTempo-1][acTempo-1].size()-1;

	analyzeGenre(); //analyze the feature vector (clustering)
	
	cout << "Analysis Done!\n";
}
void displayInfo(int x = 0){ //display analyzed audio info
	cout << endl;
	if(x == 1)cout << "1. ";
	cout << "Name: " << audioData->getName() << endl;
	if(x == 1)cout << "2. ";
	cout << "Length: " << audioData->getSongLen()/60 << ":" << audioData->getSongLen()%60 << endl;
	if(x == 1)cout << "3. ";
	cout << "Peak Tempo: " << audioData->getPeakTempo() << "\tAutocorrection Tempo: " << audioData->getACTempo() << endl;
	if(x == 1)cout << "4. Stop editing";
	else{
		if(audioData->getGenre() == "")cout << "Genre: " << "Unknown" << endl;
		else cout << "Genre: " << audioData->getGenre() << endl;		
	}
}
void audioInfo(){//audio info menu
	int input;
	displayInfo();
	do{
		cout << "\nEnter a command:\n";
		cout << "1.Edit Info 2. Main Menu\n";
		cin >> input;
	}while(input < 0 && input > 2);
	
	if(input == 1){ // edit info
		int editInput;
		do{
			cout << "Select an element to edit:\n";
			displayInfo(1);
			cin >> editInput;
		}while(editInput < 0 && editInput > 4);

		if(editInput != 5) cout << "New Value: ";
		
		if(editInput == 1){
			string editedData;
			while(getline(cin, editedData)) if(editedData != "")break;
			audioData->setName(editedData);
		}
		else if(editInput == 2){
			cout << "(Enter in seconds) ";
			int editedData;
			cin >> editedData;
			audioData->setSongLen(editedData);
		}
		else if(editInput == 3){
			int editedData;
			cin >> editedData;
			audioData->setPeakTempo(editedData);
			audioData->setACTempo(editedData);
		}

		audioInfo(); //call again to bring up menu
	}
}

bool menu(){//main menu
	int input;
	
	if(currentGenre == ""){ //only ask to choose a genre when one is not chosen, initial menu
		do{
			cout << "\nChoose from the following options:\n";
			cout << "1. Choose Genre\n";
			cout << "2. Quit\n";
			cin >> input;
		}while(input < 0 && input > 2);
		if(input == 1) loadGenre();
		else return false;
	}

	do{//main menu after a genre is chosen
		cout << "\nCurrent Genre: " << currentGenre << "\n";
		cout << "Choose from the following options:";
		
		cout << "\n1. Load audio file\n";
		
		if(currentFile != ""){ 
			cout << "2. View " << currentFile << " info\n";
			cout << "3. Quit\n";
		}
		else cout << "2. Quit\n";
			
		cin >> input;
	}while(input < 1 && input > 3);

	if(input == 1) loadAudio();
	if(input == 2){
		if(currentFile != "") audioInfo(); //if music is currently loaded
		else return false; //if not, user selected to quit
	}
	if(input == 3 && currentFile != "") return false; //user selected to quit when audio was loaded

	return true;			 
}
void saveAudioData(){//save audio data when user selected to quit program
	char path[_MAX_PATH+1];//get root path name
	GetModuleFileName(NULL,path,_MAX_PATH);
	string sp = path;
	string filePath = sp.substr(0,sp.length()-22) + "Genre Finder\\Database\\" + currentGenre + "_Database.txt";
	ofstream databaseFile(filePath.c_str());
	
	if(databaseFile.is_open()){
		for(int k = 0; k < database.size(); k++)//save all database items
			if(databaseFile.good())databaseFile << database[k].peakTempo << ' ' << database[k].acTempo << endl;
	}
}
int main() {
	printHeader();
	while(menu()); //start the menu and continue until user quits
	if(currentGenre != "" && database.size() != 0)saveAudioData(); //write AudioData out to file
	return 0;
}