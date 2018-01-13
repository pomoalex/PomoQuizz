#include "Definitions.h"
#include <SFML/Graphics.hpp>
#define TITLE "PomoQuizz"
#define BACKGROUND "background.jpg"
#define LOGO_FONT "FFF_Tusj.ttf"
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
sf::Font logo_font;
sf::Texture temp_texture;
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
	window.create(sf::VideoMode(WindowWidth, WindowHeight), TITLE, sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
	window.setVerticalSyncEnabled(true);
	window.setPosition(sf::Vector2i((DesktopMode.width - WindowWidth) / 2, (DesktopMode.height - WindowHeight) / 2));
	if (!temp_texture.loadFromFile(BACKGROUND))
	{
		perror("Cannot load background");
		exit(errno);
	}
	background.setTexture(temp_texture);
	background.setScale(WindowWidth / background.getLocalBounds().width, WindowHeight / background.getLocalBounds().height);
	if (!logo_font.loadFromFile(LOGO_FONT))
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
	text.setFont(logo_font);
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
	login_text.setFont(logo_font);
	login_text.setCharacterSize(WindowHeight * 0.1);
	login_text.setFillColor(sf::Color::Red);
	login_text.setStyle(sf::Text::Bold);
	login_text.setPosition((WindowWidth - login_text.getGlobalBounds().width) / 2,
						   (WindowHeight - login_text.getGlobalBounds().height) * 0.3);
	sf::Text username_text;
	username_text.setFont(logo_font);
	username_text.setString(username);
	username_text.setCharacterSize(WindowHeight * 0.08);
	username_text.setFillColor(sf::Color::Red);
	int usr_text_height = (login_text.getPosition().y + login_text.getGlobalBounds().height +
						   (WindowHeight - login_text.getGlobalBounds().height - login_text.getPosition().y -
							username_text.getGlobalBounds().height) *
							   0.25);
	username_text.setPosition((WindowWidth - username_text.getGlobalBounds().width) / 2, usr_text_height);
	sf::RectangleShape textbox;
	textbox.setSize(sf::Vector2f(username_text.getGlobalBounds().width * 1.6, username_text.getGlobalBounds().height * 1.5));
	textbox.setFillColor(sf::Color::Transparent);
	textbox.setOutlineThickness(5);
	textbox.setPosition((WindowWidth - textbox.getGlobalBounds().width) / 2, usr_text_height + username_text.getGlobalBounds().height * 0.3);
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
				if (textbox.getGlobalBounds().width * 0.95 <= username_text.getGlobalBounds().width)
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
		send_to(client_sd, (char *)username.c_str());
		char message[256];
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
	text.setFont(logo_font);
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
	bool double_line = false;
	sf::Text question;
	sf::Text answers[4];
	sf::Text aux;
	question.setFont(logo_font);
	question.setFillColor(sf::Color::Red);
	question.setStyle(sf::Text::Bold);
	aux.setFont(logo_font);
	aux.setFillColor(sf::Color::Red);
	aux.setStyle(sf::Text::Bold);
	question.setCharacterSize(WindowHeight * 0.06);
	aux.setCharacterSize(WindowHeight * 0.06);
	for (int index = 0; index < 4; index++)
	{
		answers[index].setFont(logo_font);
		answers[index].setFillColor(sf::Color::Red);
		answers[index].setCharacterSize(WindowHeight * 0.06);
	}
	chrono::high_resolution_clock::time_point start;
	chrono::high_resolution_clock::time_point finish;
	chrono::duration<double> elapsed;
	sf::Event event;
	while (window.isOpen() && game_state == STATE::PLAYING)
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
		if (question_nr < 10 && !answering)
		{
			start = chrono::high_resolution_clock::now();
			question_nr++;
			answering = true;
			answered = false;
			char temp[256];
			if (!recv_from(client_sd, temp))
			{
				printf("Server down !\n");
				break;
			}
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
			question.setPosition((WindowWidth - question.getGlobalBounds().width) * 0.5,
								 (WindowHeight - question.getGlobalBounds().height) * 0.35);
			float percentage = 0.75;
			while (1)
			{
				if (WindowWidth * 0.9 < question.getGlobalBounds().width)
				{
					percentage -= 0.05;
					double_line = true;
					string substr1, substr2, full_str;
					full_str = question.getString();
					int poz = full_str.length() * percentage;
					while (full_str[poz] != ' ')
						poz--;
					substr1 = full_str.substr(0, poz);
					substr2 = full_str.substr(poz, full_str.length() - 1);
					question.setString(substr1);
					aux.setString(substr2);
					question.setPosition((WindowWidth - question.getGlobalBounds().width) * 0.5,
										 (WindowHeight - question.getGlobalBounds().height) * 0.25);
					aux.setPosition((WindowWidth - aux.getGlobalBounds().width) * 0.5,
									question.getPosition().y + question.getGlobalBounds().height);
				}
				else
					break;
			}
			answers[0].setPosition((WindowWidth - answers[0].getGlobalBounds().width) * 0.25,
								   (WindowHeight - answers[0].getGlobalBounds().height) * 0.6);
			answers[1].setPosition((WindowWidth - answers[1].getGlobalBounds().width) * 0.75,
								   (WindowHeight - answers[1].getGlobalBounds().height) * 0.6);
			answers[2].setPosition((WindowWidth - answers[2].getGlobalBounds().width) * 0.25,
								   (WindowHeight - answers[2].getGlobalBounds().height) * 0.85);
			answers[3].setPosition((WindowWidth - answers[3].getGlobalBounds().width) * 0.75,
								   (WindowHeight - answers[3].getGlobalBounds().height) * 0.85);
			if (!send_to(client_sd, temp))
			{
				printf("Server down !\n");
				break;
			}
		}
		window.clear();
		window.draw(background);
		window.draw(question);
		if (double_line)
			window.draw(aux);
		for (int index = 0; index < 4; index++)
			window.draw(answers[index]);
		window.display();
	}
	game_state = STATE::LOGIN;
}