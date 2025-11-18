#pragma once

#include <random>
#include <exception>
#include <qdebug.h>

class IFilterMap
{
public:
	virtual ~IFilterMap() {}
	void GenerateMap(int w, int h, int seed, float* data) {
		if (w <= 0)
			throw new std::exception("0 Or Negative Width in GenerateMap");
		if (h <= 0)
			throw new std::exception("0 Or Negative Height in GenerateMap");

		mapWidth = w;
		mapHeight = h;
		random = std::mt19937(seed);
		GenMap(data);
	}

protected:
	virtual void GenMap(float* data) = 0;

	int mapWidth = 0, mapHeight = 0;

	std::mt19937 random;
};