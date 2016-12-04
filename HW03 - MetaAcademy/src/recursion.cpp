/* 
 * TODO: put your own comments here. Also you should leave comments on
 * each of your methods.
 */

#include <math.h>
#include "recursion.h"
#include "map.h"
#include "vector.h"
#include "set.h"
#include "gwindow.h"
#include "gobjects.h"
#include "tokenscanner.h"

using namespace std;

int gcd(int a, int b) {
    if (b == 0) {
        cout << "gcd(" << a << ", 0) = " << a << endl;
        return a;
    }
    else {
        cout << "gcd(" << a << ", " << b << ") = gcd(" << b << ", " << a % b << ")" << endl;
        return gcd(b, a % b);
    }
    return 0;
}

void serpinskii(GWindow &gw, int leftX, int leftY, int size, int order) {
    if (order == 1) {
        gw.drawLine(leftX, leftY, leftX + size, leftY);
        double h = sqrt(3) * size / 2;
        gw.drawLine(leftX, leftY, leftX + size / 2, leftY + h);
        gw.drawLine(leftX + size / 2, leftY + h, leftX + size, leftY);
    }
    else {
        serpinskii(gw, leftX, leftY, size / 2, order - 1);
        serpinskii(gw, leftX + size / 2, leftY, size / 2, order - 1);
        serpinskii(gw, leftX + size / 4, leftY + (sqrt(3) * size / 4), size / 2, order - 1);
    }

    cout << "[recursion serpinskii called]" << endl;
}

void floodFillHelper(int& count, GBufferedImage& image, int x, int y, int newColor) {

    int original_color = image.getRGB(x, y);
    image.setRGB(x, y, newColor);
    count++;

    if (image.inBounds(x - 1, y) && image.getRGB(x - 1, y) == original_color) {
        floodFillHelper(count, image, x - 1, y, newColor);
    }
    if (image.inBounds(x + 1, y) && image.getRGB(x + 1, y) == original_color) {
        floodFillHelper(count, image, x + 1, y, newColor);
    }
    if (image.inBounds(x, y + 1) && image.getRGB(x, y + 1) == original_color) {
        floodFillHelper(count, image, x, y + 1, newColor);
    }
    if (image.inBounds(x, y - 1) && image.getRGB(x, y - 1) == original_color) {
        floodFillHelper(count, image, x, y - 1, newColor);
    }

}

int floodFill(GBufferedImage& image, int x, int y, int newColor) {
    if (newColor == image.getRGB(x, y)) {
        return 0;
    }

    int count = 0;

    floodFillHelper(count, image, x, y, newColor);

    return count;
}

void personalCurriculumHelper(Set<string>& course, Map<string, Vector<string>> & prereqMap,string goal) {
    if (prereqMap.containsKey(goal)) {
        Vector<string> prereq = prereqMap[goal];
        for (string& newGoal : prereq) {
            if (! course.contains(newGoal)) {
                course.add(newGoal);
                personalCurriculumHelper(course, prereqMap, newGoal);
                cout << newGoal << endl;
            }
        }
    }
}

void personalCurriculum(Map<string, Vector<string>> & prereqMap,string goal) {
    Set<string> course;
    personalCurriculumHelper(course, prereqMap, goal);
    cout << goal << endl;
}

void generateHelper(string& sentence, const Map<string, Vector<string>>& grammar, const string& symbol) {
    if (!grammar.containsKey(symbol)) {
        sentence += symbol;
    } else {
        Vector<string> grammarRules = grammar.get(symbol);
        int random = rand() % grammarRules.size();
        TokenScanner scanner = grammarRules[random];
        Vector<string> symbolVector;
        while (scanner.hasMoreTokens()) {
           string token = scanner.nextToken();
           symbolVector.add(token);
        }

        for (const string& sym : symbolVector) {
            generateHelper(sentence, grammar, sym);
        }
    }
}

string generate(Map<string, Vector<string> > & grammar, string symbol) {
    string sentence = "";
    generateHelper(sentence, grammar, symbol);
    cout << "[recursion generate called]" << endl;
    return sentence;
}
