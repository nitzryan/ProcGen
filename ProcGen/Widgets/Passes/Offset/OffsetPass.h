#pragma once

#include <QWidget>
#include "ui_OffsetPass.h"
#include <Widgets/Passes/IPassWidget.h>
#include <Generation/MapData.h>

class OffsetPass : public IPassWidget
{
	Q_OBJECT

public:
	OffsetPass(const MapDimensions* md);
	~OffsetPass();

	OffsetPass(std::ifstream& file, const MapDimensions* md);
	void WriteToFile(std::ofstream& file) const override final;
	void GetPassOutput(float* data) override final;

private:
	Ui::OffsetPassClass ui;

	void SetupSignals();
};

