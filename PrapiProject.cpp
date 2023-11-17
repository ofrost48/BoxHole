#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

int main()
{
	//makes the window

	RenderWindow window(VideoMode(680, 900), "window");
	window.setFramerateLimit(60);

	//

	//makes the shapes

	RectangleShape rect;
	Vector2f rectanglePosition(300, 100);
	rect.setPosition(rectanglePosition);
	rect.setSize(Vector2f(100, 100));

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
	botline.setSize(Vector2f(620, 5));
	
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

}

