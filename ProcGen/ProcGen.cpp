#include "ProcGen.h"

ProcGen::ProcGen(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.pbGenerate, &QPushButton::pressed, this, [&]() {
        int seed = ui.sbSeed->value();
        int width = ui.sbWidth->value();
        int height = ui.sbHeight->value();
        double scale = ui.dsbScale->value();
        double blockSize = ui.dsbBlockSize->value();
        ui.mapWidget->GenerateMap(seed, width, height, blockSize, scale);
    });

    ui.mapWidget->GenerateMap(0, ui.sbWidth->value(), ui.sbHeight->value(), ui.dsbBlockSize->value(), ui.dsbScale->value());
}

ProcGen::~ProcGen()
{}

