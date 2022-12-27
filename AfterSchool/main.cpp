#include<stdio.h>
#include<SFML/Graphics.hpp>
#include<stdlib.h>
#include<time.h>
#include<SFML/Audio.hpp>

using namespace sf;

struct Player {
	RectangleShape sprite; //그림이 되는 부분
	int speed;
	int score;

};

struct Enemy {

};
int main(void) {

	RenderWindow window(VideoMode(640, 480), "AfterSchool"); // 윈도우 창 생성
	window.setFramerateLimit(60);

	srand(time(0));

	long start_time = clock(); //게임 시작 시간
	long spent_time;//게임 진행 시간

	Font font; 
	font.loadFromFile("C:\\windows\\Fonts\\arial.ttf");

	Text text; //score
	text.setFont(font);
	text.setCharacterSize(30);
	text.setFillColor(Color(255, 255, 255));
	text.setPosition(0, 0);
	text.setString("score : ");

	Texture bg_texture;
	bg_texture.loadFromFile("./resources/images/background.jpg");
	Sprite bg_sprite;
	bg_sprite.setTexture(bg_texture);
	bg_sprite.setPosition(0, 0);

	struct Player player;
	player.sprite.setSize(Vector2f(40, 40));
	player.sprite.setPosition(100, 100);
	player.sprite.setFillColor(Color::Red);
	player.speed = 5; //플레이어의 움직임 속도
	player.score = 0; //플레이어의 점수
	char player_str[50];

	//enemy
	const int ENEMY_NUM = 10;
	RectangleShape enemy[ENEMY_NUM];
	int enemy_life[ENEMY_NUM];
	int enemy_score = 100; //enemy를 잡았을 때 점수
	SoundBuffer enemy_explosion_buffer;
	enemy_explosion_buffer.loadFromFile("./resources/sounds/rumble.flac");
	Sound enemy_explosion_sound;
	enemy_explosion_sound.setBuffer(enemy_explosion_buffer);
	int enemy_speed[ENEMY_NUM];
	for (int i = 0; i < ENEMY_NUM; i++) {
		enemy[i].setSize(Vector2f(70, 70));
		enemy[i].setPosition(rand()%300+300, rand()%385);
		enemy[i].setFillColor(Color::Yellow);
		enemy_life[i] = 1;
		enemy_speed[i] = -(rand() % 10 * 1); 
	}

	while (window.isOpen())//윈도우가 열려 있을 때 까지 창 유지 
	{
		Event event;

		while (window.pollEvent(event)) {

			switch (event.type) {

			//x버튼을 누르면 윈도우 창 종료
			case Event::Closed:
				window.close(); //윈도우를 닫음
				break;

			//스페이스를 누르면 모든 enemy 초기화 
			case Event::KeyPressed:
			{
				if (event.key.code == Keyboard::Space) {
					for (int i = 0; i < ENEMY_NUM; i++) {
						enemy[i].setSize(Vector2f(70, 70));
						enemy[i].setPosition(rand() % 300 + 300, rand() % 385);
						enemy[i].setFillColor(Color::Yellow);
						enemy_life[i] = 1;
						enemy_speed[i] = -(rand() % 10 + 1);
						window.draw(enemy[i]);
					}
				}
			}///case

			}//switch

		}//while
		
		spent_time = clock() - start_time;

		//방향키를 눌렀을 때 플레이어 움직임
		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			player.sprite.move(-player.speed, 0);
		}
		if (Keyboard::isKeyPressed(Keyboard::Up)) {
			player.sprite.move(0, -player.speed);
		}
		if (Keyboard::isKeyPressed(Keyboard::Right)) {
			player.sprite.move(player.speed, 0);
		}
		if (Keyboard::isKeyPressed(Keyboard::Down)) {
			player.sprite.move(0, player.speed);
		}//방향키 end

		//적과 충돌
		for (int i = 0; i < ENEMY_NUM; i++) {
			if (enemy_life[i] > 0) {

				if (player.sprite.getGlobalBounds().intersects(enemy[i].getGlobalBounds())) {
					printf("enemy[%d]와 충돌\n", i);
					enemy_life[i] -= 1;
					player.score += enemy_score;
					
					if(enemy_life[i] == 0) {
						enemy_explosion_sound.play();
					}

				}//if

				enemy[i].move(enemy_speed[i], 0);
			}//if
			
		}

		window.clear(Color::Black);
		window.draw(bg_sprite);

		for (int i = 0; i < ENEMY_NUM; i++) //enemy가 살아있을때만 draw
		{
			if (enemy_life[i] > 0) {
				window.draw(enemy[i]);
			}
		}

		sprintf_s(player_str, "score : %d  time : %d\n",
			player.score, spent_time/1000);
		text.setString(player_str);

		//draw
		window.draw(player.sprite);
		window.draw(text);

		window.display();
		
	}//while

	return 0;
}