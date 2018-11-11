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
    // Formulas from http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToAngle/
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