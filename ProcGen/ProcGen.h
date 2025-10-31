#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ProcGen.h"

class ProcGen : public QMainWindow
{
    Q_OBJECT

public:
    ProcGen(QWidget *parent = nullptr);
    ~ProcGen();

private:
    Ui::ProcGenClass ui;
};

