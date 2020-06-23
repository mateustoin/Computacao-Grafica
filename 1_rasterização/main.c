#include "main.h"

GLuint tex;

//********************************************************************************************************************
void Display(void) {
    MyGlDraw();

    // Copia o framebuffer para a textura.
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, IMAGE_WIDTH, IMAGE_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, fb_ptr);

    glEnable(GL_TEXTURE_2D);

    // Desenha o quadrilátero com a textura mapeada
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glViewport(0, 0, IMAGE_WIDTH, IMAGE_HEIGHT);

    glBegin(GL_TRIANGLES);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(1.0f, 1.0f, 0.0f);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    glFlush();
    glutSwapBuffers();
    glutPostRedisplay();
}

//********************************************************************************************************************
void ExitProg(void) {
    // Libera a memória referente ao framebuffer.
    if (!fb_ptr)
        free(fb_ptr);

    printf("Exiting...\n");
}

//********************************************************************************************************************
void InitOpenGL(int *argc, char **argv) {
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(IMAGE_WIDTH, IMAGE_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("My OpenGL");

    // Ajusta a projeção ortográfica.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//********************************************************************************************************************
void InitCallBacks(void) {
    atexit(ExitProg);
    glutDisplayFunc(Display);
}

//********************************************************************************************************************
void InitDataStructures(void) {
    // Aloca o framebuffer e inicializa suas posições com 0.
    fb_ptr = (unsigned char *)malloc(IMAGE_WIDTH * IMAGE_HEIGHT * 4 * sizeof(char));

    for (int i = 0; i < IMAGE_WIDTH * IMAGE_HEIGHT; ++i) {
        fb_ptr[i * 4] = 0;
        fb_ptr[i * 4 + 1] = 0;
        fb_ptr[i * 4 + 2] = 0;
        fb_ptr[i * 4 + 3] = 255;
    }

    // Cria uma textura 2D, RGBA (8 bits por componente).
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
}

//********************************************************************************************************************
int main(int argc, char **argv) {
    // Inicializações.
    fb_ptr = NULL;
    InitOpenGL(&argc, argv);
    InitCallBacks();
    InitDataStructures();

    // Framebuffer scan loop.
    glutMainLoop();

    return 0;
}
