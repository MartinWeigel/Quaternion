// Copyright (C) 2018 Martin Weigel <mail@MartinWeigel.com>
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

// Version: 2018-11-11

#include "Quaternion.h"
#include <stdlib.h>
#include <assert.h>
#include <math.h>

void Quaternion_set(double w, double v1, double v2, double v3, Quaternion* output)
{
    assert(output != NULL);
    output->w = w;
    output->v[0] = v1;
    output->v[1] = v2;
    output->v[2] = v3;
}

void Quaternion_setIdentity(Quaternion* q)
{
    assert(q != NULL);
    Quaternion_set(1, 0, 0, 0, q);
}

void Quaternion_copy(Quaternion* q, Quaternion* output)
{
    Quaternion_set(q->w, q->v[0], q->v[1], q->v[2], output);
}

bool Quaternion_equal(Quaternion* q1, Quaternion* q2)
{
    bool equalW  = fabs(q1->w - q2->w) <= QUATERNION_EPS;
    bool equalV0 = fabs(q1->v[0] - q2->v[0]) <= QUATERNION_EPS;
    bool equalV1 = fabs(q1->v[1] - q2->v[1]) <= QUATERNION_EPS;
    bool equalV2 = fabs(q1->v[2] - q2->v[2]) <= QUATERNION_EPS;
    return equalW && equalV0 && equalV1 && equalV2;
}

void Quaternion_fprint(FILE* file, Quaternion* q)
{
    fprintf(file, "Quaternion(%.2f, %.2f, %.2f, %.2f)",
        q->w, q->v[0], q->v[1], q->v[2]);
}

void Quaternion_conjugate(Quaternion* q, Quaternion* output)
{
    assert(output != NULL);
    output->w = q->w;
    output->v[0] = -q->v[0];
    output->v[1] = -q->v[1];
    output->v[2] = -q->v[2];
}

double Quaternion_norm(Quaternion* q)
{ 
    assert(q != NULL);
    return sqrt(q->w*q->w + q->v[0]*q->v[0] + q->v[1]*q->v[1] + q->v[2]*q->v[2]);
}


void Quaternion_fromAxisAngle(double axis[3], double angle, Quaternion* output)
{
    assert(output != NULL);
    // Formula from http://www.euclideanspace.com/maths/geometry/rotations/conversions/angleToQuaternion/
    output->w = cos(angle / 2.0);
    double c = sin(angle / 2.0);
    output->v[0] = c * axis[0];
    output->v[1] = c * axis[1];
    output->v[2] = c * axis[2];    
}

void Quaternion_fromXRotation(double angle, Quaternion* output)
{
    assert(output != NULL);
    double axis[3] = {1.0, 0, 0};
    Quaternion_fromAxisAngle(axis, angle, output);
}

void Quaternion_fromYRotation(double angle, Quaternion* output)
{
    assert(output != NULL);
    double axis[3] = {0, 1.0, 0};
    Quaternion_fromAxisAngle(axis, angle, output);
}

void Quaternion_fromZRotation(double angle, Quaternion* output)
{
    assert(output != NULL);
    double axis[3] = {0, 0, 1.0};
    Quaternion_fromAxisAngle(axis, angle, output);
}

double Quaternion_toAxisAngle(Quaternion* q, double output[3])
{
    assert(output != NULL);
    // Formula from http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToAngle/
    double angle = 2.0 * acos(q->w);
    double divider = sqrt(1.0 - q->w * q->w);

    if(divider != 0.0) {
        // Calculate the axis
        output[0] = q->v[0] / divider;
        output[1] = q->v[1] / divider;
        output[2] = q->v[2] / divider;        
    } else {
        // Arbitrary normalized axis
        output[0] = 1;
        output[1] = 0;
        output[2] = 0;
    }
    return angle;
}

void Quaternion_multiply(Quaternion* q1, Quaternion* q2, Quaternion* output)
{
    assert(output != NULL);
    /*
    Formula from http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/arithmetic/index.htm
             a*e - b*f - c*g - d*h
        + i (b*e + a*f + c*h- d*g)
        + j (a*g - b*h + c*e + d*f)
        + k (a*h + b*g - c*f + d*e)
    */
    output->w =    q1->w   *q2->w    - q1->v[0]*q2->v[0] - q1->v[1]*q2->v[1] - q1->v[2]*q2->v[2];
    output->v[0] = q1->v[0]*q2->w    + q1->w   *q2->v[0] + q1->v[1]*q2->v[2] - q1->v[2]*q2->v[1];
    output->v[1] = q1->w   *q2->v[1] - q1->v[0]*q2->v[2] + q1->v[1]*q2->w    + q1->v[2]*q2->v[0];
    output->v[2] = q1->w   *q2->v[2] + q1->v[0]*q2->v[1] - q1->v[1]*q2->v[0] + q1->v[2]*q2->w   ;
}

void Quaternion_rotate(Quaternion* q, double v[3], double output[3])
{
    assert(output != NULL);

    double ww = q->w * q->w;
    double xx = q->v[0] * q->v[0];
    double yy = q->v[1] * q->v[1];
    double zz = q->v[2] * q->v[2];
    double wx = q->w * q->v[0];
    double wy = q->w * q->v[1];
    double wz = q->w * q->v[2];
    double xy = q->v[0] * q->v[1];
    double xz = q->v[0] * q->v[2];
    double yz = q->v[1] * q->v[2];

    // Formula from http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/transforms/index.htm
    // p2.x = w*w*p1.x + 2*y*w*p1.z - 2*z*w*p1.y + x*x*p1.x + 2*y*x*p1.y + 2*z*x*p1.z - z*z*p1.x - y*y*p1.x;
    // p2.y = 2*x*y*p1.x + y*y*p1.y + 2*z*y*p1.z + 2*w*z*p1.x - z*z*p1.y + w*w*p1.y - 2*x*w*p1.z - x*x*p1.y;
    // p2.z = 2*x*z*p1.x + 2*y*z*p1.y + z*z*p1.z - 2*w*y*p1.x - y*y*p1.z + 2*w*x*p1.y - x*x*p1.z + w*w*p1.z;

    output[0] = ww*v[0] + 2*wy*v[2] - 2*wz*v[1] +
                xx*v[0] + 2*xy*v[1] + 2*xz*v[2] -
                zz*v[0] - yy*v[0];
    output[1] = 2*xy*v[0] + yy*v[1] + 2*yz*v[2] +
                2*wz*v[0] - zz*v[1] + ww*v[1] -
                2*wx*v[2] - xx*v[1];
    output[2] = 2*xz*v[0] + 2*yz*v[1] + zz*v[2] -
                2*wy*v[0] - yy*v[2] + 2*wx*v[1] -
                xx*v[2] + ww*v[2];
}

void Quaternion_slerp(Quaternion* q1, Quaternion* q2, double t, Quaternion* output)
{
    // Based on http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/index.htm
    double cosHalfTheta = q1->w*q2->w + q1->v[0]*q2->v[0] + q1->v[1]*q2->v[1] + q1->v[2]*q2->v[2];

    // if q1=q2 or qa=-q2 then theta = 0 and we can return qa
    if (abs(cosHalfTheta) >= 1.0) {
        Quaternion_copy(q1, output);
        return;
    }

    double halfTheta = acos(cosHalfTheta);
    double sinHalfTheta = sqrt(1.0 - cosHalfTheta*cosHalfTheta);
    // If theta = 180 degrees then result is not fully defined
    // We could rotate around any axis normal to q1 or q2
    if (fabs(sinHalfTheta) < QUATERNION_EPS) {
        output->w = (q1->w * 0.5 + q2->w * 0.5);
        output->v[0] = (q1->v[0] * 0.5 + q2->v[0] * 0.5);
        output->v[1] = (q1->v[1] * 0.5 + q2->v[1] * 0.5);
        output->v[2] = (q1->v[2] * 0.5 + q2->v[2] * 0.5);
    }

    // Calculate Quaternion
    double ratioA = sin((1 - t) * halfTheta) / sinHalfTheta;
    double ratioB = sin(t * halfTheta) / sinHalfTheta; 
    output->w = (q1->w * ratioA + q2->w * ratioB);
    output->v[0] = (q1->v[0] * ratioA + q2->v[0] * ratioB);
    output->v[1] = (q1->v[1] * ratioA + q2->v[1] * ratioB);
    output->v[2] = (q1->v[2] * ratioA + q2->v[2] * ratioB);
}