#pragma once

#include <vector>

struct Vector2D
{
	double x;
	double y;

	static double Dot(const Vector2D& a, const Vector2D& b)
	{
		return (a.x * b.x) + (a.y * b.y);
	}
};

class PerlinPass
{
public:
	PerlinPass(int numGradients, int perlinRepCount);
	void Reinitialize(int numGradients, int perlinRepCount);

	float* GenerateMap(int seed, int width, int height, double blockSize, double outputScale);
private:
	int width, height;
	std::vector<float> output;
	std::vector<int> permutationArray;
	std::vector<int> p;
	std::vector<Vector2D> pointGradients;
	int numGradients, perlinRepCount;

	float PerlinNoise(float x, float y) const;
	float PerlinFade(float t) const;
	float Lerp(float t, float a1, float a2) const;
	Vector2D PerlinPointVector(int hash) const;
};

