#include "MountainFilterMap.h"

#include <Utilities.h>
#include <qmath.h>

MountainFilterMap::MountainFilterMap()
{
	minRanges = 0;
	maxRanges = 0;
	lengthMin = 0;
	lengthMax = 0;
	widthMin = 0;
	widthMax = 0;
	lengthPerBend = 0;
	bendAmountMin = 0;
	bendAmountMax = 0;
}

MountainFilterMap::~MountainFilterMap()
{

}

void MountainFilterMap::SetNumRanges(int min, int max)
{
	Utilities::SetMinMax(minRanges, maxRanges, min, max, "Num Ranges");
}

void MountainFilterMap::SetRangeLength(float min, float max)
{
	Utilities::SetMinMax(lengthMin, lengthMax, min, max, "Range Length");
}

void MountainFilterMap::SetRangeWidth(float min, float max)
{
	Utilities::SetMinMax(widthMin, widthMax, min, max, "Range Width");
}

void MountainFilterMap::SetLengthPerBend(float len)
{
	if (len < 0)
		throw new std::exception("Length Per Bend < 0");

	lengthPerBend = len;
}

void MountainFilterMap::SetBendAmount(float min, float max)
{
	Utilities::SetMinMax(bendAmountMin, bendAmountMax, min, max, "Bend Amount");
}

void MountainFilterMap::GenMap()
{
	if (minRanges == 0 || maxRanges == 0 ||
		lengthMin == 0 || lengthMax == 0 ||
		widthMin == 0 || widthMax == 0 ||
		lengthPerBend == 0 ||
		bendAmountMin == 0 ||
		bendAmountMax == 0)
	{
		throw new std::exception("Map Generation Requested without all info entered");
	}

	// Seed
	std::uniform_int_distribution<int> rangeDist(minRanges, maxRanges);
	int numRanges = rangeDist(random);

	std::uniform_real_distribution<float> xStart(0, width), yStart(0, height), rangeAngle(0, 2 * M_PI), rangeLength(lengthMin, lengthMax), rangeWidth(widthMin, widthMax);

	// Apply Ranges
	for (int i = 0; i < numRanges; i++)
	{
		// Generate dimensions for range
		float x = xStart(random);
		float y = yStart(random);
		float angle = rangeAngle(random);
		float length = rangeLength(random);
		float rWidth = rangeWidth(random);

		float rangeXVec = std::cos(angle);
		float rangeYVec = std::sin(angle);

		// Getcoordinates of mountain start in mountain coordinates
		float x0 = (x * rangeXVec) + (y * rangeYVec);
		float y0 = (y * rangeXVec) - (x * rangeYVec);

		for (int n = 0; n < width * height; n++)
		{
			int xTest = n % width;
			int yTest = n / width;

			// Convert to mountain coords
			float xMountainTest = (xTest * rangeXVec) + (yTest * rangeYVec) - x0;
			float yMountainTest = (yTest * rangeXVec) - (xTest * rangeYVec) - y0;

			if (xMountainTest > 0 && xMountainTest < length && std::abs(yMountainTest) < rWidth)
			{
				float mountainVal = 255.0f * (rWidth - std::abs(yMountainTest)) / rWidth;
				data[n] = std::max(data[n], mountainVal);
			}
		}
	}
}

