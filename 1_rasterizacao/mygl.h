#ifndef _MYGL_H_
#define _MYGL_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "core.h"
#include "frame_buffer.h"

#define MODULO(x) ((x)>=0?(x):-(x)) 

// Creating a Struct to keep data about pixel colors
typedef struct{ 
    unsigned char red;    // defining color red
    unsigned char green;  // defining color green
    unsigned char blue;   // defining color blue
    unsigned char alpha;  // defining opacity alpha
} Color; 

// Declaração da função que chamará as funções implementadas pelo aluno
void MyGlDraw(void);

//
// >>> Declare aqui as funções que você implementar <<<
//
void PutPixel(int x, int y, Color *);
void DrawLine(int, int, int, int, Color *, Color *);
void DrawTriangle(int, int, Color *, int, int, Color *, int, int, Color *);

void DrawSquare1(Color *);
void DrawSquare2(Color *);
void DrawSquare3(Color *);
void DrawSquare4(Color *);
void DrawSquareCenter(Color *);

#endif  // MYGL_H
