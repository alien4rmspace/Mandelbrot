#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;
using namespace std;

const unsigned int MAX_ITER = 64;
const float BASE_WIDTH = 4.0;
const float BASE_HEIGHT = 4.0;
const float BASE_ZOOM = 0.5;

enum State {CALCULATING, DISPLAYING};

class ComplexPlane : public Drawable {
public:
	ComplexPlane(unsigned short pixelWidth, unsigned short pixelHeight);
	void draw(RenderTarget& target, RenderStates states) const override;
	void updateRender();

private:
	unsigned short m_pixelWidth;
	unsigned short m_pixelHeight;
	unsigned short m_zoomCount;
	float m_aspectRatio;

	State m_state;

	Vector2i m_pixel_size;

	Vector2f m_mouseLocation;
	Vector2f m_planeCenter = { 0.f, 0.f };
	Vector2f m_planeSize;
	
	VertexArray m_vArray;

	//int countIterations(Vector2f coord);
	//void iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b);
};