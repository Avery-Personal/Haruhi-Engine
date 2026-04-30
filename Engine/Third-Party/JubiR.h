/*

    Jubi Physics Library
    --------------------

    A simple 3D physics library for C/C++ projects.

    Early Development | IN DEVELOPMENT
    Created by Averi | GitHub: @Avery-Personal

    License: ALLPU License

===========================================================
                      VERSION INFORMATION

Jubi Version: 0.1.1
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

    typedef signed char JUBI_INT8;
    typedef signed short JUBI_INT16;
    typedef signed int JUBI_INT32;
    typedef signed long long JUBI_INT64;

    typedef unsigned char JUBI_UINT8;
    typedef unsigned short JUBI_UINT16;
    typedef unsigned int JUBI_UINT32;
    typedef unsigned long long JUBI_UINT64;

    #define PI 3.14159263f

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

        JUBI_RESULT_NULL_BODY = -201,
        JUBI_RESULT_BODY_NOT_IN_WORLD = -202,
        JUBI_RESULT_BODY_NOT_VALID = -203,

        JUBI_RESULT_INVALID_VALUE = -301,
        JUBI_RESULT_NULL_VALUE = -302,

        JUBI_RESULT_FAILURE = 0,
        JUBI_RESULT_UNKNOWN = -1,
        JUBI_RESULT_ERROR = -2,
    } JubiResult;

    // JubiR is expected to be incompatible with Jubi, though usage of "Jubi" prefix is TO stay
    extern double JUBI_GRAVITY = 9.81f;

    typedef enum {
        JBODY_DIMENSION_2D,
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
        JBodyDimension Dimension;

        JVector Position;
        JVector Velocity;
        JVector Size;

        JShape Shape;
        JBodyType BodyType;

        double Mass;
    } JBody;

    

    #ifdef __cplusplus
        }
    #endif

#endif
