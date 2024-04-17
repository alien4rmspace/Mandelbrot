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
			}
		}
	}
}