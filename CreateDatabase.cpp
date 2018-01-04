#include <iostream>
#include "Database.h"
using namespace std;
int main()
{
	Database *database;
	database = new Database("Database.sqlite");
	database->query("DROP TABLE Questions;");
	database->query("CREATE TABLE Questions (\
					Question VARCHAR(100),\
					Answer1 VARCHAR(100),\
					Answer2 VARCHAR(100),\
					Answer3 VARCHAR(100),\
					Answer4 VARCHAR(100),\
					Category INTEGER);");
	database->query("INSERT INTO Questions VALUES(\
		'What is the name of McDonald`s famous clown ?',\
		'Ronald','Big Mac','Krusty','Donald',1);");
	database->query("INSERT INTO Questions VALUES(\
		'Foundue is usually cheese or chocolate served how ?',\
		'Melted','Frozen','A la mode','Overpriced',1);");
	database->query("INSERT INTO Questions VALUES(\
		'What is the essential ingredient of \"sauerkraut\" ?',\
		'Cabbage','Noodles','Onion','Potato',1);");
	database->query("INSERT INTO Questions VALUES(\
		'In which Disney film is Cruella de Vil encountered ?',\
		'101 Dalmations','Lion King','Snow White','The Aristocats',1);");
	database->query("INSERT INTO Questions VALUES(\
		'What is the name of the video game character who ate ghosts and dots ?',\
		'Pac-Man','Lara Croft','Super Mario','Crash Banicoot',1);");
	database->query("INSERT INTO Questions VALUES(\
		'In the \"Road Runner and Coyote\" cartoons, what famous sound does the Road Runner make ?',\
		'Beep ! Beep!','Ping! Ping!','Vroom! Vroom!','Aooga! Aooga!',1);");
	database->query("INSERT INTO Questions VALUES(\
		'Where should choking victims place their hands to indicate to others that they need help ?',\
		'Around the throat','On the knees','Over the eyes','On the lips',1);");
	database->query("INSERT INTO Questions VALUES(\
		'Which of these dance names is used to describe a fashionable dot ?',\
		'Hora','Swing','Lambada','Polka',1);");
	database->query("INSERT INTO Questions VALUES(\
		'What did Jack and Jill go up on the hill to fetch in the nursery rhyme ?',\
		'Pail of Water','Ice Cream','Baseball','Dog Bone',1);");
	database->query("INSERT INTO Questions VALUES(\
		'A task that is a low priority is usually put on what ?',\
		'Back Burner','Couch','Top Bunker','Staircase',1);");//10 questions
	database->query("INSERT INTO Questions VALUES(\
		'At which restaurant would you be able to order Chicken McNuggets ?',\
		'McDonald`s','Burger King','Wendy`s','Taco Bell',1);");
	database->query("INSERT INTO Questions VALUES(\
		'FBI agents Fox Mulder and Dana Scully are characters on what television show ?',\
		'The X-Files','24','Lost','Law & Order',1);");
	database->query("INSERT INTO Questions VALUES(\
		'Who wrote \"The Hobbit\" , and its sequel \"The lord of the rings\" ?',\
		'J.R.R. Tolkien','Frank Herbert','J.D. Salinger','J.K. Rowling',1);");
	database->query("INSERT INTO Questions VALUES(\
		'What unit of time, when used as an adjective, refers to something that is tiny ?',\
		'Minute','Second','Hour','Milisecond',1);");
	database->query("INSERT INTO Questions VALUES(\
		'Which artist recorded the 1982 album \"Thriller\" ?',\
		'Michael Jackson','George Michael','Madonna','Mariah Carey',1);");
	database->query("INSERT INTO Questions VALUES(\
		'At which event would an artist win an Album of the Year award ?',\
		'Grammy Awards','Emmy Awards','Golden Globe Awards','Academy Awards',1);");
	database->query("INSERT INTO Questions VALUES(\
		'At which sporting event might you witness a \"slam dunk\" ?',\
		'Basketball','Tennis','Boxing','Baseball',1);");
	database->query("INSERT INTO Questions VALUES(\
		'Kobe Bryant plays which sport professionally ?',\
		'Basketball','Soccer','Golf','Baseball',1);");
	database->query("INSERT INTO Questions VALUES(\
		'Which is the most important piece in the game of chess ?',\
		'King','Queen','Rook','Bishop',1);");
	database->query("INSERT INTO Questions VALUES(\
		'Bacon is cuts of meat taken from what animal ?',\
		'Pig','Cow','Deer','Rabbit',1);");//20 questions
	database->query("INSERT INTO Questions VALUES(\
		'Which soft drink occasionally features polar bears in advertisments ?',\
		'Coca-Cola','Sprite','Dr.Pepper','Mountain Dew',1);");
	database->query("INSERT INTO Questions VALUES(\
		'What \"Friends\" star mariied and divorced Brad Pitt ?',\
		'Jennifer Anniston','Reese Witherspoon','Courtney Cox','Lisa Kudrow',1);");
	database->query("INSERT INTO Questions VALUES(\
		'Which boxer had his ears bitten by Mike Tyson ?',\
		'Evander Holyfield','roy Jones Jr.','Lennox Lewis','Buster Douglas',1);");
	database->query("INSERT INTO Questions VALUES(\
		'What kind of factory does Willy Wonka own ?',\
		'Chocolate','Toy','Ice Cream','Software',1);");
	database->query("INSERT INTO Questions VALUES(\
		'The fictional character Chewbacca appears in which films ?',\
		'Star Wars','Harry Potter','Lord of the Rings','Star Trek',1);");
	database->query("INSERT INTO Questions VALUES(\
		'What is the name of Dorothy`s dog in the \"The Wizard of Oz\" ?',\
		'Toto','Benji','Rin Tin Tin','Lassie',1);");
	database->query("INSERT INTO Questions VALUES(\
		'What subatomic particle carries a negative electric charge ?',\
		'Electron','Neutron','Isotope','Proton',1);");
	database->query("INSERT INTO Questions VALUES(\
		'What is Batman`s real name ?',\
		'Bruce Wayne','Peter Parker','Clark Kent','David Bowie',1);");
	database->query("INSERT INTO Questions VALUES(\
		'Of the following superheroes who does not wear a cape ?',\
		'Spider-Man','Batman','Robin','Superman',1);");
	database->query("INSERT INTO Questions VALUES(\
		'Which of the followwing is not a root vegetable ?',\
		'Zucchini','Carrot','Radish','',1);");//30 questions
	database->close();
	return 0;
}
