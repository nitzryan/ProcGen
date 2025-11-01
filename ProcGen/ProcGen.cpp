#include "ProcGen.h"

ProcGen::ProcGen(QWidget *parent)
    : QMainWindow(parent), perlinPass(16, 256)
{
    ui.setupUi(this);
    connect(ui.pbGenerate, &QPushButton::pressed, this, [&]() {
        int seed = ui.sbSeed->value();
        int width = ui.sbWidth->value();
        int height = ui.sbHeight->value();
        double scale = ui.dsbScale->value();
        double blockSize = ui.dsbBlockSize->value();
        float* perlinPassData = perlinPass.GenerateMap(seed, width, height, blockSize, scale);
        std::vector<uchar> pixelData(width * height);
        for (size_t i = 0; i < width * height; i++)
        {
            pixelData[i] = (perlinPassData[i] + 1.0f) / 2.0f * 255.f;
        }

        ui.mapWidget->GenerateMap(width, height, pixelData.data());
    });

    ui.pbGenerate->click();
}

ProcGen::~ProcGen()
{}

