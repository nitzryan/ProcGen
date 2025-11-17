#include "ProcGen.h"

#include "Generation/MountainFilterMap.h"

ProcGen::ProcGen(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    connect(ui.pipeline, &GenerationPipeline::PipelineOutput, ui.mapWidget, &MapWidget::GenerateMap);

    MountainFilterMap mfm;
    MountainFilterMapArgs args;
    args.minRanges = 4;
    args.maxRanges = 8;
    args.minLength = 100;
    args.maxLength = 400;
    args.minWidth = 30;
    args.maxWidth = 80;
    args.lengthPerBend = 50;
    args.minBend = 0;
    args.maxBend = M_PI / 6;
    args.minScale = 0.4f;
    args.maxScale = 1.0f;
    args.minRangeMult = 1.0f;
    args.maxRangeMult = 3.0f;
    mfm.SetArgs(args);

    int w = 1024, h = 1024;
    mfm.GenerateMap(w, h, 0);
    QList<uchar> pixelData(w * h);
    float* mapData = mfm.GetMap();
    for (size_t i = 0; i < w * h; i++)
        pixelData[i] = 255.0f * mapData[i];
    ui.mapWidget->GenerateMap(w, h, pixelData);
}

ProcGen::~ProcGen()
{}

