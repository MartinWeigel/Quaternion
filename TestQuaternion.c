// TEST: gcc -std=c17 -Wall -Wextra TestQuaternion.c Quaternion.c -o TestQuaternion.exe; ./TestQuaternion.exe
#include <stdlib.h>
#include "Quaternion.h"

#ifndef M_PI
    #define M_PI (3.14159265358979323846)
#endif
#define TO_RAD(x) (x / 180.0 * M_PI)

void ASSERT_SAME_DOUBLE(char* description, double x, double y)
{
    if(fabs(x - y) > QUATERNION_EPS) {
        fprintf(stderr, "TEST FAILED: %s (%f != %f)\n", description, x, y);
    }
}
void ASSERT_TRUE(char* description, bool check)
{
    if(!check) {
        fprintf(stderr, "TEST FAILED: %s\n", description);
    }
}
void ASSERT_FALSE(char* description, bool check) { ASSERT_TRUE(description, !check); }

void testQuaternion_set(void)
{
    Quaternion q;
    Quaternion_set(5.1, 4.2, 3.3, 2.4, &q);
    ASSERT_SAME_DOUBLE("Quaternion_set should set w", q.w, 5.1);
    ASSERT_SAME_DOUBLE("Quaternion_set should set v[0]", q.v[0], 4.2);
    ASSERT_SAME_DOUBLE("Quaternion_set should set v[1]", q.v[1], 3.3);
    ASSERT_SAME_DOUBLE("Quaternion_set should set v[2]", q.v[2], 2.4);
}

void testQuaternion_setIdentity(void)
{
    Quaternion q;
    Quaternion_set(5.1, 4.2, 3.3, 2.4, &q);
    Quaternion_setIdentity(&q);
    ASSERT_SAME_DOUBLE("Quaternion_setIdentity has wrong w", q.w, 1);
    ASSERT_SAME_DOUBLE("Quaternion_setIdentity has wrong v[0]", q.v[0], 0);
    ASSERT_SAME_DOUBLE("Quaternion_setIdentity has wrong v[1]", q.v[1], 0);
    ASSERT_SAME_DOUBLE("Quaternion_setIdentity has wrong v[2]", q.v[2], 0);
}

void testQuaternion_copy(void)
{
    Quaternion q, r;
    Quaternion_set(5.1, 4.2, 3.3, 2.4, &q);
    Quaternion_copy(&q, &r);
    ASSERT_SAME_DOUBLE("Quaternion_copy has wrong w", r.w, q.w);
    ASSERT_SAME_DOUBLE("Quaternion_copy has wrong v[0]", r.v[0], q.v[0]);
    ASSERT_SAME_DOUBLE("Quaternion_copy has wrong v[1]", r.v[1], q.v[1]);
    ASSERT_SAME_DOUBLE("Quaternion_copy has wrong v[2]", r.v[2], q.v[2]);
}

void testQuaternion_conjugate(void)
{
    Quaternion q, c;
    Quaternion_set(5.1, 4.2, 3.3, 2.4, &q);
    Quaternion_conjugate(&q, &c);
    ASSERT_SAME_DOUBLE("Quaternion_conjugate should keep w", c.w, q.w);
    ASSERT_SAME_DOUBLE("Quaternion_conjugate should negate v[0]",c.v[0], -q.v[0]);
    ASSERT_SAME_DOUBLE("Quaternion_conjugate should negate v[1]",c.v[1], -q.v[1]);
    ASSERT_SAME_DOUBLE("Quaternion_conjugate should negate v[2]",c.v[2], -q.v[2]);
}

void testQuaternion_equal(void)
{
    Quaternion q1, q2, q3, q4, q5;
    Quaternion_set(5.1, 4.2, 3.3, 2.4, &q1);
    Quaternion_set(5.1, 4.2, 3.3, 2.4, &q2);
    Quaternion_set(5.1, 4.2 + (QUATERNION_EPS/2.0), 3.3, 2.4, &q3);
    Quaternion_set(9.1, 4.2, 3.3, 2.4, &q4);
    Quaternion_set(5.1, 4.2, 9.3, 2.4, &q5);
    ASSERT_TRUE("Quaternion_equal with same reference", Quaternion_equal(&q1, &q1));
    ASSERT_TRUE("Quaternion_equal with same values", Quaternion_equal(&q1, &q2));
    ASSERT_TRUE("Quaternion_equal with similar values", Quaternion_equal(&q1, &q3));
    ASSERT_FALSE("Quaternion_equal with different w", Quaternion_equal(&q1, &q4));
    ASSERT_FALSE("Quaternion_equal with different v", Quaternion_equal(&q1, &q5));
}

void testQuaternion_norm(void)
{
    Quaternion q, c;
    Quaternion_set(0.7071, 0.7071, 0, 0, &q);
    Quaternion_setIdentity(&c);
    ASSERT_SAME_DOUBLE("Quaternion_norm of valid q == 1", Quaternion_norm(&q), 1);
    ASSERT_SAME_DOUBLE("Quaternion_norm of identity", Quaternion_norm(&c), 1);

    Quaternion_set(4, 5, 6, 7, &q);
    ASSERT_SAME_DOUBLE("Quaternion_norm larger 1", Quaternion_norm(&q), sqrt(126));
}

void testQuaternion_normalize(void)
{
    Quaternion q, c;
    Quaternion_set(0.7071, 0.7071, 0, 0, &q);
    Quaternion_normalize(&q, &c);
    ASSERT_SAME_DOUBLE("Quaternion_normalize should stay 1", Quaternion_norm(&c), 1);

    Quaternion_set(4, 5, 6, 7, &q);
    Quaternion_normalize(&q, &c);
    ASSERT_SAME_DOUBLE("Quaternion_normalize was not 1", Quaternion_norm(&q), sqrt(126));
    ASSERT_SAME_DOUBLE("Quaternion_normalize is 1 afterwards", Quaternion_norm(&c), 1);
}

void testQuaternion_fromAxisAngle(void)
{
    Quaternion q;
    double vector[3] = {1, 0, 0};
    double angle = TO_RAD(90.0);
    Quaternion_fromAxisAngle(vector, angle, &q);
    ASSERT_SAME_DOUBLE("Quaternion_fromAxisAngle has wrong w", q.w, 0.7071);
    ASSERT_SAME_DOUBLE("Quaternion_fromAxisAngle has wrong v[0]", q.v[0], 0.7071);
    ASSERT_SAME_DOUBLE("Quaternion_fromAxisAngle has wrong v[1]", q.v[1], 0);
    ASSERT_SAME_DOUBLE("Quaternion_fromAxisAngle has wrong v[2]", q.v[2], 0);
}

void testQuaternion_fromXRotation(void)
{
    Quaternion q;
    double vector[3] = {1, 0, 0};
    double angle = TO_RAD(90.0);
    Quaternion_fromAxisAngle(vector, angle, &q);
    ASSERT_SAME_DOUBLE("Quaternion_fromXRotation has wrong w", q.w, 0.7071);
    ASSERT_SAME_DOUBLE("Quaternion_fromXRotation has wrong v[0]", q.v[0], 0.7071);
    ASSERT_SAME_DOUBLE("Quaternion_fromXRotation has wrong v[1]", q.v[1], 0);
    ASSERT_SAME_DOUBLE("Quaternion_fromXRotation has wrong v[2]", q.v[2], 0);
}

void testQuaternion_fromYRotation(void)
{
    Quaternion q;
    double vector[3] = {0, 1, 0};
    double angle = TO_RAD(90.0);
    Quaternion_fromAxisAngle(vector, angle, &q);
    ASSERT_SAME_DOUBLE("Quaternion_fromYRotation has wrong w", q.w, 0.7071);
    ASSERT_SAME_DOUBLE("Quaternion_fromYRotation has wrong v[0]", q.v[0], 0);
    ASSERT_SAME_DOUBLE("Quaternion_fromYRotation has wrong v[1]", q.v[1], 0.7071);
    ASSERT_SAME_DOUBLE("Quaternion_fromYRotation has wrong v[2]", q.v[2], 0);
}

void testQuaternion_fromZRotation(void)
{
    Quaternion q;
    double vector[3] = {0, 0, 1};
    double angle = TO_RAD(90.0);
    Quaternion_fromAxisAngle(vector, angle, &q);
    ASSERT_SAME_DOUBLE("Quaternion_fromZRotation has wrong w", q.w, 0.7071);
    ASSERT_SAME_DOUBLE("Quaternion_fromZRotation has wrong v[0]", q.v[0], 0);
    ASSERT_SAME_DOUBLE("Quaternion_fromZRotation has wrong v[1]", q.v[1], 0);
    ASSERT_SAME_DOUBLE("Quaternion_fromZRotation has wrong v[2]", q.v[2], 0.7071);
}

void testQuaternion_toAxisAngle(void)
{
    double v90[3];
    Quaternion rot90;
    Quaternion_set(0.7071, 0.7071, 0, 0, &rot90);
    double a90 = Quaternion_toAxisAngle(&rot90, v90);
    ASSERT_SAME_DOUBLE("Quaternion_toAxisAngle should calculate 90 deg angle", a90, TO_RAD(90.0));
    ASSERT_SAME_DOUBLE("Quaternion_toAxisAngle should calculate 90 deg X axis", v90[0], 1);
    ASSERT_SAME_DOUBLE("Quaternion_toAxisAngle should calculate 90 deg Y axis", v90[1], 0);
    ASSERT_SAME_DOUBLE("Quaternion_toAxisAngle should calculate 90 deg Z axis", v90[2], 0);

    double v0[3];
    Quaternion rot0;
    Quaternion_setIdentity(&rot0);
    double a0 = Quaternion_toAxisAngle(&rot0, v0);
    ASSERT_SAME_DOUBLE("Quaternion_toAxisAngle should calculate 0 deg angle", a0, 0);

    double arbAxis[3] = {0.802, 0.267, -0.534};
    double vArb[3];
    Quaternion rotArb;
    Quaternion_fromAxisAngle(arbAxis, 1.11, &rotArb);
    double aArb = Quaternion_toAxisAngle(&rotArb, vArb);
    ASSERT_SAME_DOUBLE("Quaternion_toAxisAngle should inverse fromAxisAngle() (angle)", aArb, 1.11);
    ASSERT_SAME_DOUBLE("Quaternion_toAxisAngle should inverse fromAxisAngle() (X axis)", vArb[0], arbAxis[0]);
    ASSERT_SAME_DOUBLE("Quaternion_toAxisAngle should inverse fromAxisAngle() (Y axis)", vArb[1], arbAxis[1]);
    ASSERT_SAME_DOUBLE("Quaternion_toAxisAngle should inverse fromAxisAngle() (Z axis)", vArb[2], arbAxis[2]);
}

void testQuaternion_fromEulerZYX(void)
{
    // Check test with https://www.andre-gaschler.com/rotationconverter/
    double e0[3] = {TO_RAD(90.0), 0, 0};
    Quaternion q, real;
    Quaternion_set(0.7071, 0.7071, 0, 0, &real);
    Quaternion_fromEulerZYX(e0, &q);
    ASSERT_TRUE("Quaternion_fromEulerZYX for x-rotation only", Quaternion_equal(&q, &real));

    double e1[3] = {TO_RAD(90.0), 0, TO_RAD(180.0)};
    Quaternion_set(0, 0, 0.7071, 0.7071, &real);
    Quaternion_fromEulerZYX(e1, &q);
    ASSERT_TRUE("Quaternion_fromEulerZYX example 1", Quaternion_equal(&q, &real));

    double e2[3] = {TO_RAD(90.0), 0, TO_RAD(90.0)};
    Quaternion_set(0.5, 0.5, 0.5, 0.5, &real);
    Quaternion_fromEulerZYX(e2, &q);
    ASSERT_TRUE("Quaternion_fromEulerZYX example 2", Quaternion_equal(&q, &real));

    double e3[3] = {TO_RAD(165.0), TO_RAD(63.0), TO_RAD(122.0)};
    Quaternion_set(0.5070333, 0.3501829, 0.7724199, -0.1538071, &real);
    Quaternion_fromEulerZYX(e3, &q);
    ASSERT_TRUE("Quaternion_fromEulerZYX example 3", Quaternion_equal(&q, &real));
}

void testQuaternion_toEulerZYX(void)
{
    // Check test with https://www.andre-gaschler.com/rotationconverter/
    double euler[3];
    Quaternion q;
    Quaternion_set(0.7071, 0.7071, 0, 0, &q);
    Quaternion_toEulerZYX(&q, euler);
    ASSERT_SAME_DOUBLE("Quaternion_toEulerZYX x-rotation only (X axis)", euler[0], TO_RAD(90.0));
    ASSERT_SAME_DOUBLE("Quaternion_toEulerZYX x-rotation only (Y axis)", euler[1], 0);
    ASSERT_SAME_DOUBLE("Quaternion_toEulerZYX x-rotation only (Z axis)", euler[2], 0);

    Quaternion_set(0, 0, 0.7071, 0.7071, &q);
    Quaternion_toEulerZYX(&q, euler);
    ASSERT_SAME_DOUBLE("Quaternion_toEulerZYX xz-rotation only (X axis)", euler[0], TO_RAD(90.0));
    ASSERT_SAME_DOUBLE("Quaternion_toEulerZYX xz-rotation only (Y axis)", euler[1], 0);
    ASSERT_SAME_DOUBLE("Quaternion_toEulerZYX xz-rotation only (Z axis)", euler[2], TO_RAD(180.0));

    Quaternion_set(0.5, 0.5, 0.5, 0.5, &q);
    Quaternion_toEulerZYX(&q, euler);
    ASSERT_SAME_DOUBLE("Quaternion_toEulerZYX xy-rotation (X axis)", euler[0], TO_RAD(90.0));
    ASSERT_SAME_DOUBLE("Quaternion_toEulerZYX xy-rotation (Y axis)", euler[1], 0);
    ASSERT_SAME_DOUBLE("Quaternion_toEulerZYX xy-rotation (Z axis)", euler[2], TO_RAD(90.0));

    Quaternion_set(0.5070333, 0.3501829, 0.7724199, -0.1538071, &q);
    Quaternion_toEulerZYX(&q, euler);
    ASSERT_SAME_DOUBLE("Quaternion_toEulerZYX xy-rotation (X axis)", euler[0], TO_RAD(165.0));
    ASSERT_SAME_DOUBLE("Quaternion_toEulerZYX xy-rotation (Y axis)", euler[1], TO_RAD(63.0));
    ASSERT_SAME_DOUBLE("Quaternion_toEulerZYX xy-rotation (Z axis)", euler[2], TO_RAD(122.0));
}

void testQuaternion_multiply(void)
{
    // Examples from https://www.mathworks.com/help/aerotbx/ug/quatmultiply.html
    Quaternion q1, q2, result, real;
    Quaternion_set(1.0, 0.0,  1.0, 0.0,  &q1);
    Quaternion_set(1.0, 0.5,  0.5, 0.75, &q2);
    Quaternion_set(0.5, 1.25, 1.5, 0.25, &real);
    Quaternion_multiply(&q1, &q2, &result);
    ASSERT_TRUE("Quaternion_multiply example 1", Quaternion_equal(&result, &real));

    Quaternion_set(1.0, 0.0,  1.0, 0.0,  &q1);
    Quaternion_set(0, 0, 2, 0, &real);
    Quaternion_multiply(&q1, &q1, &result);
    ASSERT_TRUE("Quaternion_multiply example 2", Quaternion_equal(&result, &real));

    Quaternion_set(1.0, 0.0,  1.0, 0.0,  &q1);
    Quaternion_set(2.0, 1.0, 0.1,  0.1, &q2);
    Quaternion_set(1.9, 1.1, 2.1, -0.9, &real);
    Quaternion_multiply(&q1, &q2, &result);
    ASSERT_TRUE("Quaternion_multiply example 3", Quaternion_equal(&result, &real));
}

void testQuaternion_rotate(void)
{
    double result[3];
    double v1[3] = {5.1, 6.8, -5.3};
    Quaternion identity;
    Quaternion_setIdentity(&identity);
    Quaternion_rotate(&identity, v1, result);
    ASSERT_SAME_DOUBLE("Quaternion_rotate example 1 (X-axis)", result[0], v1[0]);
    ASSERT_SAME_DOUBLE("Quaternion_rotate example 1 (Y-axis)", result[1], v1[1]);
    ASSERT_SAME_DOUBLE("Quaternion_rotate example 1 (Z-axis)", result[2], v1[2]);

    // Example 2 from http://web.cs.iastate.edu/~cs577/handouts/quaternion.pdf
    Quaternion q;
    double v2[3] = {1, 0, 0};
    Quaternion_set(0.5, 0.5, 0.5, 0.5, &q);
    Quaternion_rotate(&q, v2, result);
    ASSERT_SAME_DOUBLE("Quaternion_rotate example 2 (X-axis)", result[0], 0);
    ASSERT_SAME_DOUBLE("Quaternion_rotate example 2 (Y-axis)", result[1], 1);
    ASSERT_SAME_DOUBLE("Quaternion_rotate example 2 (Z-axis)", result[2], 0);

    // Example from https://gamedev.stackexchange.com/questions/119725/why-does-transforming-a-vector3-by-a-quaternion-result-in-reversed-z
    double v3[3] = {1, 0, 0};
    Quaternion_set(0.6532815, -0.270598, 0.270598, 0.6532815, &q);
    Quaternion_rotate(&q, v3, result);
    ASSERT_SAME_DOUBLE("Quaternion_rotate example 3 (X-axis)", result[0], 0);
    ASSERT_SAME_DOUBLE("Quaternion_rotate example 3 (Y-axis)", result[1], +0.7071);
    ASSERT_SAME_DOUBLE("Quaternion_rotate example 3 (Z-axis)", result[2], -0.7071);

    // Example from http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/transforms/index.htm
    double v4[3] = {1, 0, 0};
    Quaternion_set(0.7071, 0, 0, 0.7071, &q);
    Quaternion_rotate(&q, v4, result);
    ASSERT_SAME_DOUBLE("Quaternion_rotate example 4 (X-axis)", result[0], 0);
    ASSERT_SAME_DOUBLE("Quaternion_rotate example 4 (Y-axis)", result[1], 1);
    ASSERT_SAME_DOUBLE("Quaternion_rotate example 4 (Z-axis)", result[2], 0);
}

void testQuaternion_slerp(void)
{
    Quaternion q1, q2, result;
    Quaternion_set(0.6532815, -0.270598, 0.270598, 0.6532815, &q1);
    Quaternion_set(0.5, 0.5, 0.5, 0.5, &q2);

    Quaternion_slerp(&q1, &q2, 0, &result);
    ASSERT_TRUE("Quaternion_slerp with t=0", Quaternion_equal(&result, &q1));

    Quaternion_slerp(&q1, &q2, 1, &result);
    ASSERT_TRUE("Quaternion_slerp with t=1", Quaternion_equal(&result, &q2));

    Quaternion_slerp(&q1, &q2, 0.62, &result);
    ASSERT_SAME_DOUBLE("Quaternion_slerp with t=0.62 (w)", result.w, 0.6119266025696755);
    ASSERT_SAME_DOUBLE("Quaternion_slerp with t=0.62 (v[0])", result.v[0], 0.22069444274723088);
    ASSERT_SAME_DOUBLE("Quaternion_slerp with t=0.62 (v[1])", result.v[1], 0.4498729015909088);
    ASSERT_SAME_DOUBLE("Quaternion_slerp with t=0.62 (v[2])", result.v[2], 0.6119266025696755);
}

int main(void)
{
    testQuaternion_set();
    testQuaternion_setIdentity();
    testQuaternion_copy();
    testQuaternion_equal();
    testQuaternion_conjugate();
    testQuaternion_norm();
    testQuaternion_normalize();
    testQuaternion_fromAxisAngle();
    testQuaternion_fromXRotation();
    testQuaternion_fromYRotation();
    testQuaternion_fromZRotation();
    testQuaternion_toAxisAngle();
    testQuaternion_fromEulerZYX();
    testQuaternion_toEulerZYX();
    testQuaternion_multiply();
    testQuaternion_rotate();
    testQuaternion_slerp();
    return EXIT_SUCCESS;
}