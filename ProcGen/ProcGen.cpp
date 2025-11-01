#include "ProcGen.h"

ProcGen::ProcGen(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.pbGenerate, &QPushButton::pressed, this, [&]() {
        int width = ui.sbWidth->value();
        int height = ui.sbHeight->value();
        std::vector<float> data = std::vector<float>(width * height, 0);
        for (auto pass : perlinPassWidgets)
        {
            float* passData = pass->GetPassOutput(width, height);
            for (size_t i = 0; i < width * height; i++)
            {
                data[i] += passData[i];
            }
        }
        std::vector<uchar> pixelData(width * height);
        for (size_t i = 0; i < width * height; i++)
        {
            pixelData[i] = data[i] * 255.f;
        }

        ui.mapWidget->GenerateMap(width, height, pixelData.data());
    });

    PerlinPassWidget* ppw = new PerlinPassWidget("Initial Pass", 0, 128.0, 0.5, 16, 256);
    PerlinPassWidget* ppw2 = new PerlinPassWidget("Larger Pass", 10, 512.0, 0.5, 16, 256);
    perlinPassWidgets.append(ppw);
    ui.passDataLayout->addWidget(ppw);
    perlinPassWidgets.append(ppw2);
    ui.passDataLayout->addWidget(ppw2);

    ui.pbGenerate->click();


}

ProcGen::~ProcGen()
{}

