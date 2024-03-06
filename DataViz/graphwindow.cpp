#include "graphwindow.h"
#include "ui_graphwindow.h"

// Initialising the static variable
int GraphWindow::FigureCounter=0;

GraphWindow::GraphWindow(DataSet *DataSet,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GraphWindow)
{
    ui->setupUi(this);

    // Increment the figure counter:
    FigureCounter++;

    // Create the XY graph (setting te paramters of the dataset graph):
    SetGraphSetting(DataSet);

    // Setting the paramters of the figure:
    SetFigureSetting();

    // Setting the title of the figure
    this->setWindowTitle("Figure "+QString::number(FigureCounter));


    ContextMenu = new QMenu(this);

    constructMenu(ContextMenu);

}

GraphWindow::~GraphWindow()
{ // Called when the window of the figure is closed
    delete ui;
}

void GraphWindow::SetGraphSetting(DataSet *DataSet)
{ // Sets up the curve and plots it


    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->addData(DataSet);

    QPen graphPen;
    graphPen.setColor(graphColor);
    graphPen.setWidth(currentWidth);

    ui->customPlot->graph(0)->setPen(graphPen);
    ui->customPlot->graph(0)->setName(DataSet->getName());
    ui->customPlot->graph(0)->rescaleAxes();

}

void GraphWindow::SetFigureSetting()
{ // Sets up the properties of the figure (that contains the curve)

    // Remove the existing legend setup
    ui->customPlot->legend->clearItems();

    // Loop through each graph and add to the legend
    for (int i = 0; i < ui->customPlot->graphCount(); ++i) {
        ui->customPlot->legend->addItem(new QCPPlottableLegendItem(ui->customPlot->legend, ui->customPlot->graph(i)));
    }

    ui->customPlot->legend->setVisible(true);
    ui->customPlot->xAxis2->setVisible(true);
    ui->customPlot->xAxis2->setTickLabels(false);
    ui->customPlot->yAxis2->setVisible(true);
    ui->customPlot->yAxis2->setTickLabels(false);
    ui->customPlot->xAxis->setLabel("x");
    ui->customPlot->yAxis->setLabel("y");
    // make left and bottom axes always transfer their ranges to right and top axes:
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
}


void GraphWindow::constructMenu(QMenu *)
{// This function is called in the constructor to build the context menu so that it does not need to be built everytime from scratch
    qDebug() << "Construct context menu event is called";


    WidthMenu->addAction(IncreaseWidth);
    WidthMenu->addAction(DecreaseWidth);
    SettingsMenu->addMenu(WidthMenu); // Add the action to the menu


    CurveStyleMenu->addAction(LinePlot);
    CurveStyleMenu->addAction(StepPlotLeft);
    CurveStyleMenu->addAction(StepPlotRight);
    CurveStyleMenu->addAction(StepPlotCenter);
    CurveStyleMenu->addAction(ImpulsePlot);
    CurveStyleMenu->addAction(NoConnection);
    SettingsMenu->addMenu(CurveStyleMenu);

    SettingsMenu->addAction(ChangeColor);
    SettingsMenu->addAction(MultiplePlot);

    ContextMenu->addMenu(SettingsMenu); // Add the submenus to the main menu

    //connect the actions
    connect(IncreaseWidth, SIGNAL(triggered()), this, SLOT(increaseWidthAction()));
    connect(DecreaseWidth, SIGNAL(triggered()), this, SLOT(decreaseWidthAction()));

    connect(LinePlot, &QAction::triggered, this, [this]() { setLineStyle(QCPGraph::lsLine); });
    connect(StepPlotLeft, &QAction::triggered, this, [this]() { setLineStyle(QCPGraph::lsStepLeft); });
    connect(StepPlotRight, &QAction::triggered, this, [this]() { setLineStyle(QCPGraph::lsStepRight); });
    connect(StepPlotCenter, &QAction::triggered, this, [this]() { setLineStyle(QCPGraph::lsStepCenter); });
    connect(ImpulsePlot, &QAction::triggered, this, [this]() { setLineStyle(QCPGraph::lsImpulse); });
    connect(NoConnection, &QAction::triggered, this, [this]() { setLineStyle(QCPGraph::lsNone); });

    connect(ChangeColor, SIGNAL(triggered()), this, SLOT(setGraphColor2()));

    connect(MultiplePlot, SIGNAL(triggered()), this, SLOT(multipleDatasets()));

    qDebug() << "Connections made";
}

void GraphWindow::setGraphColor2()
{

    // Create a QStringList with the names of all graphs
    QStringList graphNames;
    for (int i = 0; i < ui->customPlot->graphCount(); ++i) {
        graphNames << QString("Graph %1").arg(i + 1);
    }

    // Prompt the user to select a graph
    bool ok;
    QString selectedGraph = QInputDialog::getItem(this, "Select Graph", "Choose a graph:", graphNames, 0, false, &ok);

    if (ok) {
        // Convert the selected graph name to its corresponding index
        int selectedGraphIndex = graphNames.indexOf(selectedGraph);

        // Prompt the user to choose a color
        QColor colorValue = QColorDialog::getColor(Qt::white, this, tr("Select a color"));

        if (colorValue.isValid()) {
            graphColor = colorValue;
            // Set the color of the selected graph
            ui->customPlot->graph(selectedGraphIndex)->setPen(QPen(graphColor));

            // Redraw the plot
            ui->customPlot->replot();
        }
    }
}

void GraphWindow::setLineStyle(QCPGraph::LineStyle lineStyle)
{

    // Iterate through all the graphs and set the pen style for each one
    for (int i = 0; i < ui->customPlot->graphCount(); ++i) {
        QPen linepen = ui->customPlot->graph(i)->pen();
        linepen.setStyle((Qt::PenStyle)lineStyle);
        ui->customPlot->graph(i)->setPen(linepen);
    }

    // Redraw the plot
    ui->customPlot->replot();
}


void GraphWindow::contextMenuEvent(QContextMenuEvent *event)
{
    ContextMenu->popup(event->globalPos());
    event->accept();  // Indicate that the event has been handled
}


void GraphWindow::increaseWidthAction()
{
    // Create a QStringList with the names of all graphs
    QStringList graphNames;
    for (int i = 0; i < ui->customPlot->graphCount(); ++i) {
        graphNames << QString("Graph %1").arg(i + 1);
    }

    // Prompt the user to select a graph
    bool ok;
    QString selectedGraph = QInputDialog::getItem(this, "Select Graph", "Choose a graph:", graphNames, 0, false, &ok);

    if(ok){
        // Convert the selected graph name to its corresponding index
        int selectedGraphIndex = graphNames.indexOf(selectedGraph);
        QPen widthPen = ui->customPlot->graph(selectedGraphIndex)->pen();
        currentWidth = widthPen.width();
        widthPen.setWidth(currentWidth + 1);
        ui->customPlot->graph(selectedGraphIndex)->setPen(widthPen);

        // Redraw the plot
        ui->customPlot->replot();
    }
}

void GraphWindow::decreaseWidthAction()
{
    // Create a QStringList with the names of all graphs
    QStringList graphNames;
    for (int i = 0; i < ui->customPlot->graphCount(); ++i) {
        graphNames << QString("Graph %1").arg(i + 1);
    }

    // Prompt the user to select a graph
    bool ok;
    QString selectedGraph = QInputDialog::getItem(this, "Select Graph", "Choose a graph:", graphNames, 0, false, &ok);

    if(ok){
        // Convert the selected graph name to its corresponding index
        int selectedGraphIndex = graphNames.indexOf(selectedGraph);
        QPen widthPen = ui->customPlot->graph(selectedGraphIndex)->pen();
        currentWidth = widthPen.width();
        widthPen.setWidth(currentWidth - 1);
        ui->customPlot->graph(selectedGraphIndex)->setPen(widthPen);

        // Redraw the plot
        ui->customPlot->replot();
    }
}

void GraphWindow::opendatasets(QList<DataSet*> AllDataSets){
    for(DataSet *data : AllDataSets){
        this->AllDataSets.append(data);
    }
    qDebug() << "opendatasets called";

}


void GraphWindow::multipleDatasets()
{

    qDebug() << "Multiple datasets called";

    //Put the name of all the datasets in a string list

    QStringList filenames;
    for(DataSet *data: AllDataSets){
        filenames << data->getName();
        qDebug() << data->getName();
    }

    // Prompt the user to select a graph
    bool ok;
    QString selectedGraph = QInputDialog::getItem(this, "Select Dataset", "Choose a dataset:", filenames, 0, false, &ok);

    for(DataSet *data : AllDataSets){
        if(data->getName() == selectedGraph){
            ui->customPlot->addGraph();
            ui->customPlot->graph()->addData(data);
            ui->customPlot->graph()->setName(data->getName());
            ui->customPlot->rescaleAxes();
            QColor randomColor = QColor::fromRgb(QRandomGenerator::global()->generate());
            // Set the line color of the graph
            ui->customPlot->graph()->setPen(QPen(randomColor));
            ui->customPlot->replot();
            break;
        }
    }


}
