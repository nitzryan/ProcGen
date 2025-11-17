#include "ProcGen.h"

#include "Generation/MountainFilterMap.h"

ProcGen::ProcGen(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    connect(ui.pipeline, &GenerationPipeline::PipelineOutput, ui.mapWidget, &MapWidget::GenerateMap);

    MountainFilterMap mfm;
    mfm.SetNumRanges(2, 4);
    mfm.SetBendAmount(1, 40);
    mfm.SetLengthPerBend(100);
    mfm.SetRangeLength(100, 400);
    mfm.SetRangeWidth(10, 40);

    int w = 1024, h = 1024;
    mfm.GenerateMap(w, h, 0);
    QList<uchar> pixelData(w * h);
    float* mapData = mfm.GetMap();
    for (size_t i = 0; i < w * h; i++)
        pixelData[i] = mapData[i];
    ui.mapWidget->GenerateMap(w, h, pixelData);
}

ProcGen::~ProcGen()
{}

