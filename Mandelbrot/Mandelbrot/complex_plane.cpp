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