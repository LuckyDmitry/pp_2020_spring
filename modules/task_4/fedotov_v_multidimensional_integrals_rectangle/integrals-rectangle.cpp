// Copyright 2020 Fedotov Vladislav
#include "../../modules/task_4/fedotov_v_multidimensional_integrals_rectangle/integrals-rectangle.h"
#include <iostream>
#include <cmath>

// sequential version
double getMultipleIntegralUsingRectangleMethod(
    double (*function)(double, double), double x1, double x2,
    double y1, double y2, double stepX, double stepY) {
    double resultIntegral = 0;
    double cellSquare = stepX * stepY;
    int xStepsNumber = (x2 - x1) / stepX;
    int yStepsNumber = (y2 - y1) / stepY;

    for (int i = 0; i < xStepsNumber; i++) {
        for (int j = 0; j < yStepsNumber; j++) {
            double xMiddle = (x1 + i*stepX + x1 + i*stepX + stepX) / 2;
            double yMiddle = (y1 + j*stepY + y1 + j*stepY + stepY) / 2;

            resultIntegral += function(xMiddle, yMiddle);
        }
    }

    return resultIntegral * cellSquare;  // common factor
}

// std::thread version
double getMultipleIntegralUsingRectangleMethodSTD(
    double (*function)(double, double), double x1, double x2,
    double y1, double y2, double stepX, double stepY) {
    double resultIntegral = 0;
    double cellSquare = stepX * stepY;
    int xStepsNumber = (x2 - x1) / stepX;
    int yStepsNumber = (y2 - y1) / stepY;

    int numberOfThreads = std::thread::hardware_concurrency();
    std::vector<std::thread> threadsVector;

    int stepsPerThreadNumber = xStepsNumber / numberOfThreads;
    
    for (int i = 0; i < numberOfThreads; i++) {
        threadsVector.push_back(std::thread(increaseResultIntegral, &resultIntegral, i*stepsPerThreadNumber, stepsPerThreadNumber, yStepsNumber, function, x1, x2, y1, y2, stepX, stepY));
    }

    std::for_each(threadsVector.begin(), threadsVector.end(), [](std::thread &thread) {
        thread.join();
    });

    return resultIntegral * cellSquare;  // common factor
}

void increaseResultIntegral(double* resultIntegral, int startStep,
    int numberOfSteps, int yStepsNumber, double (*function)(double, double),
    double x1, double x2, double y1, double y2, double stepX, double stepY) {
        for (int i = startStep; i < startStep + numberOfSteps; i++) {
            for (int j = 0; j < yStepsNumber; j++) {
                double xMiddle = (x1 + i*stepX + x1 + i*stepX + stepX) / 2;
                double yMiddle = (y1 + j*stepY + y1 + j*stepY + stepY) / 2;

                *resultIntegral += function(xMiddle, yMiddle);
            }
        }
}

double function1(double x, double y) {
    return x*x + y*y;
}

double function2(double x, double y) {
    return sin(x*y);
}

double function3(double x, double y) {
    return cos(x*y);
}

double function4(double x, double y) {
    return sin(x*y) - cos(x*y);
}

double function5(double x, double y) {
    return 3*x*x - y;
}

double function6(double x, double y) {
    return x*y;
}
