#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "errors.c"
#include "utils/file.c"
#include "utils/time.c"

#define TICKS_PER_SECOND 20
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
#define WINDOW_NAME "learn_opengl"

GLFWwindow* window;
GLuint shaderProgram;

void initWindow(void);
void initGlew(void);
void initShaderProgram(void);
void compileShader(char* filename, GLenum type);
void loop(void);
void render(void);
void processInput(void);
void tick(void);

int main(void)
{
    initWindow();
    initGlew();
    initShaderProgram();
    loop();

    return 0;
}

void initWindow(void)
{
    if (!glfwInit()) {
        printf("%s\n", "Failed to initialize GLFW.");

        exit(GLFW_INIT_ERROR);
    }

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, NULL, NULL);

    if (!window) {
        printf("%s\n", "Failed to create window.");
        glfwTerminate();

        exit(CREATE_WINDOW_ERROR);
    }

    glfwMakeContextCurrent(window);
}

void initGlew(void)
{
    if (GLEW_OK != glewInit()) {
        printf("%s\n", "Failed to initialize GLEW.");

        exit(GLEW_INIT_ERROR);
    }
}

void initShaderProgram(void)
{
    shaderProgram = glCreateProgram();

    compileShader("../src/shaders/vs.glsl", GL_VERTEX_SHADER);
    compileShader("../src/shaders/tcs.glsl", GL_TESS_CONTROL_SHADER);
    compileShader("../src/shaders/tes.glsl", GL_TESS_EVALUATION_SHADER);
    compileShader("../src/shaders/gs.glsl", GL_GEOMETRY_SHADER);
    compileShader("../src/shaders/fs.glsl", GL_FRAGMENT_SHADER);

    glLinkProgram(shaderProgram);
}

void compileShader(char* filename, GLenum type)
{
    char* src = readFromFile(filename);
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, (const GLchar *const *) &src, NULL);
    glCompileShader(shader);

    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

    if (isCompiled == GL_FALSE) {
        printf("%s %s.\n", "Failed to compile shader", filename);

        exit(SHADER_COMPILATION_ERROR);
    }

    glAttachShader(shaderProgram, shader);
    glDeleteShader(shader);

    free(src);
}

void loop(void)
{
    double previous = getCurrentTime();
    double lag = 0.0;
    double msPerUpdate = 1000.0 / TICKS_PER_SECOND;

    while (!glfwWindowShouldClose(window)) {
        double current = getCurrentTime();
        double elapsed = current - previous;

        previous = current;
        lag += elapsed;

        processInput();

        while (lag >= msPerUpdate) {
            tick();
            render();
            lag -= msPerUpdate;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(window, 1);
        }
    }

    glfwTerminate();
}

void render(void)
{
    const GLfloat clearColor[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    glClearBufferfv(GL_COLOR, 0, clearColor);

    glUseProgram(shaderProgram);

    glPatchParameteri(GL_PATCH_VERTICES, 3);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawArrays(GL_PATCHES, 0, 3);
}

void processInput(void)
{
}

void tick(void)
{
}