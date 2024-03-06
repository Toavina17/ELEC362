#include "datainputdialog.h"
#include "qboxlayout.h"
#include "qheaderview.h"

DataInputDialog::DataInputDialog(QWidget *parent) :
    QDialog(parent),
    tableWidget(new QTableWidget(this)),
    addButton(new QPushButton("Add Data Point", this)),
    removeButton(new QPushButton("Remove Selected", this)),
    doneButton(new QPushButton("Done", this))
{
    // Set up the table widget
    tableWidget->setColumnCount(2);
    tableWidget->setHorizontalHeaderLabels({"X", "Y"});
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Set up the layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(tableWidget);
    layout->addWidget(addButton);
    layout->addWidget(removeButton);
    layout->addWidget(doneButton);

    // Connect signals and slots
    connect(addButton, &QPushButton::clicked, this, &DataInputDialog::addDataPoint);
    connect(removeButton, &QPushButton::clicked, this, &DataInputDialog::removeDataPoint);
    connect(doneButton, &QPushButton::clicked, this, &DataInputDialog::doneButtonClicked);
}

DataInputDialog::~DataInputDialog()
{

}

QList<QPointF> DataInputDialog::getDataPoints()
{

    QList<QPointF> dataPoints;

    // Retrieve data from the table widget
    int rowCount = tableWidget->rowCount();
    for (int i = 0; i < rowCount; ++i) {
        QString xText = tableWidget->item(i, 0)->text();
        QString yText = tableWidget->item(i, 1)->text();

        bool okX, okY;
        double x = xText.toDouble(&okX);
        double y = yText.toDouble(&okY);

        if (!okX || !okY) {
            // Show an error message to the user
            QMessageBox::critical(this, "Input Error", "Invalid input for row " + QString::number(i + 1) +
                                                           ". Please enter valid numerical values.");
            return dataPoints;  // Stop execution of the function
        }

        dataPoints.append(QPointF(x, y));
    }
    return dataPoints;

}

void DataInputDialog::addDataPoint()
{
    // Add a new row to the table
    int rowCount = tableWidget->rowCount();
    tableWidget->insertRow(rowCount);
}

void DataInputDialog::removeDataPoint()
{
    // Remove the selected row from the table
    int currentRow = tableWidget->currentRow();
    if (currentRow >= 0) {
        tableWidget->removeRow(currentRow);
    }
}

void DataInputDialog::doneButtonClicked()
{
    // Close the dialog when the "Done" button is clicked

    // Retrieve the data points
    QList<QPointF> dataPoints = getDataPoints();


    // Get the file path from the user
    QString filePath = QFileDialog::getSaveFileName(this, "Save Data", QDir::currentPath(), "Text Files (*.txt)");

    if (!filePath.isEmpty()) {
        // Open the file for writing
        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);

            // Write each data point to the file
            for (const QPointF& point : dataPoints) {
                stream << point.x() << "\t" << point.y() << "\n";
            }

            // Close the file
            file.close();
        } else {
            // Handle file opening error
        }
    }

    accept();

}
