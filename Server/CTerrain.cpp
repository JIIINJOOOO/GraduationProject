#include "CTerrain.h"
#include <vector>
bool CTerrain::LoadMap(const char* fname) {
	ifstream file;
	file.open(fname, ios::binary);
	if (!file.is_open()) return false;

	// Initialize Map Size
	m_width = m_height = PIXEL_LENGTH;
	unsigned int imageSize = m_height * m_width;
	cout << m_width << "\t" << m_height << "\t" << imageSize << endl;

	// 비트맵 이미지 메모리할당
	unsigned short* bitmapImage = new unsigned short[imageSize];
	if (!bitmapImage) return false;
	file.seekg(fstream::beg);
	file.read((char*)bitmapImage, imageSize*2);
	file.close();
	
	// 0 - 30833   -29791
	m_mapPixels = new unsigned short[imageSize];
	for (int i = 0; i < imageSize; ++i)
		m_mapPixels[i] = bitmapImage[i];
	// NormalizeMap();
	// cout << OnGetHeight(0,0) << endl;
	// cout << OnGetHeight(m_width - 1, 0) << endl;
	// cout << OnGetHeight(0, m_height - 1) << endl;
	// cout << OnGetHeight(m_width-1, m_height-1) << endl;
	// cout << OnGetHeight(824, 1235) << endl;
	// cout << OnGetHeight(509,509) << endl;

	delete[] bitmapImage;
	bitmapImage = NULL;
	return true;
}

void CTerrain::NormalizeMap() {
	int size = m_width * m_height;
	for (int i = 0; i < size; ++i) {
		m_mapPixels[i] -= 32767;
		if (m_mapPixels[i] < 0) m_mapPixels[i] += 512;
		else m_mapPixels[i] -= 512;
	}
}

float CTerrain::GetHeight(float fx, float fy) {
	if (fx < 0.f || fy < 0.f || fx >= m_width || fy >= m_height)
		return 0.f;

	int x = static_cast<int>(fx);
	int y = static_cast<int>(fy);
	float xPersent = fx - x;
	float yPersent = fy - y;
	float bottomLeft = static_cast<float>(m_mapPixels[x + (y*m_width)]);
	float bottomRight = static_cast<float>(m_mapPixels[(x + 1) + (y*m_width)]);
	float topLeft = static_cast<float>(m_mapPixels[x + ((y + 1)*m_width)]);
	float topRight = static_cast<float>(m_mapPixels[(x + 1) + ((y + 1)*m_width)]);

	bool right2Left = ((y % 2) != 0);

	if (right2Left) {
		if (yPersent >= xPersent)
			bottomRight = bottomLeft + (topRight - topLeft);
		else topLeft = topRight + (bottomLeft - bottomRight);
	}
	else {
		if (yPersent < (1.f - xPersent))
			topRight = topLeft + (bottomRight - bottomLeft);
		else bottomLeft = topLeft + (bottomRight - topRight);
	}

	float topHeight = topLeft * (1 - xPersent) + topRight * xPersent;
	float bottomHeight = bottomLeft * (1 - xPersent) + bottomRight * xPersent;
	float height = bottomHeight * (1 - yPersent) + topHeight * yPersent;
	return height;
}

float CTerrain::GetHeightNormal(int x, int y) {
	if (x < 0 || y < 0 || x >= m_width || y >= m_height)
		return -1;

	int index = x + (y*m_width);
	int xAdd = (x < (m_width - 1)) ? 1 : -1;
	int yAdd = (y < (m_height = 1)) ? m_width : -m_width;

	float z1 = (float)m_mapPixels[index]*100;
	float z2 = (float)m_mapPixels[index + xAdd]*100;
	float z3 = (float)m_mapPixels[index + yAdd]*100;
	float result = 0;
	return result;
}

float CTerrain::OnGetHeight(int x, int y) {
	// 여기서 scale을 곱해야하는데 scale을 어디서 구해야하는지 모르겠다
	// 2422.905762
	
	float height = m_mapPixels[x + (y*m_width)];
	cout << height << "\t";
	//height -= 32767;
	//height -= 16383;
	// height -= 30739;

	height -= 30000;
	height /= 1.28f;
	height -= 2070;
	// height /= 12.8f;
	//
	// if (height < 0) height += 512;
	// else height -= 512;
	return height;
}

unsigned short* CTerrain::GetMap() const {
	return m_mapPixels;
}