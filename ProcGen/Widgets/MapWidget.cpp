#include "MapWidget.h"

#include <random>
#include <algorithm>
#include <qlabel.h>

Vector2D GetUnitVector(double degrees)
{
	double radians = degrees * M_PI / 180;
	return Vector2D{ cos(radians), sin(radians) };
}

#define NUM_GRADIENTS 16
std::array<Vector2D, NUM_GRADIENTS> MakePointGradients()
{
	std::array<Vector2D, NUM_GRADIENTS> gradients{};
	double degreeStepSize = 360.0 / NUM_GRADIENTS;
	for (size_t i = 0; i < NUM_GRADIENTS; i++)
	{
		gradients[i] = GetUnitVector(i * degreeStepSize);
	}
	return gradients;
}

const std::array<Vector2D, NUM_GRADIENTS> pointGradients = MakePointGradients();

MapWidget::MapWidget(QWidget* parent)
{
	width = 16;
	height = 16;
	blockSize = 40;

	noiseMap = std::vector<uchar>((width * blockSize + 1) * (height * blockSize + 1));

	layout = new QVBoxLayout(this);
	mapLabel = nullptr;

	// Perlin initialization
	p = std::vector<int>(2 * PERLIN_REP_COUNT);
	permutationArray = std::vector<int>(PERLIN_REP_COUNT);
}

MapWidget::~MapWidget()
{
	if (mapLabel != nullptr)
		delete mapLabel;

	delete layout;
}

void MapWidget::GenerateMap(int seed, float noise, float frequency)
{
	// Seed
	std::mt19937 mt(seed);
	for (int i = 0; i < PERLIN_REP_COUNT; i++)
		permutationArray[i] = i;
	std::shuffle(permutationArray.begin(), permutationArray.end(), mt);
	for (int i = 0; i < 2 * PERLIN_REP_COUNT; i++)
		p[i] = permutationArray[i % PERLIN_REP_COUNT];

	for (auto& i : noiseMap)
		i = 0;
	// Get individual points (move to grouping by blocks later)
	int w = width * blockSize + 1;
	int h = height * blockSize + 1;
	float blockStep = 1.0f / blockSize;
	for (int i = 0; i < w * h; i++)
	{
		int x_s = i % w;
		int y_s = i / w;
		float x = x_s * blockStep;
		float y = y_s * blockStep;
		noiseMap[i] = static_cast<uchar>((PerlinNoise(x, y) + 1.0) / 2.0 * 255);
	}

	QImage image = GetNoiseImage();
	
	if (mapLabel != nullptr)
	{
		layout->removeWidget(mapLabel);
		delete mapLabel;
	}
		
	mapLabel = new QLabel(this);
	mapLabel->setFixedSize(w, h);
	mapLabel->setScaledContents(true);
	mapLabel->setPixmap(QPixmap::fromImage(image));
	mapLabel->show();
	layout->addWidget(mapLabel);
	this->update();
}

QImage MapWidget::GetNoiseImage() const
{
	int w = width * blockSize + 1;
	int h = height * blockSize + 1;
	QImage image = QImage(noiseMap.data(), w, h, w, QImage::Format_Grayscale8);
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

	return Lerp(u, Lerp(v, botLeftDot, topLeftDot), Lerp(v, botRightDot, topRightDot));
}

double MapWidget::PerlinFade(double t) const
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}

double MapWidget::PerlinGrad(int hash, double x, double y) const
{
	return 0.0;
}

double MapWidget::Lerp(double t, double a1, double a2) const
{
	return a1 + t*(a2-a1);
}

Vector2D MapWidget::PerlinPointVector(int hash) const
{
	return pointGradients[hash % NUM_GRADIENTS];
}
