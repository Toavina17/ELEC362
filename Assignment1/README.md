# Assignment 1: Scientific data analysis programme

Design and implement a console-based C++ programme that reads data from multiple files
in the same folder, processes and writes the processed data into a new file. 

The input files will be named “Experiment” followed by a number. The following figure
shows a folder containing 5 files. The number of files is not known in advance. The files are
numbered consecutively (if file 6 does not exist then files 7 and above do not exist).

All files have the same format. All start from time = 0, and the time difference between any
two points in any file is 0.1 s. The files differ from one another by the temperature at a given
time, and by the number of data points (i.e. the last data point in one file can be 0.7 s while
in another it can be 1.5 seconds).
The programme must calculate the average temperature from all the data read from the
files at a given time. The output file should have three columns, namely the time, the
averaged temperature, and number of data points used in that average. For example, if 3
out of 5 files have data points at 1.5 s, the third column for t = 1.5 s should have the number
3, while for t = 0 s the third column should have 5.
