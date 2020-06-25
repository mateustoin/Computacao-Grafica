#include "mygl.h"

//Creating base colors
Color ColorWhite = {.red = 255, .green = 255, .blue = 255, .alpha = 255};
Color ColorRed = {.red = 255, .green = 0, .blue = 0, .alpha = 255};
Color ColorGreen = {.red = 0, .green = 255, .blue = 0, .alpha = 255};
Color ColorBlue = {.red = 0, .green = 0, .blue = 255, .alpha = 255};
Color ColorRandom = {.red = 100, .green = 59, .blue = 156, .alpha = 255};

void swap(int *x, int *y){
    int temp;
    temp = *x;
    *x = *y;
    *y = temp;
}

void ApplyInterpolation(Color *corAtual, Color cor1, Color cor2, float distancia){
    corAtual->red = (cor2.red*distancia) + ((1-distancia)*cor1.red);
    corAtual->green = (cor2.green*distancia) + ((1-distancia)*cor1.green);
    corAtual->blue = (cor2.blue*distancia) + ((1-distancia)*cor1.blue);
}

void PutPixel(int x, int y, Color *color){
    const unsigned int startPoint = x * 4 + y * IMAGE_WIDTH * 4;
    fb_ptr[startPoint] = color->red;
    fb_ptr[startPoint + 1] = color->green;
    fb_ptr[startPoint + 2] = color->blue;
    fb_ptr[startPoint + 3] = color->alpha;
}

void DrawLine(int x0, int y0, int x1, int y1, Color *color1, Color *color2){
    // Se a reta tiver sido desenhada nos quadrantes do lado negativo da reta X, troca os pontos para jogar no lado positivo do X
    bool changeColor = false;
    if (x1 < x0){
        swap(&x0, &x1);
        swap(&y0, &y1);
        changeColor = true;
    }

    // Calcula DeltaY e DeltaX
    int dx = x1 - x0;
    int dy = y1 - y0;

    // Inicia variável para contar iterações para interpolação e calcula distância dos pontos
    float passoIteracao = 0.0;
    float dist = sqrt((dx*dx)+(dy*dy));

    // Inicia variáveis de referência para interpolação
    Color colorRef;

    // Se changeColor for true, significa que os pontos foram trocados, então começa interpolando pela segunda cor
    if (changeColor){
        colorRef.red = color2->red;
        colorRef.green = color2->green;
        colorRef.blue = color2->blue;
    }else{ // Se não, interpola cada um com sua cor normal
        colorRef.red = color1->red;
        colorRef.green = color1->green;
        colorRef.blue = color1->blue;
    }
    
    colorRef.alpha = 255; // Alpha sempre em 255 para ter a maior visibilidade

    if (dx == 0){       // Para desenhar linhas retas basta apenas um for
        bool inverte = false;
        if (dy < 0){
            swap(&y0, &y1);
            inverte = true;
        }
            
        
        for (int i = y0; i < y1; i++){
            passoIteracao += 1/dist;
            if (!changeColor && !inverte)
                ApplyInterpolation(&colorRef, *color1, *color2, passoIteracao);
            else
                ApplyInterpolation(&colorRef, *color2, *color1, passoIteracao);
            PutPixel(x0, i, &colorRef);
        }
    }else if (dy == 0){ // Para desenhar linhas retas basta apenas um for
        if (dx < 0)
            swap(&x0, &x1);
        
        for (int i = x0; i < x1; i++){
            passoIteracao += 1/dist;
            if (!changeColor)
                ApplyInterpolation(&colorRef, *color1, *color2, passoIteracao);
            else
                ApplyInterpolation(&colorRef, *color2, *color1, passoIteracao);
            PutPixel(i, y0, &colorRef);
        }
    }else{

        if (MODULO(dy) > MODULO(dx)){ // Faz todo o desenho em relação a Y em vez de X
            bool desenhaContrario = false;
            passoIteracao += 1/dist;

            if (dy < 0){
                dy *= -1;

                desenhaContrario = true;
            }
            
            int d = 2 * dx - dy; // d inicial
            int incrE = 2 * dx;
            int incrNE = 2 * (dx - dy);

            int x = x0;
            int y = y0;
            
            PutPixel(x, y, &colorRef);

            if (desenhaContrario){ // Se o y for negativo e o desenho vai ser em relação a ele, loop compara até y ser igual a y1, seu ponto final
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
                    passoIteracao += 1/dist;
                    if (!changeColor)
                        ApplyInterpolation(&colorRef, *color1, *color2, passoIteracao);
                    else
                        ApplyInterpolation(&colorRef, *color2, *color1, passoIteracao);
                    PutPixel(x, y, &colorRef);
                }
            }else{                  // Nesse caso o desenho é em relação a Y, mas ele não é negativo, então segue a mesma lógica do X com tudo trocado por Y
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
                    passoIteracao += 1/dist;
                    if (!changeColor)
                        ApplyInterpolation(&colorRef, *color1, *color2, passoIteracao);
                    else
                        ApplyInterpolation(&colorRef, *color2, *color1, passoIteracao);
                    PutPixel(x, y, &colorRef);
                }
            }

        }
        else{ // Faz tudo em relação a X, do jeito normal!
            bool desenhaContrario = false;
            passoIteracao = 1/dist;

            if (dy < 0){
                dy *= -1;
                desenhaContrario = true;
            }

            int d = 2 * dy - dx; // d inicial
            int incrE = 2 * dy;
            int incrNE = 2 * (dy - dx);

            int x = x0;
            int y = y0;

            PutPixel(x, y, &colorRef);

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
                passoIteracao += 1/dist;
                if (!changeColor)
                    ApplyInterpolation(&colorRef, *color1, *color2, passoIteracao);
                else
                    ApplyInterpolation(&colorRef, *color2, *color1, passoIteracao);
                PutPixel(x, y, &colorRef);
            }
        }
    }
}

void DrawTriangle(int x1, int y1, Color *cor1, int x2, int y2, Color *cor2, int x3, int y3, Color *cor3){
    DrawLine(x1, y1, x2, y2, cor1, cor2);
    DrawLine(x2, y2, x3, y3, cor2, cor3);
    DrawLine(x3, y3, x1, y1, cor3, cor1);
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
    /*
     *  Teste da função PutPixel
     */
    // PutPixel(0, 0, &ColorGreen);
    // PutPixel(2, 0, &ColorBlue);
    // PutPixel(4, 0, &ColorRed);
    // PutPixel(6, 0, &ColorWhite);
    // PutPixel(8, 0, &ColorRandom);

    /* 
     *  Testando todos os quadrantes
     */ 
    // DrawLine(0, 0, 511, 511, &ColorWhite, &ColorRed);
    // DrawLine(511, 0, 0, 511, &ColorWhite, &ColorGreen);
    
    // DrawLine(256, 127, 256, 383, &ColorWhite, &ColorBlue);
    // DrawLine(127, 256, 383, 256, &ColorWhite, &ColorRandom);
    
    // DrawLine(383, 0, 127, 511, &ColorWhite, &ColorRed);
    // DrawLine(127, 0, 383, 511, &ColorWhite, &ColorBlue);


    /*
     *  Desenhando triângulo
     */
    DrawTriangle(127, 255, &ColorWhite, 383, 255, &ColorRandom, 255, 0, &ColorRed);
    DrawTriangle(0, 255, &ColorRed, 127, 255, &ColorGreen, 127, 0, &ColorBlue);
    DrawTriangle(383, 255, &ColorRandom, 511, 255, &ColorBlue, 383, 0, &ColorWhite);
    
    /*
     *  Desenhando quadrados coloridos
     */
    // DrawSquare1(&ColorRandom);
    // DrawSquare2(&ColorGreen);
    // DrawSquare3(&ColorBlue);
    // DrawSquare4(&ColorRed);
    // DrawSquareCenter(&ColorWhite);
}