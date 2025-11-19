#pragma once

#include <QWidget>
#include "ui_PipelineStepWidget.h"

#include <Widgets/Passes/IPassWidget.h>
#include <Widgets/Filters/IFilterWidget.h>
#include <Generation/MapDimensions.h>

class PipelineStepWidget : public QWidget
{
	Q_OBJECT

public:
	PipelineStepWidget(const MapDimensions* md);
	PipelineStepWidget(std::ifstream& file, const MapDimensions* md);
	~PipelineStepWidget();

	void WriteToFile(std::ofstream& file);

	float* GetPassOutput();
	void SetPositionComboBox(int length, int index);
signals:
	void PositionChanged(int index);
	void Delete();
	void OutputPassData(std::shared_ptr<float[]> data, const MapDimensions* md);
	
private:
	Ui::PipelineStepWidgetClass ui;
	void SetupWidget();
	float* stepData = nullptr;
	float* filterData = nullptr;
	int dropdownLength = 0;
	const MapDimensions* mapDimensions;

	std::vector<IPassWidget*> passes;
	std::vector<IFilterWidget*> filters;

	void OutputPassSlot(std::shared_ptr<float[]> data, const MapDimensions* md);
};

