#include "TimeSeries.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
/* 
    CITATION:

    Generative AI (ChatGPT o1 Model as well as 4o) was used to debug my functions. Details will be provided where appropriate. ChatGPT was also used to seperate the program into the proper files.
*/

// --- Constructor ---
TimeSeries::TimeSeries() { //TimeSeries constructor with appropriate variables used for all computations
    size = 0;
    capacity = 2;
    array = new double[capacity];
    array_date = new int[capacity];
}

// --- Destructor ---
TimeSeries::~TimeSeries() { //Desctructor for both arrays created in the class
    delete[] array;
    delete[] array_date;
}

// --- Getters ---
int TimeSeries::get_size() { //Return the size of the array requested
    return size;
}

int TimeSeries::get_capacity() { //Return the capacity of the array requested
    return capacity;
}

// --- Resizing ---
void TimeSeries::grow() { //Used to grow the array and double the capacity when the array is full (size = capacity)
    double* temp_array = new double[capacity * 2]; //Temp arrays used to store the values of the original arrays which will then be moved become the new arrays
    int* temp_array_date = new int[capacity * 2];

    for(int i = 0; i < size; i++){ //Assign the values properly to each array
        temp_array[i] = array[i];
        temp_array_date[i] = array_date[i];
    }

    delete[] array; //Delete the original arrays 
    delete[] array_date;

    array = temp_array; //Set the temp arrays to the main arrays used in the class
    array_date = temp_array_date;
    capacity *= 2;
}

void TimeSeries::shrink() { //Same idea as the grow function but does it in reverse. Array capacity will be cut in half when the size is a forth of the capacity
    double* temp_array = new double[capacity / 2];
    int* temp_array_date = new int[capacity / 2];

    for(int i = 0; i < size; i++){
        temp_array[i] = array[i];
        temp_array_date[i] = array_date[i];
    }

    delete[] array;
    delete[] array_date;

    array = temp_array;
    array_date = temp_array_date;
    capacity /= 2;
}

// --- Insert a single value ---
void TimeSeries::insert(double value, int date) { //Function to insert values into the array when a CSV file is loaded
    if(size == capacity){ //Used to grow the array as data is inserted
        grow();
    }
    array[size] = value;
    array_date[size] = date;
    size++;
}

// --- Load from CSV file ---
void TimeSeries::load(const std::string &row) { //Load the CSV file
    if(size != 0){ //If there is prexisiting data then remove it
        delete[] array; //Delete arrays
        delete[] array_date;
        size = 0; //Reset size
        capacity = 2; //Reset capacity
        array = new double[capacity]; //Create new arrays for date and data
        array_date = new int[capacity];
    }

    std::stringstream datapoint(row); //String to store data to insert in array
    std::string rowdata;
    int year1 = 1960; //Year starts at 1960
    int yearcount = 0; 
    int id = 0;
/*
    CITATION:

   This function was partially debugged with ChatGPT using the prompt "How do I load data from a CSV file with multiple data types?"
*/

    while(std::getline(datapoint, rowdata, ',')){ //Load valuess into array
            if(id < 4){
                id++;
                continue;
            }
            try { //Use a try-catch to see if data is valid (ChatGPT suggested see previous citation)
                double value = std::stod(rowdata);
                if(value > 0){
                    insert(value, year1 + yearcount);
                }
                yearcount++;
            } catch (std::invalid_argument&) {}
            id++;
        }
    }


// --- Calculate mean ---
double TimeSeries::mean(TimeSeries &ArrayIn) { //Take in the whole object
    double sum = 0; //Store the value of the sum of all values in array
    if(size == 0){ //If the array is empty there is no valid mean
        return 0;
    }
    for(int i = 0; i < size; i++){ //For loop to add all values in array
        if(ArrayIn.array[i] < 0){
            ArrayIn.array[i] = 0;
        }
        sum += ArrayIn.array[i];
    }
    double mean = sum / ArrayIn.get_size(); //Calculate mean
    return mean;
}

// --- Print the array ---
void TimeSeries::print(TimeSeries &ArrayIn) { //Take in both arrays by taking in the whole object
    if(size == 0){ //If the arrays are empty there is nothing to print 
        std::cout << "failure at print" << std::endl;
        return;
    }
    for(int i = 0; i < ArrayIn.get_size(); i++){ //Iterate through both arrays and print the values out with the date first
        std::cout << "(" << ArrayIn.array_date[i]
                  << "," << ArrayIn.array[i] << ") " ;
    }
    std::cout << std::endl;
}

// --- Add a new (value, date) to the array ---
/*
    CITATION:

    This function was partially debugged by ChatGPT with he prompt "I have a memory allocation issue with this add function. Where is it?" 
*/
void TimeSeries::add(double value, int date) { 
    for(int i = 0; i < size; i++){ //If the date is already in the array then print failure
        if(date == array_date[i]){
            std::cout << "failure" << std::endl;
            return;
        }
    }
    if(size == 0){ //if the array is empty create a new set of data with one value in it
        array_date[0] = date;
        array[0] = value;
        size++;
        return;
    }
    // Grow if needed
    if(size >= capacity){ //Call grow function when the array fills
        grow();
    }
    size++;

    if(date < array_date[0]){ // Insert at the beginning
        int temp_date = array_date[0]; //Temp variables to store the values that will be loaded into the new index
        double temp_value = array[0]; 
        array_date[0] = date; //Insert the given values into the array
        array[0] = value;
        for(int i = 0; i < size - 1; i++){ //Iterate through the array and move the values. Please see previous citation about memory allocation issue
            int temp_date_store = array_date[i + 1]; //Store the next values in the array in a temp variable
            double temp_value_store = array[i + 1];
            array_date[i + 1] = temp_date; //Set the current stored value to the new index
            array[i + 1] = temp_value;
            temp_date = temp_date_store; //Move the perviously stored value into the temp variable that loads the array
            temp_value = temp_value_store;
        }
    }
    // Insert at the end
    else if(date > array_date[size - 2]){ //Insert at the end of the array. If the array is too small grow will be called
        array_date[size - 1] = date;
        array[size - 1] = value;
    }
    else {  // Insert in the middle
        int index; //Variable to store which index the new value should go 
        for(int i = 0; i < size - 1; i++){ //Iterate through the array to find where the new value should slot in
            if(date > array_date[i] && date < array_date[i + 1]){
                index = i + 1;
            }
        }
        int temp_date = array_date[index]; //This has the same logic as inserting at the beginning, only it starts at index rather than 0
        double temp_value = array[index];
        array_date[index] = date;
        array[index] = value;
        for(int i = index; i < size - 1; i++){ //Please see previous citation about memory allocation issue
            int temp_date_store = array_date[i + 1];
            double temp_value_store = array[i + 1];
            array_date[i + 1] = temp_date;
            array[i + 1] = temp_value;
            temp_date = temp_date_store;
            temp_value = temp_value_store;
        }
    }
    std::cout << "success" << std::endl;
}

// --- Update an existing date's value ---
void TimeSeries::update(double value, int date) { //Load desired value to change and new data point
    bool datefound = false; //Used a boolean to state if the date is in the array
    for(int i = 0; i < size; i++){ //Iterate throught the array to find the date that wants to be updated
        if(array_date[i] == date){ //Once the date is found in the date array, use the index of the found date and update the corisponding value in the data
            array[i] = value;
            datefound = true;
        }
    }
    if(!datefound){ //If the date is never found print failure
        std::cout << "failure" << std::endl;
    }
    else {
        std::cout << "success" << std::endl;
    }
}

// --- Fit a line (linear regression) ---
void TimeSeries::fit(TimeSeries &ArrayIn) { //Take in the object
    if(size == 0){ //Prints failure if there was no data to find the slope and intercept of
        std::cout << "failure" << std::endl;
        return;
    }
    double xsum = 0; //Variables to store values of sumations in the formula provided
    double ysum = 0;
    double xysum = 0;
    double xsqrsum = 0;

    for(int i = 0; i < size; i++){ //Go through the array to calculate the values used in formula
        xsum += ArrayIn.array_date[i];
        ysum += ArrayIn.array[i];
        xysum += (ArrayIn.array_date[i] * ArrayIn.array[i]);
        xsqrsum += (ArrayIn.array_date[i] * ArrayIn.array_date[i]);
    }
    double slope = ((size * xysum) - (xsum * ysum)) / 
                   ((size * xsqrsum) - (xsum * xsum)); //Calculate the slope
    double yint = (ysum - (slope * xsum)) / size; //Calculate the y-intercept

    std::cout << "slope is " << slope << " intercept is " << yint << std::endl; //Print the slope and intercept
}