#pragma once

#include <vector>
#include <Widgets/PerlinPassWidget.h>
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
	std::vector<PerlinPassWidget*> perlinPasses;

	void AddPerlinPass();
	void RemovePerlinPass(PerlinPassWidget* pp);
	void ReadFile(const char* filename);
	void SaveFile(const char* filename);
};

