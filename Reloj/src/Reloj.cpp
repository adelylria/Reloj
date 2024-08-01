#include "Reloj.h"
#include <d2d1.h>
#include <dwrite.h>
#include <string>
#include <ctime>
#include <windows.h>

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")


Reloj::Reloj() : pRenderTarget(nullptr), pBrush(nullptr), pDWriteFactory(nullptr), pTextFormat(nullptr) {}

Reloj::~Reloj() {
    DiscardDeviceResources(); // Asegúrate de liberar los recursos al destruir el objeto
}

void Reloj::CreateDeviceResources(ID2D1Factory* pD2DFactory, HWND hwnd) {
    if (pRenderTarget) return; // Evita recrear el render target si ya existe

    RECT rc;
    GetClientRect(hwnd, &rc); // Obtiene el tamaño del cliente de la ventana
    D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

    if (!pD2DFactory) return; // Si la factoría Direct2D no está inicializada, no se puede crear el render target

    HRESULT hr = pD2DFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),                   // Propiedades del render target
        D2D1::HwndRenderTargetProperties(hwnd, size),     // Propiedades del render target para ventana
        &pRenderTarget                                    // Salida: el render target creado
    );

    if (SUCCEEDED(hr)) {
        // Crear una brocha blanca para dibujar
        hr = pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pBrush);
        if (SUCCEEDED(hr)) {
            // Crear el formato de texto con la fuente Arial y tamaño 72
            hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&pDWriteFactory));
            if (SUCCEEDED(hr)) {
                hr = pDWriteFactory->CreateTextFormat(
                    L"Arial", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL,
                    DWRITE_FONT_STRETCH_NORMAL, 72, L"en-us", &pTextFormat
                );
                if (SUCCEEDED(hr)) {
                    // Configuración de la alineación del texto
                    pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
                    pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
                }
            }
        }
    }
}

void Reloj::DiscardDeviceResources() {
    // Liberar y restablecer los recursos Direct2D
    if (pRenderTarget) {
        pRenderTarget->Release();
        pRenderTarget = nullptr;
    }
    if (pBrush) {
        pBrush->Release();
        pBrush = nullptr;
    }
    if (pTextFormat) {
        pTextFormat->Release();
        pTextFormat = nullptr;
    }
    if (pDWriteFactory) {
        pDWriteFactory->Release();
        pDWriteFactory = nullptr;
    }
}

void Reloj::DrawClock() {
    if (!pRenderTarget) return; // Asegúrate de que el render target esté disponible

    pRenderTarget->BeginDraw(); // Comienza el dibujo
    pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black)); // Limpia el fondo con color negro

    // Obtiene la hora actual y la formatea
    time_t now = time(nullptr);
    tm localTime;
    localtime_s(&localTime, &now);
    wchar_t timeStr[9];
    swprintf(timeStr, 9, L"%02d:%02d:%02d", localTime.tm_hour, localTime.tm_min, localTime.tm_sec);

    // Configura el área de dibujo para el texto
    D2D1_RECT_F layoutRect = D2D1::RectF(0, 0, 800, 600);
    pRenderTarget->DrawText(
        timeStr,           // Texto a dibujar
        wcslen(timeStr),   // Longitud del texto
        pTextFormat,       // Formato del texto
        layoutRect,        // Área de dibujo
        pBrush             // Brocha para el color
    );

    HRESULT hr = pRenderTarget->EndDraw(); // Finaliza el dibujo
    if (FAILED(hr)) {
        // Manejo de errores si EndDraw falla
    }
}
