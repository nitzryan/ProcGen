#include "MapWidget.h"

#include <random>
#include <algorithm>
#include <qlabel.h>

const Vector2D pointGradients[] = { {1, 1}, {1,-1}, {-1,1}, {-1,-1} };

MapWidget::MapWidget(QWidget* parent)
{
	width = 16;
	height = 16;
	blockSize = 40;

	// Perlin initialization
	p = std::vector<int>(2 * PERLIN_REP_COUNT);
	permutationArray = std::vector<int>(PERLIN_REP_COUNT);
}

MapWidget::~MapWidget()
{
	if (noiseMap != nullptr)
		delete noiseMap;

}

void MapWidget::GenerateMap(int seed, float noise, float frequency)
{
	// Ensure map is deleted if already called
	if (noiseMap != nullptr)
		delete noiseMap;

	noiseMap = nullptr;

	// Seed
	std::mt19937 mt(seed);
	for (int i = 0; i < PERLIN_REP_COUNT; i++)
		permutationArray[i] = i;
	std::shuffle(permutationArray.begin(), permutationArray.end(), mt);
	for (int i = 0; i < 2 * PERLIN_REP_COUNT; i++)
		p[i] = permutationArray[i % PERLIN_REP_COUNT];

	// 
	int w = width * blockSize + 1;
	int h = height * blockSize + 1;
	noiseMap = new uchar[w * h];
	float blockStep = 1.0f / blockSize;
	for (int i = 0; i < w * h; i++)
	{
		int x_s = i % w;
		int y_s = i / w;
		float x = x_s * blockStep;
		float y = y_s * blockStep;
		noiseMap[i] = static_cast<uchar>(PerlinNoise(x, y) * 255);
	}

	QImage image = GetNoiseImage();
	QLabel* label = new QLabel(this);
	label->setFixedSize(w, h);
	label->setScaledContents(true);
	label->setPixmap(QPixmap::fromImage(image));
	label->show();
	this->update();
}

QImage MapWidget::GetNoiseImage() const
{
	QImage image = QImage(noiseMap, width * blockSize + 1, height * blockSize + 1, QImage::Format_Grayscale8);
	return image;
}

double MapWidget::PerlinNoise(double x, double y) const
{
	int xi = (int)x & 255;
	int yi = (int)y & 255;

	double xf = x - (int)x;
	double yf = y - (int)y;

	double u = PerlinFade(xf);
	double v = PerlinFade(yf);

	int aa, ab, ba, bb;
	aa = p[p[xi] + yi];
	ab = p[p[xi] + yi + 1];
	ba = p[p[xi + 1] + yi];
	bb = p[p[xi + 1] + yi + 1];

	Vector2D topRightDisp = { xf - 1.0, yf - 1.0 };
	Vector2D topLeftDisp = { xf, yf - 1.0 };
	Vector2D botLeftDisp = { xf, yf };
	Vector2D botRightDisp = { xf - 1.0, yf };

	Vector2D topLeftValue = PerlinPointVector(ab);
	Vector2D topRightValue = PerlinPointVector(bb);
	Vector2D botLeftValue = PerlinPointVector(aa);
	Vector2D botRightValue = PerlinPointVector(ba);

	float topLeftDot = Vector2D::Dot(topLeftDisp, topLeftValue);
	float topRightDot = Vector2D::Dot(topRightDisp, topRightValue);
	float botLeftDot = Vector2D::Dot(botLeftDisp, botLeftValue);
	float botRightDot = Vector2D::Dot(botRightDisp, botRightValue);

	float topProportion = PerlinFade(yf);
	float rightProportion = PerlinFade(xf);
	float botProportion = 1 - topProportion;
	float leftProportion = 1 - rightProportion;

	return (topProportion * rightProportion * topRightDot) +
		(topProportion * leftProportion * topLeftDot) +
		(botProportion * rightProportion * botRightDot) +
		(botProportion * leftProportion * botLeftDot);
}

double MapWidget::PerlinFade(double t) const
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}

double MapWidget::PerlinGrad(int hash, double x, double y) const
{
	return 0.0;
}

Vector2D MapWidget::PerlinPointVector(int hash) const
{
	return pointGradients[hash % 4];
}
