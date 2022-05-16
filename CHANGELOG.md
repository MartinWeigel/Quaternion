# Changelog

## 2022-05-16
### Fixed
- Defines `M_PI` if not defined by the compiler

## 2022-04-24
### Fixed
- `Quaternion_slerp` needs else-block to avoid wrong calculations when theta = 180
- `Quaternion_slerp` uses fabs instead of abs


## 2019-11-28
### Added
- Doxygen support

## 2018-12-23
### Added
- A readme file `README.md`
- Example usage in `example.c`

### Fixed
- `Quaternion_multiply(q1, q2, out)` works correct if out points to q1 or q2
- `Quaternion_rotate(_, v, out)` works correct if out points to v
- `Quaternion_slerp(q1, q2, _, out)` works correct if out points to q1 or q2

### Changed
- `Quaternion_fprint()` does not print *Quaternion* before bracket


## 2018-11-11
### Added
- `Quaternion_set()` to set quaternion values
- `Quaternion_setIdentity()` to set the identity quaternion
- `Quaternion_copy()` to copy a quaternion
- `Quaternion_equal()` to check if two quaternions are equal
- `Quaternion_fromAxisAngle()` to calculate a quaternion from a rotation axis and angle
- `Quaternion_toAxisAngle()` to calculate the rotation axis and angle from a quaternion
- `Quaternion_fromEulerZYX()` to calculate a quaternion from euler ZYX angles
- `Quaternion_toEulerZYX()` to calculate the euler ZYX angles from a quaternion
- `Quaternion_fromXRotation()` to calculate a quaternion from a rotation around the x-axis
- `Quaternion_fromYRotation()` to calculate a quaternion from a rotation around the y-axis
- `Quaternion_fromZRotation()` to calculate a quaternion from a rotation around the z-axis
- `Quaternion_conjugate()` to create the conjugate of a quaternion
- `Quaternion_norm()` to calculate the norm of a quaternion
- `Quaternion_normalize()` normalizes a quaternion
- `Quaternion_multiply()` to multiply two quaternions
- `Quaternion_rotate()` to rotate a vector using a quaternion
- `Quaternion_slerp()` to interpolate between two quaternions
