#include "parentwindow.h"
#include "atmsp.h"
#include "histogramclass.h"
#include "ui_parentwindow.h"

ParentWindow::ParentWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ParentWindow)
{ // This block is called when the app is started
    ui->setupUi(this);
    setCentralWidget(ui->centralwidget);

    // Assuming ui->tableWidget is the name of your QTableWidget
    QTableWidget *tableWidget = ui->tableWidget;

    // Set the table properties
    tableWidget->setRowCount(0); // Initial row count
    tableWidget->setColumnCount(2); // Assuming two columns (Name and Comment)

    // Set column titles
    QStringList headers;
    headers << "Name" << "Comment";
    tableWidget->setHorizontalHeaderLabels(headers);

    connect(ui->actionDraw_Dataset, &QAction::triggered, this, &ParentWindow::on_actionDraw_Dataset_triggered);

    // Connect the itemDoubleClicked signal to the slot that handles editing
    connect(tableWidget, &QTableWidget::itemDoubleClicked, this, &ParentWindow::editComment);

    connect(ui->actionPlot_histogram, &QAction::triggered, this, &ParentWindow::on_actionPlot_histogram_triggered);

}

ParentWindow::~ParentWindow()
{ // This block is called when the app is closing (to clean up all memory allocation used when tha app started)
    delete AddedDataSetWindow;
    delete subWindow;
    delete AddedDataSet;
    delete AddedGraphWindow;
    delete ui;
}

void ParentWindow::on_actionLoad_Dataset_triggered()
{ // This block is called when the user triggers Load action to load a file

    // Opening a file dialog
    QString curPath=QDir::currentPath(); // Directs the "open file" to the current directory
    QString FileName=QFileDialog::getOpenFileName(this,tr("Open file"),curPath,tr("Text files (*.txt);;Images (*.png *.xpm *.jpg);;All files(*.*)"));


    if (FileName.isEmpty())
        return; //If no file is selected don't do anything further

    // Creating a new dataset in the app:
    AddedDataSet= new DataSet(FileName);
    if (AddedDataSet->IsDataSetValid) // Making sure the dataset is only dealt with if it was loaded succsessfully
    {

        AllDataSets.push_back(AddedDataSet); // Addding a pointer to the new dataset so that it can be accessed by the rest of the app

        // Creat a subWindow for the loaded DataSet:
        AddedDataSetWindow=new DataSetWindow(AddedDataSet,this);
        subWindow=ui->WindowsManager->addSubWindow(AddedDataSetWindow);
        AddedDataSetWindow->show(); // showing the new dataset window to the user (when it is added for the first time)


        // To enable the ParentWindow to plot the dataset when the user clicks on XYPlot option in the context menu
        // of an already displayed DataSetWidnow
        connect(AddedDataSetWindow,SIGNAL(Plot_XYPlot_SIGNAL(DataSet*)),this,SLOT(GraphWindowToBePlotted(DataSet*)));

        //To enable the ParentWindow to plot an histogram of the Data
        connect(AddedDataSetWindow, SIGNAL(Plot_Histogram_SIGNAL(DataSet*)), this, SLOT(on_actionPlot_histogram_triggered()));

        int rowPosition = ui->tableWidget->rowCount(); // Get the current number of rows
        ui->tableWidget->insertRow(rowPosition);

        // Add items to the new row
        QTableWidgetItem *nameItem = new QTableWidgetItem(AddedDataSet->getName());
        QTableWidgetItem *commentItem = new QTableWidgetItem(AddedDataSet->getComment());

        ui->tableWidget->setItem(rowPosition, 0, nameItem);    // Assuming column 0 is for names
        ui->tableWidget->setItem(rowPosition, 1, commentItem);
    }

}

void ParentWindow::GraphWindowToBePlotted(DataSet *ptr)
{ // This function is a slot that is called when the parentwindow is to plot a dataset in from a datasetwindow
    //GraphWindow *AddedGraphWindow=new GraphWindow(ptr,this);
    AddedGraphWindow = new GraphWindow(ptr,this);
    subWindow=ui->WindowsManager->addSubWindow(AddedGraphWindow);
    AddedGraphWindow->show();
    connect(this, &ParentWindow::sendOpenDatasets, AddedGraphWindow, &GraphWindow::opendatasets);
    //connect(this, &ParentWindow::ColorSelected, AddedGraphWindow, &GraphWindow::SetGraphColor);
    emit sendOpenDatasets(AllDataSets);
    qDebug() << "Connection established between ColorSelected and SetGraphColor.";
}

void ParentWindow::on_actionAbout_triggered()
{ // This function is called when the user clicks on "About" option under "About" menu
    AboutDialog* About_dlg=new AboutDialog(this);
    About_dlg->exec();
    delete About_dlg;
}

void ParentWindow::on_actionHelp_triggered()
{// This function is called when the user clicks on "Help" option under "Help" menu
    HelpDialog* Help_dlg=new HelpDialog(this);
    Help_dlg->exec();
    delete Help_dlg;
}

void ParentWindow::on_actionFunction_triggered()
{
    bool ok;
    QString expression = QInputDialog::getText(this, "Enter Mathematical Expression", "Expression:", QLineEdit::Normal, "", &ok);
    QRegularExpression regex("D\\d+");

    //assume that the x coordinates are the same
    if (ok && !expression.isEmpty()) {


        QRegularExpressionMatchIterator matchIterator = regex.globalMatch(expression);

        // Store the dataset references in a QStringList
        QStringList datasetReferences;
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            datasetReferences << match.captured();
        }

        QList<DataSet*> referencedDatasets;
        for (DataSet *data : AllDataSets) {
            for (const QString& datasetRef : datasetReferences) {
                if (data->getName() == datasetRef) {
                    // Match found, add the dataset to the list
                    referencedDatasets.append(data);
                    qDebug() << data->getName();
                }
            }
        }

        DataSet* D1 = referencedDatasets[0];
        DataSet* D2 = referencedDatasets[1];

        int dataSize = D1->Size(); //the size is assumed to be the same

        // Define the expression to be evaluated from the two datasets as text:
        std::string exps(expression.toStdString());
        qDebug() << expression;


        // Parsing: converting the text expression into a mathematical formula then evaluating it:
        ATMSP<double> ParserObj; // An object to convert the expression string to bytecode (parsing)
        ATMSB<double> ByteCodeObj; // An object to evaluate the code byte into numbers

        // Converting the text expression into a mathematical formula:
        ParserObj.parse(ByteCodeObj, exps, "D1,D2"); // Telling the parser what the formula is and what the datasets are
        qDebug() << "transformed into a mathematical formula";


        // Computing the result:
        QVector<double> Result(dataSize); // QVector to hold the results

        for (int i = 0; i < dataSize; ++i)
        {
            double* pointD1 = D1->getPoint(i);
            double* pointD2 = D2->getPoint(i);

            ByteCodeObj.var[0] = pointD1[1]; // Assuming the x-coordinate is the first element
            ByteCodeObj.var[1] = pointD2[1]; // Assuming the x-coordinate is the first element

            // Evaluating the mathematical formula for each point:
            Result[i] = ByteCodeObj.run();

        }

        // Handling errors if the parser encounters a problem (unknown symbol, for example, in the expression)
        size_t err;

        if ((err = ParserObj.parse(ByteCodeObj, exps, "D1,D2"))) {
            // You can expand this
            QString errorMessage = QString::fromStdString(ParserObj.errMessage(err));
            qCritical() << errorMessage;
        }

        QString resultString = "Result: ";
        for (double value : Result) {
            resultString += QString::number(value) + " ";
        }

        QString resultFileName = QFileDialog::getSaveFileName(this, "Save Result", QDir::currentPath(), "Text files (*.txt)");

        if (!resultFileName.isEmpty()) {
            QFile resultFile(resultFileName);

            if (resultFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&resultFile);

                for (int i = 0; i < dataSize; ++i) {
                    double* pointD1 = D1->getPoint(i); //the x value is assumed to be the same

                    out << pointD1[0] << "\t" << Result[i] << "\n";
                }

                resultFile.close();

                QMessageBox::information(this, "Save Successful", "Result has been saved to " + resultFileName);
            } else {
                QMessageBox::critical(this, "Error", "Unable to open file for writing.");
            }
        }

        //try to load that dataset
        AddedDataSet= new DataSet(resultFileName);
        if (AddedDataSet->IsDataSetValid) // Making sure the dataset is only dealt with if it was loaded succsessfully
        {
            AllDataSets.push_back(AddedDataSet); // Addding a pointer to the new dataset so that it can be accessed by the rest of the app
            // Creat a subWindow for the loaded DataSet:
            AddedDataSetWindow=new DataSetWindow(AddedDataSet,this);
            subWindow=ui->WindowsManager->addSubWindow(AddedDataSetWindow);
            AddedDataSetWindow->show();
        }
    }

}

void ParentWindow::on_actionDraw_Dataset_triggered(){
    // Create and show the data input dialog
    DataInputDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        // Retrieve the data points from the dialog
        //QList<QPointF> dataPoints = dialog.getDataPoints();
    }

}

void ParentWindow::editComment(QTableWidgetItem *item) {
    // Assuming column 1 is for comments
    if (item && item->column() == 1) {
        // Retrieve the current comment
        QString currentComment = item->text();
        // Prompt the user for a new comment
        bool ok;
        QString newComment = QInputDialog::getText(this, "Edit Comment", "Enter a new comment:", QLineEdit::Normal, currentComment, &ok);

        // If the user clicked OK, update the comment
        if (ok) {
            item->setText(newComment);
            int rowIndex = item->row();
            if (rowIndex >= 0 && rowIndex < AllDataSets.size()) {
                DataSet* selectedDataSet = AllDataSets[rowIndex];
                selectedDataSet->setComment(newComment);
            }
        }
    }
}

void ParentWindow::on_actionPlot_histogram_triggered(){
    HistogramClass *histogramGraph = new HistogramClass(AllDataSets, this);
    histogramGraph->show();
    qDebug() << "Plot histogram on";

}
