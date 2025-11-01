#include "PerlinPassWidget.h"

PerlinPassWidget::PerlinPassWidget(QString name, int seed, double blockSize, double scale, int numGradients, int perlinRepCount, QWidget* parent) 
	: QWidget(parent), perlinPass(numGradients, perlinRepCount)
{
	ui.setupUi(this);

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
		perlinPass.Reinitialize(ui.sbNumGrad->value(), ui.sbPerlinRep->value());
	};
	connect(ui.sbNumGrad, &QSpinBox::valueChanged, this, updatePerlinData);
	connect(ui.sbPerlinRep, &QSpinBox::valueChanged, this, updatePerlinData);
}

PerlinPassWidget::~PerlinPassWidget()
{
}

float* PerlinPassWidget::GetPassOutput(int width, int height)
{
	return perlinPass.GenerateMap(ui.sbSeed->value(), width, height, ui.dsbBlockSize->value(), ui.dsbScale->value());
}
