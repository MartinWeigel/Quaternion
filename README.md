# Quaternion Library for C

Quaternions are a 4D vector space that can help to store, apply, and interpolate spatial 3D rotations.
In contrast to rotation matrices and Euler angles, they are memory efficient and free of gimbal locks.
This library implements the most basic quaternion calculations and is licensed under the [ISC license](https://github.com/MartinWeigel/Quaternion/blob/master/LICENSE-ISC.txt)

## Example 1: Basic Movements and Orientations

Let us walk through a brief example of how quaternions can be useful.
The source code can be found in [`example.c`](https://github.com/MartinWeigel/Quaternion/blob/master/example.c).

Imagine you are building a 3D game and want to move a character in a 3D world.
The character needs a 3D position in the world and an orientation to be able to look around,
The following code initializes the avatars start position and orientation:

```C
double position[3] = {0, 0, 0};
Quaternion orientation;
Quaternion_setIdentity(&orientation);   // The identity quaternion represents no rotation
```

Let us rotate the character by 90 degrees around the Z-axis.
We first create a new quaternion that represents the rotation:
```C
Quaternion rotateLeft;
double angle = 90.0 / 180.0 * M_PI;             // Rotation angle in radians
Quaternion_fromZRotation(angle, &rotateLeft);   // Set rotateLeft to represent the Z-rotation
```

Now we modify the character orientation by multiplying it with the rotation:
```C
Quaternion_multiply(&rotateLeft, &orientation, &orientation);
```
*Important:* The order of the parameters is important. Swapping the quaternions would result in a different orientation.

You can imagine, that we now have a character standing on the zero position, facing *left*.
From this orientation, we can move the character two meters forward.
We create the move in absolute coordinates first and than rotate it into the characters coordinate system:
```C
double move[3] = {2, 0, 0};                    // Move in absolute coordinates
Quaternion_rotate(&orientation, move, move);   // Calculate move in character coordinate system
```

Finally, the move can be added to the characters position:
```C
position[0] += move[0];
position[1] += move[1];
position[2] += move[2];
```

The new position is `(0, 2, 0)`.
Our character moved two meters to the left, i.e. to the direction he is facing.


## Example 2: Interpolated Movements and Orientations

Now let our character walk half a circle.
To animate this movement, we would like to split this movement into 10000 steps.

First, we need to setup the movement parameters.
We start the walk with our current orientation and end 180 degrees rotated.
For the end rotation, we can multiply our orientation twice with `rotateLeft` (see above).

```C
    const double STEP_COUNT = 10000;                     // Walk 1000 steps in half circle
    const double STEP_SIZE  = 0.0314;                    // Each step moves 3.14cm
    const double TIME_STEP = 1.0 / STEP_COUNT;

    Quaternion startRotation, endRotation;
    startRotation = orientation;                                  // Start facing left
    Quaternion_multiply(&rotateLeft, &orientation, &orientation);
    Quaternion_multiply(&rotateLeft, &orientation, &endRotation); // End facing right
```

To show the correct movement of our character, it is important to know the current orientation, before stepping forward.
We interpolate the orientation between `startRotation` and `endRotation` using *spherical linear interpolation (SLERP)*.
To use SLERP, we require an interpolation parameter `t` (between 0 and 1), which we increase 10000 times.
In each iteration, we calculate the new orientation, before applying it to the move and adding it to the position.
```c
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
```

After the 10000 steps, our character is in position `(-199.899, 2.000, 0.000)`.
In total, our character walked 314 meters (10000 * 0.0314), which equals half the circumference of the circle (`C/2`).
Using the formula `D = C / PI`, the diameter of our circle was `D = 314*2 / PI ≈ 200`.
This is the distance our character traveled along the X-axis (towards his initial left).
