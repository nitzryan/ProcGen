#pragma once

#include <ui_PerlinPassWidget.h>
#include <Generation/PerlinPass.h>

class PerlinPassWidget : public QWidget
{
	Q_OBJECT

public:
	PerlinPassWidget();
	
	~PerlinPassWidget();

	PerlinPassWidget(std::ifstream& file);
	void WriteToFile(std::ofstream& file);

	void GetPassOutput(int width, int height, float* data);
private:
	Ui::PerlinPassWidget ui;
	PerlinPass* perlinPass;
	bool dropdownVisible = false;
	int dropdownLength = 0;
};

