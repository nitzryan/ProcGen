#pragma once

#include <exception>
#include <string>
#include <fstream>
#include <qspinbox.h>

namespace Utilities
{
	template <typename T> void SetMinMax(T& minContainer, T& maxContainer, T min, T max, std::string errorName)
	{
		if (min < 0)
			throw std::exception(std::string("Min " + errorName + " Below Zero").c_str());

		if (max < 0)
			throw std::exception(std::string("Max " + errorName + " Below Zero").c_str());

		if (min > max)
			throw std::exception(std::string("Min " + errorName + " Above Max").c_str());

		minContainer = min;
		maxContainer = max;
	}

	inline void UpdateSpinboxFromFile(QSpinBox* spinBox, std::ifstream& file)
	{
		int val;
		file >> val;
		spinBox->setValue(val);
	}

	inline void UpdateDSBFromFile(QDoubleSpinBox* dsb, std::ifstream& file)
	{
		double val;
		file >> val;
		dsb->setValue(val);
	}
}
