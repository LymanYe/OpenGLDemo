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
//检查当前程序错误
bool checkGlError(const char* funcName);
//获取并编译着色器对象
GLuint createShader(GLenum shaderType, const char* src);
//使用着色器生成着色器程序对象
GLuint createProgram(const char* vtxSrc, const char* fragSrc);

