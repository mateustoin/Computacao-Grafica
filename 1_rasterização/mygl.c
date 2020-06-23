#include "mygl.h"

//Creating base colors
Color ColorWhite = {.red = 255, .green = 255, .blue = 255, .alpha = 255};
Color ColorRed = {.red = 255, .green = 0, .blue = 0, .alpha = 255};
Color ColorGreen = {.red = 0, .green = 255, .blue = 0, .alpha = 255};
Color ColorBlue = {.red = 0, .green = 0, .blue = 255, .alpha = 255};
Color ColorRandom = {.red = 100, .green = 59, .blue = 156, .alpha = 255};

void swap(int *x, int *y)
{
    int temp;
    temp = *x;
    *x = *y;
    *y = temp;
}

void PutPixel(int x, int y, Color *color)
{
    const unsigned int startPoint = x * 4 + y * IMAGE_WIDTH * 4;
    fb_ptr[startPoint] = color->red;
    fb_ptr[startPoint + 1] = color->green;
    fb_ptr[startPoint + 2] = color->blue;
    fb_ptr[startPoint + 3] = color->alpha;
}

void DrawLine(int x0, int y0, int x1, int y1, Color *color)
{
    // Se a reta tiver sido desenhada nos quadrantes 3, 4, 5 ou 6, troca os pontos para jogar no lado positivo do X
    if (x1 < x0){
        swap(&x0, &x1);
        swap(&y0, &y1);
    }

    // Calcula DeltaY e DeltaX
    int dx = x1 - x0;
    int dy = y1 - y0;


    if (dx == 0){ // Para desenhar linhas retas basta apenas um for
        if (dy < 0)
            swap(&y0, &y1);
        for (int i = y0; i < y1; i++){
            PutPixel(x0, i, color);
        }
    }else if (dy == 0){
        if (dx < 0)
            swap(&x0, &x1);
        for (int i = x0; i < x1; i++){
            PutPixel(i, y0, color);
        }
    }else{

        if (MODULO(dy) > MODULO(dx)){ // Faz todo o desenho em relação a Y em vez de X
            bool desenhaContrario = false;

            if (dy < 0){
                dy *= -1;

                desenhaContrario = true;
            }
            
            int d = 2 * dx - dy; // d inicial
            int incrE = 2 * dx;
            int incrNE = 2 * (dx - dy);

            int x = x0;
            int y = y0;
            
            PutPixel(x, y, color);

            if (desenhaContrario){ // Se o y for negativo e o desenho vai ser em relação a ele
                while (y > y1){
                    if (d <= 0){
                        d += incrE;
                        if (desenhaContrario)
                            y--;
                        else
                            y++;
                    }
                    else{
                        d += incrNE;
                        if (desenhaContrario)
                            y--;
                        else
                            y++;
                        x++;
                    }
                    PutPixel(x, y, color);
                }
            }else{
                while (y <= y1){
                    if (d <= 0){
                        d += incrE;
                        if (desenhaContrario)
                            y--;
                        else
                            y++;
                    }
                    else{
                        d += incrNE;
                        if (desenhaContrario)
                            y--;
                        else
                            y++;
                        x++;
                    }
                    PutPixel(x, y, color);
                }
            }

        }
        else{ // Faz tudo em relação a X, do jeito normal!
            bool desenhaContrario = false;

            if (dy < 0){
                dy *= -1;
                desenhaContrario = true;
            }

            int d = 2 * dy - dx; // d inicial
            int incrE = 2 * dy;
            int incrNE = 2 * (dy - dx);

            int x = x0;
            int y = y0;

            PutPixel(x, y, color);

            while (x <= x1){
                if (d <= 0){
                    d += incrE;
                    x++;
                }
                else{
                    d += incrNE;
                    x++;
                    if (desenhaContrario)
                        y--;
                    else
                        y++;
                }
                PutPixel(x, y, color);
            }
        }
    }
}

void DrawTriangle(void)
{
}

void DrawSquare1(Color *color)
{
    for (int i = 0; i < 256; i++)
    {
        for (int j = 0; j < 256; j++)
        {
            PutPixel(i, j, color);
        }
    }
}

void DrawSquare2(Color *color)
{
    for (int i = 256; i < 511; i++)
    {
        for (int j = 0; j < 256; j++)
        {
            PutPixel(i, j, color);
        }
    }
}

void DrawSquare3(Color *color)
{
    for (int i = 0; i < 256; i++)
    {
        for (int j = 256; j < 511; j++)
        {
            PutPixel(i, j, color);
        }
    }
}

void DrawSquare4(Color *color)
{
    for (int i = 256; i < 511; i++)
    {
        for (int j = 256; j < 511; j++)
        {
            PutPixel(i, j, color);
        }
    }
}

void DrawSquareCenter(Color *color)
{
    for (int i = 215; i < 296; i++)
    {
        for (int j = 215; j < 296; j++)
        {
            PutPixel(i, j, color);
        }
    }
}

// Definição da função que chamará as funções implementadas pelo aluno
void MyGlDraw(void)
{
    //
    // >>> Chame aqui as funções que você implementou <<<
    //
    //PutPixel(0, 0, ColorGreen);

    DrawLine(0, 0, 511, 511, &ColorRandom);
    DrawLine(511, 0, 0, 511, &ColorGreen);
    
    DrawLine(256, 127, 256, 383, &ColorBlue);
    DrawLine(127, 256, 383, 256, &ColorRed);
    
    DrawLine(383, 0, 127, 511, &ColorWhite);
    DrawLine(127, 0, 383, 511, &ColorWhite);
    

    // DrawSquare1(&ColorRandom);
    // DrawSquare2(&ColorGreen);
    // DrawSquare3(&ColorBlue);
    // DrawSquare4(&ColorRed);
    // DrawSquareCenter(&ColorWhite);
}