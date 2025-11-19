#include "ProcGen.h"

#include <Generation/Mountain/MountainFilterMap.h>

ProcGen::ProcGen(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    connect(ui.pipeline, &GenerationPipeline::PipelineOutput, ui.mapWidget, &MapWidget::GenerateMap);
    connect(ui.pipeline, &GenerationPipeline::OutputPassData, ui.mapWidget, &MapWidget::GenerateMap2);
}

ProcGen::~ProcGen()
{}

