#include "MapWidget.h"

#include <random>
#include <algorithm>
#include <qlabel.h>

MapWidget::MapWidget(QWidget* parent) : QWidget(parent)
{
	mapLabel = nullptr;
	layout = new QVBoxLayout(this);
}

MapWidget::~MapWidget()
{
	if (mapLabel != nullptr)
		delete mapLabel;

	delete layout;
}

void MapWidget::GenerateMap(int width, int height, QList<uchar> data)
{
	
	QImage image = QImage(data.data(), width, height, width, QImage::Format_Grayscale8);
	
	if (mapLabel != nullptr)
	{
		layout->removeWidget(mapLabel);
		delete mapLabel;
	}
		
	mapLabel = new QLabel(this);
	mapLabel->setFixedSize(width, height);
	mapLabel->setScaledContents(true);
	mapLabel->setPixmap(QPixmap::fromImage(image));
	mapLabel->show();
	layout->addWidget(mapLabel);
	this->update();
}

