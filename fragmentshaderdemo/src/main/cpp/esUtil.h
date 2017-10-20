//
// Created by 叶立民 on 2017/9/27.
//

#ifndef OPENGLDEMO_ESUTIL_H
#define OPENGLDEMO_ESUTIL_H
#endif
 //OPENGLDEMO_ESUTIL_H

#include <GLES3/gl3.h>
#include <android/log.h>
#include <jni.h>
#include <stdlib.h>
#ifndef LOG_TAG
#define LOG_TAG "ES_LIB"
#endif
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

///
//  Macros
//
#ifdef WIN32
#define ESUTIL_API  __cdecl
#define ESCALLBACK  __cdecl
#else
#define ESUTIL_API
#define ESCALLBACK
#endif


#ifdef ANDROID
#include "android_native_app_glue.h"
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
typedef AAsset esFile;
#else
typedef FILE esFile;
#endif

typedef struct
{
    GLfloat   m[4][4];
} ESMatrix;

//检查当前程序错误
bool checkGlError(const char* funcName);
//获取并编译着色器对象
GLuint createShader(GLenum shaderType, const char* src);
//使用着色器生成着色器程序对象
GLuint createProgram(const char* vtxSrc, const char* fragSrc);

void ESUTIL_API esPerspective ( ESMatrix *result, float fovy, float aspect, float nearZ, float farZ );


int ESUTIL_API esGenCube ( float scale, GLfloat **vertices, GLfloat **normals,
                           GLfloat **texCoords, GLuint **indices );

void ESUTIL_API esMatrixLoadIdentity ( ESMatrix *result );

void ESUTIL_API esTranslate ( ESMatrix *result, GLfloat tx, GLfloat ty, GLfloat tz );

void ESUTIL_API esRotate ( ESMatrix *result, GLfloat angle, GLfloat x, GLfloat y, GLfloat z );

void ESUTIL_API esMatrixMultiply ( ESMatrix *result, ESMatrix *srcA, ESMatrix *srcB );

int ESUTIL_API esGenSphere ( int numSlices, float radius, GLfloat **vertices, GLfloat **normals,
                             GLfloat **texCoords, GLuint **indices );

char *ESUTIL_API esLoadTGA ( void *ioContext, const char *fileName, int *width, int *height );
