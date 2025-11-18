#include "OffsetPass.h"
#include <fstream>

OffsetPass::OffsetPass()
	: IPassWidget()
{
	ui.setupUi(this);

	ui.dsbOffset->setValue(0);
}

OffsetPass::~OffsetPass()
{}

OffsetPass::OffsetPass(std::ifstream & file) : IPassWidget()
{
	ui.setupUi(this);
	
	double offsetAmount;
	file >> offsetAmount;

	ui.dsbOffset->setValue(offsetAmount);
}

void OffsetPass::WriteToFile(std::ofstream& file) const
{
	file << "\t\t\t" << static_cast<int>(IPassWidgetNS::PASS_TYPE::OFFSET) << " " << ui.dsbOffset->value();
}

void OffsetPass::GetPassOutput(int width, int height, float* data)
{
	double offset = ui.dsbOffset->value();
	for (int i = 0; i < width * height; i++)
		data[i] += offset;
}

