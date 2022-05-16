// Copyright (C) 2022 Martin Weigel <mail@MartinWeigel.com>
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

// Version: 2022-05-16
// Usage:   gcc -std=c17 -Wall -Wextra example.c Quaternion.c -o example.exe; ./example.exe

#include <stdlib.h>
#include "Quaternion.h"

#ifndef M_PI
    #define M_PI (3.14159265358979323846)
#endif

void printState(double* position, Quaternion* orientation)
{
    // Output the curre
    printf("Character position:    (%.3f, %.3f, %.3f)\n",
        position[0], position[1], position[2]);
    printf("Character orientation: ");
    Quaternion_fprint(stdout, orientation);
    printf("\n");
}

int main(void)
{
    printf("BASIC EXAMPLE\n");
    // Setup start position and orientation
    double position[3] = {0, 0, 0};
    Quaternion orientation;
    Quaternion_setIdentity(&orientation);   // The identity quaternion represents no rotation

    // Rotate character to the left (90 degrees around z)
    Quaternion rotateLeft;
    double angle = 90.0 / 180.0 * M_PI;             // Rotation angle in radians
    Quaternion_fromZRotation(angle, &rotateLeft);   // Set rotateLeft to represent the Z-rotation

    // Apply the rotation, by multiplying the two quaternions
    // IMPORTANT: The rotation needs to be the first argument!
    Quaternion_multiply(&rotateLeft, &orientation, &orientation);

    // Move the character two meters forward
    double move[3] = {2, 0, 0};                    // Move in absolute coordinates
    Quaternion_rotate(&orientation, move, move);   // Calculate move in character coordinate system
    position[0] += move[0];                        // Move the current position
    position[1] += move[1];
    position[2] += move[2];

    // The character position should be: (0, 2, 0)
    printState(position, &orientation);
    printf("\n");


    printf("ADVANCED EXAMPLE\n");
    // Walks half a circle with 10000 steps
    const double STEP_COUNT = 10000;                     // Walk 10000 steps in half circle
    const double STEP_SIZE  = 0.0314;                    // Each step moves 3.14cm
    const double TIME_STEP = 1.0 / STEP_COUNT;

    Quaternion startRotation, endRotation;
    startRotation = orientation;                                  // Start facing left
    Quaternion_multiply(&rotateLeft, &orientation, &orientation);
    Quaternion_multiply(&rotateLeft, &orientation, &endRotation); // End facing right

    double t = 0;
    while(t < 1) {
        Quaternion_slerp(&startRotation, &endRotation, t, &orientation);

        double step[3] = {STEP_SIZE, 0, 0};
        Quaternion_rotate(&orientation, step, step);   // Calculate step in character coordinate system
        position[0] += step[0];                        // Step forward in current orientation
        position[1] += step[1];
        position[2] += step[2];

        t += TIME_STEP;
    }

    printState(position, &orientation);

    return EXIT_SUCCESS;
}
