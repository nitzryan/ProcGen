#pragma once

#include <QWidget>
#include "ui_PipelineStepWidget.h"

#include "IPassWidget.h"
#include "MountainFilterWidget.h"

class PipelineStepWidget : public QWidget
{
	Q_OBJECT

public:
	PipelineStepWidget();
	PipelineStepWidget(std::ifstream& file);
	~PipelineStepWidget();

	void WriteToFile(std::ofstream& file);

	float* GetPassOutput(int width, int height);
	void SetPositionComboBox(int length, int index);
signals:
	void PositionChanged(int index);
	void Delete();
private:
	Ui::PipelineStepWidgetClass ui;
	void SetupWidget();
	float* stepData = nullptr;
	float* filterData = nullptr;
	int dropdownLength = 0;

	std::vector<IPassWidget*> passes;
	std::vector<MountainFilterWidget*> filters;
};

