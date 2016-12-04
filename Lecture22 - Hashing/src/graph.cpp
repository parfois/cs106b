#include "graph.h"
#include <iostream>
#include <math.h>
#include "gwindow.h"
#include "gobjects.h"
using namespace std;

const int WIDTH = 500;
const int HEIGHT = 500;
const int GRAPH_OFFSET = 30;
const int X_AXIS_OFFSET = 50;
const int Y_AXIS_OFFSET = X_AXIS_OFFSET;
const int X_AXIS_Y = HEIGHT - X_AXIS_OFFSET + GRAPH_OFFSET;
const int NUM_TICKS = 5;

Graph::Graph(string title, double maxX, double maxY) {
    this->maxX = maxX;
    this->maxY = maxY;
    this->hasLastPoint = false;
    gw = new GWindow(WIDTH + GRAPH_OFFSET, HEIGHT + GRAPH_OFFSET);

    GLine xAxis(Y_AXIS_OFFSET, X_AXIS_Y, gw->getWidth(), X_AXIS_Y);
    gw->add(&xAxis);

    GLine yAxis(Y_AXIS_OFFSET, GRAPH_OFFSET, Y_AXIS_OFFSET, X_AXIS_Y);
    gw->add(&yAxis);

    GLabel yLabel("Time (micro s)");
    yLabel.setFont("times-bold-16");
    gw->add(&yLabel,10, yLabel.getFontAscent() + 5);

    GLabel xLabel("Num Elements");
    xLabel.setFont("times-bold-16");
    gw->add(&xLabel, (WIDTH - xLabel.getWidth() + Y_AXIS_OFFSET) / 2, X_AXIS_Y + 35);

    for(int i = 0; i <= NUM_TICKS; i++) {
        double delta = (WIDTH - Y_AXIS_OFFSET) / NUM_TICKS;
        int x = Y_AXIS_OFFSET + i * delta;
        int value = i * maxX / NUM_TICKS;
        GLine tick(x, X_AXIS_Y, x, X_AXIS_Y + 5);
        GLabel tickLabel(integerToString(value));
        tickLabel.setFont("times-*-16");
        gw->add(&tickLabel, x - tickLabel.getWidth()/2, X_AXIS_Y + 5 + tickLabel.getFontAscent());
        gw->add(&tick);
    }

    for(int i = 0; i <= NUM_TICKS; i++) {
        double delta = (X_AXIS_Y - GRAPH_OFFSET) / NUM_TICKS;
        int y = X_AXIS_Y - i * delta;
        double value = i * maxY / NUM_TICKS;
        string label = doubleToString(value);
        if(stringIsInteger(label)) label = label + ".0";
        GLabel tickLabel(label);
        tickLabel.setFont("times-*-16");
        GLine tick(Y_AXIS_OFFSET - 5, y, Y_AXIS_OFFSET, y);
        gw->add(&tickLabel, 15, y + tickLabel.getFontAscent() / 2);
        gw->add(&tick);
    }

    if(title != "") {
        this->title = new GLabel(title);
        this->title->setFont("times-bold-24");
        gw->add(this->title, (gw->getWidth() - this->title->getWidth())/2, this->title->getFontAscent() + 5);

    }
}

Graph::~Graph() {
    delete gw;
}

void Graph::addPoint(double x, double y) {
    double xWidth = WIDTH - Y_AXIS_OFFSET;
    double yHeight = HEIGHT - X_AXIS_OFFSET;

    double xPx = Y_AXIS_OFFSET + x / maxX * xWidth;
    double yPy = GRAPH_OFFSET + (1- y / maxY) * yHeight;
    if(xPx > 5 && yPy > 5) {
        GOval point(10, 10);
        point.setFilled(true);
        point.setFillColor(255);
        gw->add(&point, xPx - 5, yPy - 5);
    }
    if(hasLastPoint) {
        GLine line(lastX, lastY, xPx, yPy);
        gw->add(&line);
    }
    hasLastPoint = true;
    lastX = xPx;
    lastY = yPy;
}

