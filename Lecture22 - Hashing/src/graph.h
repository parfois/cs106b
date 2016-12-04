#ifndef GRAPH
#define GRAPH
#include <iostream>
#include "gwindow.h"
using namespace std;

class Graph {
public:
    Graph(string title, double maxX, double maxY);
    ~Graph();
    void addPoint(double x, double y);
private:
    double maxX;
    double maxY;
    GLabel * title;
    GWindow * gw;

    bool hasLastPoint;
    double lastX;
    double lastY;
};

#endif // GRAPH
