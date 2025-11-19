#pragma once

#include "ui_MountainFilterWidget.h"
#include <Generation/Mountain/MountainFilterMap.h>
#include <Widgets/Filters/IFilterWidget.h>
#include <Generation/MapData.h>

class MountainFilterWidget : public IFilterWidget
{
	Q_OBJECT

public:
	~MountainFilterWidget();

	MountainFilterWidget(const MapDimensions* md);
	MountainFilterWidget(std::ifstream& file, const MapDimensions* md);
	void WriteToFile(std::ofstream& file) const override;
	void GetOutput(float* data) override;
private:
	Ui::MountainFilterWidget ui;
	MountainFilterMap map;

	void UpdateFilterMap();
};

