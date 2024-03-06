#include "dataset.h"


// Initialising the static variable
int DataSet::DataSetCounter=0;


DataSet::DataSet(QString& FileName)
{

   // Reading the data
    QFile file(FileName);
    if (file.open(QIODevice::ReadOnly))
    {



              QTextStream in(&file);

              // Step 1: Count the number of rows : Because GSL reuires knowledge of matrix size in advance
              while (!in.atEnd())
              {
                  in.readLine();
                  NumberOfRows++;
              }


              // Step 2: Allocate a GSL matrix to save the data for further processing
              Matrix=gsl_matrix_alloc(NumberOfRows,2);


              // Step 3: Storing te data from the file into the matrix
              in.seek(0);
              for (int i=0;i<NumberOfRows;i++)
              {

                  QString x, y;
                  in >> x >> y; // This line assumes that the x and the y of the data are seperated by a tab or a comma

                  double x_double,y_double;
                  // This conversion makes sure the flag for te validy of the dataet is updated if
                  // the app couldn't load it because it is has a non-number for example
                  x_double=x.toDouble(&IsDataSetValid);
                  y_double=y.toDouble(&IsDataSetValid);


                  if (IsDataSetValid)
                  {
                      gsl_matrix_set(Matrix,i,0, x_double); // First column is x coordinate
                      gsl_matrix_set(Matrix,i,1, y_double); //Second column is the y coordinate


                  }

                  else // When the function fails to read:
                  {
                      // Displaye error message
                      QMessageBox ErrormsgBox;
                      ErrormsgBox.setWindowTitle("Error");
                      ErrormsgBox.setWindowIcon(QIcon(":/icons/errorSymbol.svg"));
                      ErrormsgBox.setText("Error");
                      ErrormsgBox.setInformativeText("The app encountered a non-numeric character in the dataset.");
                      ErrormsgBox.setIcon(QMessageBox::Critical);
                      ErrormsgBox.exec();

                      // Free allocated memeory because reading the file failed
                      gsl_matrix_free(Matrix);


                      // Stop reading the rest
                      break;



                  }

              }
    }


    if (IsDataSetValid) // Only increment the number of datsets if loading is succesfull
    {
    // Giving a deafult name to the dataset (D1 for first dataset, D2 for second dataset, etc):
     DataSetCounter++;
     DataSetName="D"+QString::number(DataSetCounter);
     DataSetComment = "";
    }

}

int DataSet::Size()
{ // Function to return the size of the dataset (number of rows)
    return NumberOfRows;
}

QString DataSet::getName()
{// Function to return the name of the dataset (number of rows)
    return DataSetName;
}

double *DataSet::getPoint(int i)
{ // Function to return the ith datapoint
    DataPoint[0]= gsl_matrix_get(Matrix,i,0); // x-coordinate
    DataPoint[1]= gsl_matrix_get(Matrix,i,1); // y-coordinate

    return DataPoint;

}

QString DataSet::getComment() {
    return DataSetComment;
}

void DataSet::setComment(const QString &comment) {
    DataSetComment = comment;
}

