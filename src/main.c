#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "matrix.c"
#include "vector.c"
#include "file.c"
#include "time.c"

#define TICKS_PER_SECOND 30

typedef struct {
    GLFWwindow* window;
    int width;
    int height;
    char name[40];
} window_t;

typedef struct {
    float model[16];
    float view[16];
    float projection[16];
} mvp_t;

typedef struct {
    float x;
    float y;
} cursorPosition_t;

typedef struct {
    GLuint program;
    GLuint vertices;
    GLuint colors;
} attribute_t;

static window_t window;
static mvp_t mvp;
static cursorPosition_t cursorPosition;
static GLuint shaderProgram;
static GLuint matrixLocation;
static attribute_t cube;

void initWindow(void);
void initGlew(void);
void initShaderProgram(void);
void lookAt(float* lookAt, vector_t position, vector_t target);
void compileShader(char* filename, GLenum type);
void loop(void);
void render(void);
void tick(void);
void cursorPositionCallback(GLFWwindow* window, double x, double y);
void initCube(void);
void drawCube(void);

int main(void)
{
    initWindow();
    initGlew();
    initShaderProgram();
    initCube();

    matrixIdentity(mvp.model);

    vector_t position = {0.0f, 3.0f, 15.0f};
    vector_t target = {0.0f, 0.0f, 0.0f};
    lookAt(mvp.view, position, target);

    matrixPerspective(
        mvp.projection,
        45.0f,
        (float) window.width / (float) window.height,
        0.1f,
        20.0f
    );

    loop();

    return 0;
}

void initWindow(void)
{
    if (!glfwInit()) {
        printf("%s\n", "Failed to initialize GLFW.");

        exit(1);
    }

    window.width = 800;
    window.height = 600;
    window.name[40] = "learn-opengl";
    window.window = glfwCreateWindow(window.width, window.height, window.name, NULL, NULL);
    glfwSetCursorPosCallback(window.window, cursorPositionCallback);
    glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    if (!window.window) {
        printf("%s\n", "Failed to create window.");
        glfwTerminate();

        exit(2);
    }

    glfwMakeContextCurrent(window.window);
}

void initGlew(void)
{
    if (GLEW_OK != glewInit()) {
        printf("%s\n", "Failed to initialize GLEW.");

        exit(3);
    }
}

void initShaderProgram(void)
{
    shaderProgram = glCreateProgram();

    compileShader("../shaders/vs.vert", GL_VERTEX_SHADER);
    compileShader("../shaders/fs.frag", GL_FRAGMENT_SHADER);

    glBindAttribLocation(shaderProgram, 0, "in_Position");
    glBindAttribLocation(shaderProgram, 1, "in_Color");

    glLinkProgram(shaderProgram);

    matrixLocation = glGetUniformLocation(shaderProgram, "mvp");
}

void printShaderErrorLog(GLuint shaderIndex)
{
    int maxLength = 2048;
    int actualLength = 0;
    char log[2048];

    glGetShaderInfoLog(shaderIndex, maxLength, &actualLength, log);

    printf("shader info log for GL index %i:\n%s\n", shaderIndex, log);
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
        fprintf(stderr, "ERROR: GL shader index %i did not compile\n", shader);
        printShaderErrorLog(shader);

        exit(4);
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
    double current, elapsed;

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glMatrixMode(GL_PROJECTION);
    glUseProgram(shaderProgram);

    while (!glfwWindowShouldClose(window.window)) {
        current = getCurrentTime();
        elapsed = current - previous;
        previous = current;
        lag += elapsed;

        // processInput()

        while (lag >= msPerUpdate) {
            tick();
            lag -= msPerUpdate;
        }

        render();

        glfwSwapBuffers(window.window);
        glfwPollEvents();

        if (glfwGetKey(window.window, GLFW_KEY_ESCAPE)) {
            glfwSetWindowShouldClose(window.window, 1);
        }
    }

    glfwTerminate();
}

void render(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawCube();
}

void tick(void)
{
    float mvMatrix[16];
    matrixMultiply(mvMatrix, mvp.view, mvp.model);

    float mvpMatrix[16];
    matrixMultiply(mvpMatrix, mvp.projection, mvMatrix);

    glUniformMatrix4fv(matrixLocation, 1, GL_FALSE, mvpMatrix);
}

void cursorPositionCallback(GLFWwindow* window, double x, double y)
{
    float xT = (float) x - cursorPosition.x;
    float yT = (float) y - cursorPosition.y;

    cursorPosition.x = (float) x;
    cursorPosition.y = (float) y;
}

void initCube(void)
{
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLfloat vertices[] = {
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,

        1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,

        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,

        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,

        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,

        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,

        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
    };

    glGenBuffers(1, &cube.vertices);
    glBindBuffer(GL_ARRAY_BUFFER, cube.vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLfloat colors[] = {
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,

        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,

        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,

        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f,

        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,

        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,

        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,

        0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 1.0f,

        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
    };

    glGenBuffers(1, &cube.colors);
    glBindBuffer(GL_ARRAY_BUFFER, cube.colors);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
}

void drawCube(void)
{
    glBindBuffer(GL_ARRAY_BUFFER, cube.vertices);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, cube.colors);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLES, 0, 3 * 6 * 2);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void lookAt(float* lookAt, vector_t position, vector_t target)
{
    vector_t cameraDirection, cameraRight;
    vector_t up = {0.0f, 1.0f, 0.0f};

    cameraDirection = vectorSubtract(position, target);
    vectorNormalize(&cameraDirection);

    cameraRight = vectorCross(up, cameraDirection);
    vectorNormalize(&cameraRight);

    float m1[16] = {
        cameraRight.x, up.x, cameraDirection.x, 0.0f,
        cameraRight.y, up.y, cameraDirection.y, 0.0f,
        cameraRight.z, up.z, cameraDirection.z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };

    float m2[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        -1 * position.x, -1 * position.y, -1 * position.z, 1.0f,
    };

    matrixMultiply(lookAt, m1, m2);
}