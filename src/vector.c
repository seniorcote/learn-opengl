#include <assert.h>

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