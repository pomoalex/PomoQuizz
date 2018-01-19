#include "Definitions.h"
#include <SFML/Graphics.hpp>
#define TITLE "PomoQuizz"
#define BACKGROUND "background.jpg"
#define ALTERNATIVE_FONT "FFF_Tusj.ttf"
#define FONT "OpenSans-Regular.ttf"
#define LOGO "PomoQuizz"

enum class STATE
{
	INTRO,
	LOGIN,
	CONNECTING,
	WAITING,
	PLAYING
};
STATE game_state;
mutex locker;
sf::RenderWindow window;
sf::Sprite background;
sf::Font font;
sf::Texture temp_texture;
sf::Color color = sf::Color::Red;
int WindowWidth, WindowHeight;
string username;
extern int errno;
int client_sd;

void InitWindow();

void Intro();

void DrawIntro();

void ManageLogin();

void ConnectionAttempt();

void DrawConnection(int &option);

void PomoQuizz();

void PrepareBoxes(sf::RectangleShape box[]);

void CenterInBox(sf::Text &text, sf::RectangleShape box);

void AlignLeftInBox(sf::Text &text, sf::RectangleShape box);

void CenterInBox2(sf::Text &text, sf::Text &aux, sf::RectangleShape box, bool &double_lined);

void PrepareText(sf::Text &text, sf::Font &font, sf::Color color, int size, sf::Text::Style style = sf::Text::Regular);

void GetQuestion(sf::Text &question, sf::Text answers[4]);

void GetWinner(int client_sd, string &winner, bool &done, bool &success);

void DisplayMessage(string message, float seconds, bool effect = false);

int main()
{
	InitWindow();
	game_state = STATE::LOGIN;
	while (window.isOpen())
	{
		switch (game_state)
		{
		case STATE::INTRO:
			Intro();
			break;
		case STATE::LOGIN:
			ManageLogin();
			break;
		case STATE::CONNECTING:
			ConnectionAttempt();
			break;
		case STATE::PLAYING:
			PomoQuizz();
			break;
		}
	}
}

void InitWindow()
{
	game_state = STATE::INTRO;
	sf::VideoMode DesktopMode = sf::VideoMode::getDesktopMode();
	WindowHeight = DesktopMode.height * 0.6;
	WindowWidth = DesktopMode.width * 0.6;
	window.create(sf::VideoMode(WindowWidth, WindowHeight), TITLE, sf::Style::Titlebar | sf::Style::Close);
	window.setVerticalSyncEnabled(true);
	window.setPosition(sf::Vector2i((DesktopMode.width - WindowWidth) / 2, (DesktopMode.height - WindowHeight) / 2));
	if (!temp_texture.loadFromFile(BACKGROUND))
	{
		perror("Cannot load background");
		exit(errno);
	}
	background.setTexture(temp_texture);
	background.setScale(WindowWidth / background.getLocalBounds().width, WindowHeight / background.getLocalBounds().height);
	if (!font.loadFromFile(FONT))
	{
		perror("Cannot load font !");
		exit(errno);
	}
}

void Intro()
{
	sf::Event event;
	thread execute(DrawIntro);
	execute.detach();
	while (window.isOpen() && game_state == STATE::INTRO)
	{
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			}
		}
	}
}

void DrawIntro()
{
	sf::Text text, name;
	sf::Color new_color;
	string name_str = "by Pomo Alex";
	text.setString("PomoQuizz");
	PrepareText(text, font, color, WindowHeight * 0.25, sf::Text::Bold);
	PrepareText(name, font, color, WindowHeight * 0.08);
	sf::RectangleShape box1, box2;
	box1.setSize(sf::Vector2f(WindowWidth, WindowHeight * 0.85));
	box1.setPosition(0, 0);
	CenterInBox(text, box1);
	box2.setSize(sf::Vector2f(WindowWidth * 0.3, WindowHeight * 0.2));
	box2.setPosition(WindowWidth / 2, text.getPosition().y + text.getCharacterSize());
	AlignLeftInBox(name, box2);
	sf::Event event;
	int length = 0;
	int r = 100, g = 100, b = 100;
	new_color.r = r;
	new_color.g = g;
	new_color.b = b;
	int step = 3;
	int color_type = 1;
	bool add_to_color = true;
	auto start = chrono::high_resolution_clock::now();
	while (window.isOpen() && passed_time(start) <= 3)
	{
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			}
		}
		switch (color_type)
		{
		case 1:
			if ((r + step >= 255 && add_to_color) || (r - step <= 0) && !add_to_color)
				add_to_color = !add_to_color;
			if (add_to_color)
				r += step;
			else
				r -= step;
			new_color.r = r;
			if ((r + step >= 255 && add_to_color) || (r - step <= 0) && !add_to_color)
			{
				color_type++;
				add_to_color = !add_to_color;
			}
			break;
		case 2:
			if ((g + step >= 255 && add_to_color) || (g - step <= 1) && !add_to_color)
				add_to_color = !add_to_color;
			if (add_to_color)
				g += step;
			else
				g -= step;
			new_color.g = g;
			if ((g + step >= 255 && add_to_color) || (g - step <= 1) && !add_to_color)
			{
				color_type++;
				add_to_color = !add_to_color;
			}
			break;
		case 3:
			if ((b + step >= 255 && add_to_color) || (b - step <= 1) && !add_to_color)
				add_to_color = !add_to_color;
			if (add_to_color)
				b += step;
			else
				b -= step;
			new_color.b = b;
			if ((b + step >= 255 && add_to_color) || (b - step <= 1) && !add_to_color)
			{
				color_type = 1;
				add_to_color = !add_to_color;
			}
			break;
		}
		text.setFillColor(new_color);
		name.setFillColor(new_color);
		if (length < name_str.length() && passed_time(start) > 0.6)
		{
			start = chrono::high_resolution_clock::now();
			length++;
			if (name_str[length] == ' ')
				length++;
			name.setString(name_str.substr(0, length));
		}
		window.clear();
		window.draw(background);
		window.draw(text);
		window.draw(name);
		window.display();
	}
	game_state = STATE::LOGIN;
}

void ManageLogin()
{
	sf::Text login_text;
	username = "username";
	login_text.setString("Login");
	PrepareText(login_text, font, color, WindowHeight * 0.15, sf::Text::Bold);
	login_text.setPosition((WindowWidth - login_text.getGlobalBounds().width) / 2,
						   (WindowHeight - login_text.getGlobalBounds().height) * 0.3);
	sf::Text username_text;
	username_text.setString("ppppdddd");
	PrepareText(username_text, font, color, WindowHeight * 0.08);
	int usr_text_height = (login_text.getPosition().y + login_text.getGlobalBounds().height +
						   (WindowHeight - login_text.getGlobalBounds().height - login_text.getPosition().y -
							username_text.getGlobalBounds().height) *
							   0.25);
	username_text.setPosition((WindowWidth - username_text.getGlobalBounds().width) / 2, usr_text_height);
	sf::RectangleShape textbox;
	textbox.setSize(sf::Vector2f(WindowWidth * 0.4, username_text.getGlobalBounds().height * 1.2));
	textbox.setFillColor(sf::Color::Transparent);
	textbox.setOutlineThickness(2);
	textbox.setPosition((WindowWidth - textbox.getGlobalBounds().width) / 2,
						username_text.getPosition().y - (username_text.getGlobalBounds().height - textbox.getGlobalBounds().height) / 4);
	username_text.setString("username");
	sf::Event event;
	while (window.isOpen() && game_state == STATE::LOGIN)
	{
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::TextEntered:
				if (event.text.unicode == 8)
				{
					if (username.length() > 0 && username != "username")
					{
						username = username.substr(0, username.length() - 1);
						if (username.length() == 0)
							username += "username";
					}
				}
				else if (event.text.unicode > 34 && event.text.unicode < 127)
				{
					if (username == "username")
						username = "\0";
					username += static_cast<char>(event.text.unicode);
				}
				else if (event.text.unicode == 13 && username != "username")
					game_state = STATE::CONNECTING;
				username_text.setString(username);
				username_text.setPosition((WindowWidth - username_text.getGlobalBounds().width) / 2, usr_text_height);
				if (textbox.getGlobalBounds().width * 0.9 <= username_text.getGlobalBounds().width)
				{
					username = username.substr(0, username.length() - 1);
					username_text.setString(username);
					username_text.setPosition((WindowWidth - username_text.getGlobalBounds().width) / 2, usr_text_height);
				}
			}
		}
		window.clear();
		window.draw(background);
		window.draw(login_text);
		window.draw(textbox);
		window.draw(username_text);
		window.display();
	}
	game_state = STATE::CONNECTING;
}

void ConnectionAttempt()
{
	int option = 0;
	thread execute(DrawConnection, ref(option));
	struct sockaddr_in server;
	if ((client_sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("error at socket().\n");
		exit(errno);
	}
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(SERVER_IP);
	server.sin_port = htons(PORT);
	if (connect(client_sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
		option = 2;
	else
	{
		char message[256];
		sprintf(message, "%s\n", (char *)username.c_str());
		send_to(client_sd, message);
		int auxiliary;
		if (!recv_from(client_sd, message))
			option = 2;
		else
		{
			printf("%s from server\n", message);
			auxiliary = atoi(message);
			if (auxiliary < 0)
				option = 2;
			else
				option = auxiliary;
		}
	}
	if (option == 2)
	{
		shutdown(client_sd, SHUT_RDWR);
		close(client_sd);
	}
	execute.join();
}

void DrawConnection(int &option)
{
	sf::Text text;
	string text_str = "Connecting to server ";
	int text_len = text_str.length();
	int init_option = 0;
	text.setString(text_str);
	PrepareText(text, font, color, WindowHeight * 0.1, sf::Text::Bold);
	text.setPosition((WindowWidth - text.getGlobalBounds().width) / 2, (WindowHeight - text.getGlobalBounds().height) * 0.5);
	sf::Event event;
	bool conn_started = false;
	chrono::high_resolution_clock::time_point conn_start;
	float seconds = 0.5;
	float wait_time = 2.5;
	int remaining_seconds;
	auto start = chrono::high_resolution_clock::now();
	auto abs_start = chrono::high_resolution_clock::now();
	while (window.isOpen() && (game_state == STATE::CONNECTING || option == 0 || passed_time(abs_start) < wait_time))
	{
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			}
		}
		if (init_option != option)
			if (passed_time(abs_start) > wait_time)
			{
				init_option = option;
				abs_start = chrono::high_resolution_clock::now();
			}
		if (init_option == 2 && !conn_started)
		{
			DisplayMessage("Connection failed !", 3);
			game_state = STATE::LOGIN;
			break;
		}
		if (init_option == 0)
		{
			if (passed_time(start) > seconds)
			{
				start = chrono::high_resolution_clock::now();
				if (text_str.length() < text_len + 3)
					text_str += '.';
				else
					text_str = text_str.substr(0, text_str.length() - 3);
				text.setString(text_str);
			}
		}
		if (option > 2 && !conn_started)
		{
			conn_start = chrono::high_resolution_clock::now();
			remaining_seconds = option;
			conn_started = true;
		}
		if (init_option > 2)
		{
			int temp_var = floor(option - passed_time(conn_start));
			if ((remaining_seconds != temp_var) && temp_var >= 0)
			{
				remaining_seconds = temp_var;
				char auxiliary[256];
				sprintf(auxiliary, "game starts in %d seconds", remaining_seconds);
				text.setString(auxiliary);
				text.setPosition((WindowWidth - text.getGlobalBounds().width) / 2, (WindowHeight - text.getGlobalBounds().height) * 0.5);
			}
			if (temp_var < 0)
				game_state = STATE::PLAYING;
		}
		window.clear();
		window.draw(background);
		window.draw(text);
		window.display();
	}
}

void PomoQuizz()
{
	int question_nr = 0;
	bool answering = false;
	bool answered;
	int given_answer = 0;
	bool double_lined;
	int cursor_in_box = 0;
	int score_val = 0;
	bool answered_to_all = false;
	sf::Text question, answers[4], aux, timer, hovered_answer, score;
	sf::RectangleShape box[8];
	PrepareBoxes(box);
	PrepareText(question, font, color, WindowHeight * 0.06, sf::Text::Bold);
	PrepareText(aux, font, color, WindowHeight * 0.06, sf::Text::Bold);
	PrepareText(timer, font, color, WindowHeight * 0.1, sf::Text::Bold);
	PrepareText(score, font, color, WindowHeight * 0.07);
	PrepareText(hovered_answer, font, sf::Color::Magenta, WindowHeight * 0.05);
	for (int index = 0; index < 4; index++)
		PrepareText(answers[index], font, color, WindowHeight * 0.05);
	sf::Event event;
	sf::Vector2i cursor_pos;

	auto start = chrono::high_resolution_clock::now();
	while (window.isOpen() && game_state == STATE::PLAYING)
	{
		cursor_pos = sf::Mouse::getPosition(window);
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::MouseButtonReleased:
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					for (int index = 1; index <= 4; index++)
					{
						if (box[index].getGlobalBounds().contains(static_cast<sf::Vector2f>(cursor_pos)))
						{
							given_answer = index;
							answered = true;
						}
					}
				}
				break;
			}
		}
		if (question_nr < 10 && !answering)
		{
			start = chrono::high_resolution_clock::now();
			question_nr++;
			answering = true;
			answered = false;
			given_answer = 0;
			GetQuestion(question, answers);
			CenterInBox2(question, aux, box[0], double_lined);
			CenterInBox(answers[0], box[1]);
			CenterInBox(answers[1], box[2]);
			CenterInBox(answers[2], box[3]);
			CenterInBox(answers[3], box[4]);
		}
		if (passed_time(start) >= TIME_PER_ANSWER)
			answered = true;
		if (answered)
		{
			send_to(client_sd, &given_answer, (int)sizeof(int));
			int result;
			if (!read_from(client_sd, &result, (int)sizeof(int)))
			{
				DisplayMessage("Connection failed !", 2);
				shutdown(client_sd, SHUT_RDWR);
				close(client_sd);
				game_state = STATE::LOGIN;
				break;
			}
			if (given_answer == 0)
				DisplayMessage("Out of time !", 1, true);
			else
			{
				if (result == 1)
				{
					DisplayMessage("Correct !", 1, true);
					score_val += 100;
				}
				else
					DisplayMessage("Wrong !", 1, true);
			}
			answering = false;
			if (question_nr == 10)
			{
				answered_to_all = true;
				break;
			}
			else
				continue;
		}
		char temp[100];
		sprintf(temp, "%d", (int)(TIME_PER_ANSWER - passed_time(start)));
		timer.setString(temp);
		CenterInBox(timer, box[5]);
		sprintf(temp, "Score : %d", score_val);
		score.setString(temp);
		CenterInBox(score, box[7]);
		bool cursor_in_any_box = false;
		for (int index = 1; index <= 4; index++)
		{
			if (box[index].getGlobalBounds().contains(static_cast<sf::Vector2f>(cursor_pos)))
			{
				cursor_in_any_box = true;
				hovered_answer.setString(answers[index - 1].getString());
				if (cursor_in_box == index)
				{
					if (hovered_answer.getCharacterSize() < box[index].getGlobalBounds().height * 0.35)
						hovered_answer.setCharacterSize(hovered_answer.getCharacterSize() + 2);
				}
				else
					hovered_answer.setCharacterSize(answers[0].getCharacterSize());
				CenterInBox(hovered_answer, box[index]);
				cursor_in_box = index;
			}
		}
		if (!cursor_in_any_box)
			cursor_in_box = 0;
		window.clear();
		window.draw(background);
		window.draw(timer);
		window.draw(score);
		window.draw(box[0]);
		window.draw(question);
		if (double_lined)
			window.draw(aux);
		for (int index = 1; index <= 4; index++)
		{
			if (index == cursor_in_box)
				window.draw(hovered_answer);
			else
				window.draw(answers[index - 1]);
			window.draw(box[index]);
		}
		window.draw(box[5]);
		window.draw(box[6]);
		window.draw(box[7]);
		window.display();
	}
	if (answered_to_all)
	{
		bool done = false;
		bool success = true;
		string winner;
		thread execute(GetWinner, client_sd, ref(winner), ref(done), ref(success));
		execute.detach();
		sf::sleep(sf::milliseconds(100));
		while (!done)
			DisplayMessage("Waiting for all players to finish", 0.5);
		if (success)
		{
			if (winner == username)
				winner = "You";
			winner = winner + " won !";
			DisplayMessage(winner, 5, true);
		}
	}
	game_state = STATE::LOGIN;
}

void GetWinner(int client_sd, string &winner, bool &done, bool &success)
{
	char temp[100];
	if (!recv_from(client_sd, temp))
	{
		DisplayMessage("Connection failed !", 2);
		shutdown(client_sd, SHUT_RDWR);
		close(client_sd);
		done = true;
		success = false;
	}
	else
	{
		winner = temp;
		done = true;
	}
}

void PrepareBoxes(sf::RectangleShape box[])
{
	int thickness = 2;
	int indent = WindowWidth * 0.02;
	//question
	box[0].setSize(sf::Vector2f(WindowWidth - 2 * indent, WindowHeight * 0.35 - 2 * indent));
	box[0].setOutlineColor(sf::Color::White);
	box[0].setOutlineThickness(thickness);
	box[0].setFillColor(sf::Color::Transparent);
	box[0].setPosition(indent, WindowHeight * 0.15 + indent);
	//answer1
	box[1].setSize(sf::Vector2f(WindowWidth / 2 - 1.5 * indent, WindowHeight / 4 - 1.5 * indent));
	box[1].setOutlineColor(sf::Color::White);
	box[1].setOutlineThickness(thickness);
	box[1].setFillColor(sf::Color::Transparent);
	box[1].setPosition(indent, WindowHeight / 2 + indent);
	//answer2
	box[2].setSize(sf::Vector2f(WindowWidth / 2 - 1.5 * indent, WindowHeight / 4 - 1.5 * indent));
	box[2].setOutlineColor(sf::Color::White);
	box[2].setOutlineThickness(thickness);
	box[2].setFillColor(sf::Color::Transparent);
	box[2].setPosition(WindowWidth / 2 + indent / 2, WindowHeight / 2 + indent);
	//answer3
	box[3].setSize(sf::Vector2f(WindowWidth / 2 - 1.5 * indent, WindowHeight / 4 - 1.5 * indent));
	box[3].setOutlineColor(sf::Color::White);
	box[3].setOutlineThickness(thickness);
	box[3].setFillColor(sf::Color::Transparent);
	box[3].setPosition(indent, WindowHeight * 0.75 + indent / 2);
	//answer4
	box[4].setSize(sf::Vector2f(WindowWidth / 2 - 1.5 * indent, WindowHeight / 4 - 1.5 * indent));
	box[4].setOutlineColor(sf::Color::White);
	box[4].setOutlineThickness(thickness);
	box[4].setFillColor(sf::Color::Transparent);
	box[4].setPosition(WindowWidth / 2 + indent / 2, WindowHeight * 0.75 + indent / 2);
	//timer
	box[5].setSize(sf::Vector2f(WindowWidth * 0.2, WindowHeight * 0.15 - indent));
	box[5].setOutlineColor(sf::Color::White);
	box[5].setOutlineThickness(thickness);
	box[5].setFillColor(sf::Color::Transparent);
	box[5].setPosition((WindowWidth - box[5].getGlobalBounds().width) / 2, indent);
	//username
	box[6].setSize(sf::Vector2f(WindowWidth * 0.4 - 2 * indent, WindowHeight * 0.15 - indent));
	box[6].setOutlineColor(sf::Color::White);
	box[6].setOutlineThickness(thickness);
	box[6].setFillColor(sf::Color::Transparent);
	box[6].setPosition(indent, indent);
	//score
	box[7].setSize(sf::Vector2f(WindowWidth * 0.4 - 2 * indent, WindowHeight * 0.15 - indent));
	box[7].setOutlineColor(sf::Color::White);
	box[7].setOutlineThickness(thickness);
	box[7].setFillColor(sf::Color::Transparent);
	box[7].setPosition(box[5].getPosition().x + box[5].getGlobalBounds().width + indent, indent);
}

void CenterInBox(sf::Text &text, sf::RectangleShape box)
{
	text.setPosition(box.getPosition().x + (box.getGlobalBounds().width - text.getGlobalBounds().width) / 2,
					 box.getPosition().y + (box.getGlobalBounds().height - text.getCharacterSize()) / 2);
}

void AlignLeftInBox(sf::Text &text, sf::RectangleShape box)
{
	text.setPosition(box.getPosition().x,
					 box.getPosition().y + (box.getGlobalBounds().height - text.getCharacterSize()) / 2);
}

void CenterInBox2(sf::Text &text, sf::Text &aux, sf::RectangleShape box, bool &double_lined)
{
	double_lined = false;
	CenterInBox(text, box);
	if (text.getGlobalBounds().width > 0.9 * box.getGlobalBounds().width)
	{
		double_lined = true;
		float percentage = (box.getGlobalBounds().width * 0.9) / text.getGlobalBounds().width;
		string substr1, substr2, full_str;
		full_str = text.getString();
		int poz = full_str.length() * percentage;
		while (full_str[poz] != ' ')
			poz--;
		substr1 = full_str.substr(0, poz);
		substr2 = full_str.substr(poz, full_str.length() - 1);
		text.setString(substr1);
		aux.setString(substr2);
		sf::RectangleShape box1, box2;
		box1.setSize(sf::Vector2f(box.getGlobalBounds().width, box.getGlobalBounds().height / 2));
		box1.setPosition(box.getPosition().x, box.getPosition().y);
		box2.setSize(sf::Vector2f(box.getGlobalBounds().width, box.getGlobalBounds().height / 2));
		box2.setPosition(box.getPosition().x, box.getPosition().y + box.getGlobalBounds().height / 2);
		CenterInBox(text, box1);
		CenterInBox(aux, box2);
	}
}

void PrepareText(sf::Text &text, sf::Font &font, sf::Color color, int size, sf::Text::Style style)
{
	text.setFillColor(color);
	text.setFont(font);
	text.setStyle(style);
	text.setCharacterSize(size);
}

void GetQuestion(sf::Text &question, sf::Text answers[4])
{
	char temp[300];
	if (!recv_from(client_sd, temp))
	{
		DisplayMessage("Connection failed !", 2);
		shutdown(client_sd, SHUT_RDWR);
		close(client_sd);
		game_state = STATE::LOGIN;
	}
	else
	{
		char *token;
		token = strtok(temp, "\n");
		question.setString(token);
		token = strtok(NULL, "\n");
		answers[0].setString(token);
		token = strtok(NULL, "\n");
		answers[1].setString(token);
		token = strtok(NULL, "\n");
		answers[2].setString(token);
		token = strtok(NULL, "\n");
		answers[3].setString(token);
	}
}

void DisplayMessage(string message, float seconds, bool effect)
{
	sf::Text text;
	text.setString(message);
	if (effect)
		PrepareText(text, font, color, WindowHeight * 0.01, sf::Text::Bold);
	else
		PrepareText(text, font, color, WindowHeight * 0.1, sf::Text::Bold);
	sf::RectangleShape box;
	box.setPosition(0, 0);
	box.setSize(sf::Vector2f(WindowWidth, WindowHeight));
	CenterInBox(text, box);
	auto start = chrono::high_resolution_clock::now();
	sf::Event event;
	while (window.isOpen() && passed_time(start) < seconds)
	{
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			}
		}
		window.clear();
		window.draw(background);
		window.draw(text);
		window.display();
		if (effect && text.getCharacterSize() < WindowHeight * 0.1)
		{
			text.setCharacterSize(text.getCharacterSize() + 5);
			CenterInBox(text, box);
		}
	}
}