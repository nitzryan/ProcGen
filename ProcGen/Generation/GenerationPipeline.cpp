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
		QString lastDir = QSettingsSingleton::get().value(PIPELINE_REGISTRY_NAME, QDir::homePath()).toString();
		QString filename = QFileDialog::getSaveFileName(this, "Pipeline File", lastDir, tr("Pipeline Files (*.pipeline)"));
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

		for (auto ppw : perlinPasses)
		{
			auto passResults = ppw->GetPassOutput(width, height);
			for (size_t i = 0; i < width * height; i++)
				data[i] += passResults[i];
		}

		// Convert to 0-255
		QList<uchar> pixelData(width * height);
		for (size_t i = 0; i < width * height; i++)
			pixelData[i] = (data[i] + 1.0) / 2.0 * 255.0;

		emit PipelineOutput(width, height, pixelData);
	});

	connect(ui.pbAddPerlinPass, &QPushButton::pressed, this, [&]() {
		PerlinPassWidget* ppw = new PerlinPassWidget(QString("Perlin_Pass ").append(QString::number(perlinPasses.size())),
			perlinPasses.size(), 128, 0, 16, 256);
		perlinPasses.push_back(ppw);
		AddPerlinPass(ppw, -1, true);
	});
}

GenerationPipeline::~GenerationPipeline()
{
	for (auto i : perlinPasses)
		delete i;
}

void GenerationPipeline::AddPerlinPass(PerlinPassWidget* pp, int index, bool updateAll)
{
	ui.perlinPassLayout->addWidget(pp);
	connect(pp, &PerlinPassWidget::PositionChanged, this, [this, pp](int idx) {
		ReorderPerlinPass(pp, idx);
	});

	if (updateAll)
	{
		for (size_t i = 0; i < perlinPasses.size(); i++)
		{
			perlinPasses[i]->SetPositionComboBox(perlinPasses.size(), i);
		}
	}
	else {
		pp->SetPositionComboBox(perlinPasses.size(), index);
	}
}

void GenerationPipeline::ReorderPerlinPass(PerlinPassWidget* pp, int newIndex)
{
	int oldIndex = -1;
	
	// Find current index
	for (size_t i = 0; i < perlinPasses.size(); i++)
	{
		if (perlinPasses[i] == pp)
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
			perlinPasses[i] = perlinPasses[i - 1];
			perlinPasses[i]->SetPositionComboBox(perlinPasses.size(), i);
		}
		
	}
	// Move element up in list, shift all others down
	else 
	{
		for (size_t i = oldIndex; i < newIndex; i++)
		{
			perlinPasses[i] = perlinPasses[i + 1];
			perlinPasses[i]->SetPositionComboBox(perlinPasses.size(), i);
		}
	}
	
	perlinPasses[newIndex] = pp;

	// Reorder QLayout
	QLayoutItem* item = ui.perlinPassLayout->takeAt(oldIndex);
	ui.perlinPassLayout->insertItem(newIndex, item);
}

void GenerationPipeline::RemovePerlinPass(PerlinPassWidget* pp)
{
	std::remove(perlinPasses.begin(), perlinPasses.end(), pp);
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
	perlinPasses.clear();

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
			int objType;
			file >> objType;

			if (objType == 1)
			{
				perlinPasses.push_back(new PerlinPassWidget(file));
			}
			else {
				throw std::invalid_argument("Invalid Object type: " + std::to_string(objType));
			}

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
	for (size_t i = 0; i < perlinPasses.size(); i++)
	{
		AddPerlinPass(perlinPasses[i], i, false);
	}
}

void GenerationPipeline::SaveFile(const char* filename)
{
	std::ofstream file;
	file.open(filename);

	file << ui.sbWidth->value() << " " << ui.sbHeight->value() << std::endl;
	file << perlinPasses.size() << std::endl;

	for (auto i : perlinPasses)
		i->WriteToFile(file);

	file.close();
}

