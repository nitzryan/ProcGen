#include "MountainFilterMap.h"

#include <Utilities.h>
#include <qmath.h>

MountainFilterMap::MountainFilterMap()
{

}

MountainFilterMap::~MountainFilterMap()
{

}

void MountainFilterMap::SetArgs(MountainFilterMapArgs& a)
{
	Utilities::SetMinMax(args.minRanges, args.maxRanges, a.minRanges, a.maxRanges, "Num Ranges");
	Utilities::SetMinMax(args.minLength, args.maxLength, a.minLength, a.maxLength, "Range Length");
	Utilities::SetMinMax(args.minWidth, args.maxWidth, a.minWidth, a.maxWidth, "Range Width");
	Utilities::SetMinMax(args.minBend, args.maxBend, a.minBend, a.maxBend, "Bend Amount");
	Utilities::SetMinMax(args.minScale, args.maxScale, a.minScale, a.maxScale, "Range Scale");
	Utilities::SetMinMax(args.minRangeMult, args.maxRangeMult, a.minRangeMult, a.maxRangeMult, "Range Multiplier");
	Utilities::SetMinMax(args.minBendFreq, args.maxBendFreq, a.minBendFreq, a.maxBendFreq, "Bend Frequency");
}

void MountainFilterMap::GenMap(float* data)
{
	if (!args.IsValid())
	{
		throw new std::exception("Map Generation Requested without all info entered");
	}

	// Seed
	std::uniform_int_distribution<int> rangeDist(args.minRanges, args.maxRanges);
	int numRanges = rangeDist(random);

	std::uniform_real_distribution<float> xStartDist(0, mapWidth), 
		yStartDist(0, mapHeight), 
		angleDist(0, 2 * M_PI), 
		lengthDist(args.minLength, args.maxLength), 
		widthDist(args.minWidth, args.maxWidth),
		scaleDist(args.minScale, args.maxScale),
		multDist(args.minRangeMult, args.maxRangeMult);

	// Apply Ranges
	for (int i = 0; i < numRanges; i++)
	{
		// Generate dimensions for range
		float xStart = xStartDist(random);
		float yStart = yStartDist(random);
		float angle = angleDist(random);
		float length = lengthDist(random);
		float width = widthDist(random);
		float scale = scaleDist(random);
		float rangeMult = scaleDist(random);

		float rangeXVec = std::cos(angle);
		float rangeYVec = std::sin(angle);

		// Getcoordinates of mountain start in mountain coordinates
		float x0 = (xStart * rangeXVec) + (yStart * rangeYVec);
		float y0 = (yStart * rangeXVec) - (xStart * rangeYVec);

		for (int n = 0; n < mapWidth * mapHeight; n++)
		{
			int xTest = n % mapWidth;
			int yTest = n / mapWidth;

			// Convert to mountain coords
			float xMountainTest = (xTest * rangeXVec) + (yTest * rangeYVec) - x0;
			float yMountainTest = (yTest * rangeXVec) - (xTest * rangeYVec) - y0;

			float deltaY = std::abs(yMountainTest);
			if (xMountainTest > 0 && xMountainTest < length && deltaY < width)
			{
				float mountainVal = (width - deltaY) / width;
				mountainVal *= rangeMult;
				mountainVal = std::min(mountainVal, scale);
				data[n] = std::max(data[n], mountainVal);
			}
			else if (deltaY < width)
			{
				float deltaX = std::max(-xMountainTest, xMountainTest - length);
				float r = std::sqrt((deltaX * deltaX) + (deltaY * deltaY));
				if (r < width)
				{
					float mountainVal = (width - r) / width;
					mountainVal *= rangeMult;
					mountainVal = std::min(mountainVal, scale);
					data[n] = std::max(data[n], mountainVal);
				}
			}
		}
	}
}

