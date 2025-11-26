#pragma once

#include <Generation/MapDimensions.h>
#include <Generation/MapData.h>
#include <QWidget>

namespace IPassWidgetNS
{
	enum class PASS_TYPE {
		PERLIN = 1,
		OFFSET = 2,
		PERLIN_FRACTAL = 3,
	};
}

class IPassWidget : public QWidget
{
	Q_OBJECT
public:
	explicit IPassWidget(const MapDimensions* md) : QWidget(nullptr), mapDimensions(md) {}
	virtual ~IPassWidget() {}

	virtual void WriteToFile(std::ofstream& file) const = 0;
	virtual void GetPassOutput(float* data) = 0;

	void EmitPassOutput() {
		std::shared_ptr<float[]> data(new float[mapDimensions->width * mapDimensions->height]());
		GetPassOutput(data.get());
		MapData md;
		md.heightmap = data;
		md.dimensions = *mapDimensions;
		emit OutputPassData(md);
	}
signals:
	void OutputPassData(MapData mapData);
protected:
	const MapDimensions* mapDimensions;
};