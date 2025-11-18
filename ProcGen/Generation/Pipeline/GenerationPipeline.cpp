#include "GenerationPipeline.h"

#include <fstream>
#include <stdexcept>
#include <QMessageBox>
#include <QFileDialog>
#include "QSettingsSingleton.h"

#define PIPELINE_REGISTRY_NAME "pipelineDir"
#define LAST_LOADED_REGISTRY_NAME "lastLoadedFile"

GenerationPipeline::GenerationPipeline(QWidget* parent) : QWidget(parent)
{
	ui.setupUi(this);

	// Load default (last loaded) filename
	QString lastFile = QSettingsSingleton::get().value(LAST_LOADED_REGISTRY_NAME, QString()).toString();
	if (!lastFile.isEmpty())
	{
		ReadFile(lastFile.toStdString().c_str(), false);
		QFileInfo fileInfo(lastFile);
		ui.lblFilename->setText(fileInfo.baseName());
	}

	connect(ui.pbLoadFile, &QPushButton::pressed, this, [&]() {
		QString lastDir = QSettingsSingleton::get().value(PIPELINE_REGISTRY_NAME, QDir::homePath()).toString();
		QString filename = QFileDialog::getOpenFileName(this, "Pipeline File", lastDir);
		QFileInfo fileInfo(filename);
		ui.lblFilename->setText(fileInfo.baseName());

		if (filename.isEmpty())
			return;

		QSettingsSingleton::get().setValue(PIPELINE_REGISTRY_NAME, fileInfo.absoluteDir().absolutePath());
		QSettingsSingleton::get().setValue(LAST_LOADED_REGISTRY_NAME, filename);
		ReadFile(filename.toStdString().c_str(), true);
	});

	connect(ui.pbSaveFile, &QPushButton::pressed, this, [&]() {
		QString lastFile = QSettingsSingleton::get().value(LAST_LOADED_REGISTRY_NAME, QDir::homePath()).toString();
		QString filename = QFileDialog::getSaveFileName(this, "Pipeline File", lastFile, tr("Pipeline Files (*.pipeline)"));
		if (filename.isEmpty())
			return;

		QFileInfo fileInfo(filename);
		QSettingsSingleton::get().setValue(PIPELINE_REGISTRY_NAME, fileInfo.absoluteDir().absolutePath());
		QSettingsSingleton::get().setValue(LAST_LOADED_REGISTRY_NAME, filename);
		SaveFile(filename.toStdString().c_str());
		ui.lblFilename->setText(fileInfo.baseName());
	});

	connect(ui.pbGenerate, &QPushButton::pressed, this, [&]() {
		int width = ui.sbWidth->value();
		int height = ui.sbHeight->value();
		std::vector<float> data(width * height, 0);

		try {
			for (auto ps : pipelineSteps)
			{
				auto passResults = ps->GetPassOutput();
				for (size_t i = 0; i < width * height; i++)
					data[i] += passResults[i];
			}
		}
		catch (std::exception e)
		{
			return;
		}

		// Convert to 0-255
		QList<uchar> pixelData(width * height);
		for (size_t i = 0; i < width * height; i++)
			pixelData[i] = (data[i] + 1.0) / 2.0 * 255.0;

		emit PipelineOutput(width, height, pixelData);
	});

	connect(ui.pbAddPerlinPass, &QPushButton::pressed, this, [&]() {
		PipelineStepWidget* psw = new PipelineStepWidget(&mapDimensions);
		pipelineSteps.push_back(psw);
		AddStep(psw, -1, true);
	});

	// Map width/height
	connect(ui.sbWidth, &QSpinBox::valueChanged, this, [&](int value)
	{
		mapDimensions.width = value;
	});
	connect(ui.sbHeight, &QSpinBox::valueChanged, this, [&](int value)
	{
		mapDimensions.height = value;
	});
	mapDimensions.width = ui.sbWidth->value();
	mapDimensions.height = ui.sbHeight->value();
}

GenerationPipeline::~GenerationPipeline()
{
	for (auto i : pipelineSteps)
		delete i;
}

void GenerationPipeline::AddStep(PipelineStepWidget* pp, int index, bool updateAll)
{
	ui.perlinPassLayout->addWidget(pp);
	connect(pp, &PipelineStepWidget::PositionChanged, this, [this, pp](int idx) {
		ReorderStep(pp, idx);
	});
	connect(pp, &PipelineStepWidget::Delete, this, [this, pp]() {
		RemoveStep(pp);
	});

	if (updateAll)
	{
		for (size_t i = 0; i < pipelineSteps.size(); i++)
		{
			pipelineSteps[i]->SetPositionComboBox(pipelineSteps.size(), i);
		}
	}
	else {
		pp->SetPositionComboBox(pipelineSteps.size(), index);
	}
}

void GenerationPipeline::ReorderStep(PipelineStepWidget* pp, int newIndex)
{
	int oldIndex = -1;
	
	// Find current index
	for (size_t i = 0; i < pipelineSteps.size(); i++)
	{
		if (pipelineSteps[i] == pp)
		{
			oldIndex = i;
			break;
		}
	}

	if (oldIndex == -1)
		throw std::exception("Did not find pass in index, programming error");

	// Move element down in list, shift all others up
	if (newIndex < oldIndex)
	{
		for (size_t i = oldIndex; i > newIndex; i--)
		{
			pipelineSteps[i] = pipelineSteps[i - 1];
			pipelineSteps[i]->SetPositionComboBox(pipelineSteps.size(), i);
		}
		
	}
	// Move element up in list, shift all others down
	else 
	{
		for (size_t i = oldIndex; i < newIndex; i++)
		{
			pipelineSteps[i] = pipelineSteps[i + 1];
			pipelineSteps[i]->SetPositionComboBox(pipelineSteps.size(), i);
		}
	}
	
	pipelineSteps[newIndex] = pp;

	// Reorder QLayout
	QLayoutItem* item = ui.perlinPassLayout->takeAt(oldIndex);
	ui.perlinPassLayout->insertItem(newIndex, item);
}

void GenerationPipeline::RemoveStep(PipelineStepWidget* pp)
{
	if (pipelineSteps.size() <= 1)
	{
		QMessageBox::information(nullptr, "Invalid Delete", "Couldn't delete only pass");
		return;
	}

	int index = -1;
	for (size_t i = 0; i < pipelineSteps.size(); i++)
	{
		if (pipelineSteps[i] == pp)
		{
			index = i;
			break;
		}
	}
	if (index == -1)
		throw std::exception("Did not find pass in index, programming error");

	pipelineSteps.erase(std::remove(pipelineSteps.begin(), pipelineSteps.end(), pp), pipelineSteps.end());
	QLayoutItem* item = ui.perlinPassLayout->takeAt(index);
	delete item->widget();
	delete item;
	
}

void GenerationPipeline::ReadFile(const char* filename, bool outputErrors)
{
	// Remove all existing items in layout
	QLayoutItem* item;
	while ((item = ui.perlinPassLayout->takeAt(0)) != nullptr)
	{
		delete item->widget();
		delete item;
	}
	pipelineSteps.clear();

	// Read pipeline data
	std::ifstream file;
	file.open(filename);

	try {
		if (!file.is_open())
		{
			throw std::runtime_error("Unable to open file");
		}

		// Read pipeline-wide data
		int width, height;
		file >> width >> height;
		ui.sbHeight->setValue(height);
		ui.sbWidth->setValue(width);

		int num_elements;
		file >> num_elements;

		// Read individual elements
		while (num_elements > 0)
		{
			PipelineStepWidget* w = new PipelineStepWidget(file, &mapDimensions);
			pipelineSteps.push_back(w);
			num_elements--;
		}
	}
	catch (std::exception e)
	{
		if (outputErrors)
		{
			QMessageBox errorBox;
			errorBox.critical(nullptr, "File Read Error", e.what());
		}
	}

	file.close();

	// Add items to layout
	for (size_t i = 0; i < pipelineSteps.size(); i++)
	{
		AddStep(pipelineSteps[i], i, false);
	}
}

void GenerationPipeline::SaveFile(const char* filename)
{
	std::ofstream file;
	file.open(filename);

	file << ui.sbWidth->value() << " " << ui.sbHeight->value() << std::endl;
	file << pipelineSteps.size() << std::endl;

	for (auto i : pipelineSteps)
		i->WriteToFile(file);

	file.close();
}

