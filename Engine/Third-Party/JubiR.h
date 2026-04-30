/*

    Jubi Physics Library
    --------------------

    A simple 3D physics library for C/C++ projects.

    Early Development | IN DEVELOPMENT
    Created by Averi | GitHub: @Avery-Personal

    License: ALLPU License

===========================================================
                      VERSION INFORMATION

Jubi Version: 0.2.2
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
    #include <string.h>
    #include <math.h>

    #ifdef __cplusplus
        extern "C" {
    #endif

    #define JUBI_MAX_BODIES 65535

    // JubiR is expected to be incompatible with Jubi, though usage of "Jubi" prefix is TO stay
    #define JUBI_GRAVITY 9.81f;

    #define JPI 3.14159263f
    #define JEPSILON 1e-6f
    #define JINFINITY 1e30f

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

    typedef struct {
        JVector Position;
        JQuaternion Rotation;
        JVector Scale;
    } JTransform;

    typedef struct {
        JVector Normal;

        float Distance;
    } JPlane;

    typedef struct {
        JVector Origin;
        JVector Direction;
    } JRay;

    typedef struct {
        JVector Minimum;
        JVector Maximum;
    } JAABB;

    typedef struct {
        JVector Normal;
        JVector ContactPoint;

        float Penetration;
    } JContact;

    typedef struct JubiWorld JubiWorld;
    
    typedef struct {
        //JBodyDimension Dimension;

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

    #define JUBI_SUCCEEDED(Result) ((Result) > 0)
    #define JUBI_FAILED(Result) ((Result) <= 0)

    #define JMINIMUM_FLOAT(A, B) (((A) < (B)) ? (A) : (B))
    #define JMAXIMUM_FLOAT(A, B) (((A) > (B)) ? (A) : (B))

    float JClamp(float Value, float Minimum, float Maximum);
    float JSign(float Value);
    float JAbsoluteValue(float Value);

    JVector JVectorMake(float X, float Y, float Z);
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

    JMatrix3 JMatrix3Identity(void);
    JMatrix3 JMatrix3Multiply(JMatrix3 A, JMatrix3 B);
    JVector JMatrix3MultiplyVector(JMatrix3 Matrix, JVector Vector);
    JMatrix3 JMatrix3Transpose(JMatrix3 Matrix);
    JMatrix3 JMatrix3Inverse(JMatrix3 Matrix);
    
    JMatrix3 JMatrix3Zero(void);
    JMatrix4 JMatrix4Zero(void);

    JQuaternion JQuaternionIdentity(void);
    JQuaternion JQuaternionMultiply(JQuaternion A, JQuaternion B);
    JQuaternion JQuaternionNormalize(JQuaternion Quaternion);
    JQuaternion JQuaternionFromAxisAngle(JVector Axis, float Angle);
    JVector JQuaternionRotateVector(JQuaternion Quaternion, JVector Vector);

    JMatrix4 JTransformToMatrix(JTransform Transform);
    JVector JTransformApply(JTransform Transform, JVector Vector);

    JubiBoolean JAABBIntersect(JAABB A, JAABB B);

    JubiWorld JCreateWorld();
    void JClearWorld(JubiWorld *World);
    void JDestroyWorld2D(JubiWorld *World);

    #ifndef JUBI_IMPLEMENTATION
        float JClamp(float Value, float Minimum, float Maximum) {
            if (Minimum > Maximum) {
                float Temporary = Minimum;

                Minimum = Maximum;
                Maximum = Temporary;
            }

            if (Value < Minimum)
                return Minimum;
                
            if (Value > Maximum)
                return Maximum;
                
            return Value;
        }

        float JSign(float Value) {
            if (Value > 0.0f)
                return 1.0f;

            if (Value < 0.0f)
                return -1.0f;

            return 0.0f;
        }

        float JAbsoluteValue(float Value) {
            return (Value < 0.0f) ? -Value : Value;
        }

        static JVector JVectorMake(float X, float Y, float Z) {
            JVector Result;

            Result.x = X;
            Result.y = Y;
            Result.z = Z;

            return Result;
        }

        JVector JVectorAdd(JVector A, JVector B) {
            return JVectorMake(A.x + B.x, A.y + B.y, A.z + B.z);
        }

        JVector JVectorSubtract(JVector A, JVector B) {
            return JVectorMake(A.x - B.x, A.y - B.y, A.z - B.z);
        }

        JVector JVectorScale(JVector A, float B) {
            return JVectorMake(A.x * B, A.y * B, A.z * B);
        }

        JVector JVectorDivide(JVector A, float B) {
            if (JAbsoluteValue(B) <= JEPSILON) {
                return JVectorMake(0.0f, 0.0f, 0.0f);
            }

            return JVectorMake(A.x / B, A.y / B, A.z / B);
        }

        float JVectorDot(JVector A, JVector B) {
            return (A.x * B.x) + (A.y * B.y) + (A.z * B.z);
        }

        JVector JVectorCross(JVector A, JVector B) {
            return JVectorMake((A.y * B.z) - (A.z * B.y), (A.z * B.x) - (A.x * B.z), (A.x * B.y) - (A.y * B.x));
        }

        float JVectorLength(JVector Vector) {
            return sqrtf(JVectorDot(Vector, Vector));
        }

        float JVectorLengthSqrt(JVector Vector) {
            return JVectorDot(Vector, Vector);
        }

        JVector JVectorNormalize(JVector Vector) {
            float Length = JVectorLength(Vector);
            if (Length <= JEPSILON) {
                return JVectorMake(0.0f, 0.0f, 0.0f);
            }

            return JVectorDivide(Vector, Length);
        }

        JVector JVectorLerp(JVector A, JVector B, float t) {
            return JVectorMake(A.x + (B.x - A.x) * t, A.y + (B.y - A.y) * t, A.z + (B.z - A.z) * t);
        }

        JVector JVectorMininum(JVector A, JVector B) {
            return JVectorMake(JMINIMUM_FLOAT(A.x, B.x), JMINIMUM_FLOAT(A.y, B.y), JMINIMUM_FLOAT(A.z, B.z));
        }

        JVector JVectorMaximum(JVector A, JVector B) {
            return JVectorMake(JMAXIMUM_FLOAT(A.x, B.x), JMAXIMUM_FLOAT(A.y, B.y), JMAXIMUM_FLOAT(A.z, B.z));
        }

        JVector JVectorMinimum(JVector A, JVector B) {
            return JVectorMininum(A, B);
        }

        JMatrix3 JMatrix3Identity(void) {
            JMatrix3 Result = {0};

            Result.Matrix[0][0] = 1.0f;
            Result.Matrix[1][1] = 1.0f;
            Result.Matrix[2][2] = 1.0f;

            return Result;
        }

        JMatrix3 JMatrix3Multiply(JMatrix3 A, JMatrix3 B) {
            JMatrix3 Result = {0};

            for (int Row = 0; Row < 3; ++Row) {
                for (int Column = 0; Column < 3; ++Column) {
                    Result.Matrix[Row][Column] = (A.Matrix[Row][0] * B.Matrix[0][Column]) + (A.Matrix[Row][1] * B.Matrix[1][Column]) + (A.Matrix[Row][2] * B.Matrix[2][Column]);
                }
            }

            return Result;
        }

        JVector JMatrix3MultiplyVector(JMatrix3 Matrix, JVector Vector) {
            return JVectorMake((Matrix.Matrix[0][0] * Vector.x) + (Matrix.Matrix[0][1] * Vector.y) + (Matrix.Matrix[0][2] * Vector.z), (Matrix.Matrix[1][0] * Vector.x) + (Matrix.Matrix[1][1] * Vector.y) + (Matrix.Matrix[1][2] * Vector.z), (Matrix.Matrix[2][0] * Vector.x) + (Matrix.Matrix[2][1] * Vector.y) + (Matrix.Matrix[2][2] * Vector.z));
        }

        JMatrix3 JMatrix3Transpose(JMatrix3 Matrix) {
            JMatrix3 Result;

            for (int Row = 0; Row < 3; ++Row) {
                for (int Column = 0; Column < 3; ++Column) {
                    Result.Matrix[Row][Column] = Matrix.Matrix[Column][Row];
                }
            }

            return Result;
        }

        JMatrix3 JMatrix3Inverse(JMatrix3 Matrix) {
            float M00 = Matrix.Matrix[0][0], M01 = Matrix.Matrix[0][1], M02 = Matrix.Matrix[0][2];
            float M10 = Matrix.Matrix[1][0], M11 = Matrix.Matrix[1][1], M12 = Matrix.Matrix[1][2];
            float M20 = Matrix.Matrix[2][0], M21 = Matrix.Matrix[2][1], M22 = Matrix.Matrix[2][2];

            float C00 = (M11 * M22) - (M12 * M21);
            float C01 = (M02 * M21) - (M01 * M22);
            float C02 = (M01 * M12) - (M02 * M11);

            float Determinant = (M00 * C00) + (M10 * C01) + (M20 * C02);
            if (JAbsoluteValue(Determinant) <= JEPSILON) {
                return JMatrix3Zero();
            }

            float InverseDeterminant = 1.0f / Determinant;

            JMatrix3 Result;

            Result.Matrix[0][0] = C00 * InverseDeterminant;
            Result.Matrix[0][1] = ((M02 * M20) - (M00 * M22)) * InverseDeterminant;
            Result.Matrix[0][2] = ((M00 * M21) - (M01 * M20)) * InverseDeterminant;

            Result.Matrix[1][0] = C01 * InverseDeterminant;
            Result.Matrix[1][1] = ((M00 * M22) - (M02 * M20)) * InverseDeterminant;
            Result.Matrix[1][2] = ((M01 * M20) - (M00 * M21)) * InverseDeterminant;

            Result.Matrix[2][0] = C02 * InverseDeterminant;
            Result.Matrix[2][1] = ((M01 * M12) - (M02 * M11)) * InverseDeterminant;
            Result.Matrix[2][2] = ((M00 * M11) - (M01 * M10)) * InverseDeterminant;

            return Result;
        }

        JQuaternion JQuaternionIdentity(void) {
            JQuaternion Result;

            Result.x = 0.0f;
            Result.y = 0.0f;
            Result.z = 0.0f;
            Result.w = 1.0f;

            return Result;
        }

        JQuaternion JQuaternionMultiply(JQuaternion A, JQuaternion B) {
            JQuaternion Result;

            Result.x = (A.w * B.x) + (A.x * B.w) + (A.y * B.z) - (A.z * B.y);
            Result.y = (A.w * B.y) - (A.x * B.z) + (A.y * B.w) + (A.z * B.x);
            Result.z = (A.w * B.z) + (A.x * B.y) - (A.y * B.x) + (A.z * B.w);
            Result.w = (A.w * B.w) - (A.x * B.x) - (A.y * B.y) - (A.z * B.z);

            return Result;
        }

        JQuaternion JQuaternionNormalize(JQuaternion Quaternion) {
            float Length = sqrtf((Quaternion.x * Quaternion.x) + (Quaternion.y * Quaternion.y) + (Quaternion.z * Quaternion.z) + (Quaternion.w * Quaternion.w));
            if (Length <= JEPSILON) {
                return JQuaternionIdentity();
            }

            Quaternion.x /= Length;
            Quaternion.y /= Length;
            Quaternion.z /= Length;
            Quaternion.w /= Length;

            return Quaternion;
        }

        JQuaternion JQuaternionFromAxisAngle(JVector Axis, float Angle) {
            float AxisLength = JVectorLength(Axis);
            if (AxisLength <= JEPSILON) {
                return JQuaternionIdentity();
            }

            JVector DividedAxis = JVectorDivide(Axis, AxisLength);

            float HalfAngle = Angle * 0.5f;
            float Sin = sinf(HalfAngle);
            float Cos = cosf(HalfAngle);

            JQuaternion Result;

            Result.x = DividedAxis.x * Sin;
            Result.y = DividedAxis.y * Sin;
            Result.z = DividedAxis.z * Sin;
            Result.w = Cos;

            return JQuaternionNormalize(Result);
        }

        JVector JQuaternionRotateVector(JQuaternion Quaternion, JVector Vector) {
            Quaternion = JQuaternionNormalize(Quaternion);

            JVector QVector = JVectorMake(Quaternion.x, Quaternion.y, Quaternion.z);
            JVector Scale = JVectorScale(JVectorCross(QVector, Vector), 2.0f);
            JVector Result = JVectorAdd(Vector, JVectorAdd(JVectorScale(Scale, Quaternion.w), JVectorCross(QVector, Scale)));

            return Result;
        }

        JMatrix4 JTransformToMatrix(JTransform Transform) {
            Transform.Rotation = JQuaternionNormalize(Transform.Rotation);

            float X = Transform.Rotation.x;
            float Y = Transform.Rotation.y;
            float Z = Transform.Rotation.z;
            float W = Transform.Rotation.w;

            float XX = X * X;
            float YY = Y * Y;
            float ZZ = Z * Z;
            float XY = X * Y;
            float XZ = X * Z;
            float YZ = Y * Z;
            float WX = W * X;
            float WY = W * Y;
            float WZ = W * Z;

            float SX = Transform.Scale.x;
            float SY = Transform.Scale.y;
            float SZ = Transform.Scale.z;

            JMatrix4 Result = {0};

            Result.Matrix[0][0] = (1.0f - 2.0f * (YY + ZZ)) * SX;
            Result.Matrix[0][1] = (2.0f * (XY - WZ)) * SY;
            Result.Matrix[0][2] = (2.0f * (XZ + WY)) * SZ;
            Result.Matrix[0][3] = Transform.Position.x;

            Result.Matrix[1][0] = (2.0f * (XY + WZ)) * SX;
            Result.Matrix[1][1] = (1.0f - 2.0f * (XX + ZZ)) * SY;
            Result.Matrix[1][2] = (2.0f * (YZ - WX)) * SZ;
            Result.Matrix[1][3] = Transform.Position.y;

            Result.Matrix[2][0] = (2.0f * (XZ - WY)) * SX;
            Result.Matrix[2][1] = (2.0f * (YZ + WX)) * SY;
            Result.Matrix[2][2] = (1.0f - 2.0f * (XX + YY)) * SZ;
            Result.Matrix[2][3] = Transform.Position.z;

            Result.Matrix[3][0] = 0.0f;
            Result.Matrix[3][1] = 0.0f;
            Result.Matrix[3][2] = 0.0f;
            Result.Matrix[3][3] = 1.0f;

            return Result;
        }

        JVector JTransformApply(JTransform Transform, JVector Vector) {
            Transform.Rotation = JQuaternionNormalize(Transform.Rotation);

            JVector Scaled = JVectorMake(Vector.x * Transform.Scale.x, Vector.y * Transform.Scale.y, Vector.z * Transform.Scale.z);
            JVector Rotated = JQuaternionRotateVector(Transform.Rotation, Scaled);

            return JVectorAdd(Rotated, Transform.Position);
        }

        JubiBoolean JAABBIntersect(JAABB A, JAABB B) {
            JVector AMinimum = JVectorMininum(A.Minimum, A.Maximum);
            JVector AMaximum = JVectorMaximum(A.Minimum, A.Maximum);

            JVector BMinimum = JVectorMininum(B.Minimum, B.Maximum);
            JVector BMaximum = JVectorMaximum(B.Minimum, B.Maximum);

            if (AMaximum.x < BMinimum.x || AMinimum.x > BMaximum.x)
                return JUBI_FALSE;

            if (AMaximum.y < BMinimum.y || AMinimum.y > BMaximum.y)
                return JUBI_FALSE;

            if (AMaximum.z < BMinimum.z || AMinimum.z > BMaximum.z)
                return JUBI_FALSE;

            return JUBI_TRUE;
        }

        JubiWorld JCreateWorld() {
            JubiWorld WORLD;

            WORLD.BodyCount = 0;
            WORLD.Gravity = JUBI_GRAVITY;
            WORLD.Destroyed = JUBI_FALSE;

            return WORLD;
        }

        void JClearWorld(JubiWorld *World) {
            if (World == NULL)
                return;

            if (World -> Destroyed)
                return;

            World -> BodyCount = 0;
        }

        void JDestroyWorld(JubiWorld *World) {
            if (World == NULL) return;
            if (World -> Destroyed) return;

            for (int i=0; i < JUBI_MAX_BODIES; ++i) {
                World -> Bodies[i] = (JBody){0};
            }

            World -> BodyCount = 0;
            World -> Gravity = 0.0f;
            World -> Destroyed = JUBI_TRUE;
        }

        JubiBoolean JWorldIsDestroyed(JubiWorld *World) {
            if (World == NULL)
                return JUBI_FALSE;

            return World -> Destroyed == JUBI_TRUE ? JUBI_TRUE : JUBI_FALSE;
        }

        JubiResult JIsWorldValid(JubiWorld *World) {
            if (World == NULL)
                return JUBI_RESULT_NULL_WORLD;
                
            if (World -> BodyCount < 0)
                return JUBI_RESULT_WORLD_CORRUPTED;
                
            if (World -> BodyCount > JUBI_MAX_BODIES)
                return JUBI_RESULT_WORLD_FULL;
                
            if (World -> Destroyed == JUBI_TRUE)
                return JUBI_RESULT_WORLD_DESTROYED;
                
            return JUBI_RESULT_SUCCESS;
        }
    #endif

    #ifdef __cplusplus
        }
    #endif

#endif
