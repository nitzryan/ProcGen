#include "PerlinPassWidget.h"

#include <fstream>

PerlinPassWidget::PerlinPassWidget(QString name, int seed, double blockSize, double scale, int numGradients, int perlinRepCount, QWidget* parent) 
	: QWidget(parent)
{
	ui.setupUi(this);

	perlinPass = new PerlinPass(numGradients, perlinRepCount);

	connect(ui.dropdownButton, &QToolButton::pressed, this, [&]() {
		dropdownVisible = !dropdownVisible;
		ui.contentsWidget->setVisible(dropdownVisible);

		if (dropdownVisible)
			ui.dropdownButton->setArrowType(Qt::UpArrow);
		else
			ui.dropdownButton->setArrowType(Qt::DownArrow);
	});
	ui.dropdownButton->setText(name);
	ui.contentsWidget->setVisible(false);

	ui.sbSeed->setValue(seed);
	ui.dsbBlockSize->setValue(blockSize);
	ui.dsbScale->setValue(scale);
	ui.sbNumGrad->setValue(numGradients);
	ui.sbPerlinRep->setValue(perlinRepCount);

	auto updatePerlinData = [&]() {
		perlinPass->Reinitialize(ui.sbNumGrad->value(), ui.sbPerlinRep->value());
	};
	connect(ui.sbNumGrad, &QSpinBox::valueChanged, this, updatePerlinData);
	connect(ui.sbPerlinRep, &QSpinBox::valueChanged, this, updatePerlinData);

	connect(ui.cbPosition, &QComboBox::currentIndexChanged, this, &PerlinPassWidget::PositionChanged);
}

PerlinPassWidget::PerlinPassWidget(std::ifstream& file) : QWidget(nullptr)
{
	std::string name;
	file >> name;
	int seed, numGradients, perlinRepCount;
	double blockSize, scale;
	file >> seed;
	file >> numGradients;
	file >> perlinRepCount;
	file >> blockSize;
	file >> scale;

	ui.setupUi(this);

	perlinPass = new PerlinPass(numGradients, perlinRepCount);

	connect(ui.dropdownButton, &QToolButton::pressed, this, [&]() {
		dropdownVisible = !dropdownVisible;
		ui.contentsWidget->setVisible(dropdownVisible);

		if (dropdownVisible)
			ui.dropdownButton->setArrowType(Qt::UpArrow);
		else
			ui.dropdownButton->setArrowType(Qt::DownArrow);
		});
	ui.dropdownButton->setText(name.c_str());
	ui.contentsWidget->setVisible(false);

	ui.sbSeed->setValue(seed);
	ui.dsbBlockSize->setValue(blockSize);
	ui.dsbScale->setValue(scale);
	ui.sbNumGrad->setValue(numGradients);
	ui.sbPerlinRep->setValue(perlinRepCount);

	auto updatePerlinData = [&]() {
		perlinPass->Reinitialize(ui.sbNumGrad->value(), ui.sbPerlinRep->value());
		};
	connect(ui.sbNumGrad, &QSpinBox::valueChanged, this, updatePerlinData);
	connect(ui.sbPerlinRep, &QSpinBox::valueChanged, this, updatePerlinData);

	connect(ui.cbPosition, &QComboBox::currentIndexChanged, this, &PerlinPassWidget::PositionChanged);
}

void PerlinPassWidget::WriteToFile(std::ofstream& file)
{
	file << "1\n";
	file << ui.dropdownButton->text().toStdString() + " ";
	file << ui.sbSeed->value() << " " << ui.sbNumGrad->value() << " " << ui.sbPerlinRep->value() << " " << ui.dsbBlockSize->value() << " " << ui.dsbScale->value() << "\n";
}

PerlinPassWidget::~PerlinPassWidget()
{
	delete perlinPass;
}

float* PerlinPassWidget::GetPassOutput(int width, int height)
{
	return perlinPass->GenerateMap(ui.sbSeed->value(), width, height, ui.dsbBlockSize->value(), ui.dsbScale->value());
}

void PerlinPassWidget::SetPositionComboBox(int length, int index)
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
