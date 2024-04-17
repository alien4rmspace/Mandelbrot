#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "complex_plane.h"

using namespace std;
using namespace sf;

void handleInput(RenderWindow& window) {
	Event event;

	while (window.pollEvent(event)) {
		switch (event.type) {
			case Event::Closed: {
				window.close();
				break;
			}
			case Event::KeyPressed:
				if (event.key.code == Keyboard::Escape) {
					window.close();
					break;
				}
				// Spaceholder if we plan to add more screens.
				if (event.key.code == Keyboard::Space) {
					cout << "loaded" << endl;
					break;
				}
			case Event::MouseButtonPressed: {
				if (event.mouseButton.button == Mouse::Left) {
					cout << "Left mouse button was pressed" << endl;
					cout << "mouse x: " << event.mouseButton.x << endl;
					cout << "mouse y: " << event.mouseButton.y << endl;

					// Implement zoomIn() here
				}
				if (event.mouseButton.button == Mouse::Right) {
					cout << "Right mouse button was pressed" << endl;
					cout << "mouse x: " << event.mouseButton.x << endl;
					cout << "mouse y: " << event.mouseButton.y << endl;

					// Implement zoomOut() here
				}
				break;
			}
			default: break;
		}
	}
}

int main() {
	RenderWindow window;
	unsigned short windowWidth = VideoMode::getDesktopMode().width / 2;
	unsigned short windowHeight = VideoMode::getDesktopMode().height / 2;
	window.create(VideoMode(windowWidth, windowHeight), "Mandelbrot", Style::Default);

	ComplexPlane complexPlane(windowWidth, windowHeight);

	while (window.isOpen()) {
		handleInput(window);
		complexPlane.updateRender();
		//complexPlane.loadText();

		// Drawing Stage
		window.clear();
		window.draw(complexPlane);
		//window.draw(text);

		window.display();
	}

	return 0;
}