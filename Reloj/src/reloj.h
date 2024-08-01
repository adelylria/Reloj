#ifndef RELOJ_H
#define RELOJ_H

#include <d2d1.h>
#include <dwrite.h>
#include <string>
#include <ctime>

// Clase Reloj para manejar el dibujo del reloj y los recursos gráficos
class Reloj {
public:
    Reloj();  // Constructor: inicializa los recursos a nullptr
    ~Reloj(); // Destructor: libera los recursos

    // Crea los recursos gráficos necesarios para Direct2D
    void CreateDeviceResources(ID2D1Factory* pD2DFactory, HWND hwnd);

    // Libera los recursos gráficos
    void DiscardDeviceResources();

    // Dibuja el reloj en la ventana
    void DrawClock();

private:
    ID2D1HwndRenderTarget* pRenderTarget; // Render target para Direct2D
    ID2D1SolidColorBrush* pBrush;         // Brocha de color para dibujar
    IDWriteFactory* pDWriteFactory;       // Factoría DirectWrite
    IDWriteTextFormat* pTextFormat;       // Formato de texto para DirectWrite
};

#endif // RELOJ_H
