#pragma once

#include <Generation/MapDimensions.h>
#include <Generation/MapData.h>
#include <qwidget.h>

namespace IFilterWidgetNS
{
	enum class FILTER_TYPE {
		MOUNTAIN = 1
	};
}

class IFilterWidget : public QWidget
{
	Q_OBJECT
public:
	explicit IFilterWidget(const MapDimensions* md) : QWidget(nullptr), mapDimensions(md) {}
	virtual ~IFilterWidget() {}

	virtual void WriteToFile(std::ofstream& file) const = 0;
	virtual void GetOutput(float* data) = 0;

	void EmitPassOutput() {
		std::shared_ptr<float[]> data(new float[mapDimensions->width * mapDimensions->height]());
		GetOutput(data.get());
		MapData md;
		md.heightmap = data;
		md.dimensions = *mapDimensions;
		emit OutputPassData(md);
	}
signals:
	void OutputPassData(MapData mapData);
	void DeleteFilter();
protected:
	const MapDimensions* mapDimensions;
};