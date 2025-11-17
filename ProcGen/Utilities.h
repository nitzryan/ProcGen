#pragma once

#include <exception>
#include <string>

namespace Utilities
{
	template <typename T> void SetMinMax(T& minContainer, T& maxContainer, T min, T max, std::string errorName)
	{
		if (min < 0)
			throw new std::exception(std::string("Min " + errorName + " Below Zero").c_str());

		if (max < 0)
			throw new std::exception(std::string("Max " + errorName + " Below Zero").c_str());

		if (min > max)
			throw new std::exception(std::string("Min " + errorName + " Above Max").c_str());

		minContainer = min;
		maxContainer = max;
	}
}
