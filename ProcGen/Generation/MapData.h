#pragma once

#include <memory>
#include <Generation/MapDimensions.h>

struct MapData
{
	std::shared_ptr<float[]> heightmap;
	MapDimensions dimensions;
};