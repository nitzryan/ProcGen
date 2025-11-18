#pragma once

#include <ui_PerlinPassWidget.h>
#include <Generation/PerlinPass.h>
#include "IPassWidget.h"

class PerlinPassWidget : public IPassWidget
{
	Q_OBJECT

public:
	PerlinPassWidget();
	
	~PerlinPassWidget();

	PerlinPassWidget(std::ifstream& file);
	void WriteToFile(std::ofstream& file) const override final;

	void GetPassOutput(int width, int height, float* data) override final;
private:
	Ui::PerlinPassWidget ui;
	PerlinPass* perlinPass;
};

