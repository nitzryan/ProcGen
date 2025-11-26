#include "PipelineStepWidget.h"

#include <qmessagebox.h>

#include <fstream>
#include <mutex>

#include <Widgets/QStringUtilities.h>
#include <Widgets/Passes/Perlin/PerlinPassWidget.h>
#include <Widgets/Passes/Offset/OffsetPass.h>
#include <Widgets/Passes/PerlinFractal/PerlinFractalWidget.h>
#include <Widgets/Filters/Mountain/MountainFilterWidget.h>

PipelineStepWidget::PipelineStepWidget(const MapDimensions* md)
	: mapDimensions(md)
{
	ui.setupUi(this);
	passes.push_back(new PerlinPassWidget(mapDimensions));
	connect((PerlinPassWidget*)passes.back(), &PerlinPassWidget::OutputPassData, this, &PipelineStepWidget::OutputPassData);

	SetupWidget();
}

PipelineStepWidget::PipelineStepWidget(std::ifstream& file, const MapDimensions* md)
	: mapDimensions(md)
{
	ui.setupUi(this);

	std::string name;
	file >> name;
	QString n = QString(name.c_str());
	QString outputName = QStringUtilities::FromFileQString(n);
	ui.lePassName->setText(outputName);

	// Read in passes
	int numPasses;
	file >> numPasses;
	if (numPasses <= 0)
		throw std::exception("Expected a positive number of passes in PipelineStepWidget");

	passes.reserve(numPasses);
	for (int i = 0; i < numPasses; i++)
	{
		int elementType;
		file >> elementType;
		switch (elementType)
		{
		case static_cast<int>(IPassWidgetNS::PASS_TYPE::PERLIN):
			passes.push_back(new PerlinPassWidget(file, mapDimensions));
			break;
		case static_cast<int>(IPassWidgetNS::PASS_TYPE::OFFSET):
			passes.push_back(new OffsetPass(file, mapDimensions));
			break;
		case static_cast<int>(IPassWidgetNS::PASS_TYPE::PERLIN_FRACTAL):
			passes.push_back(new PerlinFractalWidget(file, mapDimensions));
			break;
		default:
			throw std::exception("Invalid element type received at PipelineStepWidget");
		}	

		SetupPassSignals(passes.back());
	}

	// Read in filters
	int numFilters;
	file >> numFilters;
	if (numFilters != 0)
	{
		filters.reserve(numFilters);
		for (int i = 0; i < numFilters; i++)
		{
			int filterType;
			file >> filterType;
			if (filterType == 1)
			{
				MountainFilterWidget* w = new MountainFilterWidget(file, mapDimensions);
				filters.push_back(w);
			}
			else 
				throw new std::exception("Invalid filter type received at PipelineStepWidget");

			SetupFilterSignals(filters.back());
		}
	}

	// Setup button callbacks
	connect(ui.pbAddPass, &QPushButton::pressed, this, &PipelineStepWidget::AddPassCallback);
	connect(ui.pbAddFilter, &QPushButton::pressed, this, &PipelineStepWidget::AddFilterCallback);

	SetupWidget();
}

PipelineStepWidget::~PipelineStepWidget()
{
	for (auto pass : passes)
		delete pass;

	for (auto filter : filters)
		delete filter;

	if (stepData != nullptr)
		delete[] stepData;

	if (filterData != nullptr)
		delete[] filterData;
}

void PipelineStepWidget::WriteToFile(std::ofstream& file)
{
	QString name = ui.lePassName->text();
	file << "\t" << QStringUtilities::ToFileQString(name).toStdString() + "\n";
	file << "\t\t" << passes.size() << "\n";
	for (auto p : passes)
		p->WriteToFile(file);
	file << "\t\t" << filters.size() << "\n";
	for (auto f : filters)
		f->WriteToFile(file);
}

float* PipelineStepWidget::GetPassOutput()
{
	int width = mapDimensions->width;
	int height = mapDimensions->height;

	if (stepData != nullptr)
		delete[] stepData;
	stepData = new float[width * height];
	for (int i = 0; i < width * height; i++)
		stepData[i] = 0;
	
	for (auto p : passes)
		p->GetPassOutput(stepData);

	if (filterData != nullptr)
		delete[] filterData;
	filterData = new float[width * height];
	for (size_t i = 0; i < width * height; i++)
		filterData[i] = 0;

	for (auto f : filters)
	{
		f->GetOutput(filterData);
	}

	if (!filters.empty())
	{
		for (int i = 0; i < width * height; i++)
			stepData[i] *= filterData[i];
	}

	return stepData;
}

void PipelineStepWidget::SetPositionComboBox(int length, int index)
{
	ui.cbPosition->blockSignals(true);

	if (dropdownLength != length)
	{
		dropdownLength = length;

		ui.cbPosition->clear();
		for (int i = 0; i < dropdownLength; i++)
		{
			ui.cbPosition->addItem(QString::number(i + 1), i);
		}
	}

	ui.cbPosition->setCurrentIndex(index);

	ui.cbPosition->blockSignals(false);
}

void PipelineStepWidget::SetupWidget()
{
	// Layout of steps
	connect(ui.cbPosition, &QComboBox::currentIndexChanged, this, &PipelineStepWidget::PositionChanged);
	connect(ui.pbDelete, &QPushButton::pressed, this, &PipelineStepWidget::Delete);

	for (auto p : passes)
		ui.passLayout->addWidget(p);
	for (auto f : filters)
		ui.filterLayout->addWidget(f);

	// Dropdown visibility
	connect(ui.dropdownButton, &QToolButton::pressed, this, [&]() {
		contentsVisible = !contentsVisible;

		if (contentsVisible)
		{
			ui.dropdownButton->setArrowType(Qt::ArrowType::UpArrow);
		}
		else {
			ui.dropdownButton->setArrowType(Qt::ArrowType::DownArrow);
		}

		ui.filterWidget->setVisible(contentsVisible);
		ui.passWidget->setVisible(contentsVisible);
		ui.viewWidget->setVisible(contentsVisible);
	});

	ui.filterWidget->setVisible(contentsVisible);
	ui.passWidget->setVisible(contentsVisible);
	ui.viewWidget->setVisible(contentsVisible);

	// View Specific parts of step
	connect(ui.pbViewPerlin, &QPushButton::pressed, this, [&]() {
		std::shared_ptr<float[]> data(new float[mapDimensions->width * mapDimensions->height]());
		for (auto p : passes)
			p->GetPassOutput(data.get());
		MapData md;
		md.heightmap = data;
		md.dimensions = *mapDimensions;
		emit OutputPassData(md);
	});

	connect(ui.pbViewFilters, &QPushButton::pressed, this, [&]() {
		std::shared_ptr<float[]> data(new float[mapDimensions->width * mapDimensions->height]());
		for (auto f : filters)
			f->GetOutput(data.get());
		MapData md;
		md.heightmap = data;
		md.dimensions = *mapDimensions;
		emit OutputPassData(md);
	});

	connect(ui.pbViewCombined, &QPushButton::pressed, this, [&]() {
		std::shared_ptr<float[]> data(new float[mapDimensions->width * mapDimensions->height]());
		float* filterData = new float[mapDimensions->width * mapDimensions->height]();
		for (auto p : passes)
			p->GetPassOutput(data.get());
		for (auto f : filters)
			f->GetOutput(filterData);


		if (!filters.empty())
		{
			for (int i = 0; i < mapDimensions->width * mapDimensions->height; i++)
				data[i] *= filterData[i];
		}

		delete[] filterData;
		MapData md;
		md.heightmap = data;
		md.dimensions = *mapDimensions;
		emit OutputPassData(md);
	});
}

void PipelineStepWidget::AddPassCallback()
{
	// Get user choice
	QMessageBox messageBox;
	messageBox.setWindowTitle("Choose Pass Type");
	messageBox.setText("Choose Pass Type");

	QPushButton* perlinPassButton = messageBox.addButton("Perlin", QMessageBox::ActionRole);
	QPushButton* offsetPassButton = messageBox.addButton("Offset", QMessageBox::ActionRole);
	QPushButton* perlinFractalButtton = messageBox.addButton("Perlin Fractal", QMessageBox::ActionRole);
	QPushButton* cancelButton = messageBox.addButton("Cancel", QMessageBox::RejectRole);

	messageBox.setDefaultButton(cancelButton);
	messageBox.exec();

	// Create chosen pass

	void* clickedButton = messageBox.clickedButton();
	if (clickedButton == cancelButton)
		return;

	if (clickedButton == perlinPassButton)
	{
		passes.push_back(new PerlinPassWidget(mapDimensions));
	}
	else if (clickedButton == offsetPassButton)
	{
		passes.push_back(new OffsetPass(mapDimensions));
	}
	else if (clickedButton == perlinFractalButtton)
	{
		passes.push_back(new PerlinFractalWidget(mapDimensions));
	}

	// Add Widget to view
	ui.passLayout->addWidget(passes.back());
	SetupPassSignals(passes.back());
}

void PipelineStepWidget::AddFilterCallback()
{
	// Get user choice
	QMessageBox messageBox;
	messageBox.setWindowTitle("Choose Filter Type");
	messageBox.setText("Choose Filter Type");

	QPushButton* mountainFilterButton = messageBox.addButton("Mountain", QMessageBox::ActionRole);
	QPushButton* cancelButton = messageBox.addButton("Cancel", QMessageBox::RejectRole);

	messageBox.setDefaultButton(cancelButton);
	messageBox.exec();

	// Create chosen pass

	void* clickedButton = messageBox.clickedButton();
	if (clickedButton == cancelButton)
		return;

	if (clickedButton == mountainFilterButton)
	{
		filters.push_back(new MountainFilterWidget(mapDimensions));
	}

	// Add Widget to view
	ui.filterLayout->addWidget(filters.back());
	SetupFilterSignals(filters.back());
}

void PipelineStepWidget::SetupPassSignals(const IPassWidget* w)
{
	connect(w, &IPassWidget::OutputPassData, this, &PipelineStepWidget::OutputPassData);
	connect(w, &IPassWidget::DeletePass, this, &PipelineStepWidget::PassDeleted);
}

void PipelineStepWidget::SetupFilterSignals(const IFilterWidget* w)
{
	connect(w, &IFilterWidget::OutputPassData, this, &PipelineStepWidget::OutputPassData);
	connect(w, &IFilterWidget::DeleteFilter, this, &PipelineStepWidget::FilterDeleted);
}

void PipelineStepWidget::PassDeleted()
{
	void* obj = sender();

	if (obj == nullptr)
		throw std::exception("PipelineStepWidget received PassDeleted from null sender");

	// Ensure no race conditions
	std::lock_guard<std::mutex> lock(passDeleteMutex);

	// Find pass that sent signal
	int passIndex = -1;
	for (int i = 0; i < passes.size(); i++)
	{
		if (passes[i] == obj)
		{
			passIndex = i;
			break;
		}
	}

	if (passIndex == -1)
		throw std::exception("Pass was not found to be deleted");

	// Delete Values
	QLayoutItem* item = ui.passLayout->takeAt(passIndex);
	delete item->widget();
	delete item;

	// Remove from stored passes
	passes.erase(passes.begin() + passIndex);
}

void PipelineStepWidget::FilterDeleted()
{
	void* obj = sender();

	if (obj == nullptr)
		throw std::exception("PipelineStepWidget received FilterDeleted from null sender");

	// Ensure no race conditions
	std::lock_guard<std::mutex> lock(filterDeleteMutex);

	// Find pass that sent signal
	int filterIndex = -1;
	for (int i = 0; i < filters.size(); i++)
	{
		if (filters[i] == obj)
		{
			filterIndex = i;
			break;
		}
	}

	if (filterIndex == -1)
		throw std::exception("Filter was not found to be deleted");

	// Delete Values
	QLayoutItem* item = ui.filterLayout->takeAt(filterIndex);
	delete item->widget();
	delete item;

	// Remove from stored passes
	filters.erase(filters.begin() + filterIndex);
}


