#pragma once

#include "ui_MountainFilterWidget.h"
#include <Generation/MountainFilterMap.h>

class MountainFilterWidget : public QWidget
{
	Q_OBJECT

public:
	~MountainFilterWidget();

	MountainFilterWidget();
	MountainFilterWidget(std::ifstream& file);
	void WriteToFile(std::ofstream& file);
	void GetOuput(int width, int height, float* data);

signals:

private:
	Ui::MountainFilterWidget ui;
	MountainFilterMap map;

	void UpdateFilterMap();
};

