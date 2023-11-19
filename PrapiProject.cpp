#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "box2d/box2d.h"
#include <stdio.h>
using namespace std;
using namespace sf;


Vector2f mtpfp(b2Vec2 position)
{
	float x = position(0);
	float y = position(1);

	x = x * 32;
	y = y * 32;

	Vector2f a(x, y);

	return a;

}

int main()
{

	//makes the box2d world - this  holds everything inside it

	b2Vec2 gravity(0.0f, 10.0f);
	b2World world(gravity);

	//

	//creates the ground body for box2d - this stops things falling out the screen

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, 37.0f);

	b2Body* groundBody = world.CreateBody(&groundBodyDef);

	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.0f, 10.0f);

	groundBody->CreateFixture(&groundBox, 0.0f);

	//

	//makes the window

	RenderWindow window(VideoMode(680, 900), "window");
	window.setFramerateLimit(60);


	//

	//makes the shape 

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(10.0f, 5.0f);
	b2Body* body = world.CreateBody(&bodyDef);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(1.0f, 1.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;

	body->CreateFixture(&fixtureDef);

	RectangleShape rect;
	Vector2f rectanglePosition(300, 100);
	rect.setPosition(rectanglePosition);
	rect.setSize(Vector2f(50, 50));

	//

	//makes the outer lines

	RectangleShape leftline;
	leftline.setPosition(20, 20);
	leftline.setSize(Vector2f(5, 860));

	RectangleShape rightline;
	rightline.setPosition(640, 20);
	rightline.setSize(Vector2f(5, 860));

	RectangleShape topline;
	topline.setPosition(20, 20);
	topline.setSize(Vector2f(620, 5));

	RectangleShape botline;
	botline.setPosition(20, 880);
	botline.setSize(Vector2f(625, 5));

	//

	//sets the timestep and iterations for the box2d sim

	float timeStep = 1.0f / 60.0f;

	int32 velocityIterations = 8;
	int32 positionIterations = 3;

	//

	//while window is open is where any thing that happens goes

	while (window.isOpen())
	{

		//event checker - checks and event is running and closes window if needed

		Event event;

		while (window.pollEvent(event))
		{

			if (event.type == Event::Closed) window.close();

			if (Keyboard::isKeyPressed(Keyboard::Escape)) window.close();

		}

		//

		//sim loop for box2d

		for (int32 i = 0; i < 60; ++i)
		{
			world.Step(timeStep, velocityIterations, positionIterations);
			b2Vec2 position = body->GetPosition();
			Vector2f sfmlPos = mtpfp(position);
			rect.setPosition(sfmlPos);
			float angle = body->GetAngle();
			rect.setRotation(angle);
			printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);

			//render 

			window.clear();
			window.draw(rect);
			window.draw(leftline);
			window.draw(rightline);
			window.draw(topline);
			window.draw(botline);
			window.display();

			//

		}

		//

	}

}

