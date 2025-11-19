#include "MountainFilterWidget.h"
#include <QMessageBox>
#include <fstream>
#include <Utilities.h>

MountainFilterWidget::~MountainFilterWidget()
{}

MountainFilterWidget::MountainFilterWidget(const MapDimensions* md) 
	: IFilterWidget(md)
{
	ui.setupUi(this);
	
	ui.sbRangesMin->setValue(1);
	ui.sbRangesMax->setValue(3);

	ui.dsbLengthMin->setValue(100);
	ui.dsbLengthMax->setValue(400);

	ui.dsbWidthMin->setValue(30);
	ui.dsbWidthMax->setValue(80);

	ui.dsbBendFreqMin->setValue(50);
	ui.dsbBendFreqMax->setValue(200);

	ui.dsbBendAmountMin->setValue(0);
	ui.dsbBendAmountMax->setValue(1.0);

	ui.dsbScaleMin->setValue(0.4);
	ui.dsbScaleMax->setValue(1.0);

	ui.dsbMultMin->setValue(1.0);
	ui.dsbMultMax->setValue(3.0);

	connect(ui.pbView, &QPushButton::pressed, this, &IFilterWidget::EmitPassOutput);

	UpdateFilterMap();
}

MountainFilterWidget::MountainFilterWidget(std::ifstream & file, const MapDimensions* md)
	: IFilterWidget(md)
{
	ui.setupUi(this);
	
	Utilities::UpdateSpinboxFromFile(ui.sbSeed, file);
	Utilities::UpdateSpinboxFromFile(ui.sbRangesMin, file);
	Utilities::UpdateSpinboxFromFile(ui.sbRangesMax, file);
	Utilities::UpdateDSBFromFile(ui.dsbLengthMin, file);
	Utilities::UpdateDSBFromFile(ui.dsbLengthMax, file);
	Utilities::UpdateDSBFromFile(ui.dsbWidthMin, file);
	Utilities::UpdateDSBFromFile(ui.dsbWidthMax, file);
	Utilities::UpdateDSBFromFile(ui.dsbBendFreqMin, file);
	Utilities::UpdateDSBFromFile(ui.dsbBendFreqMax, file);
	Utilities::UpdateDSBFromFile(ui.dsbBendAmountMin, file);
	Utilities::UpdateDSBFromFile(ui.dsbBendAmountMax, file);
	Utilities::UpdateDSBFromFile(ui.dsbScaleMin, file);
	Utilities::UpdateDSBFromFile(ui.dsbScaleMax, file);
	Utilities::UpdateDSBFromFile(ui.dsbMultMin, file);
	Utilities::UpdateDSBFromFile(ui.dsbMultMax, file);

	connect(ui.pbView, &QPushButton::pressed, this, &IFilterWidget::EmitPassOutput);

	UpdateFilterMap();
}

void MountainFilterWidget::WriteToFile(std::ofstream& file) const
{
	file << "\t\t" << 1 << " ";
	file << ui.sbSeed->value() << " ";
	file << ui.sbRangesMin->value() << " ";
	file << ui.sbRangesMax->value() << " ";
	file << ui.dsbLengthMin->value() << " ";
	file << ui.dsbLengthMax->value() << " ";
	file << ui.dsbWidthMin->value() << " ";
	file << ui.dsbWidthMax->value() << " ";
	file << ui.dsbBendFreqMin->value() << " ";
	file << ui.dsbBendFreqMax->value() << " ";
	file << ui.dsbBendAmountMin->value() << " ";
	file << ui.dsbBendAmountMax->value() << " ";
	file << ui.dsbScaleMin->value() << " ";
	file << ui.dsbScaleMax->value() << " ";
	file << ui.dsbMultMin->value() << " ";
	file << ui.dsbMultMax->value() << "\n";
}

void MountainFilterWidget::GetOutput(float* data)
{
	UpdateFilterMap();
	
	map.GenerateMap(mapDimensions->width, mapDimensions->height, ui.sbSeed->value(), data);
}

void MountainFilterWidget::UpdateFilterMap()
{
	MountainFilterMapArgs args;
	args.minRanges = ui.sbRangesMin->value();
	args.maxRanges = ui.sbRangesMax->value();
	args.minLength = ui.dsbLengthMin->value();
	args.maxLength = ui.dsbLengthMax->value();
	args.minWidth = ui.dsbWidthMin->value();
	args.maxWidth = ui.dsbWidthMax->value();
	args.minBendFreq = ui.dsbBendFreqMin->value();
	args.maxBendFreq = ui.dsbBendFreqMax->value();
	args.minBend = ui.dsbBendAmountMin->value();
	args.maxBend = ui.dsbBendAmountMax->value();
	args.minScale = ui.dsbScaleMin->value();
	args.maxScale = ui.dsbScaleMax->value();
	args.minRangeMult = ui.dsbMultMin->value();
	args.maxRangeMult = ui.dsbMultMax->value();

	try {
		map.SetArgs(args);
	}
	catch (std::exception e)
	{
		QMessageBox errorBox;
		errorBox.critical(nullptr, "Mountain Filter Creation Error", e.what());
		throw std::exception();
	}
	
}

