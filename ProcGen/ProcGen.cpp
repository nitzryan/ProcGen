#include "ProcGen.h"

ProcGen::ProcGen(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.pbGenerate, &QPushButton::pressed, this, [&]() {
        int seed = ui.sbSeed->value();
        ui.mapWidget->GenerateMap(seed, 0, 0);
    });

    //ui.mapWidget->GenerateMap(100, 0, 0);
}

ProcGen::~ProcGen()
{}

