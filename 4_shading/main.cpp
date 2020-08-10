/*****************************************************************************
 * Este programa é baseado no exemplo "Hello-Triangle" disponível 
 * em 
 *     https://learnopengl.com/Getting-started/Hello-Triangle
 * 
 *     A principal diferença entre este programa e o exemplo acima é que neste
 * os vertex e fragment shaders são carregados a partir de arquivos externos, 
 * ao inves de estarem hard coded no código fonte. 
 *     Isso da mais flexibilidade para se fazerem experimentos com os shaders, 
 * pois nao e necessario se recompilar o programa a cada vez que os shaders forem
 * alterados.
 ****************************************************************************/
#include "main.h"

GLuint buttons[3] = {0,0,0};
GLfloat rotate_x = 0.0f;
GLfloat rotate_y = 0.0f;
GLint last_x = 0;
GLint last_y = 0;

char* frag_shader_source = NULL;
char* vertex_shader_source = NULL;
unsigned int shader_program;
unsigned int vbo; // Vertex buffer object ID
unsigned int vao; // Vertex array object ID

//********************************************************************************************************************
// A função LoadShader() é baseada em https://stackoverflow.com/a/174552/6107321
void LoadShader(char* file_name, char** shader_source) {
    long length;
    FILE* f = fopen(file_name, "rb");

    if (f) {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);

        (*shader_source) = (char*)malloc(length + 1);
        if ((*shader_source)) {
            fread((*shader_source), 1, length, f);
            (*shader_source)[length] = '\0';
        }

        fclose(f);
    } else {
        printf("Could not load the shader file %s\nExiting...", file_name);
        exit(EXIT_FAILURE);
    }
}

//********************************************************************************************************************
void Display(void) {
    // Limpa a tela e o depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Define a posição da Viewport dentro da janela OpenGL
    glViewport(0, 0, IMAGE_WIDTH, IMAGE_HEIGHT);

    // Seleciona o Shader Program a ser utilizado.
    glUseProgram(shader_program);

    // Matriz Model ///////////////////////////////////////////////////////////
    float ty_array[16] = {1.0f, 0.0f, 0.0f, 0.0f, 
                          0.0f, 1.0f, 0.0f, 0.0f, 
                          0.0f, 0.0f, 1.0f, 0.0f, 
                          0.0f, -0.5f, 0.0f, 1.0f};

    float s_array[16] = {0.6f, 0.0f, 0.0f, 0.0f, 
                         0.0f, 0.6f, 0.0f, 0.0f, 
                         0.0f, 0.0f, 0.6f, 0.0f, 
                         0.0f, 0.0f, 0.0f, 1.0f};

    // rotation about Y axis
    float ry_array[16] = {glm::cos(glm::radians(90.0f)), 0.0f, -glm::sin(glm::radians(90.0f)), 0.0f, 
                          0.0f, 1.0f, 0.0f, 0.0f, 
                          glm::sin(glm::radians(90.0f)), 0.0f, glm::cos(glm::radians(90.0f)), 0.0f, 
                          0.0f, 0.0f, 0.0f, 1.0f};

    // rotation about X axis
    float rot_x_array[16] = {1.0f, 0.0f, 0.0f, 0.0f, 
                             0.0f, glm::cos(rotate_x), glm::sin(rotate_x), 0.0f, 
                             0.0f, -glm::sin(rotate_x), glm::cos(rotate_x), 0.0f, 
                             0.0f, 0.0f, 0.0f, 1.0f};

    // rotation about Y axis
    float rot_y_array[16] = {glm::cos(rotate_y), 0.0f, -glm::sin(rotate_y), 0.0f, 
                             0.0f, 1.0f, 0.0f, 0.0f, 
                             glm::sin(rotate_y), 0.0f, glm::cos(rotate_y), 0.0f, 
                             0.0f, 0.0f, 0.0f, 1.0f};

    glm::mat4 model_mat = glm::make_mat4(rot_y_array) * glm::make_mat4(rot_x_array) * glm::make_mat4(s_array) * glm::make_mat4(ty_array) * glm::make_mat4(ry_array);

    // Matriz View ////////////////////////////////////////////////////////////
    glm::mat4 view_mat = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.5f), 
                                     glm::vec3(0.0f, 0.0f, 0.0f), 
                                     glm::vec3(0.0f, 1.0f, 0.0f));

    // Matriz Projection //////////////////////////////////////////////////////
    glm::mat4 proj_mat = glm::perspective(glm::radians(55.0f), (float) IMAGE_WIDTH / IMAGE_HEIGHT, 0.1f, 100.f);

    // Matriz ModelViewProjection /////////////////////////////////////////////
    glm::mat4 model_view_proj_mat = proj_mat * view_mat * model_mat;

    // unsigned int transformLoc;
    // GL_CHECK(transformLoc = glGetUniformLocation(shader_program, "transform"));
    // glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model_view_proj_mat));

    unsigned int model_loc;
    GL_CHECK(model_loc = glGetUniformLocation(shader_program, "model_mat"));
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model_mat));

    unsigned int view_loc;
    GL_CHECK(view_loc = glGetUniformLocation(shader_program, "view_mat"));
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view_mat));

    unsigned int proj_loc;
    GL_CHECK(proj_loc = glGetUniformLocation(shader_program, "proj_mat"));
    glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(proj_mat));

    // Ativa o Vertex Array Object selecionado.
    glBindVertexArray(vao);

    // Desenha as tres primeiras primitivias, comecando pela de indice 0.
    glDrawArrays(GL_TRIANGLES, 0, (sizeof(vertices) / sizeof(float)) / 9);

    glFlush();            //
    glutSwapBuffers();    //
    glutPostRedisplay();  //
}

//********************************************************************************************************************
void Mouse(int b, int s, int x, int y) {
    last_x = x;
    last_y = y;

    switch (b) {
        case GLUT_LEFT_BUTTON:
            buttons[0] = ((GLUT_DOWN == s) ? 1 : 0);
            break;

        case GLUT_MIDDLE_BUTTON:
            buttons[1] = ((GLUT_DOWN == s) ? 1 : 0);
            break;

        case GLUT_RIGHT_BUTTON:
            buttons[2] = ((GLUT_DOWN == s) ? 1 : 0);
            break;

        default:
            break;
    }
}

//********************************************************************************************************************
void Motion(int x, int y) {
    int diff_x = x - last_x;
    int diff_y = y - last_y;

    last_x = x;
    last_y = y;

    if (buttons[0] == 1) {
        rotate_y += 0.01f * diff_x;
        rotate_x += 0.01f * diff_y;
    }
}

//********************************************************************************************************************
void ExitProg(void) {
    if (vertex_shader_source) {
        free(vertex_shader_source);
        vertex_shader_source = NULL;
    }

    if (frag_shader_source) {
        free(frag_shader_source);
        frag_shader_source = NULL;
    }

    printf("Exiting...\n");
}

//********************************************************************************************************************
int main(int argc, char** argv) {
    // Inicializa a GLUT
    glutInit(&argc, argv);

    // Cria um color buffer onde cada pixel é representado por 4 bytes (RGBA)
    // Cria um depth buffer (para resolver a oclusão)
    // Cria dois color buffers para reduzir o flickering
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);

    // Define as dimensões do color buffer (ou a área útil do OpenGL na janela)
    glutInitWindowSize(IMAGE_WIDTH, IMAGE_HEIGHT);

    // Posição do canto superior esquerdo da janela OpenGL em relação a tela do computador.
    glutInitWindowPosition(100, 100);

    // Título da janela
    glutCreateWindow("Modern OpenGL: Mateus Antonio - 4 Shading");

    // Load the OpenGL extensions
    GLenum err = glewInit();
    if (GLEW_OK != err) {
      /* Problem: glewInit failed, something is seriously wrong. */
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
      return EXIT_FAILURE; 
    }

    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

    // Carrega o codigo fonte do Vertex shader
    LoadShader("vertex_shader.glsl", &vertex_shader_source);

    printf("%s\n", vertex_shader_source);

    unsigned int vertex_shader;

    // Cria um identificador para o Vertex Shader
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);

    // Vincula o código fonte do Vertex Shader ao seu identificador
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);

    // Compila dinamicamente (em tempo de execucao) o Vertex Shader
    glCompileShader(vertex_shader);

    // Imprime eventuais mensagens de erro de compilacao do Vertex Shader
    int success;
    char info_log[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", info_log);
    }

    // Carrega o codigo fonte do Fragment shader
    LoadShader("fragment_shader.glsl", &frag_shader_source);

    printf("%s\n", frag_shader_source);

    unsigned int fragment_shader;

    // Cria um identificador para o Fragment Shader
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    // Vincula o código fonte do Fragment Shader ao seu identificador
    glShaderSource(fragment_shader, 1, &frag_shader_source, NULL);

    // Compila dinamicamente (em tempo de execucao) o Fragment Shader
    glCompileShader(fragment_shader);

    // Imprime eventuais mensagens de erro de compilacao do Fragment Shader
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", info_log);
    }

    // Cria um identificador para um Shader program (vertex + fragment shader)
    shader_program = glCreateProgram();

    // Vincula os Fragment e Vertex Shaders ao Program Shader
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);

    // Linka o Fragment e Vertex Shader para formarem o Program Shader 
    glLinkProgram(shader_program);

    // Imprime eventuais mensagens de erro de linkagem do Program Shader
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", info_log);
    }

    // Deleta os Fragment e Vertex Shaders, já que eles já foram incorporados 
    // ao Program Shader e não são mais necessários.
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // Ativa o Vertex Array Object (VAO)
    glBindVertexArray(vao);

    // Cria um novo identificador de buffer 
    glGenBuffers(1, &vbo);

    // Vincula o buffer criado a um Vertex Buffer Object (VBO)
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Carrega as propriedades (coordenadas + cores) dos vértices no VBO
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

    // Atributo 'posição' do vértice
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Atributo 'normal' do vértice
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Atributo 'cor' do vértice
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Define a cor a ser utilizada para limpar o color buffer a cada novo frame
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);            

    // Habilita o teste de profundidade (oclusão).
    GL_CHECK(glEnable(GL_DEPTH_TEST));

    atexit(ExitProg);          // deifne o callback de saída do programa

    glutDisplayFunc(Display);  // define o callback que renderizará cada frame
    glutMouseFunc(Mouse);      // define o callback para clique do mouse
    glutMotionFunc(Motion);    // define o callback para o movimento do mouse

    glutMainLoop();            // Framebuffer scan loop.

    return EXIT_SUCCESS;
}
