#pragma once

#include <QWidget>
#include <vector>
#include <QVBoxLayout>
#include <qboxlayout.h>
#include <qlabel.h>

class MapWidget : public QWidget
{
	Q_OBJECT

public:
	MapWidget(QWidget* parent = nullptr);
	~MapWidget();

	void GenerateMap(int width, int height, QList<uchar> data);
	
private:

	QVBoxLayout* layout;
	QLabel* mapLabel;
};

