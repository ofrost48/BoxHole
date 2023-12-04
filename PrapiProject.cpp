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

using namespace std;		//standard namespace 
using namespace sf;			//sfml namespace 


Vector2f mtpfp(b2Vec2 position)
{
	//function to change a b2Vec2 to a Vector2f

	float x = position(0);
	float y = position(1);

	x = x * 50;		//conversion rate for box2d to sfml (https://box2d.org/posts/2011/12/pixels/)
	y = y * 50;

	Vector2f a(x, y);	//creates a vector2f with the converted values

	return a;

	//

}

int main()
{

	//makes the box2d world - this  holds everything inside - also has the gravity 

	b2Vec2 gravity(0.0f, 10.0f);
	b2World world(gravity);

	//

	//creates the ground body for box2d - this stops things falling out the screen

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, 27.2f);					//defines the size of the groundbox

	b2Body* groundBody = world.CreateBody(&groundBodyDef);		//creates the groundbox in the world 

	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.0f, 10.0f);							//sets the size of the groundbox

	groundBody->CreateFixture(&groundBox, 0.0f);				//creates a fixture between the groundbox body and the box itself 

	//

	//wall bodies - functionally the same as the ground box

	b2BodyDef wallBody1Def;
	wallBody1Def.position.Set(-1.0f, 0.0f);

	b2Body* wall1Body = world.CreateBody(&wallBody1Def);

	b2PolygonShape wall1Box;
	wall1Box.SetAsBox(1.0f, 50.0f);

	wall1Body->CreateFixture(&wall1Box, 0.0f);

	b2BodyDef wallBody2Def;
	wallBody2Def.position.Set(13.5f, 0.0f);

	b2Body* wall2Body = world.CreateBody(&wallBody2Def);

	b2PolygonShape wall2Box;
	wall2Box.SetAsBox(1.0f, 50.0f);

	wall2Body->CreateFixture(&wall2Box, 0.0f);
#

	b2BodyDef roofBodyDef;
	roofBodyDef.position.Set(0.0f, -10.f);

	b2Body* roofBody = world.CreateBody(&roofBodyDef);

	b2PolygonShape roofBox;
	roofBox.SetAsBox(50.0f, 10.0f);

	roofBody->CreateFixture(&roofBox, 0.0f);

	//

	//makes the window

	RenderWindow window(VideoMode(680, 900), "window");		//creates/names/scales the sfml window
	window.setFramerateLimit(60);							//limits the framerate of the window

	//

	//makes the shape definition and the first box

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(10.0f, 2.5f);						//sets the starting position of the boxes
	b2Body* body[200];										//creates an array of all the boxes that can be made during runtime
	body[1] = { world.CreateBody(&bodyDef) };				//creates the body in the world
	body[1]->SetFixedRotation(true);						//fixes rotation so no spinning


	b2PolygonShape dynamicBox;								//sets shape as dynamic box so that it can be moved 
	dynamicBox.SetAsBox(0.5f, 0.5f);						//sets the size of the boxes 

	b2FixtureDef fixtureDef;								
	fixtureDef.shape = &dynamicBox;							//assigns the fixture to the dynamic box shape
	fixtureDef.density = 1.0f;								//assigns the density of the box 
	fixtureDef.friction = 0.05f;							//assigns the friction of the box
	
	//restitution is how much bounce it has 
	fixtureDef.restitution = 0.5f;							//assigns the amount of bounce each box has 

	body[1]->CreateFixture(&fixtureDef);					//creates the fixture between body and shape fort he boxes

	RectangleShape rect[150];								//creates an array of empty rectangle shapes 
	rect[1].setSize(Vector2f(50, 50));						//sets the size of the first one 

	//

	//body for distance joint

	b2BodyDef jointBodyDef;
	jointBodyDef.type = b2_staticBody;						//sets the joint as static 
	jointBodyDef.position.Set(6.25f, 2.5f);					//position if the joints body
	b2Body* jointBody = world.CreateBody(&jointBodyDef);	//creates the joints body

	//

	//making distance joint
	
	b2DistanceJointDef jointDef;
	jointDef.Initialize(jointBody, body[1], jointBodyDef.position, bodyDef.position);	//defines how the first joint will initialize 
	jointDef.collideConnected = true;													//adds collisions to the joint
	b2Joint* pendulum = world.CreateJoint(&jointDef);									//creates the first joint

	//

	//makes the outer lines - sets the size and position of 4 sfml boxes to act as the walls and roof/floor

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

	RectangleShape test;
	test.setSize(Vector2f(50, 50));
	test.setPosition(50, 50);

	//

	//sets the timestep and iterations for the box2d sim

	float timeStep = 1.0f / 60.0f;

	int32 velocityIterations = 8;
	int32 positionIterations = 3;

	//

	//variables used in making more boxes

	int j = 1;					//tracks how many boxes have been made 
	bool makeNew = false;		//is true when a new box has to be made 

	//

	//while window is open is where any thing that happens goes

	while (window.isOpen())
	{

		//event checker - checks and event is running and closes window if needed/on esc

		Event event;

		while (window.pollEvent(event))
		{

			if (event.type == Event::Closed) window.close();				//checks if event closed is true and if it is it closes the window

			if (Keyboard::isKeyPressed(Keyboard::Escape)) window.close();	//closes window if the escape key is pressed 

			//drops box if key released 

			if (event.type == Event::KeyReleased)		//if a key is released 
			{
				if (pendulum != NULL)					//if there is no active distance joint 
				{
					world.DestroyJoint(pendulum);		//destroys the distance joint
					pendulum = NULL;					//sets pendulum to null stops this from happening repeatedly
					j = j + 1;							//increments j as a new box will be made after this 
					makeNew = true;						//sets makenew to true so a new box will be created 
				}
			}

			//

		}

		//

		//sim loop for box2d

		for (int32 i = 0; i < 30; ++i)
		{

			world.Step(timeStep, velocityIterations, positionIterations);
			
			//making boxes 

			if (makeNew == true)																		//if makenew is true 
			{
				makeNew = false;																		//sets makenew to false as to not repeat 
				body[j] = world.CreateBody(&bodyDef);													//creates the body for the new box
				body[j]->SetFixedRotation(true);														//fixes rotation so no spinning 
				body[j]->CreateFixture(&fixtureDef);													//creates the fixture
				rect[j].setSize(Vector2f(50, 50));														//creates the rectangle for sfml 
				jointDef.Initialize(jointBody, body[j], jointBodyDef.position, bodyDef.position);		//defines initialization for the new distance joint
				pendulum = world.CreateJoint(&jointDef);												//creates new distance joint
			}

			//

			window.clear(); //clears window

			for (int k = 0; k < j; ++k)			//loops for the number of boxes in play
			{

				//gets and sets the position for both box2d and sfml

				int l = k + 1;								//sets l to the value of j by incrementing k
				b2Vec2 position = body[l]->GetPosition();	//gets the position of the box2d body 
				Vector2f sfmlPos = mtpfp(position);			//goes to a function that converts the value from metres(box2d) to pixels(sfml)
				rect[l].setPosition(sfmlPos);				//sets the sfml rectangle to match the box2d body
				
				//rotation turned off temoprarily until a fix for misalligned boxes is found
				
				//float angle = body[l]->GetAngle();		//gets the angle of the box from box2d
				//angle = angle * -57.2958;					//converts from anticlockwise radians to clockwise degrees	 
				//rect[l].setRotation(angle);				//sets the angle of the sfml rectangle 

				//

				

				//render - draws all of the objects on the screen

				window.draw(leftline);
				window.draw(rightline);
				window.draw(topline);
				window.draw(botline);
				window.draw(rect[l]);
				window.draw(test);

				//

			}

			window.display(); //displays drawn objects

		}

		//

	}

}



