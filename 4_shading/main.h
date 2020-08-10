#ifndef MAIN_H
#define MAIN_H

#define GLEW_STATIC

#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>

#include <iostream>

#include "../third_party/glm/glm/gtc/type_ptr.hpp"
#include "../third_party/glm/glm/mat4x4.hpp"

#include "teapot.h"

#define IMAGE_WIDTH 800 // Largura da janela OpenGL em pixels.
#define IMAGE_HEIGHT 800 // Altura da janela OpenGL em pixels.

// The debug function/macro below were extracted from:
//     https://stackoverflow.com/a/11258474/6107321
void CheckOpenGLError(const char* stmt, const char* fname, int line) {
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        printf("OpenGL error %08x, at %s:%i - for %s\n", err, fname, line,
               stmt);
        abort();
    }
}

#ifdef DEBUG
#define GL_CHECK(stmt)                               \
    do {                                             \
        stmt;                                        \
        CheckOpenGLError(#stmt, __FILE__, __LINE__); \
    } while (0)
#else
#define GL_CHECK(stmt) stmt
#endif

#endif  // MAIN_H
