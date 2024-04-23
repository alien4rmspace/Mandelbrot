#include "complex_plane.h"

ComplexPlane::ComplexPlane(unsigned short pixelWidth, unsigned short pixelHeight) {
	this->m_pixelWidth = pixelWidth;
	this->m_pixelHeight = pixelHeight;
	this->m_aspectRatio = float(pixelHeight) / float(pixelWidth);
	this->m_planeCenter = { 0, 0 };
	this->m_planeSize = { BASE_WIDTH, BASE_HEIGHT * m_aspectRatio };
	this->m_zoomCount = 0;
	this->m_state = State::CALCULATING;
	this->m_vArray.setPrimitiveType(Points);
	this->m_vArray.resize(pixelWidth * pixelHeight);
}

void ComplexPlane::draw(RenderTarget& target, RenderStates state) const {
	target.draw(m_vArray);
}

void ComplexPlane::updateRender() {
	if (m_state == CALCULATING) {

		Clock clock;

		int incrementBy = 4;

		//for (int y = 0; y < this->m_pixelHeight; y++) {
		//	for (int x = 0; x < this->m_pixelWidth; x++) {
		//		this->m_vArray[x + y * m_pixelWidth].position = { (float)x, (float)y };
		//		Vector2i screenCoord = { x, y };
		//		Vector2f mapPixel = mapPixelToCoords(screenCoord);

		//		Uint8 r, g, b = 0;
		//		iterationsToRGB(countIterations(mapPixel), r, g, b);
		//		this->m_vArray[x + y * m_pixelWidth].color = { r, g, b };
		//	}
		//}

		auto threadUpdate1_function = [this]() {
			for (int y = 0; y < this->m_pixelHeight; y += 4) {
				for (int x = 0; x < this->m_pixelWidth; x++) {
					this->m_vArray[x + y * m_pixelWidth].position = { (float)x, (float)y };
					Vector2i screenCoord = { x, y };
					Vector2f mapPixel = mapPixelToCoords(screenCoord);

					Uint8 r, g, b = 0;
					iterationsToRGB(countIterations(mapPixel), r, g, b);
					this->m_vArray[x + y * m_pixelWidth].color = { r, g, b };
				}
			}
			};

		auto threadUpdate2_function = [this]() {
			for (int y = 1; y < this->m_pixelHeight; y += 4) {
				for (int x = 0; x < this->m_pixelWidth; x++) {
					this->m_vArray[x + y * m_pixelWidth].position = { (float)x, (float)y };
					Vector2i screenCoord = { x, y };
					Vector2f mapPixel = mapPixelToCoords(screenCoord);

					Uint8 r, g, b = 0;
					iterationsToRGB(countIterations(mapPixel), r, g, b);
					this->m_vArray[x + y * m_pixelWidth].color = { r, g, b };
				}
			}
			};
		auto threadUpdate3_function = [this]() {
			for (int y = 2; y < this->m_pixelHeight; y += 4) {
				for (int x = 0; x < this->m_pixelWidth; x++) {
					this->m_vArray[x + y * m_pixelWidth].position = { (float)x, (float)y };
					Vector2i screenCoord = { x, y };
					Vector2f mapPixel = mapPixelToCoords(screenCoord);

					Uint8 r, g, b = 0;
					iterationsToRGB(countIterations(mapPixel), r, g, b);
					this->m_vArray[x + y * m_pixelWidth].color = { r, g, b };
				}
			}
			};

		auto threadUpdate4_function = [this]() {
			for (int y = 3; y < this->m_pixelHeight; y += 4) {
				for (int x = 0; x < this->m_pixelWidth; x++) {
					this->m_vArray[x + y * m_pixelWidth].position = { (float)x, (float)y };
					Vector2i screenCoord = { x, y };
					Vector2f mapPixel = mapPixelToCoords(screenCoord);

					Uint8 r, g, b = 0;
					iterationsToRGB(countIterations(mapPixel), r, g, b);
					this->m_vArray[x + y * m_pixelWidth].color = { r, g, b };
				}
			}
			};

		thread threadUpdate1(threadUpdate1_function);
		thread threadUpdate2(threadUpdate2_function);
		thread threadUpdate3(threadUpdate3_function);
		thread threadUpdate4(threadUpdate4_function);

		threadUpdate1.join();
		threadUpdate2.join();
		threadUpdate3.join();
		threadUpdate4.join();

		Time timer = clock.getElapsedTime();
		cout << "Milliseconds taken to update " << timer.asMilliseconds() << endl;

		m_state = DISPLAYING;
	}
}

void ComplexPlane::zoomIn() {
	this->m_zoomCount++;
	
	float newX = BASE_WIDTH * (pow(BASE_ZOOM, this->m_zoomCount));
	float newY = BASE_HEIGHT * this->m_aspectRatio * (pow(BASE_ZOOM, this->m_zoomCount));


	this->m_planeSize = { newX , newY };
	m_state = CALCULATING;
}

void ComplexPlane::zoomOut() {
	this->m_zoomCount--;
	
	float newX = BASE_WIDTH * (pow(BASE_ZOOM, this->m_zoomCount));
	float newY = BASE_HEIGHT * this->m_aspectRatio * (pow(BASE_ZOOM, this->m_zoomCount));


	this->m_planeSize = { newX , newY };
	m_state = CALCULATING;
}

void ComplexPlane::setCenter(Vector2i mousePixel) {
	Vector2f complexCoord = mapPixelToCoords(mousePixel);

	this->m_planeCenter = complexCoord;
	m_state = CALCULATING;
}

void ComplexPlane::setMouseLocation(Vector2i mousePixel) {
	Vector2f complexCoord = mapPixelToCoords(mousePixel);

	this->m_mouseLocation = complexCoord;
}

void ComplexPlane::loadText(Text& text) {
	stringstream stream;
	stream << "Mandelbrot Set" << endl;
	stream << "Center: (" << m_planeCenter.x << "," << m_planeCenter.y << ")" << endl;
	stream << fixed << setprecision(2) << "Cursor: (" << m_mouseLocation.x << "," << m_mouseLocation.y << ")" << endl;
	stream << "Left-click to Zoom in" << endl;
	stream << "Right-click to Zoom out" << endl;

	text.setString(stream.str());
}

size_t ComplexPlane::countIterations(Vector2f coord) {
	size_t iterations = 0;
	const double threshold = 2.0;

	double re = coord.x;
	double im = coord.y;

	complex<double> c(re, im);
	complex<double> z(0, 0);

	while (abs(z) < threshold && iterations < MAX_ITER) {
		z = z * z + c;
		iterations++;
	}
	return iterations;
}

void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b) {
	if (count >= 64){
		r = 0;
		b = 0;
		g = 0;
	}
	else if (count >= 54) {
		r = 255;
		b = 0;
		g = 0;
	}
	else if (count >= 45) {
		r = 255;
		b = 255;
		g = 0;
	}
	else if (count >= 34) {
		r = 255;
		b = 255;
		g = 255;
	}
	else if (count >= 24) {
		r = 0;
		b = 255;
		g = 255;
	}
	else if (count >= 14) {
		r = 112;
		b = 128;
		g = 144;

	}
	else if (count >= 0) {
		r = 220;
		b = 220;
		g = 220;
	}
}

Vector2f ComplexPlane::mapPixelToCoords(Vector2i mousePixel) {
	Vector2i displayPixel_x = { 0, m_pixelWidth }; // I'm not sure how to dynamically change this, this is just assuming monitor is 1080p
	Vector2i displayPixel_y = { m_pixelHeight, 0 };
	
	float normalized_x = static_cast<float>(mousePixel.x - displayPixel_x.x) / (displayPixel_x.y - displayPixel_x.x);
	float normalized_y = static_cast<float>(mousePixel.y - displayPixel_y.x) / (displayPixel_y.y - displayPixel_y.x);

	float mapped_x = normalized_x * (this->m_planeSize.x) + (this->m_planeCenter.x - this->m_planeSize.x / 2);
	float mapped_y = normalized_y * (this->m_planeSize.y) + (this->m_planeCenter.y - this->m_planeSize.y / 2);
	
	return Vector2f (mapped_x , mapped_y);
}