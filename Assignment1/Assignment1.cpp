// Assignment1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include "Time.h";
using namespace std;


//function to read a file
void openfile(string name) {
    string line;
    ifstream myfile(name);
    getline(myfile, line); //to take out the first line "Temperature and time" 
}


int main(){

    string line;
    double lasttime = 0; //highest time from the csv files(
    ifstream myfile("Experiment1.csv");
    getline(myfile, line); // to take out the first line because it is useless
    
    while (getline(myfile, line, ',')) { // reads the time  
        getline(myfile, line); //reads temperature
        double num = stod(line);
        string name = "time" + to_string(num); // the name of the class
        if (num > lasttime) {
            //create a new class and update temp
            
            

        }
        else {
            //find the class that is relevant to that temperature 
        }

        cout << line << endl;

    }


    
    
    
    //cout << line << endl;

    //cout << "Hello World!\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
