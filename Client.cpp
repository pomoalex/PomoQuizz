#include "Definitions.h"
#include <SFML/Graphics.hpp>

#define TITLE "PomoQuizz"
#define BACKGROUND "Vegeta.jpg"
#define LOGO_FONT "FFF_Tusj.ttf"
#define LOGO "PomoQuizz"

sf::Sprite background;
sf::Text text;
sf::Font logo_font;
sf::Texture temp_texture;
int WindowWidth, WindowHeight;
extern int errno;

void InitWindow(sf::RenderWindow &window)
{
	sf::VideoMode DesktopMode = sf::VideoMode::getDesktopMode();
	WindowHeight = DesktopMode.height * 0.6;
	WindowWidth = DesktopMode.width * 0.6;
	window.create(sf::VideoMode(WindowWidth, WindowHeight), TITLE, sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
	window.setVerticalSyncEnabled(true);
	window.setPosition(sf::Vector2i((DesktopMode.width - WindowWidth) / 2, (DesktopMode.height - WindowHeight) / 2));
}

void DrawIntro(sf::RenderWindow &window)
{
	if(!temp_texture.loadFromFile(BACKGROUND))
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
	text.setFont(logo_font);
	text.setString(LOGO);
	text.setCharacterSize(WindowHeight * 0.25);
	text.setFillColor(sf::Color::Red);
	text.setStyle(sf::Text::Bold);
	text.setPosition((WindowWidth - text.getGlobalBounds().width) / 2, (WindowHeight - text.getGlobalBounds().height) * 0.4);
	window.clear();
	window.draw(background);
	//sf::sleep(sf::milliseconds(2000));
	window.draw(text);
	window.display();
}

int main()
{
	sf::RenderWindow window;
	InitWindow(window);
	DrawIntro(window);
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear();
		window.draw(background);
		//sf::sleep(sf::milliseconds(2000));
		window.draw(text);
		window.display();
	}
	int client_sd;
	struct sockaddr_in server;
	char username[256];
	recv_from(0, username);
	if ((client_sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("error at socket().\n");
		return errno;
	}
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(SERVER_IP);
	server.sin_port = htons(PORT);
	if (connect(client_sd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
	{
		perror("error at connect().\n");
		return errno;
	}
	fflush(stdout);
	send_to(client_sd, username);
	char message[256];
	recv_from(client_sd, message);
	printf("%s\n", message);
	recv_from(client_sd, message);
	printf("%s\n", message);
	while (1)
		;
	close(client_sd);
}