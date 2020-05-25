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
    double angle = 90.0 / 180.0 * M_PI;             // Rotation angle in radians
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
    printf("\n");

    float rotationAngleDegree = 30.0;
    printf("ROTATION % 08.3f degree on z axis EXAMPLE\n", rotationAngleDegree);
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

            printf("\t%u rotation\n\t\t", i);
            simplePrintState(position_rot, &rotated_orientation);
        }
    }

    rotationAngleDegree = 90.0;
    printf("ROTATION % 08.3f degree on z and y axis EXAMPLE\n", rotationAngleDegree);
    {
        Quaternion rotated_orientation;
        Quaternion_setIdentity(&rotated_orientation);   // The identity quaternion represents no rotation

        double rotationAngle = rotationAngleDegree / 180.0 * M_PI;             // Rotation angle in radians
        
        Quaternion rotated_orientation_z;
        Quaternion_setIdentity(&rotated_orientation_z);
        Quaternion_fromZRotation(rotationAngle, &rotated_orientation_z);   // Set rotateLeft to represent the Z-rotation
        printf("rotated_orientation_z\t"); Quaternion_fprint(stdout, &rotated_orientation_z); printf("\n");

        Quaternion rotated_orientation_z_conj;
        Quaternion_setIdentity(&rotated_orientation_z_conj);   // The identity quaternion represents no rotation

        Quaternion_conjugate(&rotated_orientation_z, &rotated_orientation_z_conj);
        printf("rotated_orientation_z_conj\t"); Quaternion_fprint(stdout, &rotated_orientation_z_conj); printf("\n");

        Quaternion rotated_orientation_y;
        Quaternion_setIdentity(&rotated_orientation_y);
        Quaternion_fromYRotation(rotationAngle, &rotated_orientation_y);   // Set rotateLeft to represent the Z-rotation
        printf("rotated_orientation_y\t"); Quaternion_fprint(stdout, &rotated_orientation_y); printf("\n");

        Quaternion rotated_orientation_y_conj;
        Quaternion_setIdentity(&rotated_orientation_y_conj);   // The identity quaternion represents no rotation

        Quaternion_conjugate(&rotated_orientation_y, &rotated_orientation_y_conj);
        printf("rotated_orientation_y_conj\t"); Quaternion_fprint(stdout, &rotated_orientation_y_conj); printf("\n");

        Quaternion rotated_orientation_comb, rotated_orientation_comb_conj, rotated_orientation_comb_conj_1;
        Quaternion_setIdentity(&rotated_orientation_comb);
        Quaternion_setIdentity(&rotated_orientation_comb_conj);
        Quaternion_setIdentity(&rotated_orientation_comb_conj_1);

        Quaternion_multiply(&rotated_orientation_y, &rotated_orientation_z, &rotated_orientation_comb);
        Quaternion_multiply(&rotated_orientation_z_conj, &rotated_orientation_y_conj, &rotated_orientation_comb_conj);
        
        Quaternion_conjugate(&rotated_orientation_comb, &rotated_orientation_comb_conj_1);

        printf("rotated_orientation_comb\t"); Quaternion_fprint(stdout, &rotated_orientation_comb); printf("\n");
        printf("rotated_orientation_comb_conj\t"); Quaternion_fprint(stdout, &rotated_orientation_comb_conj); printf("\n");
        printf("rotated_orientation_comb_conj_1\t"); Quaternion_fprint(stdout, &rotated_orientation_comb_conj_1); printf("\n");

        double axisAngle[3] = {0, 0, 0};
        double angle = Quaternion_toAxisAngle(&rotated_orientation_comb, axisAngle);
        printf("Axis angle:   % 08.3f (% 08.3f, % 08.3f, % 08.3f)\n", angle,
        axisAngle[0], axisAngle[1], axisAngle[2]);

        double position_rot[3] = {10, 0, 0};
        // Rotations
        for (size_t i = 0; i < (size_t)(180.0 / rotationAngleDegree * 2.0); i++)
        {
            Quaternion_rotate(&rotated_orientation_comb, position_rot, position_rot);   // Calculate move in character coordinate system

            printf("\t%u rotation\n\t\t", i);
            simplePrintState(position_rot, &rotated_orientation);
        }
    }

    rotationAngleDegree = 90.0;
    printf("ROTATION % 08.3f degree on x y and z axis EXAMPLE\n", rotationAngleDegree);
    {
        Quaternion rotated_orientation;
        Quaternion_setIdentity(&rotated_orientation);   // The identity quaternion represents no rotation

        double rotationAngle = rotationAngleDegree / 180.0 * M_PI;             // Rotation angle in radians
/*         Quaternion_fromZRotation(rotationAngle, &rotated_orientation);   // Set rotateLeft to represent the Z-rotation
        printf("Quaternion_fromZRotation\t"); Quaternion_fprint(stdout, &rotated_orientation); printf("\n");

        Quaternion_fromYRotation(rotationAngle, &rotated_orientation);   // Set rotateLeft to represent the Z-rotation
        printf("Quaternion_fromYRotation\t"); Quaternion_fprint(stdout, &rotated_orientation); printf("\n");

        Quaternion_fromZRotation(rotationAngle, &rotated_orientation);   // Set rotateLeft to represent the Z-rotation
        printf("Quaternion_fromZRotation\t"); Quaternion_fprint(stdout, &rotated_orientation); printf("\n");
 */
        Quaternion rotated_orientation_comb_temp, rotated_orientation_comb;
        Quaternion_setIdentity(&rotated_orientation_comb_temp);
        Quaternion_setIdentity(&rotated_orientation_comb);

        Quaternion rotated_orientation_z;
        Quaternion_setIdentity(&rotated_orientation_z);
        Quaternion_fromZRotation(rotationAngle, &rotated_orientation_z);

        Quaternion rotated_orientation_y;
        Quaternion_setIdentity(&rotated_orientation_y);
        Quaternion_fromYRotation(rotationAngle, &rotated_orientation_y);

        Quaternion rotated_orientation_x;
        Quaternion_setIdentity(&rotated_orientation_x);
        Quaternion_fromXRotation(rotationAngle, &rotated_orientation_x);

        Quaternion_multiply(&rotated_orientation_z, &rotated_orientation_y, &rotated_orientation_comb_temp);
        Quaternion_multiply(&rotated_orientation_comb_temp, &rotated_orientation_x, &rotated_orientation_comb);
        printf("rotated_orientation_comb\t"); Quaternion_fprint(stdout, &rotated_orientation_comb); printf("\n");
        
/*         double axisAngle[3] = {0, 0, 0};
        double angle = Quaternion_toAxisAngle(&rotated_orientation_comb, axisAngle);
        printf("Axis angle:   % 08.3f (% 08.3f, % 08.3f, % 08.3f)\n", angle,
        axisAngle[0], axisAngle[1], axisAngle[2]); */

        double position_rot[3] = {10, 10, 10};
        // Rotations
        for (size_t i = 0; i < (size_t)(180.0 / rotationAngleDegree * 2.0); i++)
        {
            Quaternion_rotate(&rotated_orientation_comb, position_rot, position_rot);   // Calculate move in character coordinate system

            printf("\t%u rotation\n\t\t", i);
            simplePrintState(position_rot, &rotated_orientation_comb);
        }
    }

    rotationAngleDegree = 45.0;
    printf("ROTATION % 08.3f degree on x y and z axis EXAMPLE AxisAngle\n", rotationAngleDegree);
    {
        Quaternion rotated_orientation;
        Quaternion_setIdentity(&rotated_orientation);   // The identity quaternion represents no rotation

        double rotationAngle = rotationAngleDegree / 180.0 * M_PI;             // Rotation angle in radians
        double eulerAngles[3] = {rotationAngle, rotationAngle, rotationAngle};

        Quaternion_fromAxisAngle(eulerAngles, rotationAngleDegree, &rotated_orientation);
        printf("rotated_orientation\t"); Quaternion_fprint(stdout, &rotated_orientation); printf("\n");

        double axisAngle[3] = {0, 0, 0};
        double angle = Quaternion_toAxisAngle(&rotated_orientation, axisAngle);
        printf("Axis angle:   % 08.3f (% 08.3f, % 08.3f, % 08.3f)\n", angle,
        axisAngle[0], axisAngle[1], axisAngle[2]);

        double position_rot[3] = {10, 0, 0};
        // Rotations
        for (size_t i = 0; i < (size_t)(180.0 / rotationAngleDegree * 2.0); i++)
        {
            Quaternion_rotate(&rotated_orientation, position_rot, position_rot);   // Calculate move in character coordinate system

            printf("\t%u rotation\n\t\t", i);
            simplePrintState(position_rot, &rotated_orientation);
        }
    }

    printf("Hemisphere sensor layout EXAMPLE\n");
    {
        double axisAngle[3] = {0, 0, 0};
        Quaternion hemispher_sensors[14];
        double angles[14][3];
        for (size_t i = 0; i < sizeof(hemispher_sensors)/sizeof(hemispher_sensors[0]); i++)
        {
            Quaternion_setIdentity(&hemispher_sensors[i]);
        }
        
        int idx = 0;
        for (size_t i = 0; i < 2; i++)
        {
            for (size_t j = 0; j < 8; j++)
            {
                if (i> 0 && j % 2 == 0)
                {
                    continue;
                }
                double yaw =  45.0 * i;
                double pitch =  45.0 * j;
                double roll =  0.0;
                angles[idx][0] = yaw;
                angles[idx][1] = pitch;
                angles[idx][2] = roll;
                
                // printf("Quaternion_fromEulerZYX y: % 08.3f p: % 08.3f r: % 08.3f\t:\t", yaw, pitch, roll);
                {
                    double eulerAngles[3] = {yaw/ 180.0 * M_PI, pitch/ 180.0 * M_PI, roll/ 180.0 * M_PI};
                    Quaternion rotated_orientation;
                    Quaternion_fromEulerZYX(eulerAngles, &rotated_orientation);
                    
                    hemispher_sensors[idx].v[0] = rotated_orientation.v[0];
                    hemispher_sensors[idx].v[1] = rotated_orientation.v[1];
                    hemispher_sensors[idx].v[2] = rotated_orientation.v[2];
                    hemispher_sensors[idx].w = rotated_orientation.w;
                    idx = idx +1;
                    // Quaternion_fprint(stdout, &rotated_orientation); printf("\n");
                }
            }
        }

        double yaw =  90.0;
        double pitch =  90.0;
        double roll =  0.0;
        angles[idx][0] = yaw;
        angles[idx][1] = pitch;
        angles[idx][2] = roll;

        // printf("Quaternion_fromEulerZYX y: % 08.3f p: % 08.3f r: % 08.3f\t:\t", yaw, pitch, roll);
        {
            double eulerAngles[3] = {yaw/ 180.0 * M_PI, pitch/ 180.0 * M_PI, roll/ 180.0 * M_PI};
            Quaternion rotated_orientation;
            Quaternion_fromEulerZYX(eulerAngles, &rotated_orientation);
            hemispher_sensors[idx].v[0] = rotated_orientation.v[0];
            hemispher_sensors[idx].v[1] = rotated_orientation.v[1];
            hemispher_sensors[idx].v[2] = rotated_orientation.v[2];
            hemispher_sensors[idx].w = rotated_orientation.w;
            idx = idx +1;
            // Quaternion_fprint(stdout, &rotated_orientation); printf("\n");
        }

        double position_rot[3] = {1.0, 0, 0};
        for (size_t i = 0; i < idx; i++)
        {
            // Quaternion_rotate(&hemispher_sensors[i], position_rot, position_rot);
            printf("% 08.3f, % 08.3f, % 08.3f\t", angles[i][0], angles[i][1], angles[i][2]);
            simplePrintState(position_rot, &hemispher_sensors[i]);
        }
        
    }


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
