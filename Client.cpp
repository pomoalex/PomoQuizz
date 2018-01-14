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
sf::Sprite background;
sf::Font font;
sf::Texture temp_texture;
sf::Color color = sf::Color::Red;
int WindowWidth, WindowHeight;
string username;
extern int errno;
int client_sd;

void InitWindow(sf::RenderWindow &window);

void Intro(sf::RenderWindow &window);

void DrawIntro(sf::RenderWindow &window);

void ManageLogin(sf::RenderWindow &window);

void ConnectionAttempt(sf::RenderWindow &window);

void DrawConnection(sf::RenderWindow &window, int &option);

void PomoQuizz(sf::RenderWindow &window);

void PrepareBoxes(sf::RectangleShape box[]);

void CenterInBox(sf::Text &text, sf::RectangleShape box);

void CenterInBox2(sf::Text &text, sf::Text &aux, sf::RectangleShape box, bool &double_lined);

void PrepareText(sf::Text &text, sf::Font &font, sf::Color color, int size, sf::Text::Style style = sf::Text::Regular);

void GetQuestion(sf::Text &question, sf::Text answers[4]);

void DisplayMessage(sf::RenderWindow &window, string message, float seconds, bool effect = false);

int main()
{
	sf::RenderWindow window;
	InitWindow(window);
	game_state = STATE::LOGIN;
	while (window.isOpen())
	{
		switch (game_state)
		{
		case STATE::INTRO:
			Intro(window);
			break;
		case STATE::LOGIN:
			ManageLogin(window);
			break;
		case STATE::CONNECTING:
			ConnectionAttempt(window);
			break;
		case STATE::PLAYING:
			PomoQuizz(window);
			break;
		}
	}
}

void InitWindow(sf::RenderWindow &window)
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

void Intro(sf::RenderWindow &window)
{
	sf::Event event;
	thread execute(DrawIntro, ref(window));
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

void DrawIntro(sf::RenderWindow &window)
{
	sf::Text text;
	text.setFont(font);
	text.setString(LOGO);
	text.setCharacterSize(WindowHeight * 0.25);
	text.setFillColor(sf::Color::Red);
	text.setStyle(sf::Text::Bold);
	text.setPosition((WindowWidth - text.getGlobalBounds().width) / 2, (WindowHeight - text.getGlobalBounds().height) * 0.4);
	auto start = chrono::high_resolution_clock::now();
	chrono::high_resolution_clock::time_point finish;
	chrono::duration<double> elapsed;
	int seconds = 3;
	while (1)
	{
		window.clear();
		window.draw(background);
		window.draw(text);
		window.display();
		finish = chrono::high_resolution_clock::now();
		elapsed = finish - start;
		if (elapsed.count() > seconds)
			break;
	}
	game_state = STATE::LOGIN;
}

void ManageLogin(sf::RenderWindow &window)
{
	sf::Text login_text;
	username = "username";
	login_text.setString("Login");
	login_text.setFont(font);
	login_text.setCharacterSize(WindowHeight * 0.15);
	login_text.setFillColor(sf::Color::Red);
	login_text.setStyle(sf::Text::Bold);
	login_text.setPosition((WindowWidth - login_text.getGlobalBounds().width) / 2,
						   (WindowHeight - login_text.getGlobalBounds().height) * 0.3);
	sf::Text username_text;
	username_text.setFont(font);
	username_text.setString("ppppdddd");
	username_text.setCharacterSize(WindowHeight * 0.08);
	username_text.setFillColor(sf::Color::Red);
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

void ConnectionAttempt(sf::RenderWindow &window)
{
	int option = 0;
	thread execute(DrawConnection, ref(window), ref(option));
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
		recv_from(client_sd, message);
		printf("%s from server\n", message);
		auxiliary = atoi(message);
		if (auxiliary < 0)
			option = 2;
		else
			option = auxiliary;
	}
	if (option == 2)
	{
		shutdown(client_sd, SHUT_RDWR);
		close(client_sd);
	}
	execute.join();
}

void DrawConnection(sf::RenderWindow &window, int &option)
{
	sf::Text text;
	string text_str = "Connecting to server ";
	int text_len = text_str.length();
	int init_option = 0;
	text.setFont(font);
	text.setString(text_str);
	text.setCharacterSize(WindowHeight * 0.10);
	text.setFillColor(sf::Color::Red);
	text.setStyle(sf::Text::Bold);
	text.setPosition((WindowWidth - text.getGlobalBounds().width) / 2, (WindowHeight - text.getGlobalBounds().height) * 0.5);
	sf::Event event;
	bool conn_started = false;
	chrono::high_resolution_clock::time_point finish;
	chrono::high_resolution_clock::time_point conn_start;
	chrono::duration<double> conn_elapsed;
	chrono::duration<double> elapsed;
	chrono::duration<double> total_elapsed;
	float seconds = 0.5;
	float wait_time = 2.5;
	int remaining_seconds;
	auto start = chrono::high_resolution_clock::now();
	auto abs_start = chrono::high_resolution_clock::now();
	total_elapsed = finish - abs_start;
	while (window.isOpen() && (game_state == STATE::CONNECTING || option == 0 || total_elapsed.count() < wait_time))
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
		finish = chrono::high_resolution_clock::now();
		total_elapsed = finish - abs_start;
		if (init_option != option)
			if (total_elapsed.count() > wait_time)
			{
				init_option = option;
				abs_start = chrono::high_resolution_clock::now();
				finish = chrono::high_resolution_clock::now();
				total_elapsed = finish - abs_start;
			}
		if (init_option == 2 && !conn_started)
		{
			text_str = "Connection failed !";
			text.setString(text_str);
			text.setPosition((WindowWidth - text.getGlobalBounds().width) / 2, (WindowHeight - text.getGlobalBounds().height) * 0.5);
			game_state = STATE::LOGIN;
		}
		if (init_option == 0)
		{
			elapsed = finish - start;
			if (elapsed.count() > seconds)
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
			finish = chrono::high_resolution_clock::now();
			conn_elapsed = finish - conn_start;
			int temp_var = floor(option - conn_elapsed.count());
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

void PomoQuizz(sf::RenderWindow &window)
{
	int question_nr = 0;
	bool answering = false;
	bool answered;
	int given_answer = 0;
	bool double_lined;
	int cursor_in_box = 0;
	int score_val = 0;
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
			read_from(client_sd, &result, (int)sizeof(int));
			if (given_answer == 0)
				DisplayMessage(window, "Out of time !", 1, true);
			else
			{
				if (result == 1)
				{
					DisplayMessage(window, "Correct !", 1, true);
					score_val += 100;
				}
				else
					DisplayMessage(window, "Wrong !", 1, true);
			}
			answering = false;
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
	game_state = STATE::LOGIN;
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
	recv_from(client_sd, temp);
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

void DisplayMessage(sf::RenderWindow &window, string message, float seconds, bool effect)
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