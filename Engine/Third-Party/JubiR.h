/*

    Jubi Physics Library
    --------------------

    A simple 3D physics library for C/C++ projects.

    Early Development | IN DEVELOPMENT
    Created by Averi | GitHub: @Avery-Personal

    License: ALLPU License

===========================================================
                      VERSION INFORMATION

Jubi Version: 0.1.5
C Language Standard: C99
C++ Language Standard: C++98 or C++20

Jubi Version Macro(s) & README Versions are official, stable/testable versions that work, all versions after are experimental.

===========================================================
                     LICENSE INFORMATION

The following software is protected under the ALLPU license.
More detailed information is present at the root LICENSE
file AND OR the end of the file.

*/

/*

    JubiR Physics Library

    The Jubi "R", derived from the "R" series, is a REMADE, from SCRATCH version based off the Jubi Physics Library.
    Jubi is available under the ALLPU License, found under LICENSES/ of said repository, ALONGSIDE LICENSE file of said repository

    Copyright © 2025-2026 Averi

    Said Third-Party file is made DURING development of usage, that is of software for the Haruhi Engine.
    JubiR in usage is of MODIFICATION from its 'original' source of what it's to be if were of repository.

*/

#ifndef JUBIR_H
#define JUBIR_H

    #include <stdio.h>

    #include <math.h>

    #ifdef __cplusplus
        extern "C" {
    #endif

    #define JUBI_MAX_BODIES 65535

    #define PI 3.14159263f

    typedef signed char JUBI_INT8;
    typedef signed short JUBI_INT16;
    typedef signed int JUBI_INT32;
    typedef signed long long JUBI_INT64;

    typedef unsigned char JUBI_UINT8;
    typedef unsigned short JUBI_UINT16;
    typedef unsigned int JUBI_UINT32;
    typedef unsigned long long JUBI_UINT64;

    typedef int JIndex;

    typedef enum {
        JUBI_FALSE = 0,
        JUBI_TRUE = 1
    } JubiBoolean;

    typedef enum {
        JUBI_RESULT_SUCCESS = 1,

        JUBI_RESULT_NULL_WORLD = -101,
        JUBI_RESULT_WORLD_CORRUPTED = -102,
        JUBI_RESULT_WORLD_DESTROYED = -103,
        JUBI_RESULT_WORLD_FULL = -104,
        JUBI_RESULT_WORLD_STEP_FAILED = -105,

        JUBI_RESULT_NULL_BODY = -201,
        JUBI_RESULT_BODY_NOT_IN_WORLD = -202,
        JUBI_RESULT_BODY_NOT_VALID = -203,
        JUBI_RESULT_BODY_STAIC = -204,
        JUBI_RESULT_BODY_ALREADY_EXISTS = -205,

        JUBI_RESULT_INVALID_VALUE = -301,
        JUBI_RESULT_NULL_VALUE = -302,
        JUBI_RESULT_INVALID_ENUM = -303,
        JUBI_RESULT_INVALID_STATE = -304,

        JUBI_RESULT_NULL_SHAPE = -401,
        JUBI_RESULT_INVALID_SHAPE = -402,
        JUBI_RESULT_UNSUPPORTED_SHAPE = -403,

        JUBI_RESULT_FAILURE = 0,
        JUBI_RESULT_UNKNOWN = -1,
        JUBI_RESULT_ERROR = -2,
    } JubiResult;

    // JubiR is expected to be incompatible with Jubi, though usage of "Jubi" prefix is TO stay
    extern double JUBI_GRAVITY = 9.81f;

    typedef enum {
        JBODY_DIMENSION_2D, // Invalid as of CURRENT
        JBODY_DIMENSION_3D
    } JBodyDimension;

    typedef enum {
        JSHAPE_CIRCLE,
        JSHAPE_SQUARE,

        // 3

        JSHAPE_SPHERE,
        JSHAPE_BOX,
    } JShape;

    typedef enum {
        JBODY_STATIC,
        JBODY_DYNAMIC
    } JBodyType;

    typedef struct {
        struct {
            float x, y, z;
        };

        // Vector 3 to be Z, of dynamic usage
        float Vector[3];
    } JVector;

    typedef struct {
        float Matrix[3][3];
    } JMatrix3;

    typedef struct {
        float Matrix[4][4];
    } JMatrix4;

    typedef struct {
        float x, y, z, w;
    } JQuaternion;

    typedef struct JubiWorld JubiWorld;
    
    typedef struct {
        JBodyDimension Dimension;

        JVector Position;
        JVector Velocity;
        JVector Size;

        JShape Shape;
        JBodyType BodyType;

        JVector Force;
        JVector AccumulatedForce;

        double Mass;
        double InverseMass;
        
        float Restitution;
        float Friction;

        union {
            JubiBoolean CAN_ROTATE; // Naturally assumed?
        } ShapeData;

        JIndex Index;
        JubiWorld *World;
    } JBody;

    typedef struct JubiWorld {
        JBody Bodies[JUBI_MAX_BODIES];

        int BodyCount;

        float Gravity;

        JubiBoolean Destroyed;
    } JubiWorld;

    JVector JVectorAdd(JVector A, JVector B);
    JVector JVectorSubtract(JVector A, JVector B);
    JVector JVectorScale(JVector A, float B);
    JVector JVectorDivide(JVector A, float B);

    float JVectorDot(JVector A, JVector B);
    JVector JVectorCross(JVector A, JVector B);

    float JVectorLength(JVector Vector);
    float JVectorLengthSqrt(JVector Vector);
    JVector JVectorNormalize(JVector Vector);

    JVector JVectorLerp(JVector A, JVector B, float t);
    JVector JVectorMininum(JVector A, JVector B);
    JVector JVectorMaximum(JVector A, JVector B);

    JMatrix3 JMatrix3Identity();
    JMatrix3 JMatrix3Multiply(JMatrix3 A, JMatrix3 B);
    JVector JMatrix3MultiplyVector(JMatrix3 Matrix, JVector Vector);

    JMatrix3 JMatrix3Transpose(JMatrix3 m);
    JMatrix3 JMatrix3Inverse(JMatrix3 m);

    #ifdef JUBI_IMPLEMENTATION


        JubiWorld JCreateWorld() {
            JubiWorld WORLD;

            WORLD.BodyCount = 0;
            WORLD.Gravity = JUBI_GRAVITY;
            WORLD.Destroyed = JUBI_FALSE;

            return WORLD;
        }
    #endif

    #ifdef __cplusplus
        }
    #endif

#endif
