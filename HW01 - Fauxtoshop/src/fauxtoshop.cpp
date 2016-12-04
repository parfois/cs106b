// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own.
// TODO: rewrite this comment

#include <iostream>
#include "console.h"
#include "gwindow.h"
#include "grid.h"
#include "simpio.h"
#include "strlib.h"
#include "gbufferedimage.h"
#include "gevents.h"
#include "math.h" //for sqrt and exp in the optional Gaussian kernel
using namespace std;

static const int    WHITE = 0xFFFFFF;
static const int    BLACK = 0x000000;
static const int    GREEN = 0x00FF00;
static const double PI    = 3.14159265;

void     doFauxtoshop(GWindow &gw, GBufferedImage &img);

bool     openImageFromFilename(GBufferedImage& img, string filename);
bool 	saveImageToFilename(const GBufferedImage &img, string filename);
void     getMouseClickLocation(int &row, int &col);

//HELP FUNCTIONS by Yani
void scatter(GWindow &gw, GBufferedImage& img);
int pickNearbyColor(Grid<int>& originalPixel, const int i, const int j, const int scatterDegree);

void detectEdge(GWindow &gw, GBufferedImage& img);
double getColorDistance(int color1, int color2);
double getMaxDiff(Grid<int>& imgPixel, int i, int j);

void greenScreen(GWindow &gw, GBufferedImage &img);

void compareImages(GBufferedImage & img);


/* STARTER CODE FUNCTION - DO NOT EDIT
 *
 * This main simply declares a GWindow and a GBufferedImage for use
 * throughout the program. By asking you not to edit this function,
 * we are enforcing that the GWindow have a lifespan that spans the
 * entire duration of execution (trying to have more than one GWindow,
 * and/or GWindow(s) that go in and out of scope, can cause program
 * crashes).
 */
int main() {
    GWindow gw;
    gw.setTitle("Fauxtoshop");
    gw.setVisible(true);
    GBufferedImage img;
    doFauxtoshop(gw, img);
    return 0;
}

/* This is yours to edit. Depending on how you approach your problem
 * decomposition, you will want to rewrite some of these lines, move
 * them inside loops, or move them inside helper functions, etc.
 *
 * TODO: rewrite this comment.
 */

void doFauxtoshop(GWindow &gw, GBufferedImage &img) {
    //prints a welcome message
    cout << "Welcome to Fauxtoshop!" << endl;
    //user prompt the image
    string imageName = getLine("Enter name of image file to open (or blank to quit): ");
    openImageFromFilename(img, imageName);
    gw.setCanvasSize(img.getWidth(), img.getHeight());
    gw.add(&img,0,0);
    //user prompt the filter option to apply to the image
    cout << "Opening image file, may take a minute..." << endl;
    int filterOption = getInteger("Which image filter would you like to apply?\n          1 - Scatter\n          2 - Edge detection\n          3 - \"Green screen\" with another image\n          4 - Compare image with another image\n          Your choice: ");

    switch(filterOption) {
        case 1:
        scatter(gw, img);
        break;

        case 2:
        detectEdge(gw, img);
        break;

        case 3:
        greenScreen(gw, img);
        break;

        case 4:
        compareImages(img);
        break;

    }
}

void askAndSave(const GBufferedImage& img) {
    string filename = getLine("Enter filename to save image (or blank to skip saving): ");
    if (filename.size() != 0) {
        saveImageToFilename(img, filename);
    }
}

int pickNearbyColor(Grid<int>& originalPixel, const int i, const int j, const int scatterDegree) {

    int randR = rand() % (scatterDegree * 2) - scatterDegree;
    int randC = rand() % (scatterDegree * 2) - scatterDegree;

    int pixelR = i + randR;
    int pixelC = j + randC;

    while (pixelR < 0 || pixelR >= originalPixel.numRows()) {
        randR = rand() % (scatterDegree * 2 + 1) - scatterDegree;
        pixelR = i + randR;

    }

    while (pixelC < 0 || pixelC >= originalPixel.numCols()) {
        randC = rand() % (scatterDegree * 2 + 1) - scatterDegree;
        pixelC = j + randC;
    }
    return originalPixel[pixelR][pixelC];
}

void scatter(GWindow& gw, GBufferedImage& img) {
    srand (time(NULL));

    int scatterDegree = 0;
    while (scatterDegree <= 0 || scatterDegree > 100) {
        scatterDegree = getInteger("Enter a degree of scatter [1 - 100]: ");
    }
    Grid<int> originalPixel = img.toGrid();
    Grid<int> scatterPixel(originalPixel.numRows(), originalPixel.numCols());

    for (int i = 0; i < scatterPixel.numRows(); i++) {
        for (int j = 0; j < scatterPixel.numCols(); j++) {
            scatterPixel[i][j] = pickNearbyColor(originalPixel, i, j, scatterDegree);
        }
    }

    GBufferedImage scatterImg;
    scatterImg.fromGrid(scatterPixel);
    gw.clear();
    gw.add(&scatterImg, 0, 0);

    askAndSave(scatterImg);
}

double getColorDistance(const int color1, const int color2) {
    int r1, g1, b1, r2, g2, b2;
    GBufferedImage::getRedGreenBlue(color1,	r1, g1, b1);
    GBufferedImage::getRedGreenBlue(color2,	r2, g2, b2);
    int diffRed = r2 - r1;
    int diffBlue = b2 - b1;
    int diffGreen = g2 - g1;
    return sqrt(diffRed * diffRed + diffBlue * diffBlue + diffGreen * diffGreen);
}

double getMaxDiff(Grid<int>& imgPixel, const int i, const int j) {
    double maxDiff = 0.0;
    double diff [8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    if (imgPixel.inBounds(i - 1, j - 1)) {
        diff[0] = getColorDistance(imgPixel[i - 1][j - 1], imgPixel[i][j]);
    }
    if (imgPixel.inBounds(i - 1, j)) {
        diff[1] = getColorDistance(imgPixel[i - 1][j], imgPixel[i][j]);
    }
    if (imgPixel.inBounds(i - 1, j + 1)) {
        diff[2] = getColorDistance(imgPixel[i - 1][j + 1], imgPixel[i][j]);
    }
    if (imgPixel.inBounds(i, j - 1)) {
        diff[3] = getColorDistance(imgPixel[i][j - 1], imgPixel[i][j]);
    }
    if (imgPixel.inBounds(i, j + 1)) {
        diff[4] = getColorDistance(imgPixel[i][j + 1], imgPixel[i][j]);
    }
    if (imgPixel.inBounds(i + 1, j - 1)) {
        diff[5] = getColorDistance(imgPixel[i + 1][j - 1], imgPixel[i][j]);
    }
    if (imgPixel.inBounds(i + 1, j)) {
        diff[6] = getColorDistance(imgPixel[i + 1][j], imgPixel[i][j]);
    }
    if (imgPixel.inBounds(i + 1, j + 1)) {
        diff[7] = getColorDistance(imgPixel[i + 1][j + 1], imgPixel[i][j]);
    }
    for (double d : diff) { //(auto& d : diff) if don't know type of diff or type is large
        if (d > maxDiff) {
            maxDiff = d;
        }
    }
    return maxDiff;
}

void detectEdge(GWindow& gw, GBufferedImage& img) {
    int threshold = 0;
    while (threshold <= 0) {
        threshold = getInteger("Enter threshold for edge detection: ");
    }

    Grid<int> imgPixel = img.toGrid();
    Grid<int> edgePixel(imgPixel.numRows(), imgPixel.numCols());

    for (int i = 0; i < imgPixel.numRows(); i++) {
        for (int j = 0; j < imgPixel.numCols(); j++) {
            double maxDiff = getMaxDiff(imgPixel, i, j);
            if (threshold < maxDiff) {
                edgePixel[i][j] = BLACK;
            }
            else {
                edgePixel[i][j] = WHITE;
            }
        }
    }

    GBufferedImage edgeImg;
    edgeImg.fromGrid(edgePixel);
    gw.clear();
    gw.add(&edgeImg,0,0);

    askAndSave(edgeImg);
}

void greenScreen(GWindow& gw, GBufferedImage& img) {
    cout << "Now choose another file to add to your background image.\n";
    string stickerImgName = getLine("Enter name of image file to open: ");
    cout << "Opening image file, may take a minute...\n";
    GBufferedImage stickerImg;
    openImageFromFilename(stickerImg, stickerImgName);
    Grid<int> stickerGrid = stickerImg.toGrid();
    Grid<int> backGrid = img.toGrid();

    int tolerance = getInteger("Now choose a tolerance threshold: ");

    string locationStr = getLine("Enter location to place image as \"(row,col)\" (or blank to use mouse): ");
    int row;
    int col;

    if (locationStr.empty()) {
        getMouseClickLocation(row, col);
    }
    else {
        int pos = locationStr.rfind(",");
        string locationRow = locationStr.substr(1, pos - 1);
        string locationCol = locationStr.substr(pos + 1, locationStr.size() - 2 - pos);
        row = stringToInteger(locationRow, 10);
        col = stringToInteger(locationCol, 10);
    }
    int i = 0;
    while (i + row < backGrid.numRows() && i < stickerGrid.numRows()) {
        int j = 0;
        while (j + col < backGrid.numCols() && j < stickerGrid.numCols()) {
            if (tolerance < getColorDistance(GREEN, stickerGrid[i][j])) {
                    backGrid[i + row][j + col] = stickerGrid[i][j];
            }
            j++;
        }
        i++;
    }

    GBufferedImage greenImg;
    greenImg.fromGrid(backGrid);
    gw.clear();
    gw.add(&greenImg, 0, 0);

    askAndSave(greenImg);

}

void compareImages(GBufferedImage& img) {
    cout << "Now choose another image file to compare to.\n";
    string img2Str = getLine("Enter name of image file to open: ");
    cout << "Opening image file, may take a minute...\n";
    GBufferedImage img2;
    openImageFromFilename(img2, img2Str);

    int diffCount = img.countDiffPixels(img2);

    if (diffCount == 0) {
        cout << "These images are the same!\n";
    }
    else {
        cout << "These images differ in " << diffCount << " pixel locations!\n";
    }
    askAndSave(img2);
}


/* STARTER CODE HELPER FUNCTION - DO NOT EDIT
 *
 * Attempts to open the image file 'filename'.
 *
 * This function returns true when the image file was successfully
 * opened and the 'img' object now contains that image, otherwise it
 * returns false.
 */
bool openImageFromFilename(GBufferedImage& img, string filename) {
    try {
        img.load(filename);
    }
    catch (...) {
        return false;
    }
    return true;
}

/* STARTER CODE HELPER FUNCTION - DO NOT EDIT
 *
 * Attempts to save the image file to 'filename'.
 *
 * This function returns true when the image was successfully saved
 * to the file specified, otherwise it returns false.
 */
bool saveImageToFilename(const GBufferedImage &img, string filename) {
    try {
        img.save(filename);
    }
    catch (...) {
        return false;
    }
    return true;
}

/* STARTER CODE HELPER FUNCTION - DO NOT EDIT
 *
 * Waits for a mouse click in the GWindow and reports click location.
 *
 * When this function returns, row and col are set to the row and
 * column where a mouse click was detected.
 */
void getMouseClickLocation(int &row, int &col) {
    GMouseEvent me;
    do {
        me = getNextEvent(MOUSE_EVENT);
    } while (me.getEventType() != MOUSE_CLICKED);
    row = me.getY();
    col = me.getX();
}

/* OPTIONAL HELPER FUNCTION
 *
 * This is only here in in case you decide to impelment a Gaussian
 * blur as an OPTIONAL extension (see the suggested extensions part
 * of the spec handout).
 *
 * Takes a radius and computes a 1-dimensional Gaussian blur kernel
 * with that radius. The 1-dimensional kernel can be applied to a
 * 2-dimensional image in two separate passes: first pass goes over
 * each row and does the horizontal convolutions, second pass goes
 * over each column and does the vertical convolutions. This is more
 * efficient than creating a 2-dimensional kernel and applying it in
 * one convolution pass.
 *
 * This code is based on the C# code posted by Stack Overflow user
 * "Cecil has a name" at this link:
 * http://stackoverflow.com/questions/1696113/how-do-i-gaussian-blur-an-image-without-using-any-in-built-gaussian-functions
 *
 */
Vector<double> gaussKernelForRadius(int radius) {
    if (radius < 1) {
        Vector<double> empty;
        return empty;
    }
    Vector<double> kernel(radius * 2 + 1);
    double magic1 = 1.0 / (2.0 * radius * radius);
    double magic2 = 1.0 / (sqrt(2.0 * PI) * radius);
    int r = -radius;
    double div = 0.0;
    for (int i = 0; i < kernel.size(); i++) {
        double x = r * r;
        kernel[i] = magic2 * exp(-x * magic1);
        r++;
        div += kernel[i];
    }
    for (int i = 0; i < kernel.size(); i++) {
        kernel[i] /= div;
    }
    return kernel;
}
