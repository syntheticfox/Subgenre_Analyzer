# Subgenre Analyzer
An artificial intelligence project to determine the subgenre of a song given a genre.

This is the command line version of the project. To see the project with a GUI created using QT, view the [GUI](https://github.com/ChrisTheEngineer/Subgenre_Analyzer/tree/GUI) branch.

#Project Description
Project originally created using  Visual Studio 2015 RC on Windows 8.1

The following project has a user select a specific genre from a database of genres. Next, the user will be prompted to select a song to analyze. Once selected, the program will try to predict the subgenre of the song by analyzing the tempo of the song two different ways and using clustering. The user can view all the data of the particular song and edit said data. Once the program closes out, it will update the subgenre database to help make better prediticions in the future.

To add a new genre, the user should create a text file in the [Genres](https://github.com/ChrisTheEngineer/Subgenre_Analyzer/tree/CMD/Genre Finder/Genre Finder/Genres) folder. The file name should be the name of the genre. Below is an example of how each subgenre should be written:
```
[Subgenre name], [minimum tempo], [maximum tempo]
```

Files containing a more in-depth description and details of the project are located in the [Documents](https://github.com/ChrisTheEngineer/Subgenre_Analyzer/tree/CMD/Documents) section.
