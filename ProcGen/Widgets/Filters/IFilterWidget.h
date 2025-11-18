#pragma once

#include <Generation/MapDimensions.h>
#include <qwidget.h>

namespace IFilterWidgetNS
{
	enum class FILTER_TYPE {
		MOUNTAIN = 1
	};
}

class IFilterWidget : public QWidget
{
public:
	explicit IFilterWidget(const MapDimensions* md) : QWidget(nullptr), mapDimensions(md) {}
	virtual ~IFilterWidget() {}

	virtual void WriteToFile(std::ofstream& file) const = 0;
	virtual void GetOutput(float* data) = 0;

protected:
	const MapDimensions* mapDimensions;
};