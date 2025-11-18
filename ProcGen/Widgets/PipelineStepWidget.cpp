#include "PipelineStepWidget.h"

#include <fstream>
#include "QStringUtilities.h"

PipelineStepWidget::PipelineStepWidget()
{
	ui.setupUi(this);
	ppw = new PerlinPassWidget();

	SetupWidget();
}

PipelineStepWidget::PipelineStepWidget(std::ifstream& file)
{
	ui.setupUi(this);

	std::string name;
	file >> name;
	QString n = QString(name.c_str());
	QString outputName = QStringUtilities::FromFileQString(n);
	ui.lePassName->setText(outputName);
	
	int elementType;
	file >> elementType;
	if (elementType == 1)
		ppw = new PerlinPassWidget(file);
	else
		throw new std::exception("Invalid element type received at PipelineStepWidget");

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
				MountainFilterWidget* w = new MountainFilterWidget(file);
				filters.push_back(w);
			}
			else 
				throw new std::exception("Invalid filter type received at PipelineStepWidget");
		}
	}

	SetupWidget();
}

PipelineStepWidget::~PipelineStepWidget()
{
	delete ppw;
	for (auto filter : filters)
		delete filter;

	if (stepData != nullptr)
		delete stepData;

	if (filterData != nullptr)
		delete filterData;
}

void PipelineStepWidget::WriteToFile(std::ofstream& file)
{
	QString name = ui.lePassName->text();
	file << "\t" << QStringUtilities::ToFileQString(name).toStdString() + "\n";
	ppw->WriteToFile(file);
	file << "\t\t" << filters.size();
	for (auto f : filters)
		f->WriteToFile(file);
}

float* PipelineStepWidget::GetPassOutput(int width, int height)
{
	if (stepData != nullptr)
		delete stepData;
	stepData = new float[width * height];
	
	ppw->GetPassOutput(width, height, stepData);

	if (filterData != nullptr)
		delete filterData;
	filterData = new float[width * height];
	for (size_t i = 0; i < width * height; i++)
		filterData[i] = 0;

	for (auto f : filters)
	{
		f->GetOuput(width, height, filterData);
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
	connect(ui.cbPosition, &QComboBox::currentIndexChanged, this, &PipelineStepWidget::PositionChanged);
	connect(ui.pbDelete, &QPushButton::pressed, this, &PipelineStepWidget::Delete);

	ui.perlinPassLayout->addWidget(ppw);
	for (auto f : filters)
		ui.filtersLayout->addWidget(f);
}

