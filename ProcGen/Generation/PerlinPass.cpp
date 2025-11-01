#include "PerlinPass.h"

#include <random>
#include <algorithm>
#include <qmath.h>

Vector2D GetUnitVector(double degrees)
{
	double radians = degrees * M_PI / 180;
	return Vector2D{ cos(radians), sin(radians) };
}

std::vector<Vector2D> MakePointGradients(int numGradients)
{
	std::vector<Vector2D> gradients(numGradients);
	double degreeStepSize = 360.0 / numGradients;
	for (size_t i = 0; i < numGradients; i++)
	{
		gradients[i] = GetUnitVector(i * degreeStepSize);
	}
	return gradients;
}

PerlinPass::PerlinPass(int numGradients, int perlinRepCount)
{
	Reinitialize(numGradients, perlinRepCount);

	width = 1;
	height = 1;
}

void PerlinPass::Reinitialize(int numGradients, int perlinRepCount)
{
	pointGradients = MakePointGradients(numGradients);
	p = std::vector<int>(2 * perlinRepCount);
	permutationArray = std::vector<int>(perlinRepCount);

	this->numGradients = numGradients;
	this->perlinRepCount = perlinRepCount;
}

float* PerlinPass::GenerateMap(int seed, int w, int h, double blockSize, double outputScale)
{
	if (w != width || h != height)
	{
		width = w;
		height = h;
		output = std::vector<float>(width * height);
	}

	// Seed permutation array
	std::mt19937 mt(seed);
	for (int i = 0; i < perlinRepCount; i++)
		permutationArray[i] = i;
	std::shuffle(permutationArray.begin(), permutationArray.end(), mt);
	for (int i = 0; i < 2 * perlinRepCount; i++)
		p[i] = permutationArray[i % perlinRepCount];

	
	// Get individual points (move to grouping by blocks later)
	for (auto& i : output)
		i = 0;
	for (int i = 0; i < width * height; i++)
	{
		float x = (i % width) / blockSize;
		float y = (i / width) / blockSize;
		output[i] = (PerlinNoise(x, y) + 1.0f) / 2.0f * outputScale;
	}

	return output.data();
}

float PerlinPass::PerlinNoise(float x, float y) const
{
	int xi = (int)x & 255;
	int yi = (int)y & 255;

	float xf = x - (int)x;
	float yf = y - (int)y;

	float u = PerlinFade(xf);
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

float PerlinPass::PerlinFade(float t) const
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}

float PerlinPass::Lerp(float t, float a1, float a2) const
{
	return a1 + t * (a2 - a1);
}

Vector2D PerlinPass::PerlinPointVector(int hash) const
{
	return pointGradients[hash % numGradients];
}