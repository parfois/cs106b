#include "demoutil.h"
#include "gwindow.h"
#include "gbufferedimage.h"
#include "filelib.h"
#include "strlib.h"
#include <math.h>
#include <assert.h>

int getColor(int v) {
    switch(v){
    case 0: return 0x00ffffff;
    case 1: return 0x000000ff;
    case 2: return 0x0000ffff;
    case 3: return 0x0000ff00;
    case 4: return 0x00ffff00;
    }
    return 0x00ff0000;
}

void colorSquare(GBufferedImage * img, int rBase, int cBase, int v) {
    int color = getColor(v);
    img->setRGB(cBase * 2 + 0, rBase * 2 + 0, color);
    img->setRGB(cBase * 2 + 1, rBase * 2 + 0, color);
    img->setRGB(cBase * 2 + 0, rBase * 2 + 1, color);
    img->setRGB(cBase * 2 + 1, rBase * 2 + 1, color);
}

void visualizeHashCounts(int * hashCounts, int size) {
    int baseSize = (sqrt(size) + 1);
    int gridSize = baseSize * 2;
    GWindow * gw = new GWindow(gridSize, gridSize);
    GBufferedImage* img = new GBufferedImage(0, 0, gridSize, gridSize);
    for(int i = 0; i < size; i++) {
        int rowBase = i / baseSize;
        int colBase = i % baseSize;
        colorSquare(img, rowBase, colBase, hashCounts[i]);
    }
    gw->add(img);
}

dataT loadData() {

    dataT data;

    string programRoot = "/Users/cpiech/Documents/Teaching/Interview/Code/HashMap/";

    string wikiBase = programRoot + "data/";
    for(char ch = 'a'; ch <= 'z'; ch++) {
        string dir = wikiBase + ch;
        cout << dir << endl;
        if(!fileExists(dir)) continue;
        Vector<string> list;
        listDirectory(dir, list);
        for(string fileName : list) {
            Vector<string> cols = stringSplit(fileName, ".");
            string articleName = cols[0];
            string type = cols[1];
            if(type == "htm" && !stringContains(articleName, "%")) {
                articleName = toLowerCase(articleName);
                articleName = stringReplace(articleName, "_", " ");
                string articleText = readEntireFile(dir + "/" + fileName);
                data.articles.put(articleName, articleText);
            }
        }
    }



    string txt = readEntireFile(programRoot + "toyData/words.txt");
    vector<string> lines = stringSplit(txt, "\n");
    for(int i = 0; i < lines.size(); i++) {
        vector<string> cols = stringSplit(lines[i], ",");
        data.words.put(cols[0], cols[1]);
    }

    return data;
}
