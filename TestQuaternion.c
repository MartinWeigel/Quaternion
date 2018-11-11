// TEST: gcc -Wall -Wextra .\TestQuaternion.c .\Quaternion.c -o TestQuaternion.exe; .\TestQuaternion.exe
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "Quaternion.h"

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

void testQuaternion_set()
{
    Quaternion q;
    Quaternion_set(5.1, 4.2, 3.3, 2.4, &q);
    ASSERT_SAME_DOUBLE("Quaternion_set should set w", q.w, 5.1);
    ASSERT_SAME_DOUBLE("Quaternion_set should set v[0]", q.v[0], 4.2);
    ASSERT_SAME_DOUBLE("Quaternion_set should set v[1]", q.v[1], 3.3);
    ASSERT_SAME_DOUBLE("Quaternion_set should set v[2]", q.v[2], 2.4);
}

void testQuaternion_conjugate()
{
    Quaternion q, c;
    Quaternion_set(5.1, 4.2, 3.3, 2.4, &q);
    Quaternion_conjugate(&q, &c);
    ASSERT_SAME_DOUBLE("Quaternion_conjugate should keep w", c.w, q.w);
    ASSERT_SAME_DOUBLE("Quaternion_conjugate should negate v[0]",c.v[0], -q.v[0]);
    ASSERT_SAME_DOUBLE("Quaternion_conjugate should negate v[1]",c.v[1], -q.v[1]);
    ASSERT_SAME_DOUBLE("Quaternion_conjugate should negate v[2]",c.v[2], -q.v[2]);
}

void testQuaternion_equal()
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


void testQuaternion_norm()
{
    Quaternion q, c;
    Quaternion_set(0.7071, 0.7071, 0, 0, &q);
    Quaternion_setIdentity(&c);
    ASSERT_SAME_DOUBLE("Quaternion_norm of valid q", Quaternion_norm(&q), 1);
    ASSERT_SAME_DOUBLE("Quaternion_norm of identity", Quaternion_norm(&c), 1);
}

void testQuaternion_setIdentity()
{
    Quaternion q;
    Quaternion_set(5.1, 4.2, 3.3, 2.4, &q);
    Quaternion_setIdentity(&q);
    ASSERT_SAME_DOUBLE("Quaternion_setIdentity has wrong w", q.w, 1);
    ASSERT_SAME_DOUBLE("Quaternion_setIdentity has wrong v[0]", q.v[0], 0);
    ASSERT_SAME_DOUBLE("Quaternion_setIdentity has wrong v[1]", q.v[1], 0);
    ASSERT_SAME_DOUBLE("Quaternion_setIdentity has wrong v[2]", q.v[2], 0);
}

void testQuaternion_fromAxisAngle()
{
    Quaternion q;
    double vector[3] = {1, 0, 0};
    double angle = 90.0 / 180.0 * M_PI;
    Quaternion_fromAxisAngle(vector, angle, &q);
    ASSERT_SAME_DOUBLE("Quaternion_fromAxisAngle has wrong w", q.w, 0.7071);
    ASSERT_SAME_DOUBLE("Quaternion_fromAxisAngle has wrong v[0]", q.v[0], 0.7071);
    ASSERT_SAME_DOUBLE("Quaternion_fromAxisAngle has wrong v[1]", q.v[1], 0);
    ASSERT_SAME_DOUBLE("Quaternion_fromAxisAngle has wrong v[2]", q.v[2], 0);
}

void testQuaternion_fromXRotation()
{
    Quaternion q;
    double vector[3] = {1, 0, 0};
    double angle = 90.0 / 180.0 * M_PI;
    Quaternion_fromAxisAngle(vector, angle, &q);
    ASSERT_SAME_DOUBLE("Quaternion_fromXRotation has wrong w", q.w, 0.7071);
    ASSERT_SAME_DOUBLE("Quaternion_fromXRotation has wrong v[0]", q.v[0], 0.7071);
    ASSERT_SAME_DOUBLE("Quaternion_fromXRotation has wrong v[1]", q.v[1], 0);
    ASSERT_SAME_DOUBLE("Quaternion_fromXRotation has wrong v[2]", q.v[2], 0);
}

void testQuaternion_fromYRotation()
{
    Quaternion q;
    double vector[3] = {0, 1, 0};
    double angle = 90.0 / 180.0 * M_PI;
    Quaternion_fromAxisAngle(vector, angle, &q);
    ASSERT_SAME_DOUBLE("Quaternion_fromYRotation has wrong w", q.w, 0.7071);
    ASSERT_SAME_DOUBLE("Quaternion_fromYRotation has wrong v[0]", q.v[0], 0);
    ASSERT_SAME_DOUBLE("Quaternion_fromYRotation has wrong v[1]", q.v[1], 0.7071);
    ASSERT_SAME_DOUBLE("Quaternion_fromYRotation has wrong v[2]", q.v[2], 0);
}

void testQuaternion_fromZRotation()
{
    Quaternion q;
    double vector[3] = {0, 0, 1};
    double angle = 90.0 / 180.0 * M_PI;
    Quaternion_fromAxisAngle(vector, angle, &q);
    ASSERT_SAME_DOUBLE("Quaternion_fromZRotation has wrong w", q.w, 0.7071);
    ASSERT_SAME_DOUBLE("Quaternion_fromZRotation has wrong v[0]", q.v[0], 0);
    ASSERT_SAME_DOUBLE("Quaternion_fromZRotation has wrong v[1]", q.v[1], 0);
    ASSERT_SAME_DOUBLE("Quaternion_fromZRotation has wrong v[2]", q.v[2], 0.7071);
}

void testQuaternion_toAxisAngle()
{
    double v90[3];
    Quaternion rot90;
    Quaternion_set(0.7071, 0.7071, 0, 0, &rot90);
    double a90 = Quaternion_toAxisAngle(&rot90, v90);
    ASSERT_SAME_DOUBLE("Quaternion_toAxisAngle should calculate 90 deg angle", a90, 90.0 / 180.0 * M_PI);
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

void testQuaternion_multiply()
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

int main()
{
    testQuaternion_set();
    testQuaternion_setIdentity();
    testQuaternion_equal();
    testQuaternion_conjugate();
    testQuaternion_norm();
    testQuaternion_fromAxisAngle();
    testQuaternion_fromXRotation();
    testQuaternion_fromYRotation();
    testQuaternion_fromZRotation();
    testQuaternion_toAxisAngle();
    testQuaternion_multiply();
}