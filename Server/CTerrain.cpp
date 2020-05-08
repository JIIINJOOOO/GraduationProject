#include "CTerrain.h"
bool CTerrain::LoadMap(const char* fname) {
	// ifstream mapFile;
	// mapFile.open(fname, fstream::binary);	// 터레인파일 바이너릴 오픈
	// if (!mapFile.is_open()) return false;
	FILE* filePtr = nullptr;
	if (fopen_s(&filePtr, fname, "rb") != 0) return false;

	BITMAPFILEHEADER fileHeader;
	if (fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr) != 1)
		return false;

	BITMAPINFOHEADER infoHeader;
	if (fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr) != 1)
		return false;

	// Initialize Map Size
	m_width = infoHeader.biWidth;
	m_height = infoHeader.biHeight;
	int imageSize = m_height * m_width * 3;
	
	// 비트맵 이미지 메모리할당
	unsigned char* bitmapImage = new unsigned char[imageSize];
	if (!bitmapImage) return false;

	fseek(filePtr, fileHeader.bfOffBits, SEEK_SET);
	if (fread(bitmapImage, 1, imageSize, filePtr) != imageSize)
		return false;
	if (fclose(filePtr) != 0) return false;

	m_map = new Position[m_width*m_height];
	if (!m_map) return false;
	
	int dataBuf = 0;
	for (int x = 0; x < m_width; ++x)
		for (int y = 0; y < m_height; ++y) {
			unsigned char height = bitmapImage[dataBuf];
			int idx = (m_height*y) + x;
			m_map[idx].x = static_cast<float>(x);
			m_map[idx].y = static_cast<float>(y);
			m_map[idx].z = static_cast<float>(height);
			dataBuf += 3;
		}

	delete[] bitmapImage;
	bitmapImage = NULL;
	return true;
}

void CTerrain::NormalizeMap() {
	for (int x = 0; x < m_width; ++x)
		for (int y = 0; y < m_height; ++y)
			m_map[(m_height*y) + x].z /= 15.f;
}

bool CTerrain::InitializeBuffers() {
	m_vertexCount = (m_width - 1)*(m_height - 1) * 12;
	m_indexCount = m_vertexCount;

	Vertex* vertex = new Vertex[m_vertexCount];
	if (!vertex) return false;

	unsigned long* index = new unsigned long[m_indexCount];
	if (!index) return false;

	int idx = 0;
	for (int x = 0; x < m_width - 1; ++x)
		for (int y = 0; y < m_height - 1; ++y) {
			int mapidx[4];
			mapidx[0] = (m_height*y) + x;	// 왼쪽 아래
			mapidx[1] = (m_height*y) + x + 1;	// 오른쪽 아래
			mapidx[2] = (m_height*(y + 1)) + x;	// 왼쪽 위
			mapidx[3] = (m_height*(y + 1)) + x + 1;	// 오른쪽 위

			// 왼쪽 위
			vertex[idx].pos = Position(m_map[mapidx[2]]);
			vertex[idx].color = Color(1.f, 1.f, 1.f, 1.f);
			index[idx] = idx;
			idx++;
			// 오른쪽 위
			vertex[idx].pos = Position(m_map[mapidx[3]]);
			vertex[idx].color = Color(1.f, 1.f, 1.f, 1.f);
			index[idx] = idx;
			idx++;
			// 왼 아
			vertex[idx].pos = Position(m_map[mapidx[0]]);
			vertex[idx].color = Color(1.f, 1.f, 1.f, 1.f);
			index[idx] = idx;
			idx++;
			// 왼 아
			vertex[idx].pos = Position(m_map[mapidx[0]]);
			vertex[idx].color = Color(1.f, 1.f, 1.f, 1.f);
			index[idx] = idx;
			idx++;
			// 왼 위
			vertex[idx].pos = Position(m_map[mapidx[2]]);
			vertex[idx].color = Color(1.f, 1.f, 1.f, 1.f);
			index[idx] = idx;
			idx++;
			// 왼 아
			vertex[idx].pos = Position(m_map[mapidx[0]]);
			vertex[idx].color = Color(1.f, 1.f, 1.f, 1.f);
			index[idx] = idx;
			idx++;
			// 오 위
			vertex[idx].pos = Position(m_map[mapidx[3]]);
			vertex[idx].color = Color(1.f, 1.f, 1.f, 1.f);
			index[idx] = idx;
			idx++;
			// 오 우;
			vertex[idx].pos = Position(m_map[mapidx[3]]);
			vertex[idx].color = Color(1.f, 1.f, 1.f, 1.f);
			index[idx] = idx;
			idx++;
			// 오 아
			vertex[idx].pos = Position(m_map[mapidx[1]]);
			vertex[idx].color = Color(1.f, 1.f, 1.f, 1.f);
			index[idx] = idx;
			idx++;
			// 오 아
			vertex[idx].pos = Position(m_map[mapidx[0]]);
			vertex[idx].color = Color(1.f, 1.f, 1.f, 1.f);
			index[idx] = idx;
			idx++;
			// 왼 아
			vertex[idx].pos = Position(m_map[mapidx[0]]);
			vertex[idx].color = Color(1.f, 1.f, 1.f, 1.f);
			index[idx] = idx;
			idx++;
		}


}