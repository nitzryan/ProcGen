#include "PerlinFractalWidget.h"

#include <random>
#include <fstream>

PerlinFractalWidget::PerlinFractalWidget(const MapDimensions* md)
	: IPassWidget(md)
{
	ui.setupUi(this);
	Setup(16, 256);
	ui.sbSeed->setValue(0);
	ui.sbNumLayers->setValue(4);
	ui.dsbBlockMult->setValue(0.5);
	ui.dsbBlockInit->setValue(256);
	ui.dsbScaleMult->setValue(0.5);
	ui.dsbScaleInit->setValue(1);
}

PerlinFractalWidget::~PerlinFractalWidget()
{
	delete perlinPass;
}

PerlinFractalWidget::PerlinFractalWidget(std::ifstream & file, const MapDimensions * md)
	: IPassWidget(md)
{
	ui.setupUi(this);

	// Read File
	int seed, numLayers, numGradients, perlinRepCount;
	double blockSizeMult, blockSizeInit, scaleMult, scaleInit;
	file >> seed >> numLayers >> numGradients >> perlinRepCount >> blockSizeMult >> blockSizeInit >> scaleMult >> scaleInit;

	// Set initial values
	ui.sbSeed->setValue(seed);
	ui.sbNumLayers->setValue(numLayers);
	ui.dsbBlockMult->setValue(blockSizeMult);
	ui.dsbBlockInit->setValue(blockSizeInit);
	ui.dsbScaleMult->setValue(scaleMult);
	ui.dsbScaleInit->setValue(scaleInit);

	// Setup connections and perlin pass
	Setup(numGradients, perlinRepCount);
}

void PerlinFractalWidget::WriteToFile(std::ofstream& file) const
{
	file << "\t\t\t" << static_cast<int>(IPassWidgetNS::PASS_TYPE::PERLIN_FRACTAL) << " ";
	file << ui.sbSeed->value() << " "
		<< ui.sbNumLayers->value() << " "
		<< ui.sbNumGradients->value() << " "
		<< ui.sbPerlinRepCount->value() << " "
		<< ui.dsbBlockMult->value() << " "
		<< ui.dsbBlockInit->value() << " "
		<< ui.dsbScaleMult->value() << " "
		<< ui.dsbScaleInit->value() << "\n";
}

void PerlinFractalWidget::GetPassOutput(float* data)
{
	std::mt19937 mt(ui.sbSeed->value());
	std::uniform_int_distribution<int> seedDist(
		std::numeric_limits<int>::min(), std::numeric_limits<int>::max()
	);

	double blockSize = ui.dsbBlockInit->value();
	double scale = ui.dsbScaleInit->value();

	double blockMult = ui.dsbBlockMult->value();
	double scaleMult = ui.dsbScaleMult->value();

	for (size_t i = 0; i < ui.sbNumLayers->value(); i++)
	{
		perlinPass->GenerateMap(seedDist(mt), mapDimensions->width, mapDimensions->height, blockSize, scale, data);

		blockSize *= blockMult;
		scale *= scaleMult;
	}
}

void PerlinFractalWidget::Setup(int numGradients, int perlinRepCount)
{
	perlinPass = new PerlinPass(numGradients, perlinRepCount);

	ui.sbNumGradients->setValue(numGradients);
	ui.sbPerlinRepCount->setValue(perlinRepCount);

	auto updatePerlinArchitecture = [&]() {
		perlinPass->Reinitialize(ui.sbNumGradients->value(), ui.sbPerlinRepCount->value());
		};
	connect(ui.sbNumGradients, &QSpinBox::valueChanged, this, updatePerlinArchitecture);
	connect(ui.sbPerlinRepCount, &QSpinBox::valueChanged, this, updatePerlinArchitecture);

	// Signals to parent
	connect(ui.pbView, &QPushButton::pressed, this, &IPassWidget::EmitPassOutput);
	connect(ui.pbDelete, &QPushButton::pressed, this, &IPassWidget::DeletePass);
}

