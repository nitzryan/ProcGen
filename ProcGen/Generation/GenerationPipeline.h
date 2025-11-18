#pragma once

#include <vector>
#include <Widgets/PipelineStepWidget.h>
#include <QWidget>
#include <ui_GenerationPipeline.h>

class GenerationPipeline : public QWidget
{
	Q_OBJECT

public:
	GenerationPipeline(QWidget* parent = nullptr);
	~GenerationPipeline();

signals:
	void PipelineOutput(int width, int height, QList<uchar> data);
private:
	Ui::GenerationPipeline ui;
	std::vector<PipelineStepWidget*> pipelineSteps;

	void AddStep(PipelineStepWidget* pp, int index, bool updateAll);
	void ReorderStep(PipelineStepWidget* pp, int newIndex);
	void RemoveStep(PipelineStepWidget* pp);
	void ReadFile(const char* filename, bool outputErrors);
	void SaveFile(const char* filename);
};

