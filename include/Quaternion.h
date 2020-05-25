// Copyright (C) 2019 Martin Weigel <mail@MartinWeigel.com>
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

/**
 * @file    Quaternion.h
 * @brief   A basic quaternion library written in C
 * @date    2019-11-28
 */
#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

/**
 * Maximum floating point difference that is considered as equal.
 */
#define QUATERNION_EPS (1e-4)

/**
 * Data structure to hold a quaternion.
 */
typedef struct Quaternion {
    double w;       /**< Scalar part */
    double v[3];    /**< Vector part */
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
 * Copies one quaternion to another.
 */
void Quaternion_copy(Quaternion* q, Quaternion* output);

/**
 * Tests if all quaternion values are equal (using QUATERNION_EPS).
 */
bool Quaternion_equal(Quaternion* q1, Quaternion* q2);

/**
 * Print the quaternion to a given file (e.g., stderr).
 */
void Quaternion_fprint(FILE* file, Quaternion* q);

/**
 * Set the quaternion to the equivalent of axis-angle rotation.
 * @param axis
 *      The axis of the rotation (should be normalized).
 * @param angle
 *      Rotation angle in radians.
 */
void Quaternion_fromAxisAngle(double axis[3], double angle, Quaternion* output);

/**
 * Calculates the rotation vector and angle of a quaternion.
 * @param output
 *      A 3D vector of the quaternion rotation axis.
 * @return
 *      The rotation angle in radians.
 */
double Quaternion_toAxisAngle(Quaternion* q, double output[3]);

/**
 * Set the quaternion to the equivalent of euler angles.
 * @param eulerZYX
 *      Euler angles in ZYX, but stored in array as [x'', y', z].
 */
void Quaternion_fromEulerZYX(double eulerZYX[3], Quaternion* output);

/**
 * Calculates the euler angles of a quaternion.
 * @param output
 *      Euler angles in ZYX, but stored in array as [x'', y', z].
 */
void Quaternion_toEulerZYX(Quaternion* q, double output[3]);

/**
 * Set the quaternion to the equivalent a rotation around the X-axis.
 * @param angle
 *      Rotation angle in radians.
 */
void Quaternion_fromXRotation(double angle, Quaternion* output);

/**
 * Set the quaternion to the equivalent a rotation around the Y-axis.
 * @param angle
 *      Rotation angle in radians.
 */
void Quaternion_fromYRotation(double angle, Quaternion* output);

/**
 * Set the quaternion to the equivalent a rotation around the Z-axis.
 * @param angle
 *      Rotation angle in radians.
 */
void Quaternion_fromZRotation(double angle, Quaternion* output);

/**
 * Calculates the norm of a given quaternion:
 * norm = sqrt(w*w + v1*v1 + v2*v2 + v3*v3)
 */
double Quaternion_norm(Quaternion* q);

/**
 * Normalizes the quaternion.
 */
void Quaternion_normalize(Quaternion* q, Quaternion* output);

/**
 * Calculates the conjugate of the quaternion: (w, -v)
 */
void Quaternion_conjugate(Quaternion* q, Quaternion* output);

/**
 * Multiplies two quaternions: output = q1 * q2
 * @param q1
 *      The rotation to apply on q2.
 * @param q2
 *      The orientation to be rotated.
 */
void Quaternion_multiply(Quaternion* q1, Quaternion* q2, Quaternion* output);

/**
 * Applies quaternion rotation to a given vector.
 */
void Quaternion_rotate(Quaternion* q, double v[3], double output[3]);

/**
 * Interpolates between two quaternions.
 * @param t
 *      Interpolation between the two quaternions [0, 1].
 *      0 is equal with q1, 1 is equal with q2, 0.5 is the middle between q1 and q2.
 */
void Quaternion_slerp(Quaternion* q1, Quaternion* q2, double t, Quaternion* output);
