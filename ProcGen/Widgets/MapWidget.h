#pragma once

#include <QtWidgets/qwidget.h>
#include <vector>

struct Vector2D
{
	float x;
	float y;

	static float Dot(const Vector2D& a, const Vector2D& b)
	{
		return (a.x * b.x) + (a.y * b.y);
	}
};

class MapWidget : public QWidget
{
	Q_OBJECT

public:
	MapWidget(QWidget* parent = nullptr);
	~MapWidget();

	void GenerateMap(int seed, float noise, float frequency);
	
private:
	int width, height;
	int blockSize;
	std::vector<uchar> noiseMap;
	QImage GetNoiseImage() const;

	// Perlin generation variables
	std::vector<int> permutationArray;
	std::vector<int> p;
	const int PERLIN_REP_COUNT = 256;
	double PerlinNoise(double x, double y) const;
	double PerlinFade(double t) const;
	double PerlinGrad(int hash, double x, double y) const;
	double Lerp(double t, double a1, double a2) const;
	Vector2D PerlinPointVector(int hash) const;
};

