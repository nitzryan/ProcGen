#include "GenerationPipeline.h"

#include <fstream>
#include <QMessageBox>
#include <QFileDialog>

GenerationPipeline::GenerationPipeline(QWidget* parent) : QWidget(parent)
{
	ui.setupUi(this);

	// (TODO): Load default (last loaded) filename

	connect(ui.pbLoadFile, &QPushButton::pressed, this, [&]() {
		QString filename = QFileDialog::getOpenFileName(this, "Pipeline File");
		QFileInfo fileInfo(filename);
		ui.lblFilename->setText(fileInfo.baseName());

		if (filename.isEmpty())
			return;

		ReadFile(filename.toStdString().c_str());
	});

	connect(ui.pbSaveFile, &QPushButton::pressed, this, [&]() {
		QString filename = QFileDialog::getSaveFileName(this, "Pipeline File", QString(), tr("Pipeline Files (*.pipeline)"));
		if (filename.isEmpty())
			return;

		SaveFile(filename.toStdString().c_str());
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
}

GenerationPipeline::~GenerationPipeline()
{
	for (auto i : perlinPasses)
		delete i;
}

void GenerationPipeline::AddPerlinPass()
{
	PerlinPassWidget* ppw = new PerlinPassWidget(QString("Perlin Pass ").append(QString::number(perlinPasses.size())),
		perlinPasses.size(), 128, 0, 16, 256);

	// TODO: Add connections to allow for deletion
}

void GenerationPipeline::RemovePerlinPass(PerlinPassWidget* pp)
{
	std::remove(perlinPasses.begin(), perlinPasses.end(), pp);
}

void GenerationPipeline::ReadFile(const char* filename)
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
		int width, height;
		file >> width >> height;
		ui.sbHeight->setValue(height);
		ui.sbWidth->setValue(width);

		int num_elements;
		file >> num_elements;

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
		QMessageBox errorBox;
		errorBox.critical(nullptr, "File Read Error", e.what());
	}

	file.close();

	// Add items to layout
	for (auto ppw : perlinPasses)
	{
		ui.perlinPassLayout->addWidget(ppw);
	}
}

void GenerationPipeline::SaveFile(const char* filename)
{
	std::ofstream file;
	file.open(filename);

	file << ui.sbWidth->value() << " " << ui.sbHeight->value() << std::endl;

	for (auto i : perlinPasses)
		i->WriteToFile(file);

	file.close();
}

