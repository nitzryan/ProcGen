#include "PerlinPassWidget.h"

#include <fstream>
#include <Widgets/QStringUtilities.h>

PerlinPassWidget::PerlinPassWidget(const MapDimensions* md)
	: IPassWidget(md)
{
	ui.setupUi(this);

	int numGradients = 16;
	int perlinRepCount = 256;
	perlinPass = new PerlinPass(numGradients, perlinRepCount);

	ui.sbSeed->setValue(0);
	ui.dsbBlockSize->setValue(64);
	ui.dsbScale->setValue(0.1);
	ui.sbNumGrad->setValue(numGradients);
	ui.sbPerlinRep->setValue(perlinRepCount);

	SetupSignals();
}

PerlinPassWidget::PerlinPassWidget(std::ifstream& file, const MapDimensions* md) 
	: IPassWidget(md)
{
	int seed, numGradients, perlinRepCount;
	double blockSize, scale;
	file >> seed;
	file >> numGradients;
	file >> perlinRepCount;
	file >> blockSize;
	file >> scale;

	ui.setupUi(this);

	perlinPass = new PerlinPass(numGradients, perlinRepCount);

	ui.sbSeed->setValue(seed);
	ui.dsbBlockSize->setValue(blockSize);
	ui.dsbScale->setValue(scale);
	ui.sbNumGrad->setValue(numGradients);
	ui.sbPerlinRep->setValue(perlinRepCount);

	SetupSignals();
}

void PerlinPassWidget::WriteToFile(std::ofstream& file) const
{
	file << "\t\t\t" << static_cast<int>(IPassWidgetNS::PASS_TYPE::PERLIN) << " ";
	file << ui.sbSeed->value() << " " << ui.sbNumGrad->value() << " " << ui.sbPerlinRep->value() << " " << ui.dsbBlockSize->value() << " " << ui.dsbScale->value() << "\n";
}

PerlinPassWidget::~PerlinPassWidget()
{
	delete perlinPass;
}

void PerlinPassWidget::GetPassOutput(float* data)
{
	perlinPass->GenerateMap(ui.sbSeed->value(), mapDimensions->width, mapDimensions->height, ui.dsbBlockSize->value(), ui.dsbScale->value(), data);
}

void PerlinPassWidget::SetupSignals()
{
	auto updatePerlinData = [&]() {
		perlinPass->Reinitialize(ui.sbNumGrad->value(), ui.sbPerlinRep->value());
		};
	connect(ui.sbNumGrad, &QSpinBox::valueChanged, this, updatePerlinData);
	connect(ui.sbPerlinRep, &QSpinBox::valueChanged, this, updatePerlinData);

	connect(ui.pbView, &QPushButton::pressed, this, &IPassWidget::EmitPassOutput);
	connect(ui.pbDelete, &QPushButton::pressed, this, &IPassWidget::DeletePass);
}

