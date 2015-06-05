# Subgenre Analyzer
An artificial intelligence project to determine the subgenre of a song given a general genre.

This is the GUI version of the project using Qt. To view the command line version, view the [CMD](https://github.com/ChrisTheEngineer/Subgenre_Analyzer/tree/CMD) branch.

#Project Description
Project originally created using Qt Creator 3.4.1 based on Qt 5.4.2 on Windows 8.1

**Initial Program:**

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="/Documents/Images/Initial Program.PNG?raw=true" alt="Initial Program">

The user can select a genre from the avaliable [genres](/Subgenre_Analyzer/Genre_Finder_GUI/Genres). Additoanlly, the user can load audio from their computer. The 'Load Audio' button will open a prompt to select a song. The user must load audio and select a genre and click the 'Update Genre Directory' before they can select the 'Analyze' option.

**Analyze Audio:**

<img src="/Documents/Images/Analyze Audio.PNG?raw=true" alt="Analyze Audio">

Once the user selects 'Analyze', the audio's infromation will be displayed in a seperate information. This includes the name, song length, peak and autocorrection tempo, and the subgenre the program predicted. There is a visual representation of the feature vector, which shows all previous analyzed songs including the tempo ranges of each subgenre.

**Editing and Zoom:**

<img src="/Documents/Images/Edit and Zoom.png?raw=true" alt="Edit and Zoom">

The user can edit all parts of the song data excluding the song length by double clicking on the value of any piece of information. Additionally the user can zoom in and out of the visual feature vector. When the user is zoomed in, the image can be moved around.
