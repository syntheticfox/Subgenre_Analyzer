#include <QApplication>
#include "GUI_Object.h"



int main(int argc, char *argv[]){
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    GUI_Objects object;


    engine.rootContext()->setContextProperty("Genre_Finder",&object);
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));
    createDirectory();//

    return app.exec();
}
