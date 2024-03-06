#ifndef DATASET_H
#define DATASET_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include "gsl/gsl_matrix.h"

/********************************
 *
 *  This class is defined to handle the datasets,
 *  an object of this class represents the set of points making a dataset
 *
 *  The dataset gets stroed into a GSL Matrix, and object from GSL library
 *  which allows it to be compatable with so many functions avialable as part
 *  of GSL library
 *
 *
**********************************/

class DataSet
{

private:
    int NumberOfRows=0; // Assuming that a datset only has two columns
    double DataPoint[2]; // An array containing the information of 2 datapoints
    gsl_matrix *Matrix; // GSL MAtrix object to store the data
    static int DataSetCounter; // Number of Datasets in the app at any moment ( defined as static because it is shared among all objects of this class)
    QString DataSetName; // Name of the Dataset
    QString DataSetComment; //Comment about the dataset

public:
    DataSet(QString& FileName);
    int Size(); // function to get the size of the dataset (currenlty the number of rows only)
    QString getName(); // Function to get the name of the dataset
    double* getPoint(int i); // Function to return the ith datapoint

    QString getComment(); // New function to retrieve the dataset comment
    void setComment(const QString &comment); // New function to set the dataset comment

    bool IsDataSetValid=true; // Used to detect and handle error subsquently
};

#endif // DATASET_H
