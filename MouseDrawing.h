#pragma once

#include <Windows.h>
#include <vector>

using namespace std;

void CenterWindow(HWND hWnd);

class Point {
public:
    int x;
    int y;

    Point(int x = 0, int y = 0) : x(x), y(y) {}
};