#include <stdio.h>
#include <math.h>
#include <assert.h>

#define PI 3.1415926535

typedef struct {
    GLfloat x, y, z;
} vector_t;

GLfloat vectorLength(vector_t v)
{
    // todo: Не стоит извлекать корень из 0?

    return sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2));
}

GLfloat vectorDot(vector_t v1, vector_t v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

vector_t vectorCross(vector_t v1, vector_t v2)
{
    vector_t v3;

    v3.x = v1.y * v2.z - v1.z * v2.y;
    v3.y = v1.z * v2.x - v1.x * v2.z;
    v3.z = v1.x * v2.y - v1.y * v2.x;

    return v3;
}

vector_t vectorSubtract(vector_t v1, vector_t v2)
{
    vector_t v3;

    v3.x = v1.x - v2.x;
    v3.y = v1.y - v2.y;
    v3.z = v1.z - v2.z;

    return v3;
}

void vectorNormalize(vector_t* v)
{
    float length = vectorLength(*v);

    assert(length > 0.0f);

    v->x /= length;
    v->y /= length;
    v->z /= length;
}

float degreesToRadians(float degrees)
{
    return (float) (degrees * (PI / 180.0f));
}

void matrixNormalize(float* x, float* y, float* z)
{
    float d = sqrtf((*x) * (*x) + (*y) * (*y) + (*z) * (*z));

    *x /= d;
    *y /= d;
    *z /= d;
}

void matrixIdentity(float* matrix)
{
    matrix[0] = 1;
    matrix[1] = 0;
    matrix[2] = 0;
    matrix[3] = 0;
    matrix[4] = 0;
    matrix[5] = 1;
    matrix[6] = 0;
    matrix[7] = 0;
    matrix[8] = 0;
    matrix[9] = 0;
    matrix[10] = 1;
    matrix[11] = 0;
    matrix[12] = 0;
    matrix[13] = 0;
    matrix[14] = 0;
    matrix[15] = 1;
}

void matrixTranslate(float* matrix, float dx, float dy, float dz)
{
    matrix[0] = 1;
    matrix[1] = 0;
    matrix[2] = 0;
    matrix[3] = 0;
    matrix[4] = 0;
    matrix[5] = 1;
    matrix[6] = 0;
    matrix[7] = 0;
    matrix[8] = 0;
    matrix[9] = 0;
    matrix[10] = 1;
    matrix[11] = 0;
    matrix[12] = dx;
    matrix[13] = dy;
    matrix[14] = dz;
    matrix[15] = 1;
}

void matrixRotate(float* matrix, float x, float y, float z, float angle)
{
    matrixNormalize(&x, &y, &z);

    float s = sinf(angle);
    float c = cosf(angle);
    float m = 1 - c;

    matrix[0] = m * x * x + c;
    matrix[1] = m * x * y - z * s;
    matrix[2] = m * z * x + y * s;
    matrix[3] = 0;
    matrix[4] = m * x * y + z * s;
    matrix[5] = m * y * y + c;
    matrix[6] = m * y * z - x * s;
    matrix[7] = 0;
    matrix[8] = m * z * x - y * s;
    matrix[9] = m * y * z + x * s;
    matrix[10] = m * z * z + c;
    matrix[11] = 0;
    matrix[12] = 0;
    matrix[13] = 0;
    matrix[14] = 0;
    matrix[15] = 1;
}

void matrixMultiply(float* matrix, float* a, float* b)
{
    float result[16];

    for (int c = 0; c < 4; c++) {
        for (int r = 0; r < 4; r++) {
            int index = c * 4 + r;
            float total = 0;

            for (int i = 0; i < 4; i++) {
                int p = i * 4 + r;
                int q = c * 4 + i;

                total += a[p] * b[q];
            }

            result[index] = total;
        }
    }

    for (int i = 0; i < 16; i++) {
        matrix[i] = result[i];
    }
}

void matrixPrint(float* matrix)
{
    printf(
        "%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n",
        matrix[0],
        matrix[1],
        matrix[2],
        matrix[3],
        matrix[4],
        matrix[5],
        matrix[6],
        matrix[7],
        matrix[8],
        matrix[9],
        matrix[10],
        matrix[11],
        matrix[12],
        matrix[13],
        matrix[14],
        matrix[15]
    );
}

void matrixFrustum(float* matrix, float left, float right, float bottom, float top, float znear, float zfar)
{
    float temp, temp2, temp3, temp4;
    temp = 2.0f * znear;
    temp2 = right - left;
    temp3 = top - bottom;
    temp4 = zfar - znear;
    matrix[0] = temp / temp2;
    matrix[1] = 0.0f;
    matrix[2] = 0.0f;
    matrix[3] = 0.0f;
    matrix[4] = 0.0f;
    matrix[5] = temp / temp3;
    matrix[6] = 0.0f;
    matrix[7] = 0.0f;
    matrix[8] = (right + left) / temp2;
    matrix[9] = (top + bottom) / temp3;
    matrix[10] = (-zfar - znear) / temp4;
    matrix[11] = -1.0f;
    matrix[12] = 0.0f;
    matrix[13] = 0.0f;
    matrix[14] = (-temp * zfar) / temp4;
    matrix[15] = 0.0f;
}

void matrixPerspective(float* matrix, float fov, float aspect, float znear, float zfar)
{
    float ymax, xmax;
    ymax = znear * tanf(fov * PI / 360.0);
    xmax = ymax * aspect;
    matrixFrustum(matrix, -xmax, xmax, -ymax, ymax, znear, zfar);
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