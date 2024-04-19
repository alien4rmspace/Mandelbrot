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
		for (int y = 0; y < this->m_pixelHeight; y++) {
			for (int x = 0; x < this->m_pixelWidth; x++) {
				this->m_vArray[x + y * m_pixelWidth].position = { (float)x, (float)y };
				// mapPixelToCoords here
				//countIterations();
				Uint8 r, g, b = 0;
				//iterationsToRGB(countIterations(mapPixelToCoords()), r, g, b);
				this->m_vArray[x + y * m_pixelWidth].color = { r, g, b };
			}
		}

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
	if (count > 54 && count < MAX_ITER){
		r = 255;
	}
	if (count > 44 && count < MAX_ITER) {
		b = 255;
	}
	if (count > 34 && count < MAX_ITER) {
		r = 0;
	}
	if (count > 24 && count < MAX_ITER) {
		g = 255;
	}
	if (count > 14 && count < MAX_ITER) {
		b = 0;
	}
	if (count > 4 && count < MAX_ITER) {
		r = 255;
	}
}

Vector2f ComplexPlane::mapPixelToCoords(Vector2i mousePixel) {
	Vector2i displayPixel_x = { 0, 1920 }; // I'm not sure how to dynamically change this, this is just assuming monitor is 1080p
	Vector2i displayPixel_y = { 1080, 0 };
	
	float normalized_x = static_cast<float>(mousePixel.x - displayPixel_x.x) / (displayPixel_x.y - displayPixel_x.x);
	float normalized_y = static_cast<float>(mousePixel.y - displayPixel_y.x) / (displayPixel_y.y - displayPixel_y.x);

	float mapped_x = normalized_x * (this->m_planeSize.x) + (this->m_planeCenter.x - this->m_planeSize.x / 2);
	float mapped_y = normalized_y * (this->m_planeSize.y) + (this->m_planeCenter.y - this->m_planeSize.y / 2);

	return Vector2f (mapped_x , mapped_y);
}