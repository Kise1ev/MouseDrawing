#include "MouseDrawing.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, LPSTR lpCmdLine, int nCmdShow) {
    HWND hWnd;
    MSG msg;
    WNDCLASSEX wc{};
    TCHAR szClassName[] = TEXT("MouseDrawing");

    wc.cbSize = sizeof(wc);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbWndExtra = 0;
    wc.cbClsExtra = 0;
    wc.hInstance = hInst;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = szClassName;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, TEXT("Register failure"), TEXT("Error"), MB_OK);
        return 0;
    }

    hWnd = CreateWindow(szClassName, TEXT("MouseDrawing"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInst, NULL);
    if (!hWnd) {
        MessageBox(NULL, TEXT("Create windows failure"), TEXT("Error"), MB_OK);
        return 0;
    }

    CenterWindow(hWnd);
    ShowWindow(hWnd, nCmdShow);

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HDC hDC;
    static int x = 0, y = 0;
    static BOOL flag = false;
    static vector<vector<Point>> lines;
    vector<Point> line;
    HDC hDC1;
    PAINTSTRUCT psPaint;

    switch (uMsg) {
    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        ReleaseDC(hWnd, hDC);
        break;
    case WM_CREATE:
        hDC = GetDC(hWnd);
        SetDCPenColor(hDC, RGB(rand() % 256, rand() % 256, rand() % 256));
        break;
    case WM_LBUTTONDOWN:
        flag = true;
        x = LOWORD(lParam);
        y = HIWORD(lParam);
        MoveToEx(hDC, x, y, NULL);
        line.push_back(Point(x, y));
        break;
    case WM_LBUTTONUP:
        if (flag) {
            flag = false;
            lines.push_back(move(line));
        }
        break;
    case WM_MOUSEMOVE:
        if (flag) {
            x = LOWORD(lParam);
            y = HIWORD(lParam);
            LineTo(hDC, x, y);
            line.push_back(Point(x, y));
        }
        break;
    case WM_PAINT:
        hDC1 = BeginPaint(hWnd, &psPaint);
        for (const auto& line : lines) {
            SetDCPenColor(hDC1, GetDCPenColor(hDC));
            if (!line.empty()) {
                MoveToEx(hDC1, line[0].x, line[0].y, NULL);
                for (size_t i = 1; i < line.size(); i++) {
                    LineTo(hDC1, line[i].x, line[i].y);
                }
            }
        }
        EndPaint(hWnd, &psPaint);
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}

void CenterWindow(HWND hWnd) {
    RECT rect;
    GetWindowRect(hWnd, &rect);

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    int windowWidth = rect.right - rect.left;
    int windowHeight = rect.bottom - rect.top;

    int x = (screenWidth - windowWidth) / 2;
    int y = (screenHeight - windowHeight) / 2;
    SetWindowPos(hWnd, NULL, x, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}