//
// Created by 叶立民 on 2017/9/27.
//
#include "esUtil.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ES_PI 3.1415926535897932384626433832795f

bool checkGlError(const char *funcName) {
    GLint err = glGetError();
    if (err != GL_NO_ERROR) {
        ALOGE("GL error after %s(): 0x%08x\n", funcName, err);
        return true;
    }
    return false;
}

GLuint createShader(GLenum shaderType, const char *src) {
    GLuint shader = glCreateShader(shaderType);
    if (!shader) {
        checkGlError("glCreateShader");
        return 0;
    }
    glShaderSource(shader, 1, &src, NULL);
    GLint compiled = GL_FALSE;
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint infoLogLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);
        if (infoLogLen > 0) {
            GLchar *infoLog = (GLchar *) malloc(infoLogLen);
            if (infoLog) {
                glGetShaderInfoLog(shader, infoLogLen, NULL, infoLog);
                ALOGE("Could not compile %s shader:\n%s\n",
                      shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment",
                      infoLog);
                free(infoLog);
            }
        }
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLuint createProgram(const char *vtxSrc, const char *fragSrc) {
    GLuint vtxShader = 0;
    GLuint fragShader = 0;
    GLuint program = 0;
    GLint linked = GL_FALSE;
    vtxShader = createShader(GL_VERTEX_SHADER, vtxSrc);
    if (!vtxShader)
        goto exit;
    fragShader = createShader(GL_FRAGMENT_SHADER, fragSrc);
    if (!fragShader)
        goto exit;
    program = glCreateProgram();
    if (!program) {
        checkGlError("glCreateProgram");
        goto exit;
    }
    glAttachShader(program, vtxShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        ALOGE("Could not link program");
        GLint infoLogLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLen);
        if (infoLogLen) {
            GLchar *infoLog = (GLchar *) malloc(infoLogLen);
            if (infoLog) {
                glGetProgramInfoLog(program, infoLogLen, NULL, infoLog);
                ALOGE("Could not link program:\n%s\n", infoLog);
                free(infoLog);
            }
        }
        glDeleteProgram(program);
        program = 0;
    }
    exit:
    glDeleteShader(vtxShader);
    glDeleteShader(fragShader);
    return program;
}

int ESUTIL_API esGenCube ( float scale, GLfloat **vertices, GLfloat **normals,
                           GLfloat **texCoords, GLuint **indices )
{
    int i;
    int numVertices = 24;
    int numIndices = 36;

    GLfloat cubeVerts[] =
            {
                    -0.5f, -0.5f, -0.5f,
                    -0.5f, -0.5f,  0.5f,
                    0.5f, -0.5f,  0.5f,
                    0.5f, -0.5f, -0.5f,
                    -0.5f,  0.5f, -0.5f,
                    -0.5f,  0.5f,  0.5f,
                    0.5f,  0.5f,  0.5f,
                    0.5f,  0.5f, -0.5f,
                    -0.5f, -0.5f, -0.5f,
                    -0.5f,  0.5f, -0.5f,
                    0.5f,  0.5f, -0.5f,
                    0.5f, -0.5f, -0.5f,
                    -0.5f, -0.5f, 0.5f,
                    -0.5f,  0.5f, 0.5f,
                    0.5f,  0.5f, 0.5f,
                    0.5f, -0.5f, 0.5f,
                    -0.5f, -0.5f, -0.5f,
                    -0.5f, -0.5f,  0.5f,
                    -0.5f,  0.5f,  0.5f,
                    -0.5f,  0.5f, -0.5f,
                    0.5f, -0.5f, -0.5f,
                    0.5f, -0.5f,  0.5f,
                    0.5f,  0.5f,  0.5f,
                    0.5f,  0.5f, -0.5f,
            };

    GLfloat cubeNormals[] =
            {
                    0.0f, -1.0f, 0.0f,
                    0.0f, -1.0f, 0.0f,
                    0.0f, -1.0f, 0.0f,
                    0.0f, -1.0f, 0.0f,
                    0.0f, 1.0f, 0.0f,
                    0.0f, 1.0f, 0.0f,
                    0.0f, 1.0f, 0.0f,
                    0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, -1.0f,
                    0.0f, 0.0f, -1.0f,
                    0.0f, 0.0f, -1.0f,
                    0.0f, 0.0f, -1.0f,
                    0.0f, 0.0f, 1.0f,
                    0.0f, 0.0f, 1.0f,
                    0.0f, 0.0f, 1.0f,
                    0.0f, 0.0f, 1.0f,
                    -1.0f, 0.0f, 0.0f,
                    -1.0f, 0.0f, 0.0f,
                    -1.0f, 0.0f, 0.0f,
                    -1.0f, 0.0f, 0.0f,
                    1.0f, 0.0f, 0.0f,
                    1.0f, 0.0f, 0.0f,
                    1.0f, 0.0f, 0.0f,
                    1.0f, 0.0f, 0.0f,
            };

    GLfloat cubeTex[] =
            {
                    0.0f, 0.0f,
                    0.0f, 1.0f,
                    1.0f, 1.0f,
                    1.0f, 0.0f,
                    1.0f, 0.0f,
                    1.0f, 1.0f,
                    0.0f, 1.0f,
                    0.0f, 0.0f,
                    0.0f, 0.0f,
                    0.0f, 1.0f,
                    1.0f, 1.0f,
                    1.0f, 0.0f,
                    0.0f, 0.0f,
                    0.0f, 1.0f,
                    1.0f, 1.0f,
                    1.0f, 0.0f,
                    0.0f, 0.0f,
                    0.0f, 1.0f,
                    1.0f, 1.0f,
                    1.0f, 0.0f,
                    0.0f, 0.0f,
                    0.0f, 1.0f,
                    1.0f, 1.0f,
                    1.0f, 0.0f,
            };

    // Allocate memory for buffers
    if ( vertices != NULL )
    {
        *vertices = (GLfloat *) malloc (sizeof ( GLfloat ) * 3 * numVertices );
        memcpy ( *vertices, cubeVerts, sizeof ( cubeVerts ) );

        for ( i = 0; i < numVertices * 3; i++ )
        {
            ( *vertices ) [i] *= scale;
        }
    }

    if ( normals != NULL )
    {
        *normals = (GLfloat *) malloc (sizeof ( GLfloat ) * 3 * numVertices );
        memcpy ( *normals, cubeNormals, sizeof ( cubeNormals ) );
    }

    if ( texCoords != NULL )
    {
        *texCoords = (GLfloat *) malloc (sizeof ( GLfloat ) * 2 * numVertices );
        memcpy ( *texCoords, cubeTex, sizeof ( cubeTex ) ) ;
    }


    // Generate the indices
    if ( indices != NULL )
    {
        GLuint cubeIndices[] =
                {
                        0, 2, 1,
                        0, 3, 2,
                        4, 5, 6,
                        4, 6, 7,
                        8, 9, 10,
                        8, 10, 11,
                        12, 15, 14,
                        12, 14, 13,
                        16, 17, 18,
                        16, 18, 19,
                        20, 23, 22,
                        20, 22, 21
                };

        *indices = (GLuint *) malloc (sizeof ( GLuint ) * numIndices );
        memcpy ( *indices, cubeIndices, sizeof ( cubeIndices ) );
    }

    return numIndices;
}

//
/// \brief Generates a square grid consisting of triangles.  Allocates memory for the vertex data and stores
///        the results in the arrays.  Generate index list as TRIANGLES.
/// \param size create a grid of size by size (number of triangles = (size-1)*(size-1)*2)
/// \param vertices If not NULL, will contain array of float3 positions
/// \param indices If not NULL, will contain the array of indices for the triangle strip
/// \return The number of indices required for rendering the buffers (the number of indices stored in the indices array
///         if it is not NULL ) as a GL_TRIANGLES
//
int ESUTIL_API esGenSquareGrid ( int size, GLfloat **vertices, GLuint **indices )
{
    int i, j;
    int numIndices = ( size - 1 ) * ( size - 1 ) * 2 * 3;

    // Allocate memory for buffers
    if ( vertices != NULL )
    {
        int numVertices = size * size;
        float stepSize = ( float ) size - 1;
        *vertices = (GLfloat *) malloc (sizeof ( GLfloat ) * 3 * numVertices );

        for ( i = 0; i < size; ++i ) // row
        {
            for ( j = 0; j < size; ++j ) // column
            {
                ( *vertices ) [ 3 * ( j + i * size )     ] = i / stepSize;
                ( *vertices ) [ 3 * ( j + i * size ) + 1 ] = j / stepSize;
                ( *vertices ) [ 3 * ( j + i * size ) + 2 ] = 0.0f;
            }
        }
    }

    // Generate the indices
    if ( indices != NULL )
    {
        *indices = (GLuint *) malloc (sizeof ( GLuint ) * numIndices );

        for ( i = 0; i < size - 1; ++i )
        {
            for ( j = 0; j < size - 1; ++j )
            {
                // two triangles per quad
                ( *indices ) [ 6 * ( j + i * ( size - 1 ) )     ] = j + ( i )   * ( size )    ;
                ( *indices ) [ 6 * ( j + i * ( size - 1 ) ) + 1 ] = j + ( i )   * ( size ) + 1;
                ( *indices ) [ 6 * ( j + i * ( size - 1 ) ) + 2 ] = j + ( i + 1 ) * ( size ) + 1;

                ( *indices ) [ 6 * ( j + i * ( size - 1 ) ) + 3 ] = j + ( i )   * ( size )    ;
                ( *indices ) [ 6 * ( j + i * ( size - 1 ) ) + 4 ] = j + ( i + 1 ) * ( size ) + 1;
                ( *indices ) [ 6 * ( j + i * ( size - 1 ) ) + 5 ] = j + ( i + 1 ) * ( size )    ;
            }
        }
    }

    return numIndices;
}

void ESUTIL_API
esMatrixLoadIdentity ( ESMatrix *result )
{
    memset ( result, 0x0, sizeof ( ESMatrix ) );
    result->m[0][0] = 1.0f;
    result->m[1][1] = 1.0f;
    result->m[2][2] = 1.0f;
    result->m[3][3] = 1.0f;
}

void ESUTIL_API
esTranslate ( ESMatrix *result, GLfloat tx, GLfloat ty, GLfloat tz )
{
    result->m[3][0] += ( result->m[0][0] * tx + result->m[1][0] * ty + result->m[2][0] * tz );
    result->m[3][1] += ( result->m[0][1] * tx + result->m[1][1] * ty + result->m[2][1] * tz );
    result->m[3][2] += ( result->m[0][2] * tx + result->m[1][2] * ty + result->m[2][2] * tz );
    result->m[3][3] += ( result->m[0][3] * tx + result->m[1][3] * ty + result->m[2][3] * tz );
}

void ESUTIL_API
esRotate ( ESMatrix *result, GLfloat angle, GLfloat x, GLfloat y, GLfloat z )
{
    GLfloat sinAngle, cosAngle;
    GLfloat mag = sqrtf ( x * x + y * y + z * z );

    sinAngle = sinf ( angle * ES_PI / 180.0f );
    cosAngle = cosf ( angle * ES_PI / 180.0f );

    if ( mag > 0.0f )
    {
        GLfloat xx, yy, zz, xy, yz, zx, xs, ys, zs;
        GLfloat oneMinusCos;
        ESMatrix rotMat;

        x /= mag;
        y /= mag;
        z /= mag;

        xx = x * x;
        yy = y * y;
        zz = z * z;
        xy = x * y;
        yz = y * z;
        zx = z * x;
        xs = x * sinAngle;
        ys = y * sinAngle;
        zs = z * sinAngle;
        oneMinusCos = 1.0f - cosAngle;

        rotMat.m[0][0] = ( oneMinusCos * xx ) + cosAngle;
        rotMat.m[0][1] = ( oneMinusCos * xy ) - zs;
        rotMat.m[0][2] = ( oneMinusCos * zx ) + ys;
        rotMat.m[0][3] = 0.0F;

        rotMat.m[1][0] = ( oneMinusCos * xy ) + zs;
        rotMat.m[1][1] = ( oneMinusCos * yy ) + cosAngle;
        rotMat.m[1][2] = ( oneMinusCos * yz ) - xs;
        rotMat.m[1][3] = 0.0F;

        rotMat.m[2][0] = ( oneMinusCos * zx ) - ys;
        rotMat.m[2][1] = ( oneMinusCos * yz ) + xs;
        rotMat.m[2][2] = ( oneMinusCos * zz ) + cosAngle;
        rotMat.m[2][3] = 0.0F;

        rotMat.m[3][0] = 0.0F;
        rotMat.m[3][1] = 0.0F;
        rotMat.m[3][2] = 0.0F;
        rotMat.m[3][3] = 1.0F;

        esMatrixMultiply ( result, &rotMat, result );
    }
}

void ESUTIL_API
esMatrixMultiply ( ESMatrix *result, ESMatrix *srcA, ESMatrix *srcB )
{
    ESMatrix    tmp;
    int         i;

    for ( i = 0; i < 4; i++ )
    {
        tmp.m[i][0] =  ( srcA->m[i][0] * srcB->m[0][0] ) +
                       ( srcA->m[i][1] * srcB->m[1][0] ) +
                       ( srcA->m[i][2] * srcB->m[2][0] ) +
                       ( srcA->m[i][3] * srcB->m[3][0] ) ;

        tmp.m[i][1] =  ( srcA->m[i][0] * srcB->m[0][1] ) +
                       ( srcA->m[i][1] * srcB->m[1][1] ) +
                       ( srcA->m[i][2] * srcB->m[2][1] ) +
                       ( srcA->m[i][3] * srcB->m[3][1] ) ;

        tmp.m[i][2] =  ( srcA->m[i][0] * srcB->m[0][2] ) +
                       ( srcA->m[i][1] * srcB->m[1][2] ) +
                       ( srcA->m[i][2] * srcB->m[2][2] ) +
                       ( srcA->m[i][3] * srcB->m[3][2] ) ;

        tmp.m[i][3] =  ( srcA->m[i][0] * srcB->m[0][3] ) +
                       ( srcA->m[i][1] * srcB->m[1][3] ) +
                       ( srcA->m[i][2] * srcB->m[2][3] ) +
                       ( srcA->m[i][3] * srcB->m[3][3] ) ;
    }

    memcpy ( result, &tmp, sizeof ( ESMatrix ) );
}

void ESUTIL_API
esFrustum ( ESMatrix *result, float left, float right, float bottom, float top, float nearZ, float farZ )
{
    float       deltaX = right - left;
    float       deltaY = top - bottom;
    float       deltaZ = farZ - nearZ;
    ESMatrix    frust;

    if ( ( nearZ <= 0.0f ) || ( farZ <= 0.0f ) ||
         ( deltaX <= 0.0f ) || ( deltaY <= 0.0f ) || ( deltaZ <= 0.0f ) )
    {
        return;
    }

    frust.m[0][0] = 2.0f * nearZ / deltaX;
    frust.m[0][1] = frust.m[0][2] = frust.m[0][3] = 0.0f;

    frust.m[1][1] = 2.0f * nearZ / deltaY;
    frust.m[1][0] = frust.m[1][2] = frust.m[1][3] = 0.0f;

    frust.m[2][0] = ( right + left ) / deltaX;
    frust.m[2][1] = ( top + bottom ) / deltaY;
    frust.m[2][2] = - ( nearZ + farZ ) / deltaZ;
    frust.m[2][3] = -1.0f;

    frust.m[3][2] = -2.0f * nearZ * farZ / deltaZ;
    frust.m[3][0] = frust.m[3][1] = frust.m[3][3] = 0.0f;

    esMatrixMultiply ( result, &frust, result );
}

void ESUTIL_API
esPerspective ( ESMatrix *result, float fovy, float aspect, float nearZ, float farZ )
{
    GLfloat frustumW, frustumH;

    frustumH = tanf ( fovy / 360.0f * ES_PI ) * nearZ;
    frustumW = frustumH * aspect;

    esFrustum ( result, -frustumW, frustumW, -frustumH, frustumH, nearZ, farZ );
}
