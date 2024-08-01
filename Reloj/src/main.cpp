#include <windows.h>
#include <d2d1.h>
#include <dwrite.h>
#include "Reloj.h"

// Variables globales
ID2D1Factory* pD2DFactory = nullptr; // Factoría Direct2D
Reloj reloj; // Instancia de la clase Reloj

// Manejador de mensajes de la ventana
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0); // Termina el programa cuando se cierra la ventana
        return 0;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        BeginPaint(hwnd, &ps);
        reloj.DrawClock(); // Dibuja el reloj
        EndPaint(hwnd, &ps);
    }
    return 0;
    case WM_SIZE:
        reloj.DiscardDeviceResources(); // Libera recursos si cambia el tamaño
        reloj.CreateDeviceResources(pD2DFactory, hwnd); // Crea nuevos recursos
        return 0;
    case WM_TIMER:
        InvalidateRect(hwnd, nullptr, TRUE); // Solicita un repintado de la ventana cada segundo
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam); // Procesa otros mensajes
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    // Registro de la clase de ventana
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;        // Función del manejador de mensajes
    wc.hInstance = hInstance;           // Instancia de la aplicación
    wc.lpszClassName = "ClockWindowClass"; // Nombre de la clase de ventana
    RegisterClass(&wc);

    // Crear la ventana
    HWND hwnd = CreateWindowEx(
        0, "ClockWindowClass", "Reloj Digital",
        WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600, nullptr, nullptr, hInstance, nullptr
    );

    if (hwnd == nullptr) return 0;

    ShowWindow(hwnd, nCmdShow); // Muestra la ventana

    // Inicializar Direct2D
    HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
    if (SUCCEEDED(hr)) {
        reloj.CreateDeviceResources(pD2DFactory, hwnd); // Crear los recursos Direct2D
        SetTimer(hwnd, 1, 1000, nullptr); // Establecer un temporizador para actualizar el reloj

        // Bucle principal de mensajes
        MSG msg;
        while (GetMessage(&msg, nullptr, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    // Liberar los recursos Direct2D al salir
    reloj.DiscardDeviceResources();
    if (pD2DFactory) pD2DFactory->Release();

    return 0;
}
