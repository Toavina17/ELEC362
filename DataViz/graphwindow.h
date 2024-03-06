#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

/********************************
 *
 *  This class is defined to show a graph in a window,
 *  an object of this class is the window in which a graph is plotted
 *
 *  In this class, a figure is the frame of the plot of the curves, its
 *  properties include the axes, the grid, the title, the legend.
 *
 *  In this class, a graph is the curve plotted, its properties include
 *  the width of the line, its style, and its colour.
 *
 *
**********************************/



#include <QDialog>
#include <QMenu>
#include "dataset.h"
#include "qpen.h"
#include "qcustomplot.h"
#include "documentinfo.h"


namespace Ui {
class GraphWindow;
}

class GraphWindow : public QDialog
{
    Q_OBJECT

public:
    explicit GraphWindow(DataSet *DataSet,QWidget *parent = nullptr);
    ~GraphWindow();
    void SetGraphSetting(DataSet *DataSet); // Function to control the setting of the curve (dataset represenation in the figure)
    void SetFigureSetting(); // Function to control the setting of the figure itself


public slots:
    //void SetGraphColor(const QColor &color);
    void contextMenuEvent(QContextMenuEvent *event);
    void constructMenu(QMenu *);
    void increaseWidthAction();
    void decreaseWidthAction();
    void setLineStyle(QCPGraph::LineStyle lineStyle);
    void setGraphColor2();
    void multipleDatasets();

    void opendatasets(QList<DataSet*> AllDataSets);

private:
    Ui::GraphWindow *ui;
    QColor graphColor = Qt::blue;
    int currentWidth = 0;
    static int FigureCounter; // Number of Figures created in the app ( defined as static because it is shared among all objects of this class)

    //Change Width
    QMenu *ContextMenu;
    QMenu *SettingsMenu = new QMenu("Curve properties", this);
    QMenu* WidthMenu = new QMenu("Change curve width", this);
    QAction* IncreaseWidth = new QAction("Increase width", this);
    QAction* DecreaseWidth = new QAction("Decrease width", this);

    //Curve Style
    QMenu* CurveStyleMenu = new QMenu("Choose curve style", this);
    QAction* LinePlot = new QAction("Line plot", this);
    QAction* StepPlotLeft = new QAction("Step plot left", this);
    QAction* StepPlotRight = new QAction("Step plot right", this);
    QAction* StepPlotCenter = new QAction("Step plot center", this);
    QAction* ImpulsePlot = new QAction("Impulse plot", this);
    QAction* NoConnection = new QAction("No connection plot", this);
    QAction* ScatterPlot = new QAction("Scatter plot", this);
    QAction* MultiplePlot = new QAction("Plot multiple datasets", this);

    //Color
    QAction* ChangeColor = new QAction("Change color", this);

    QList<DataSet *> AllDataSets;


signals:
    void colorChanged(const QColor &color);

};

#endif // GRAPHWINDOW_H
