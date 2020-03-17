#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "utils/file.c"
#include "utils/time.c"

const int TICKS_PER_SECOND = 20;

GLFWwindow* window;
GLuint shaderProgram;

void processInput()
{
}

void tick()
{
}

void compileShaders(void)
{
    GLuint vertexShader, fragmentShader, tesselationControlShader, tesselationEvaluationShader;
    shaderProgram = glCreateProgram();

    const GLchar* shaderSource = readFromFile("../src/shaders/vertex.glsl");
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &shaderSource, NULL);
    glCompileShader(vertexShader);

    shaderSource = readFromFile("../src/shaders/tesselation_control.glsl");
    tesselationControlShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(tesselationControlShader, 1, &shaderSource , NULL);
    glCompileShader(tesselationControlShader);

    shaderSource = readFromFile("../src/shaders/tesselation_evaluation.glsl");
    tesselationEvaluationShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(tesselationEvaluationShader, 1, &shaderSource , NULL);
    glCompileShader(tesselationEvaluationShader);

    shaderSource = readFromFile("../src/shaders/fragment.glsl");
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &shaderSource , NULL);
    glCompileShader(fragmentShader);

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, tesselationControlShader);
    glAttachShader(shaderProgram, tesselationEvaluationShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(tesselationControlShader);
    glDeleteShader(tesselationEvaluationShader);
    glDeleteShader(fragmentShader);

    free(shaderSource);
}

void render()
{
    const GLfloat clearColor[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    glClearBufferfv(GL_COLOR, 0, clearColor);

    glUseProgram(shaderProgram);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawArrays(GL_PATCHES, 0, 3);
}

int main(void)
{
    if (!glfwInit()) {
        printf("%s\n", "Failed to initialize GLFW.");

        return 1;
    }

    window = glfwCreateWindow(640, 480, "Window", NULL, NULL);

    if (!window) {
        printf("%s\n", "Failed to create window.");
        glfwTerminate();

        return 2;
    }

    glfwMakeContextCurrent(window);

    if (GLEW_OK != glewInit()) {
        printf("%s\n", "Failed to initialize GLEW.");

        return 3;
    }

    compileShaders();

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

    return 0;
}