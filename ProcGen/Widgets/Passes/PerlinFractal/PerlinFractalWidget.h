#pragma once

#include "ui_PerlinFractalWidget.h"
#include <Widgets/Passes/IPassWidget.h>
#include <Generation/MapData.h>
#include <Generation/Perlin/PerlinPass.h>

class PerlinFractalWidget : public IPassWidget
{
	Q_OBJECT

public:
	PerlinFractalWidget(const MapDimensions* md);
	~PerlinFractalWidget();
	PerlinFractalWidget(std::ifstream& file, const MapDimensions* md);
	
	void WriteToFile(std::ofstream& file) const override final;
	void GetPassOutput(float* data) override final;

private:
	Ui::PerlinFractalWidgetClass ui;
	PerlinPass* perlinPass;

	void Setup(int numGradients, int perlinRepCount);
};

