#include "datasetwindow.h"
#include "ui_datasetwindow.h"

DataSetWindow::DataSetWindow(DataSet* DataSet,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataSetWindow)
{// This is called when a dataset is to be plot in a table displayed in a mdisubwindow
    ui->setupUi(this);

    // Initialising the member data
    DisplayedDataSet=DataSet;


    // Setting up the table
        ui->Table->setColumnCount(2); // 2 columns one for x and one for y
        QTableWidgetItem* TableItem=nullptr; // Variable used to ppulate the table

        // Setting the header lalebels of the columns
        QStringList ColumnHeaders;
        ColumnHeaders<<"x"<<"y";
        ui->Table->setHorizontalHeaderLabels(ColumnHeaders);


        // Populating the table
        for (int i=0;i<DataSet->Size();i++)
        {
            ui->Table->insertRow(i); // Adds a new row to the table

            double* ValuePtr=DataSet->getPoint(i);
            QString x_value=QString::number(*ValuePtr);
            QString y_value= QString::number(*(++ValuePtr));


           TableItem=new  QTableWidgetItem(x_value,0); // Reading x coordinate
           ui->Table->setItem(i,0,TableItem);// Setting the item in the first column to x


           TableItem=new  QTableWidgetItem(y_value,0); // Reading y coordinate
           ui->Table->setItem(i,1,TableItem);// Setting the item in the second column to y


        }


       // Setting the title of the window
       this->setWindowTitle("Dataset: "+DataSet->getName());

       // Constructing the context menu so it is ready to be called whenever
       ConstructContextMenu(ContextMenu);


       // Setting icons for actions:
        const QIcon XYPlot_icon=QIcon(":/icons/graph.svg");
        XYPlot->setIcon(XYPlot_icon);



       // connecting actions to responses via signal-slot mechanism:
        connect(XYPlot,SIGNAL(triggered()),this,SLOT(DataSetToBePlotted()));
        connect(Histogram, SIGNAL(triggered()), this, SLOT(PlotHistogram()));

}

DataSetWindow::~DataSetWindow()
{ // This  function is called when the window is closed (used for cleanup)
    delete PlotSubMenu;
    delete ContextMenu;
    delete XYPlot;
    delete DisplayedDataSet;
    delete ui;
}

void DataSetWindow::contextMenuEvent(QContextMenuEvent *event)
{ // This function is called when the user right-clicks on the datasetwindow
   ContextMenu->popup(event->globalPos()); // displaying the menu where the user clicks
}

void DataSetWindow::ConstructContextMenu(QMenu *)
{// This function is called in the constructor to build the context menu so that it does not need to be built everytime from scratch
    PlotSubMenu->addAction(XYPlot); // Add the action to the menu
    PlotSubMenu->addAction(Histogram);
    ContextMenu->addMenu(PlotSubMenu); // Add the submenus to the main menu
}


void DataSetWindow::DataSetToBePlotted()
{// A signal to tell the parent widnow that the dataset must be plotted when the user choses XYPlot
 // from the context Menu of the DatSetWidnow

   emit Plot_XYPlot_SIGNAL(DisplayedDataSet);
}


void DataSetWindow::PlotHistogram(){
    //A signal to tell the parent window that the dataset must be plotted in a histogram

    emit Plot_Histogram_SIGNAL(DisplayedDataSet);
}
