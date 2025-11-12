#pragma once

#include <ui_PerlinPassWidget.h>
#include <Generation/PerlinPass.h>

class PerlinPassWidget : public QWidget
{
	Q_OBJECT

public:
	PerlinPassWidget(QString name, int seed, double blockSize, double scale, int numGradients, int perlinRepCount, QWidget* parent = nullptr);
	
	~PerlinPassWidget();

	PerlinPassWidget(std::ifstream& file);
	void WriteToFile(std::ofstream& file);

	float* GetPassOutput(int width, int height);
	void SetPositionComboBox(int length, int index);
signals:
	void PositionChanged(int index);
	void Delete();
private:
	Ui::PerlinPassWidget ui;
	PerlinPass* perlinPass;
	bool dropdownVisible = false;
	int dropdownLength = 0;
};

