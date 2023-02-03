#include <windows.h>
#include <tchar.h>
#include <stdbool.h>

// Window dimensions
#define WIDTH 1800
#define HEIGHT 1000

// Grid square size
#define GRID_SIZE 20

// Number of squares in the grid
#define GRID_WIDTH (WIDTH / GRID_SIZE)
#define GRID_HEIGHT (HEIGHT / GRID_SIZE)

// Boolean array to keep track of each square
bool grid1[GRID_WIDTH][GRID_HEIGHT];
bool grid2[GRID_WIDTH][GRID_HEIGHT];
bool started = false;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    bool GetGrid(int x, int y)
    {
        if(x >= GRID_WIDTH)
            return false;
        if(x < 0)
            return false;
        if(y >= GRID_HEIGHT)
            return false;
        if(y < 0)
            return false;
        return grid1[x][y];
    }
    bool GetNotGrid(int x, int y){
        if(x >= GRID_WIDTH)
            return false;
        if(x < 0)
            return false;
        if(y >= GRID_HEIGHT)
            return false;
        if(y < 0)
            return false;
        return grid2[x][y];
    }
    void SetGrid(int x, int y, bool value){
        grid1[x][y] = value;
        grid2[x][y] = value;
    }
    void SetUseGrid(int x, int y, bool value){
        grid1[x][y] = value;
    }
    void SetNotGrid(int x, int y, bool value){
        grid2[x][y] = value;
    }
    bool comparegrids(){
        for(int x = 0; x < GRID_WIDTH; ++x){
            for(int y = 0; y < GRID_HEIGHT; ++y){
                if(grid1[x][y] != grid2[x][y])
                    return false;
            }
        }
        return true;
    }
    void FlipGrids(){
        for(int x = 0; x < GRID_WIDTH; ++x){
            for(int y = 0; y < GRID_HEIGHT; ++y){
                SetUseGrid(x,y, GetNotGrid(x,y));
            }
        }
        InvalidateRect(hwnd, NULL, TRUE);
    }
    bool LifeLogic(int x, int y){
        int i = 0;
        if(GetGrid(x-1,y-1))
            ++i;
        if(GetGrid(x,y-1))
            ++i;
        if(GetGrid(x+1,y-1))
            ++i;
        if(GetGrid(x-1,y))
            ++i;
        if(GetGrid(x+1,y))
            ++i;
        if(GetGrid(x-1,y+1))
            ++i;
        if(GetGrid(x,y+1))
            ++i;
        if(GetGrid(x+1,y+1))
            ++i;
        if(GetGrid(x,y)){
            if(i < 2)
                return false;
            if(i > 3)
                return false;
            return true;
        } 
        else {
            if(i == 3)
                return true;
            return false;
        }
    }
    switch (msg)
    {
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Draw the grid
            for (int i = 0; i < GRID_WIDTH; ++i)
            {
                for (int j = 0; j < GRID_HEIGHT; ++j)
                {
                    HBRUSH brush = (GetGrid(i,j)) ? (HBRUSH)GetStockObject(BLACK_BRUSH) : (HBRUSH)GetStockObject(WHITE_BRUSH);
                    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);
                    Rectangle(hdc, i * GRID_SIZE, j * GRID_SIZE, (i + 1) * GRID_SIZE, (j + 1) * GRID_SIZE);
                    SelectObject(hdc, oldBrush);
                }
            }

            EndPaint(hwnd, &ps);
            break;
        }

        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        case WM_LBUTTONUP:
        {
            if(!started)
            {
                int x = LOWORD(lParam);
                int y = HIWORD(lParam);
                int i = x / GRID_SIZE;
                int j = y / GRID_SIZE;

                SetGrid(i,j,!GetGrid(i,j));
                InvalidateRect(hwnd, NULL, TRUE);
            }
            break;
        }
        case WM_RBUTTONUP:
        {
            for(int x = 0; x < GRID_WIDTH; ++x){
                for(int y = 0; y < GRID_HEIGHT; ++y){
                    SetNotGrid(x, y, LifeLogic(x,y));
                }
            }
            FlipGrids();
            break;
        }
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);
    wc.lpszClassName = _T("MyWindowClass");

    if (!RegisterClass(&wc))
        return 1;

    HWND hwnd = CreateWindow(
        _T("MyWindowClass"),
        _T("My Window"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        WIDTH,
        HEIGHT,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hwnd)
        return 1;

    ShowWindow(hwnd, nCmdShow);

    // Set all the squares in the grid to false initially
    for (int i = 0; i < GRID_WIDTH; ++i)
    {
        for (int j = 0; j < GRID_HEIGHT; ++j)
        {
            grid1[i][j] = false;
            grid2[i][j] = false;
        }
    }

    // Main message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}