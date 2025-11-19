#pragma once

#include <ui_PerlinPassWidget.h>
#include <Generation/Perlin/PerlinPass.h>
#include <Widgets/Passes/IPassWidget.h>

class PerlinPassWidget : public IPassWidget
{
	Q_OBJECT

public:
	PerlinPassWidget(const MapDimensions* md);
	
	~PerlinPassWidget();

	PerlinPassWidget(std::ifstream& file, const MapDimensions* md);
	void WriteToFile(std::ofstream& file) const override final;

	void GetPassOutput(float* data) override final;

signals:
	void OutputPassData(std::shared_ptr<float[]> data, const MapDimensions* md);
private:
	Ui::PerlinPassWidget ui;
	PerlinPass* perlinPass;
};

