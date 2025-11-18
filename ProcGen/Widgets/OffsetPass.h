#pragma once

#include <QWidget>
#include "ui_OffsetPass.h"
#include "IPassWidget.h"

class OffsetPass : public IPassWidget
{
	Q_OBJECT

public:
	OffsetPass();
	~OffsetPass();

	OffsetPass(std::ifstream& file);
	void WriteToFile(std::ofstream& file) const override final;
	void GetPassOutput(int width, int height, float* data) override final;
private:
	Ui::OffsetPassClass ui;
};

