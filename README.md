To compile the game run "make" command in your terminal , then start the server by running "./Server" in your terminal (terminal must be opened in the folder that contains the app) and to start playing run "./Client" (same condition) . 
If you want to play on multiple devices (only in the same network) , open "Definitions.h" and replace the #define for the IP adress with the adress of the device where the server will be running , the recompile using make .
Requirements to compile the project :
- sqlite3
- sfml
- g++