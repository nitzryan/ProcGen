#include "ProcGen.h"

#include "Generation/MountainFilterMap.h"

ProcGen::ProcGen(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    connect(ui.pipeline, &GenerationPipeline::PipelineOutput, ui.mapWidget, &MapWidget::GenerateMap);
}

ProcGen::~ProcGen()
{}

