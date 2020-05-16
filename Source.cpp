#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "SFML/Window.hpp"
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <Windows.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;


string s[9] = { "Sprites/Topdown/Audi.png","Sprites/Topdown/Black_viper.png", "Sprites/Topdown/Car.png", "Sprites/Topdown/Mini_truck.png",
"Sprites/Topdown/Ambulance.png","Sprites/Topdown/Mini_van.png","Sprites/Topdown/Police.png","Sprites/Topdown/taxi.png","Sprites/Topdown/truck.png"};


struct Car {
	string color;
	sf::Sprite obj;
	sf::Texture texture;
	int speed;
};

Car getCar()
{
	Car car;
	car.color = s[rand() % 9];
	car.speed = rand() % 10 + 5;
	car.texture.loadFromFile(car.color);
	return car;
}

bool Gran(unsigned int x, unsigned int y) {
	if (x >= 0 && x <= 722 && y >= 0 && y <= 650)
		return true;
	else
		return false;
}

void Die() {

}


void main1() {
	int highscore=0;
	int score = 0;
	sf::Text ScoreText;
	sf::Text TheEnd;
	sf::Font font;
	font.loadFromFile("Fonts/1942.ttf");

	ScoreText.setFont(font);
	ScoreText.setCharacterSize(30);
	ScoreText.setPosition(400,0);
	ScoreText.setFillColor(sf::Color::White);

	TheEnd.setFont(font);
	TheEnd.setCharacterSize(30);
	TheEnd.setPosition(200, 100);
	TheEnd.setFillColor(sf::Color::Green);




	sf::RenderWindow window(sf::VideoMode(800, 800), "Super Original Game");
	window.setFramerateLimit(120);
	bool alive = true;


	sf::Music music;//создаем объект музыки
	music.openFromFile("Audio/Wasted.wav");//загружаем файл

	sf::Music carmus;//создаем объект музыки
	carmus.openFromFile("Audio/cars.wav");//загружаем файл
	carmus.play();


	sf::Texture texture;
	texture.loadFromFile("Sprites/wasted1.png");
	sf::Texture roadTexture;
	roadTexture.loadFromFile("Sprites/Road1.png");
	sf::Texture carTexture;
	carTexture.loadFromFile("Sprites/PNG/tdrc01_car01_b.png");
	sf::Texture explosion;
	explosion.loadFromFile("Sprites/exp3.png");
	sf::Sprite expS(explosion);

	sf::Sprite died(texture);
	sf::Sprite road1(roadTexture);
	sf::Sprite road2(roadTexture);
	sf::Sprite sprite(carTexture);

	road2.setPosition(0, -800);

	int speed[4] = { rand() % 10 + 5,rand() % 10 + 5 ,rand() % 10 + 5 ,rand() % 10 + 5 };


	sprite.setPosition(550, 649);
	Car cars[4];
	for (int i = 0; i < 4; i++) {
		cars[i] = getCar();
		cars[i].obj.setTexture(cars[i].texture);
		cars[i].obj.setPosition(50 + 200 * i, -(int)cars[i].texture.getSize().y);
	}
	bool key = true;
	int count = 0;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (event.type == sf::Event::KeyPressed && Gran(sprite.getPosition().x, sprite.getPosition().y) && alive) {
				switch (event.key.code) {
				case sf::Keyboard::Key::Left:
					sprite.move(-10, 0);
					break;
				case sf::Keyboard::Key::Right:
					sprite.move(10, 0);
					break;
				case sf::Keyboard::Key::Up:
					sprite.move(0, -10);
					break;
				case sf::Keyboard::Key::Down:
					sprite.move(0, 10);
					break;
				}
			}
		}
		for (int i = 0; i < 4; i++) {
			cars[i].obj.move(0.f, 0.5f * cars[i].speed);
		}
		road1.move(0.f, 2.f);
		road2.move(0.f, 2.f);
		window.clear();

		window.draw(road1);
		window.draw(road2);

		window.draw(sprite);
		stringstream ScoreConverter;
		ScoreConverter << score;
		ScoreText.setString(ScoreConverter.str());

		for (int i = 0; i < 4; i++) {
			window.draw(cars[i].obj);
		}

		if (road1.getPosition().y >= 800) {
			road1.setPosition(0, -800);
		}
		if (road2.getPosition().y >= 800) {
			road2.setPosition(0, -800);
		}
		for (int i = 0; i < 4; i++) {
			if (cars[i].obj.getPosition().y > 800)
			{
				cars[i] = getCar();
				cars[i].obj.setTexture(cars[i].texture);
				cars[i].obj.setPosition(50 + 200 * i, -(int)cars[i].texture.getSize().y);
				if (alive) score += 100;

				
			}
		}
		if (sprite.getPosition().y > 650) {
			sprite.setPosition(sprite.getPosition().x, 650);
		}
		if (sprite.getPosition().y < 0) {
			sprite.setPosition(sprite.getPosition().x, 0);
		}
		if (sprite.getPosition().x > 722) {
			sprite.setPosition(722, sprite.getPosition().y);
		}
		if (sprite.getPosition().y < 0) {
			sprite.setPosition(0, sprite.getPosition().y);
		}
		for (int i = 0; i < 4; i++) {
			if (sprite.getGlobalBounds().intersects(cars[i].obj.getGlobalBounds())) {
				alive = false;
				
				std::ifstream fin("highscore.txt");
				fin >> highscore;
				if (score > highscore)
				{
					std::ofstream fout("highscore.txt");
					fout << score;
				}
				stringstream HighScoreConverter;
				HighScoreConverter << "Last record - "  << highscore;
				TheEnd.setString(HighScoreConverter.str());

				
				window.setFramerateLimit(10);
				break;
			}
		}
		if (!alive) {
			count++;
			expS.setPosition(sprite.getPosition().x - 20, sprite.getPosition().y);
		}
		if (count > 10)
			window.draw(expS);
		if (count > 20) {
			sf::Texture expS2;
			expS2.loadFromFile("Sprites/exp2.png");
			explosion.update(expS2);
			expS.setTexture(explosion);
			window.draw(expS);
		}
		if (count > 30) {
			sf::Texture expS2;
			expS2.loadFromFile("Sprites/exp1.png");
			explosion.update(expS2);
			expS.setTexture(explosion);
			window.draw(expS);
		}
		if (!alive && key) {
			music.play();
			key = false;
		}
		if (!alive && count > 26) {
			window.draw(died);
		}
		if (count == 70)
			break;
		window.draw(ScoreText);
		window.draw(TheEnd);
		window.display();
	}
}



void main2() {
	setlocale(LC_ALL, "");
	sf::IpAddress ip = sf::IpAddress::getLocalAddress();
	sf::TcpSocket socket;
	sf::Packet packet1;
	sf::Packet packet2;
	char type;
	char mode = ' ';
	int x1 = 0;
	int y1 = 0;
	int x2 = 0;
	int y2 = 0;
	char buffer[2000];
	size_t received;
	string text = "connect to: ";

	cout << ip << endl;
	cout << "Выберите тип подключения:  c -клиент, s -сервер" << endl;
	cin >> type;
	if (type == 's') {
		sf::TcpListener listener;
		listener.listen(2000);
		listener.accept(socket);
		text += "Server";
		mode = 's';

	}
	else if (type == 'c') {
		cout << "new ip:";
		cin >> ip;
		socket.connect(ip, 2000);
		text += "client";
		mode = 'r';
	}
	text += "hi";
	socket.send(text.c_str(), text.length() + 1);
	socket.receive(buffer, sizeof(buffer), received);
	cout << buffer << endl;

	sf::RenderWindow window(sf::VideoMode(800, 800), buffer);
	window.setFramerateLimit(120);
	bool alive = true;
	bool alive2 = true;


	sf::Music music;
	music.openFromFile("Audio/Wasted.wav");

	sf::Music carmus;
	carmus.openFromFile("Audio/cars.wav");
	carmus.play();


	sf::Texture texture;
	texture.loadFromFile("Sprites/wasted1.png");
	sf::Texture roadTexture;
	roadTexture.loadFromFile("Sprites/Road1.png");
	sf::Texture carTexture1;
	carTexture1.loadFromFile("Sprites/PNG/tdrc01_car01_b.png");
	sf::Texture carTexture2;
	carTexture2.loadFromFile("Sprites/PNG/tdrc01_car01_e.png");
	sf::Texture explosion;
	explosion.loadFromFile("Sprites/exp3.png");
	sf::Sprite expS(explosion);

	sf::Sprite died(texture);
	sf::Sprite road1(roadTexture);
	sf::Sprite road2(roadTexture);
	sf::Sprite sprite(carTexture1);
	sf::Sprite sprite2(carTexture2);
	int move1=0, move2=0;
	road2.setPosition(0, -800);


	int highscore = 0;
	int score = 0;
	sf::Text ScoreText;
	sf::Text TheEnd;
	sf::Font font;
	font.loadFromFile("Fonts/1942.ttf");

	ScoreText.setFont(font);
	ScoreText.setCharacterSize(30);
	ScoreText.setPosition(400, 0);
	ScoreText.setFillColor(sf::Color::White);

	TheEnd.setFont(font);
	TheEnd.setCharacterSize(30);
	TheEnd.setPosition(200, 100);
	TheEnd.setFillColor(sf::Color::Green);




	int speed[4] = { rand() % 10 + 5,rand() % 10 + 5 ,rand() % 10 + 5 ,rand() % 10 + 5 };


	sprite.setPosition(550, 649);
	sprite2.setPosition(150, 649);
	Car cars[4];
	for (int i = 0; i < 4; i++) {
		cars[i] = getCar();
		cars[i].obj.setTexture(cars[i].texture);
		cars[i].obj.setPosition(50 + 200 * i, -(int)cars[i].texture.getSize().y);
	}
	bool key = true;
	int count = 0;

	while (window.isOpen())
	{
		sf::Event event;
		if (mode == 's') {
			x2 = sprite.getPosition().x;
			y2 = sprite.getPosition().y;
			packet2 << x2 << y2;
			socket.send(packet2);
			packet2.clear();

			socket.receive(packet1);
			if (packet1 >> x1 >> y1) {
				sprite2.setPosition(x1, y1);
			}
		}
		else if (mode == 'r') {
			x1 = sprite2.getPosition().x;
			y1 = sprite2.getPosition().y;
			packet1 << x1 << y1;
			socket.send(packet1);
			packet1.clear();

			socket.receive(packet2);
			if (packet2 >> x2 >> y2) {
				sprite.setPosition(x2, y2);
			}
		}

		if (mode == 's') {
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed) {
					window.close();
				}

				if (event.type == sf::Event::KeyPressed && Gran(sprite.getPosition().x, sprite.getPosition().y) && alive) {
					switch (event.key.code) {
					case sf::Keyboard::Key::Left:
						sprite.move(-10, 0);
						move1 = 1;
						break;
					case sf::Keyboard::Key::Right:
						sprite.move(10, 0);
						move1 = 2;
						break;
					case sf::Keyboard::Key::Up:
						sprite.move(0, -10);
						move1 = 3;
						break;
					case sf::Keyboard::Key::Down:
						sprite.move(0, 10);
						move1 = 4;
						break;
					}
				}			
			}

		}

		else if (mode == 'r') {
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed) {
					window.close();
				}
				if (event.type == sf::Event::KeyPressed && Gran(sprite2.getPosition().x, sprite2.getPosition().y) && alive2) {
					switch (event.key.code) {
					case sf::Keyboard::Key::Left:
						sprite2.move(-10, 0);
						move2 = 1;
						break;
					case sf::Keyboard::Key::Right:
						sprite2.move(10, 0);
						move2 = 2;
						break;
					case sf::Keyboard::Key::Up:
						sprite2.move(0, -10);
						move2 = 3;
						break;
					case sf::Keyboard::Key::Down:
						sprite2.move(0, 10);
						move2 = 4;
						break;
					}
				}
			}
		}

		for (int i = 0; i < 4; i++) {
			cars[i].obj.move(0.f, 0.5f * cars[i].speed);
		}
		road1.move(0.f, 2.f);
		road2.move(0.f, 2.f);
		window.clear();

		window.draw(road1);
		window.draw(road2);

		window.draw(sprite);
		window.draw(sprite2);
		

		stringstream ScoreConverter;
		ScoreConverter << score;
		ScoreText.setString(ScoreConverter.str());

		for (int i = 0; i < 4; i++) {
			window.draw(cars[i].obj);
		}

		if (road1.getPosition().y >= 800) {
			road1.setPosition(0, -800);
		}
		if (road2.getPosition().y >= 800) {
			road2.setPosition(0, -800);
		}
		for (int i = 0; i < 4; i++) {
			if (cars[i].obj.getPosition().y > 800)
			{
				cars[i] = getCar();
				cars[i].obj.setTexture(cars[i].texture);
				cars[i].obj.setPosition(50 + 200 * i, -(int)cars[i].texture.getSize().y);
				if(alive) score += 100;
			}
		}
		if (sprite.getPosition().y > 650) {
			sprite.setPosition(sprite.getPosition().x, 650);
		}
		if (sprite.getPosition().y < 0) {
			sprite.setPosition(sprite.getPosition().x, 0);
		}
		if (sprite.getPosition().x > 722) {
			sprite.setPosition(722, sprite.getPosition().y);
		}
		if (sprite.getPosition().x < 0) {
			sprite.setPosition(0, sprite.getPosition().y);
		}

		if (sprite2.getPosition().y > 650) {
			sprite2.setPosition(sprite2.getPosition().x, 650);
		}
		if (sprite2.getPosition().y < 0) {
			sprite2.setPosition(sprite2.getPosition().x, 0);
		}
		if (sprite2.getPosition().x > 722) {
			sprite2.setPosition(722, sprite2.getPosition().y);
		}
		if (sprite2.getPosition().x < 0) {
			sprite2.setPosition(0, sprite2.getPosition().y);
		}

		for (int i = 0; i < 4; i++) {
			if (sprite.getGlobalBounds().intersects(cars[i].obj.getGlobalBounds())) {
				alive = false;

				std::ifstream fin("highscore.txt");
				fin >> highscore;
				if (score > highscore)
				{
					std::ofstream fout("highscore.txt");
					fout << score;
				}
				stringstream HighScoreConverter;
				HighScoreConverter << "Last record - " << highscore;
				TheEnd.setString(HighScoreConverter.str());

				window.setFramerateLimit(10);
				break;
			}
			if (sprite2.getGlobalBounds().intersects(cars[i].obj.getGlobalBounds())) {
				alive2 = false;

				std::ifstream fin("highscore.txt");
				fin >> highscore;
				if (score > highscore)
				{
					std::ofstream fout("highscore.txt");
					fout << score;
				}
				stringstream HighScoreConverter;
				HighScoreConverter << "Last record - " << highscore;
				TheEnd.setString(HighScoreConverter.str());

				window.setFramerateLimit(10);
				break;
			}
		}
		if (!alive) {
			count++;
			expS.setPosition(sprite.getPosition().x - 20, sprite.getPosition().y);
		}
		if (!alive2) {
			count++;
			expS.setPosition(sprite2.getPosition().x - 20, sprite2.getPosition().y);
		}
		if (count > 10)
			window.draw(expS);
		if (count > 20) {
			sf::Texture expS2;
			expS2.loadFromFile("Sprites/exp2.png");
			explosion.update(expS2);
			expS.setTexture(explosion);
			window.draw(expS);
		}
		if (count > 30) {
			sf::Texture expS2;
			expS2.loadFromFile("Sprites/exp1.png");
			explosion.update(expS2);
			expS.setTexture(explosion);
			window.draw(expS);
		}
		if ((!alive || !alive2) && key) {
			music.play();
			key = false;
		}

		if ((!alive||!alive2) && count > 26) {
			window.draw(died);
		}
		if (count == 70)
			break;
		window.draw(ScoreText);
		window.draw(TheEnd);
		window.display();
		if (mode == 'r')
			sleep(sf::milliseconds(10));
	}
}

void main() {
	setlocale(LC_ALL, "");
	char type='0';
	while (type != 's' && type != 'm') {
		cout << "Выберите тип подключения:  s - одиночная, m - мультиплеер" << endl;
		cin >> type;
		if (type == 's')
			main1();
		else if (type == 'm')
			main2();
	}
}