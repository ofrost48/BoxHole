//sfml tutorial used to make - https://www.youtube.com/watch?v=R87RUYUfgwM&list=PLlRog_mTDrIy4UN7L6w6gZuYR9MqYFgEA&index=15&t=6s

//sfml documentation - https://www.sfml-dev.org/documentation/2.6.1/

//box2d documentation - https://box2d.org/documentation/

#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "box2d/box2d.h"
#include "box2d/b2_api.h"
#include "box2d/b2_joint.h"
#include "box2d/b2_distance_joint.h"
#include "box2d/b2_world.h"
#include <stdio.h>
using namespace std;
using namespace sf;


Vector2f mtpfp(b2Vec2 position)
{
	//function to change a b2Vec2 to a Vector2f

	float x = position(0);
	float y = position(1);

	x = x * 50;
	y = y * 50;

	Vector2f a(x, y);

	return a;

	//

}

int main()
{

	//makes the box2d world - this  holds everything inside it

	b2Vec2 gravity(0.0f, 10.0f);
	b2World world(gravity);

	//

	//creates the ground body for box2d - this stops things falling out the screen

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, 27.7f);

	b2Body* groundBody = world.CreateBody(&groundBodyDef);

	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.0f, 10.0f);

	groundBody->CreateFixture(&groundBox, 0.0f);

	//

	//wall bodies

	b2BodyDef wallBody1Def;
	wallBody1Def.position.Set(-1.5f, 0.0f);

	b2Body* wall1Body = world.CreateBody(&wallBody1Def);

	b2PolygonShape wall1Box;
	wall1Box.SetAsBox(1.0f, 50.0f);

	wall1Body->CreateFixture(&wall1Box, 0.0f);

	b2BodyDef wallBody2Def;
	wallBody2Def.position.Set(14.0f, 0.0f);

	b2Body* wall2Body = world.CreateBody(&wallBody2Def);

	b2PolygonShape wall2Box;
	wall2Box.SetAsBox(1.0f, 50.0f);

	wall2Body->CreateFixture(&wall2Box, 0.0f);
#

	b2BodyDef roofBodyDef;
	roofBodyDef.position.Set(0.0f, -10.5f);

	b2Body* roofBody = world.CreateBody(&roofBodyDef);

	b2PolygonShape roofBox;
	roofBox.SetAsBox(50.0f, 10.0f);

	roofBody->CreateFixture(&roofBox, 0.0f);

	//

	//makes the window

	RenderWindow window(VideoMode(680, 900), "window");
	window.setFramerateLimit(60);

	//

	//need to make into a function

	//makes the shape 

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(10.0f, 2.5f);
	b2Body* body = world.CreateBody(&bodyDef);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(1.0f, 1.0f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.1f;
	
	//restitution is how much bounce it has 
	fixtureDef.restitution = 5.0f;

	body->CreateFixture(&fixtureDef);

	RectangleShape rect;
	rect.setSize(Vector2f(50, 50));

	//

	//body for distance joint

	b2BodyDef jointBodyDef;
	jointBodyDef.type = b2_staticBody;
	jointBodyDef.position.Set(6.25f, 2.5f);
	b2Body* jointBody = world.CreateBody(&jointBodyDef);

	//

	//making distance joint
	
	b2DistanceJointDef jointDef;
	jointDef.Initialize(jointBody, body, jointBodyDef.position, bodyDef.position);
	jointDef.collideConnected = true;
	b2Joint* pendulum = world.CreateJoint(&jointDef);

	//

	//^make function with above 

	//makes the outer lines

	RectangleShape leftline;
	leftline.setPosition(21, 20);
	leftline.setSize(Vector2f(5, 860));

	RectangleShape rightline;
	rightline.setPosition(649, 20);
	rightline.setSize(Vector2f(5, 860));

	RectangleShape topline;
	topline.setPosition(21, 20);
	topline.setSize(Vector2f(628, 5));

	RectangleShape botline;
	botline.setPosition(21, 880);
	botline.setSize(Vector2f(633, 5));

	//

	//sets the timestep and iterations for the box2d sim

	float timeStep = 1.0f / 60.0f;

	int32 velocityIterations = 8;
	int32 positionIterations = 3;

	//

	//while window is open is where any thing that happens goes

	while (window.isOpen())
	{

		//event checker - checks and event is running and closes window if needed/on esc

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

			//gets and sets the position for both box2d and sfml

			b2Vec2 position = body->GetPosition();
			Vector2f sfmlPos = mtpfp(position);
			rect.setPosition(sfmlPos);
			float angle = body->GetAngle();
			rect.setRotation(angle);

			//

			//render - draws all of the objects on the screen

			window.clear();
			window.draw(rect);
			window.draw(leftline);
			window.draw(rightline);
			window.draw(topline);
			window.draw(botline);
			window.display();

			//

			//destroys joint if "a" is pressed 

			if (Keyboard::isKeyPressed(Keyboard::A))
			{
				if (pendulum != NULL) 
				{
					world.DestroyJoint(pendulum);
					pendulum = NULL;
				}
			}

			//

		}

		//

	}

}



