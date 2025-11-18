#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ProcGen.h"
#include <Generation/Perlin/PerlinPass.h>
#include <Widgets/Passes/Perlin/PerlinPassWidget.h>

class ProcGen : public QMainWindow
{
    Q_OBJECT

public:
    ProcGen(QWidget *parent = nullptr);
    ~ProcGen();

private:
    Ui::ProcGenClass ui;

    QList<PerlinPassWidget*> perlinPassWidgets;
};

