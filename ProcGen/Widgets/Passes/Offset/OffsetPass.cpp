#include "OffsetPass.h"
#include <fstream>

OffsetPass::OffsetPass(const MapDimensions* md)
	: IPassWidget(md)
{
	ui.setupUi(this);

	ui.dsbOffset->setValue(0);

	SetupSignals();
}

OffsetPass::~OffsetPass()
{}

OffsetPass::OffsetPass(std::ifstream & file, const MapDimensions* md) 
	: IPassWidget(md)
{
	ui.setupUi(this);
	
	double offsetAmount;
	file >> offsetAmount;

	ui.dsbOffset->setValue(offsetAmount);

	SetupSignals();
}

void OffsetPass::WriteToFile(std::ofstream& file) const
{
	file << "\t\t\t" << static_cast<int>(IPassWidgetNS::PASS_TYPE::OFFSET) << " " << ui.dsbOffset->value();
}

void OffsetPass::GetPassOutput(float* data)
{
	double offset = ui.dsbOffset->value();
	for (int i = 0; i < mapDimensions->width * mapDimensions->height; i++)
		data[i] += offset;
}

void OffsetPass::SetupSignals()
{
	connect(ui.pbView, &QPushButton::pressed, this, &IPassWidget::EmitPassOutput);
	connect(ui.pbDelete, &QPushButton::pressed, this, &IPassWidget::DeletePass);
}

