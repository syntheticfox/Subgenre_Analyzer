import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.1
import QtQuick.Window 2.1
import Qt.labs.folderlistmodel 2.1
import QtQuick.Controls.Styles 1.2

ApplicationWindow {
    id: mainWindow
    visible: true


    x: Screen.desktopAvailableWidth/2 - mainWindow.width/2
    y: Screen.desktopAvailableHeight/2 - (mainWindow.height + Genre_Finder.get_MaxTempo() + 50)/2

    maximumHeight : 300 //not resizable
    maximumWidth : 550
    minimumHeight : 300
    minimumWidth : 550

    onClosing: Genre_Finder.save_audioData()

    title: qsTr("Genre Finder")
    color: "#c0d9f7"

    Rectangle {
        id: background

        width: 550
        height: 300
        color: "#c0d9f7"
        anchors.horizontalCenterOffset: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        anchors.horizontalCenter: analyzeAudioButton.horizontalCenter
        anchors.top: parent.top
        anchors.left: parent.left

    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")

            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit()
            }
        }
    }

    FileDialog {
        id: fileDialog
        title: "Please choose an Audio File"

        onAccepted: {
            audioFileText.text = Genre_Finder.load_Audio(fileDialog.fileUrl.toString())
            if(audioFileText.text == ""){
                status.color = "#F78181"
                statusMsg.text = "Error Loading Audio"
                status.opacity = 1.0
                statusTimer.start()

                if(loadAutioFile.audioLoaded)loadAudioFile.audioLoaded = false
                if(analyzeAudioButton.enabled)analyzeAudioButton.enabled = false
            }
            else{
                status.color = "#89bfbc"
                statusMsg.text = "Audio Successfully Loaded"
                status.opacity = 1.0
                statusTimer.start()

                if(!loadAutioFile.audioLoaded) loadAutioFile.audioLoaded = true;
                if(genreSelector.genreLoaded)analyzeAudioButton.enabled = true
            }
       }
       onRejected: {
           console.log("Cancelled")
       }
     }

    Button {
        id: loadAutioFile
        text: "Load Audio"
        x: 44
        y: 122
        width: 100
        height: 30
        property bool audioLoaded: false
        onClicked: fileDialog.open()

    }

    Text {
        id: audioFileText
        x: 198
        y: 130
        width: 168
        height: 13
        anchors.right: analyzeAudioButton.right
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignLeft
        font.pixelSize: 12
    }
    Button {
        id: analyzeAudioButton
        x: 184
        y: 203
        width: 182
        height: 53
        text: qsTr("Analyze")
        scale: 1
        enabled: false
        onClicked: {
            songName.text = Genre_Finder.get_Name()
            songLen.text = "<u>Song Length:</u> " + Genre_Finder.get_Len()
            peakTempo.text = Genre_Finder.get_peakTempo()
            acTempo.text = Genre_Finder.get_acTempo()
            subgenre.text = Genre_Finder.get_subGenre()
            //featureVector.requestPaint()
            //featureVector.update()
            featureVector.source = Genre_Finder.get_currentPath() //must change source to refresh image
            analyzeWindow.show()
        }
    }
    Button {
        id: updateGenreDirectory
        x: 303
        y: 66

        width: 129
        height: 30
        text: qsTr("Update Genre Directory")
        scale: 1
        checked: false
        enabled: true

        onClicked: {
            Genre_Finder.save_audioData()
            analyzeWindow.close()
            //Load the genre
            if(Genre_Finder.set_Genre(genreSelector.currentIndex)){
                updateGenreDirectory.enabled = false
                genreSelector.genreLoaded = true
                status.color = "#89bfbc"
                statusMsg.text = "Genre Successfully Loaded"
                status.opacity = 1.0
                statusTimer.start()

                if(audioFileText.text != "") analyzeAudioButton.enabled = true

            }
            else {
                status.color = "#F78181"
                statusMsg.text = "Error Loading Genre"
                status.opacity = 1.0
                statusTimer.start()
            }
        }
    }


    ComboBox {
        id: genreSelector
        x: 129
        y: 66
        width: 168
        height: 28

        model: ListModel{
            id: genreSelectorModel
        }
        property int counter: 0
        property string str: ""
        property bool genreLoaded: false

        function showGenres(){
            genreSelectorModel.clear()

            if(Genre_Finder.show_Genres(genreSelectorModel)){
                while(1){
                    str = Genre_Finder.get_Genre(counter)
                    if(str == "") break
                    genreSelectorModel.append({"text":str})
                    counter++
                }
                counter = 0
                return true
            }
            else return false
        }

        onCurrentIndexChanged:{
            if(genreSelector.currentText == "") {
                if(!showGenres()){
                    updateGenreDirectory.enabled = false
                    status.color = "#F78181"
                    statusMsg.text = "Missing Genre Folder"
                    status.opacity = 1.0
                    statusTimer.start()
                }
                genreLoaded = false
            }
            else{
                genreLoaded = Genre_Finder.genre_loaded(genreSelector.currentText);
                if(genreLoaded){
                    if(loadAutioFile.audioLoaded && !analyzeAudioButton.enabled)analyzeAudioButton.enabled = true
                    updateGenreDirectory.enabled = false
                }
                else{
                    if(analyzeAudioButton.enabled)analyzeAudioButton.enabled = false
                    updateGenreDirectory.enabled = true
                }
            }
        }

        onPressedChanged:{
            if(genreSelector.pressed){
                if(!showGenres()){
                    updateGenreDirectory.enabled = false
                    status.color = "#F78181"
                    statusMsg.text = "Missing Genre Folder"
                    status.opacity = 1.0
                    statusTimer.start()
                    genreLoaded = false
                }
            }
        }
    }


   Window {
        id: analyzeWindow
        title: "Analyzed Audio"

        maximumHeight : featureVector.height + 50 //not resizable
        maximumWidth : 675
        minimumHeight : featureVector.height + 50
        minimumWidth : 675

        x: mainWindow.x - analyzeWindow.width/2 + mainWindow.width/2//mainWindow.width/2
        y: mainWindow.y + mainWindow.height + 39 //39 value of border

        Rectangle {
            id: analyzeRect
            color: background.color
            anchors.fill: parent

            MouseArea {
               anchors.fill: parent
               onClicked: {
                   if(songName.focus){
                       songName.focus = false
                       Genre_Finder.set_Name(songName.text)
                   }
                   if(peakTempo.focus){
                       peakTempo.focus = false
                       if(peakTempo.text == "") peakTempo.text = "0"
                       Genre_Finder.set_PeakTempo(peakTempo.text)
                   }
                   if(acTempo.focus){
                       acTempo.focus = false
                       if(acTempo.text == "") acTempo.text = "0"
                       Genre_Finder.set_acTempo(acTempo.text)
                   }
                   if(!subgenre.visible){
                       subgenreSelect.enabled = false
                       subgenreSelect.visible = false
                       subgenreConfirm.enabled = false
                       subgenreConfirm.visible = false
                       subgenre.visible = true
                   }
               }

            }
        }

        Rectangle{
            id: imgRect
            height: Genre_Finder.get_MaxTempo()
            width: Genre_Finder.get_MaxTempo()
            x: analyzeWindow.width - featureVector.width - 25
            y: 25
            clip: true

            Image{
                id: featureVector
                height: Genre_Finder.get_MaxTempo()
                width: Genre_Finder.get_MaxTempo()
                asynchronous: true
                source: Genre_Finder.get_currentPath()
                transform: imgZoom
                cache: false


                Scale{
                    id: imgZoom
                    property real scale: 1.0
                    property real min: 1.0
                    property real max: 10.0

                    origin.x: Genre_Finder.get_MaxTempo()/2
                    origin.y: Genre_Finder.get_MaxTempo()/2
                    //onXScaleChanged: {featureVector.x = 0 - Genre_Finder.get_MaxTempo()*(scale-1)}
                   // onYScaleChanged: {featureVector.y = 0 - Genre_Finder.get_MaxTempo()*(scale-1)}
                    xScale: scale
                    yScale: scale
                }
            }
            MouseArea{
                id: imgMouse
                anchors.fill: parent
                hoverEnabled: true
                onWheel: {
                    console.log(featureVector.x, featureVector.y, featureVector.height)
                    if(imgZoom.scale == imgZoom.min){
                        imgZoom.origin.x = imgMouse.mouseX
                        imgZoom.origin.y = imgMouse.mouseY
                    }
                    if(wheel.angleDelta.y > 0 && imgZoom.scale <= imgZoom.max)zoomSilder.value += 0.2
                    else if(wheel.angleDelta.y < 0 && imgZoom.scale >= imgZoom.min){
                        //drag.start()
                        //drag.maximumX = imgZoom.origin.x*(imgZoom.scale-1)
                        //drag.maximumY = imgZoom.origin.y*(imgZoom.scale-1)

                        zoomSilder.value -= 0.2
                    }
                }
                onClicked: console.log(imgZoom.origin.x+imgZoom.origin.x*(imgZoom.scale-1), imgZoom.origin.y+imgZoom.origin.y*(imgZoom.scale-1))
                drag.target: featureVector
                drag.minimumX: 0
                drag.maximumX: imgZoom.origin.x*(imgZoom.scale-1)
                drag.minimumY: 0
                drag.maximumY: imgZoom.origin.y*(imgZoom.scale-1)


            }
        }
        Slider {
            id: zoomSilder
            x: imgRect.x - 25
            y: imgRect.y
            orientation: 0 //vertical
           // value: imgZoom.scale
            height: featureVector.height
            minimumValue: imgZoom.min
            maximumValue: imgZoom.max
            stepSize: 0.1

            onValueChanged: imgZoom.scale = zoomSilder.value
        }

        Text {
            id: nameText
            x: 45
            y: analyzeWindow.height * (65/350)
            text: "Name:"
            font.underline: true
            font.pixelSize: 13
        }
        Rectangle {
            id: songNameArea
            width: songName.width
            height: songName.height
            x: nameText.x + nameText.width + 6
            y: nameText.y
            color: analyzeRect.color


            TextInput {
                id: songName
                selectionColor: "#acc3de"
                horizontalAlignment: Text.AlignLeft
                font.pixelSize: nameText.font.pixelSize

                Keys.onReturnPressed:{

                    songName.focus = false
                    Genre_Finder.set_Name(songName.text)
                }
                Keys.onEnterPressed:{
                    songName.focus = false
                    Genre_Finder.set_Name(songName.text)
                }

                MouseArea {
                    anchors.fill: parent
                    onDoubleClicked: {

                        if(!subgenre.visible){
                            subgenreSelect.enabled = false
                            subgenreSelect.visible = false
                            subgenreConfirm.enabled = false
                            subgenreConfirm.visible = false
                            subgenre.visible = true
                        }
                        songName.selectAll()
                        songName.focus = true
                    }
                }
            }
        }

        Text {
            id: songLen
            x: nameText.x
            y: nameText.y + analyzeWindow.height * (50/350)
            font.pixelSize: 13
        }


        Text {
            id: peakText
            x: nameText.x
            y: nameText.y + analyzeWindow.height * (100/350)
            text: "Peak Tempo:"
            font.underline: true
            font.pixelSize: 13
        }
        Rectangle {
            id: peakArea
            width: peakTempo.width
            height: peakTempo.height
            x: peakText.x + peakText.width + 6
            y: peakText.y
            color: analyzeRect.color


            TextInput {
                id: peakTempo
                selectionColor: "#acc3de"
                horizontalAlignment: Text.AlignLeft
                font.pixelSize: nameText.font.pixelSize
                validator: IntValidator{bottom: 0; top: Genre_Finder.get_MaxTempo()}


                Keys.onReturnPressed:{
                    peakTempo.focus = false
                    if(peakTempo.text == "") peakTempo.text = "0"
                    Genre_Finder.set_PeakTempo(peakTempo.text)
                }
                Keys.onEnterPressed: {
                    peakTempo.focus = false
                    if(peakTempo.text == "") peakTempo.text = "0"
                    Genre_Finder.set_PeakTempo(peakTempo.text)
                }

                MouseArea {
                    anchors.fill: parent
                    onDoubleClicked: {
                        if(!subgenre.visible){
                            subgenreSelect.enabled = false
                            subgenreSelect.visible = false
                            subgenreConfirm.enabled = false
                            subgenreConfirm.visible = false
                            subgenre.visible = true
                        }
                        peakTempo.selectAll()
                        peakTempo.focus = true
                    }
                }
            }
        }

        Text {
            id: acText
            x: nameText.x
            y: nameText.y + analyzeWindow.height * (150/350)
            text: "Autocorrection Tempo:"
            font.underline: true
            font.pixelSize: 13
        }
        Rectangle {
            id: acArea
            width: acTempo.width
            height: acTempo.height
            x: acText.x + acText.width + 6
            y: acText.y
            color: analyzeRect.color


            TextInput {
                id: acTempo
                selectionColor: "#acc3de"
                horizontalAlignment: Text.AlignLeft
                font.pixelSize: nameText.font.pixelSize
                validator: IntValidator{bottom: 0; top: Genre_Finder.get_MaxTempo()}

                Keys.onReturnPressed:{
                    acTempo.focus = false
                    if(acTempo.text == "") acTempo.text = "0"
                    Genre_Finder.set_acTempo(acTempo.text)
                }
                Keys.onEnterPressed: {
                    acTempo.focus = false
                    if(acTempo.text == "") acTempo.text = "0"
                    Genre_Finder.set_acTempo(acTempo.text)
                }

                MouseArea {
                    anchors.fill: parent
                    onDoubleClicked: {
                        if(!subgenre.visible){
                            subgenreSelect.enabled = false
                            subgenreSelect.visible = false
                            subgenreConfirm.enabled = false
                            subgenreConfirm.visible = false
                            subgenre.visible = true
                        }
                        acTempo.selectAll()
                        acTempo.focus = true
                    }
                }
            }
        }

        ComboBox {
            id: subgenreSelect
            x: subgenre.x
            y: subgenre.y - 4
            enabled: false
            visible: false
            height: subgenre.height + 9

            model:ListModel{
                        id: subgenreSelectModel
                    }
                    property int counter: 0
                    property string str: ""

                    function showsubgenres(){
                        subgenreSelectModel.clear()

                        while(1){
                            str = Genre_Finder.show_subgenres(counter)
                            if(str == "") break
                            subgenreSelectModel.append({"text":str})
                            counter++
                        }
                        counter = 0
                    }



        }
        Text {
            id: subgenreText
            x: nameText.x
            y: nameText.y + analyzeWindow.height * (200/350)
            text: "Subgenre:"
            font.pixelSize: 13
            font.underline: true
        }

        Text {
            id: subgenre
            x: subgenreText.x + subgenreText.width + 6
            y: subgenreText.y
            font.pixelSize: 13

            MouseArea {
                anchors.fill: parent
                onDoubleClicked: {
                    if(songName.focus){
                        songName.focus = false
                        Genre_Finder.set_Name(songName.text)
                    }
                    if(peakTempo.focus){
                        peakTempo.focus = false
                        if(peakTempo.text == "") peakTempo.text = "0"
                        Genre_Finder.set_PeakTempo(peakTempo.text)
                    }
                    if(acTempo.focus){
                        acTempo.focus = false
                        if(acTempo.text == "") acTempo.text = "0"
                        Genre_Finder.set_acTempo(acTempo.text)
                    }

                    subgenreSelect.showsubgenres()
                    subgenreSelect.currentIndex = subgenreSelect.find(subgenre.text)
                    subgenreSelect.enabled = true
                    subgenreSelect.visible = true
                    subgenreConfirm.enabled = true
                    subgenreConfirm.visible = true
                    subgenre.visible = false

                }
            }
        }
        Button {
            id: subgenreConfirm
            x: subgenreSelect.x + subgenreSelect.width + 3
            y: subgenreSelect.y - 1
            enabled: false
            visible: false
            height: subgenreSelect.height + 2
            width: 25
            text: "✔"

            onClicked: {
                Genre_Finder.set_subgenre(subgenreSelect.currentText)
                subgenre.text = subgenreSelect.currentText
                subgenreSelect.enabled = false
                subgenreSelect.visible = false
                subgenreConfirm.enabled = false
                subgenreConfirm.visible = false
                subgenre.visible = true

            }
        }
    }

    Text {
        id: debug
        x: 37
        y: 237
        font.pixelSize: 12
    }

    Text {
        id: genreLabel
        x: 83
        y: 74
        text: "Genre:"
        font.pixelSize: 12
    }

    Rectangle {
        id: status
        visible: true
        opacity: 0
        x: 0
        y: 0
        width: 550
        height: 30
        anchors.horizontalCenterOffset: 175
        anchors.leftMargin: 0
        anchors.topMargin: 0
        anchors.left: parent.left
        anchors.horizontalCenter: genreLabel.horizontalCenter
        anchors.top: parent.top


        MouseArea {
            anchors.fill: parent
            onClicked: {
                status.opacity = 0
                statusTimer.running = false
            }
        }
        Text {
            id: statusMsg
            anchors.centerIn: parent
            font.pixelSize: 14
        }
        Timer{
            id: statusTimer
            interval: 80
            onTriggered: {
                if(status.opacity == 0)statusTimer.repeat = false
                else {
                    status.opacity = status.opacity - 0.02
                    statusTimer.running = true
                }

            }

        }
    }

    Rectangle {
        id: testAnalyzeWindow
        x: -55
        y: 355
        width: 675
        height: 350
        color: "#ffffff"



    }



}
