#pragma once

#include <QWidget>
#include "ui_OffsetPass.h"
#include <Widgets/Passes/IPassWidget.h>

class OffsetPass : public IPassWidget
{
	Q_OBJECT

public:
	OffsetPass(const MapDimensions* md);
	~OffsetPass();

	OffsetPass(std::ifstream& file, const MapDimensions* md);
	void WriteToFile(std::ofstream& file) const override final;
	void GetPassOutput(float* data) override final;

signals:
	void OutputPassData(std::shared_ptr<float[]> data, const MapDimensions* md);
private:
	Ui::OffsetPassClass ui;
};

