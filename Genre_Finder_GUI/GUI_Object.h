#ifndef GUI_OBJECT_H
#define GUI_OBJECT_H


//#include <QDebug>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include "GenreFinder.h"
#include <QQmlContext>
#include <QtGui/QGuiApplication>

class GUI_Objects: public QObject {
    Q_OBJECT
public:
    Q_INVOKABLE QString load_Audio(QString path){
        QString str;
        return str.fromStdString(loadAudio(path.toStdString()));}
    Q_INVOKABLE bool show_Genres(){
        return loadGenre();
    }
    Q_INVOKABLE QString get_Genre(int k){
        return getGenre(k);
    }
    Q_INVOKABLE bool set_Genre(int k){
        return setGenre(k);
    }

    Q_INVOKABLE QString get_Name(){
        return QString::fromStdString(getAudioData()->getName());
    }
    Q_INVOKABLE QString get_Len(){
        if(getAudioData()->getSongLen()%60 < 10) return QString::number(getAudioData()->getSongLen()/60) + ":0" + QString::number(getAudioData()->getSongLen()%60);
        else return QString::number(getAudioData()->getSongLen()/60) + ":" + QString::number(getAudioData()->getSongLen()%60);
    }
    Q_INVOKABLE QString get_peakTempo(){
        return QString::number(getAudioData()->getPeakTempo());

    }
    Q_INVOKABLE QString get_acTempo(){
        return QString::number(getAudioData()->getACTempo());

    }
    Q_INVOKABLE QString get_subGenre(){
        QString subgenre = QString::fromStdString(analyzeGenre());
        createImage();

        return subgenre;
    }
    Q_INVOKABLE int get_MaxTempo(){
        return getMaxTempo();
    }
    Q_INVOKABLE void save_audioData(){
        saveAudioData();
    }
    Q_INVOKABLE void set_Name(QString name){
        getAudioData()->setName(name.toStdString());
    }
    Q_INVOKABLE void set_PeakTempo(QString peak){
        getAudioData()->setPeakTempo(peak.toInt());
    }
    Q_INVOKABLE void set_acTempo(QString peak){
        getAudioData()->setACTempo(peak.toInt());
    }
    Q_INVOKABLE bool genre_loaded(QString genre){
        return genreLoaded(genre.toStdString());
    }
    Q_INVOKABLE void set_subgenre(QString subgenre){
        getAudioData()->setGenre(subgenre.toStdString());
    }
    Q_INVOKABLE QString show_subgenres(int k){
        return QString::fromStdString(showSubgenres(k));
    }
    Q_INVOKABLE QUrl get_currentPath(){
        return QUrl::fromLocalFile(QString::fromStdString(getCurrentPath()) + "Database/Feature_Vector.tif");
    }


    GUI_Objects(){}
    ~GUI_Objects(){}
};
bool createDirectory();
#endif // GUI_OBJECT_H
