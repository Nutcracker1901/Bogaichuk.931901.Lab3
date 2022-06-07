/*-----------------------------------------
   SINEWAVE.C -- Sine Wave Using Polyline
  -----------------------------------------*/

#include <windows.h>
#include <math.h>
#include <d2d1.h>
#pragma comment(lib, "d2d1")
#include "Functions.h"
//#include "Functions.cpp"



void Upd_Stage(HMENU menu, UINT_PTR curr, UINT_PTR* prev, TempFunction* handler) {/////////////REFACTOR
    EnableMenuItem(menu, curr, MF_GRAYED);
    EnableMenuItem(menu, *prev, MF_ENABLED);
    *prev = curr;
    Functions* func = new SinusoidFunction;
    switch (curr) {
    case SQUAREROOTF:
        func = new SquareRootFunction;
        break;
    case PARABOLAF:
        func = new ParabolaFunction;
        break;
    case HYPERBOLAF:
        func = new HyperbolaFunction;
        break;
    case SINUSOIDF:
        func = new SinusoidFunction;
        break;
    }
    handler->SetFunc(func);
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

template <class T> void SafeRelease(T** ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
}

class Direct2DResources {
    ID2D1Factory* pFactory;
    ID2D1HwndRenderTarget* pRenderTarget;
    ID2D1SolidColorBrush* pBrush;
public:
    Direct2DResources() : pFactory(NULL), pRenderTarget(NULL), pBrush(NULL) {}

    HRESULT CreateGraphicsResources(HWND hwnd)
    {
        HRESULT hr = S_OK;
        if (pRenderTarget == NULL)
        {
            RECT rc;
            GetClientRect(hwnd, &rc);
            D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);
            hr = pFactory->CreateHwndRenderTarget(
                D2D1::RenderTargetProperties(),
                D2D1::HwndRenderTargetProperties(hwnd, size),
                &pRenderTarget);
            if (SUCCEEDED(hr))
            {
                const D2D1_COLOR_F color = D2D1::ColorF(0.0f, 0.0f, 0.0f);
                hr = pRenderTarget->CreateSolidColorBrush(color, &pBrush);
            }
        }
        return hr;
    }

    void DiscardGraphicsResources()
    {
        SafeRelease(&pRenderTarget);
        SafeRelease(&pBrush);
    }

    void Destroy()
    {
        DiscardGraphicsResources();
        SafeRelease(&pFactory);
    }

    void OnPaint(HWND hwnd, TempFunction* func)
    {
        HRESULT hr = CreateGraphicsResources(hwnd);
        if (SUCCEEDED(hr))
        {
            D2D1_SIZE_F size;
            PAINTSTRUCT ps;
            BeginPaint(hwnd, &ps);
            D2D1_POINT_2F apt[1000];

            pRenderTarget->BeginDraw();
            pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
            size = pRenderTarget->GetSize();
            pRenderTarget->DrawLine(
                D2D1::Point2F(size.width / 2, 0.0f),
                D2D1::Point2F(size.width / 2, size.height),
                pBrush,
                1.0f
            );
            pRenderTarget->DrawLine(
                D2D1::Point2F(0.0f, size.height / 2),
                D2D1::Point2F(size.width, size.height / 2),
                pBrush,
                1.0f
            );

            func->RunFunction(size.width, size.height, apt);
            for (int i = 0; i < 1000 - 1; i++) {
                pRenderTarget->DrawLine(
                    apt[i],
                    apt[i + 1],
                    pBrush,
                    2.0f
                );
            }

            hr = pRenderTarget->EndDraw();
            if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
            {
                DiscardGraphicsResources();
            }
            EndPaint(hwnd, &ps);
        }
    }

    void Resize(HWND hwnd)
    {
        if (pRenderTarget != NULL)
        {
            RECT rc;
            GetClientRect(hwnd, &rc);
            D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);
            pRenderTarget->Resize(size);
            InvalidateRect(hwnd, NULL, FALSE);
        }
    }

    BOOL Create() {
        if (FAILED(D2D1CreateFactory(
            D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory)))
        {
            return -1;
        }
        return 0;
    }
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR szCmdLine, int iCmdShow)
{
    static TCHAR szAppName[] = TEXT("Lab3");
    HWND         hwnd;
    MSG          msg;
    WNDCLASS     wndclass;
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;

    RegisterClass(&wndclass);

    HMENU hDropDownMenu = CreateMenu();

    AppendMenu(hDropDownMenu, MF_STRING, SQUAREROOTF, L"Square Root");
    EnableMenuItem(hDropDownMenu, SINUSOIDF, MF_GRAYED);
    AppendMenu(hDropDownMenu, MF_STRING, PARABOLAF, L"Parabola");
    AppendMenu(hDropDownMenu, MF_STRING, HYPERBOLAF, L"Hyperbola");
    AppendMenu(hDropDownMenu, MF_STRING, SINUSOIDF, L"Sinusoid");

    HMENU hmenu = CreateMenu();

    AppendMenu(hmenu, MF_POPUP, (UINT_PTR)hDropDownMenu, L"Functions");

    hwnd = CreateWindow(szAppName, TEXT("Lab 3 System Programming - WinAPI"),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, hmenu, hInstance, NULL);

    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int cxClient, cyClient;
    static UINT_PTR lastMenuItem = SINUSOIDF;
    static TempFunction* func = new TempFunction(new SinusoidFunction);
    static Direct2DResources* d2dr;

    switch (message)
    {
    case WM_CREATE:
        d2dr = new Direct2DResources();
        return d2dr->Create();
    case WM_COMMAND:
    {
        HMENU hDropDownMenu = GetSubMenu(GetMenu(hwnd), 0);
        switch (wParam)
        {
        case SQUAREROOTF:
            Upd_Stage(hDropDownMenu, SQUAREROOTF, &lastMenuItem, func);
            break;
        case PARABOLAF:
            Upd_Stage(hDropDownMenu, PARABOLAF, &lastMenuItem, func);
            break;
        case HYPERBOLAF:
            Upd_Stage(hDropDownMenu, HYPERBOLAF, &lastMenuItem, func);
            break;
        case SINUSOIDF:
            Upd_Stage(hDropDownMenu, SINUSOIDF, &lastMenuItem, func);
            break;
        }
        InvalidateRect(hwnd, NULL, TRUE);
    }
    case WM_SIZE:
        d2dr->Resize(hwnd);
        return 0;

    case WM_PAINT:
    {
        d2dr->OnPaint(hwnd, func);
        return 0;
    }
    case WM_DESTROY:
        d2dr->Destroy();
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}