#include <math.h>

typedef struct {
    double x;
    double y;
    double z;
} vec3_t;

double vec3Length(vec3_t v);
double vec3Dot(vec3_t v1, vec3_t v2);
vec3_t vec3Cross(vec3_t v1, vec3_t v2);

double vec3Length(vec3_t v)
{
    return sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2));
}

double vec3Dot(vec3_t v1, vec3_t v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

vec3_t vec3Cross(vec3_t v1, vec3_t v2)
{
    vec3_t v3;

    v3.x = v1.y * v2.z - v1.z * v2.y;
    v3.y = v1.z * v2.x - v1.x * v2.z;
    v3.z = v1.x * v2.y - v1.y * v2.x;

    return v3;
}