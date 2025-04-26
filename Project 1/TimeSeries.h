#ifndef TIMESERIES_H
#define TIMESERIES_H

#include <string>

class TimeSeries {
public:
    // Constructor & Destructor
    TimeSeries();
    ~TimeSeries();

    // Getter methods
    int get_size();
    int get_capacity();

    // Array resizing
    void grow();
    void shrink();

    // Inserting/Loading data
    void insert(double value, int date);
    void load(std::string filename);

    // Calculations & Printing
    double mean(TimeSeries &ArrayIn);
    void print(TimeSeries &ArrayIn);
    int is_monotonic(TimeSeries &ArrayIn);

    // Adding/Updating data
    void add(double value, int date);
    void update(double value, int date);

    // Linear regression
    void fit(TimeSeries &ArrayIn);

    // We’ll keep these public, matching your original code
    double* array;     // Dynamically allocated array of values
    int* array_date;   // Dynamically allocated array of dates
    int size;          // Current number of elements
    int capacity;      // Capacity of the array

};

#endif // TIMESERIES_H