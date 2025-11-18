#pragma once

#include <Generation/MapDimensions.h>

namespace IPassWidgetNS
{
	enum class PASS_TYPE {
		PERLIN = 1,
		OFFSET = 2
	};
}

class IPassWidget : public QWidget
{
public:
	explicit IPassWidget(const MapDimensions* md) : QWidget(nullptr), mapDimensions(md) {}
	virtual ~IPassWidget() {}

	virtual void WriteToFile(std::ofstream& file) const = 0;
	virtual void GetPassOutput(float* data) = 0;

protected:
	const MapDimensions* mapDimensions;
};