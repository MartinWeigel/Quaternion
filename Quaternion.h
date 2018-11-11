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

#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#define QUATERNION_EPS (1e-4)

typedef struct Quaternion {
    double w;
    double v[3];
} Quaternion;

/**
 * Sets the given values to the output quaternion.
 */
void Quaternion_set(double w, double v1, double v2, double v3, Quaternion* output);

/**
 * Sets quaternion to its identity.
 */
void Quaternion_setIdentity(Quaternion* q);

/**
 * Tests if all quaternion values are equal (using QUATERNION_EPS).
 */
bool Quaternion_equal(Quaternion* q1, Quaternion* q2);

/**
 * Print the quaternion to a given file (e.g. stderr).
 */
void Quaternion_fprint(FILE* file, Quaternion* q);

/**
 * Set the quaternion to the equivalent of axis-angle rotation.
 * @axis
 *      The axis of the rotation (should be normalized).
 * @angle
 *      Rotation angle in radians.
 */
void Quaternion_fromAxisAngle(double axis[3], double angle, Quaternion* output);

/**
 * Set the quaternion to the equivalent a rotation around the X-axis.
 * @angle
 *      Rotation angle in radians.
 */
void Quaternion_fromXRotation(double angle, Quaternion* output);

/**
 * Set the quaternion to the equivalent a rotation around the Y-axis.
 * @angle
 *      Rotation angle in radians.
 */
void Quaternion_fromYRotation(double angle, Quaternion* output);

/**
 * Set the quaternion to the equivalent a rotation around the Z-axis.
 * @angle
 *      Rotation angle in radians.
 */
void Quaternion_fromZRotation(double angle, Quaternion* output);

/**
 * Calculates the norm of a given quaternion:
 * norm = sqrt(w*w + v1*v1 + v2*v2 + v3*v3)
 */
double Quaternion_norm(Quaternion* q);

/**
 * Calculates the conjugate of the quaternion: (w, -v)
 */
void Quaternion_conjugate(Quaternion* q, Quaternion* output);

/**
 * Calculates the rotation vector and angle of a quaternion.
 * @output
 *      A 3D vector of the quaternion rotation axis.
 * @return
 *      The rotation angle in radians.
 */
double Quaternion_toAxisAngle(Quaternion* q, double output[3]);

// TODO: The following functions are planned, but not yet implemented
// double Quaternion_getZA(Quaternion* q);
// double Quaternion_getYA(Quaternion* q);
// double Quaternion_getXA(Quaternion* q);

// void Quaternion_multiply(Quaternion* q1, Quaternion* q2, Quaternion* output);
// void Quaternion_rotate(Quaternion* q, double p[3], double output[3]);

// void Quaternion_slerp(Quaternion* q1, Quaternion* q2, double interpolation, Quaternion* output);
