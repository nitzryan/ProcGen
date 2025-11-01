#pragma once

#include <ui_PerlinPassWidget.h>
#include <Generation/PerlinPass.h>

class PerlinPassWidget : public QWidget
{
	Q_OBJECT

public:
	PerlinPassWidget(QString name, int seed, double blockSize, double scale, int numGradients, int perlinRepCount, QWidget* parent = nullptr);
	~PerlinPassWidget();

	float* GetPassOutput(int width, int height);
private:
	Ui::PerlinPassWidget ui;
	PerlinPass perlinPass;
	bool dropdownVisible = false;
};

