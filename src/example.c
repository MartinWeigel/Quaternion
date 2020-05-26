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

// Version: 2018-12-22
// Usage:   gcc -Wall -Wextra .\example.c .\Quaternion.c -o example.exe; .\example.exe

#include <stdlib.h>
#include "../include/example.h"

#define TO_RAD(x) (x / 180.0 * M_PI)

void printState(double* position, Quaternion* orientation)
{
    // Output the curre
    printf("Character position:    (% 08.3f, % 08.3f, % 08.3f)\n",
        position[0], position[1], position[2]);
    printf("Character orientation: ");
    Quaternion_fprint(stdout, orientation);
    printf("\n");
}


void simplePrintState(double* position, Quaternion* orientation)
{
    // Output the curre
    Quaternion_fprint(stdout, orientation);
    printf("(% 08.3f, % 08.3f, % 08.3f)",
        position[0], position[1], position[2]);
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
    double angle = TO_RAD(90.0);             // Rotation angle in radians
    Quaternion_fromZRotation(angle, &rotateLeft);   // Set rotateLeft to represent the Z-rotation
    Quaternion_fprint(stdout, &rotateLeft); printf("\n");

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

    printf("--------------------------------------------------------------------------------\n");
    float rotationAngleDegree = 30.0;
    printf("Quaternion_fromZRotation % 08.3f degree on z axis EXAMPLE\n", rotationAngleDegree);
    {
        Quaternion rotated_orientation;
        Quaternion_setIdentity(&rotated_orientation);   // The identity quaternion represents no rotation

        double rotationAngle = rotationAngleDegree / 180.0 * M_PI;             // Rotation angle in radians
        Quaternion_fromZRotation(rotationAngle, &rotated_orientation);   // Set rotateLeft to represent the Z-rotation
        Quaternion_fprint(stdout, &rotated_orientation); printf("\n");

        double axisAngle[3] = {0, 0, 0};
        double angle = Quaternion_toAxisAngle(&rotated_orientation, axisAngle);
        printf("Axis angle:   % 08.3f (% 08.3f, % 08.3f, % 08.3f)\n", angle,
        axisAngle[0], axisAngle[1], axisAngle[2]);

        double position_rot[3] = {10, 0, 0};
        // Rotations
        for (size_t i = 0; i < (size_t)(180.0 / rotationAngleDegree * 2.0); i++)
        {
            Quaternion_rotate(&rotated_orientation, position_rot, position_rot);   // Calculate move in character coordinate system

            printf("\t%lu rotation\n\t\t", i);
            simplePrintState(position_rot, &rotated_orientation);
        }
    }

    printf("--------------------------------------------------------------------------------\n");
    {
        double roll_deg =  0.0;
        double pitch_deg =  45.0;
        double yaw_deg =  45.0;

        printf("Quaternion_fromEulerZYX r: % 08.3f°  p: % 08.3f° y: % 08.3f°:\n\t", roll_deg, pitch_deg, yaw_deg);
        double eulerAngles[3] = {TO_RAD(roll_deg), TO_RAD(pitch_deg), TO_RAD(yaw_deg)};
        Quaternion rotated_orientation;
        Quaternion_fromEulerZYX(eulerAngles, &rotated_orientation);
        printf("Quaternion_fromEulerZYX\t\t"); Quaternion_fprint(stdout, &rotated_orientation); printf("\n\t");

        double axisAngle[3] = {0, 0, 0};
        double angle = Quaternion_toAxisAngle(&rotated_orientation, axisAngle);
        printf("Axis angle:\t\t% 08.3f (% 08.3f, % 08.3f, % 08.3f)\n", angle,
        axisAngle[0], axisAngle[1], axisAngle[2]);

        double position_rot[3] = {10, 10, 10};
        // Rotations
        for (size_t i = 0; i < (size_t)(180.0 / yaw_deg * 2.0); i++)
        {
            Quaternion_rotate(&rotated_orientation, position_rot, position_rot);   // Calculate move in character coordinate system

            printf("\t%lu rotation\n\t\t", i);
            simplePrintState(position_rot, &rotated_orientation);
        }
    }

    printf("--------------------------------------------------------------------------------\n");
    {
        double roll_deg =  45.0;
        double pitch_deg =  45.0;
        double yaw_deg =  45.0;

        printf("Quaternion_fromEulerZYX r: % 08.3f°  p: % 08.3f° y: % 08.3f°:\n\t", roll_deg, pitch_deg, yaw_deg);
        double eulerAngles[3] = {TO_RAD(roll_deg), TO_RAD(pitch_deg), TO_RAD(yaw_deg)};
        Quaternion rotated_orientation;
        Quaternion_fromEulerZYX(eulerAngles, &rotated_orientation);
        printf("Quaternion_fromEulerZYX\t\t"); Quaternion_fprint(stdout, &rotated_orientation); printf("\n\t");

        double axisAngle[3] = {0, 0, 0};
        double angle = Quaternion_toAxisAngle(&rotated_orientation, axisAngle);
        printf("Axis angle:\t\t% 08.3f (% 08.3f, % 08.3f, % 08.3f)\n", angle,
        axisAngle[0], axisAngle[1], axisAngle[2]);

        double position_rot[3] = {10, 10, 10};
        // Rotations
        for (size_t i = 0; i < (size_t)(180.0 / yaw_deg * 2.0); i++)
        {
            Quaternion_rotate(&rotated_orientation, position_rot, position_rot);   // Calculate move in character coordinate system

            printf("\t%lu rotation\n\t\t", i);
            simplePrintState(position_rot, &rotated_orientation);
        }
    }

    printf("--------------------------------------------------------------------------------\n");
    printf("Hemisphere sensor layout EXAMPLE\n");
    {   
        const int numberElements = 13;
        Quaternion hemispher_sensors[numberElements];
        double angles[numberElements][3];

        for (size_t i = 0; i < sizeof(hemispher_sensors)/sizeof(hemispher_sensors[0]); i++)
        {
            Quaternion_setIdentity(&hemispher_sensors[i]);
        }
        
        size_t idx = 0;
        for (size_t i = 0; i < 2; i++)
        {
            for (size_t j = 0; j < 8; j++)
            {
                if (i> 0 && j % 2 == 0)
                {
                    continue;
                }

                double roll =  0.0;
                double pitch =  45.0 * i;
                double yaw =  45.0 * j;

                angles[idx][0] = roll;
                angles[idx][1] = pitch;
                angles[idx][2] = yaw;
                
                {
                    double eulerAngles[3] = {TO_RAD(roll), TO_RAD(pitch), TO_RAD(yaw)};
                    Quaternion rotated_orientation;
                    Quaternion_fromEulerZYX(eulerAngles, &rotated_orientation);
                    
                    hemispher_sensors[idx].v[0] = rotated_orientation.v[0];
                    hemispher_sensors[idx].v[1] = rotated_orientation.v[1];
                    hemispher_sensors[idx].v[2] = rotated_orientation.v[2];
                    hemispher_sensors[idx].w = rotated_orientation.w;
                    idx = idx +1;
                }
            }
        }

        double roll =  0.0;
        double pitch =  90.0;
        double yaw =  90.0;

        angles[idx][0] = roll;
        angles[idx][1] = pitch;
        angles[idx][2] = yaw;

        {
            double eulerAngles[3] = {TO_RAD(roll), TO_RAD(pitch), TO_RAD(yaw)};
            Quaternion rotated_orientation;
            Quaternion_fromEulerZYX(eulerAngles, &rotated_orientation);
            hemispher_sensors[idx].v[0] = rotated_orientation.v[0];
            hemispher_sensors[idx].v[1] = rotated_orientation.v[1];
            hemispher_sensors[idx].v[2] = rotated_orientation.v[2];
            hemispher_sensors[idx].w = rotated_orientation.w;
            idx = idx + 1;
        }

        double position_rot[3] = {1.0, 0, 0};
        printf(" Roll(X)    Pitch(Y)   Yaw(Z)\t\tW\tX\tY\tZ\t\tX\tY\tZ\n");
        for (size_t i = 0; i < idx; i++)
        {
            printf("% 08.3f°, % 08.3f°, % 08.3f°\t", angles[i][0], angles[i][1], angles[i][2]);
            simplePrintState(position_rot, &hemispher_sensors[i]);
        }
    }

    printf("================================================================================\n");
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
