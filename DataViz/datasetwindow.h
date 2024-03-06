#ifndef DATASETWINDOW_H
#define DATASETWINDOW_H

/********************************
 *
 *  This class is defined to show the dataset in a window,
 *  an object of this class is the window in which a dataset is shown to the user
 *
 *  A dataset should only be visible to the user through a DataSetWindow object
 *
 *
 *
 *
**********************************/


#include <QDialog>
#include <QMenu>
#include <QContextMenuEvent>
#include <QAction>
#include "dataset.h"

namespace Ui {
class DataSetWindow;
}

class DataSetWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DataSetWindow(DataSet* Datase,QWidget *parent = nullptr);
    ~DataSetWindow();
    void contextMenuEvent(QContextMenuEvent *event);
    void ConstructContextMenu(QMenu *);


public slots:
    void DataSetToBePlotted(); // Local slot to forward a single to the parent window
    void PlotHistogram();


signals:
    void Plot_XYPlot_SIGNAL(DataSet *ptr); // A signal to tell the parent window that the user wants this DataSet to be plot
    void Plot_Histogram_SIGNAL(DataSet *ptr);


private:
    Ui::DataSetWindow *ui;

    DataSet *DisplayedDataSet=nullptr; // A Variable referring to dataset dislayed in the datasetwindow

    // List of actions in the app (defined in the code rather than the UI)
    QAction* XYPlot=new QAction("XY Plot", this);
    QAction* Histogram=new QAction("Histogram Plot", this);


    //Creating the menus (all accessable from the context menu):
    QMenu *ContextMenu=new QMenu(this);
    QMenu *PlotSubMenu=new QMenu("Plot"); // Plot section

};

#endif // DATASETWINDOW_H
