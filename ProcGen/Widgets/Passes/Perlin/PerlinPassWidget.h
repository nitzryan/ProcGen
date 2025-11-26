#pragma once

#include <ui_PerlinPassWidget.h>
#include <Generation/Perlin/PerlinPass.h>
#include <Widgets/Passes/IPassWidget.h>
#include <Generation/MapData.h>

class PerlinPassWidget : public IPassWidget
{
	Q_OBJECT

public:
	PerlinPassWidget(const MapDimensions* md);
	
	~PerlinPassWidget();

	PerlinPassWidget(std::ifstream& file, const MapDimensions* md);
	void WriteToFile(std::ofstream& file) const override final;

	void GetPassOutput(float* data) override final;

private:
	Ui::PerlinPassWidget ui;
	PerlinPass* perlinPass;

	void SetupSignals();
};

