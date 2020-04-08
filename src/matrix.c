#include <math.h>
#include <stdio.h>

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
    ymax = znear * tanf(fov * 3.1415926535 / 360.0);
    xmax = ymax * aspect;
    matrixFrustum(matrix, -xmax, xmax, -ymax, ymax, znear, zfar);
}