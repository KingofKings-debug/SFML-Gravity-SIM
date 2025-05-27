#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
const int start = 0;
const unsigned int acturalres = 900;
const unsigned int res = 40000;
const unsigned int fps = 160;
const float gcont= 400;
const int num = 3;

#define s 500



struct particle {
	sf::Vector2f *points=(sf::Vector2f*)malloc(s*sizeof(sf::Vector2f));
	int radius;
	sf::Vector2f position;
	int mass;
	sf::Vector2f acceleration;
	sf::Vector2f velocity;
	sf::Vector2f center;

	particle()=default;
	particle(int rad, sf::Vector2f pos,int m):
		radius{rad},
		position{pos},
		mass{m}
	{}

	//convert the actual postion to viewport
	sf::Vector2f acutualposition() {
		sf::Vector2f mappedvalue = center / float(res)*float(acturalres);
		return mappedvalue;
	}
	//update the position according to velocity
	void updatepos(float dt) {
		position.x += velocity.x ;
		position.y += velocity.y;
	}
	//changes the velocity acording to acceleration
	void accelerate(float dt) {
		this->velocity += this->acceleration * dt;

		
	}
	//changes the acceleration
	void calculateforce(sf::Vector2f obj,int ma){
		float distance = sqrt(pow(center.x - obj.x, 2) + pow(center.y - obj.y, 2));
		if (distance <= radius+10) {
			acceleration = {0,0};
			velocity = { 0,0 };
			return;
		}
		float force = (gcont * ma) / pow(distance, 2);
		sf::Vector2f dir = obj - position;
		dir /= distance;
		this->acceleration += dir*force;
	}

//	void getvelocity() {
	//	cout << acceleration.x << " " << velocity.y << endl;}

	

	

};
int main() {
	const float dtime = float(1.0 / float(fps));
	struct particle mainlist[num];

	sf::RenderWindow window(sf::VideoMode({ acturalres,acturalres }), "Gravity");
	window.setFramerateLimit(fps);
	mainlist[0] = struct particle(5, {8000,8000}, 700);
	mainlist[1] = struct particle(7, { 9000,9500 }, 3000);
	mainlist[2] = struct particle(10, { 11000,13000 }, 40000);
	mainlist[0].velocity = { 2,0 };
	mainlist[1].velocity = { -1.5,1.0 };
	mainlist[2].velocity = { 0,0 };
	
	/*
	int x = 0, y = 0;
	for (int i = start;i < num;i++) {
		srand(time(NULL));
		mainlist[i] = struct particle(5, { float(x),float(y) }, 30);
		
		x += 200;
		if (x >= res) {
			y += 300;
			x = 0;
		}

	}
	*/
	
	
	
	
	
	
	sf::Color corlors[3] = { sf::Color::Red,sf::Color::Blue,sf::Color::Cyan };
	int u = 0;
	while (window.isOpen()) {
		sf::RenderTexture texture({ acturalres,acturalres });
		texture.clear();
		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>())
				window.close();
		}

		
		

		for (int i = start;i < num;i++) {
			//mainlist[i].points[u] = mainlist[i].acutualposition();
			//drawing the circles

			/*for (int t = 0;t < s + 1;t++) {
				float distance = sqrt(pow(mainlist[i].center.x - mainlist[i].points[u].x, 2) + pow(mainlist[i].center.y - mainlist[i].points[u].y, 2));
				if (distance <= 30) {
					continue;
				}
				sf::CircleShape point;
				point.setPosition(mainlist[i].points[t]);
				point.setRadius(1);
				point.setFillColor(sf::Color::Red);
				texture.draw(point);
			} */
			
			sf::CircleShape circle;
			circle.setPosition(mainlist[i].acutualposition());
			circle.setFillColor(corlors[i%3]);
			
			
			
			circle.setRadius(mainlist[i].radius);
			
			texture.draw(circle);
			/*if (mainlist[i].center.x<0 || mainlist[i].center.x>res) {
				mainlist[i].acceleration.x *=-1;
				mainlist[i].velocity.x *= -1.0f;
			}
			if (mainlist[i].center.y<0 || mainlist[i].center.y>res) {
				mainlist[i].acceleration.y *= -1.f;
				mainlist[i].velocity.y *= -1.0f;
			}*/
			
			for (int j = 0;j < num;j++) {
				if (i == j) {
					continue;
				}
				else {
					mainlist[i].center = { mainlist[i].position.x + (mainlist[i].radius),mainlist[i].position.y + (mainlist[i].radius)};
					mainlist[i].calculateforce(mainlist[j].center, mainlist[j].mass);
					
				}
				mainlist[i].accelerate(dtime);
				mainlist[i].updatepos(dtime);
				mainlist[i].acceleration = {0,0};
			}
			
		}
		
		
		u = u <= s ? u+1 : 0;
		
		texture.display();
		sf::Texture tex = texture.getTexture();
		sf::Sprite sp(tex);
		window.clear();
		window.draw(sp);
		window.display();
	}
	


	return 0;
}