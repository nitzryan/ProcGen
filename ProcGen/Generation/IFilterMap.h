#pragma once

#include <random>
#include <exception>
#include <qdebug.h>

class IFilterMap
{
public:
	virtual ~IFilterMap() {
		qDebug() << "Goodbye";
		
		if (data != nullptr)
			delete data;

		data = nullptr;
	}
	float* GetMap() const { return data; }
	void GenerateMap(int w, int h, int seed) {
		if (w <= 0)
			throw new std::exception("0 Or Negative Width in GenerateMap");
		if (h <= 0)
			throw new std::exception("0 Or Negative Height in GenerateMap");
		
		// Regenerate values if changed
		if (w != mapWidth || h != mapHeight)
		{
			mapWidth = w;
			mapHeight = h;
			if (data != nullptr)
				delete data;
			data = new float[mapWidth * mapHeight];
		}

		// Zero out data
		for (int i = 0; i < mapWidth * mapHeight; i++)
			data[i] = 0;

		random = std::mt19937(seed);

		GenMap();
	}

protected:
	virtual void GenMap() = 0;

	int mapWidth = 0, mapHeight = 0;
	float* data = nullptr;

	std::mt19937 random;
};