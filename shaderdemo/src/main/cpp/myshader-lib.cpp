//
// Created by 叶立民 on 2017/9/26.
//

#include "esUtil.h"

static const char VERTEX_SHADER[] = {
        "#version 300 es\n"
                "layout(location = 0) in vec4 vPosition;\n"
                "void main()\n"
                "{\n"
                "gl_Position = vPosition;\n"
                "}\n"
};

static const char FRAGMENT_SHADER[] = {
        "#version 300 es\n"
                "precision mediump float;\n"
                "out vec4 fragColor;\n"
                "void main()\n"
                "{\n"
                "fragColor = vec4(1.0,0.0,0.0,1.0);"
                "}\n"
};

static float vertices[]{
        -0.5f, -0.5, 0,
        0.5f, -0.5f, 0,
        0, 0.5f, 0,
};


extern "C" {
JNIEXPORT jboolean JNICALL
Java_com_lyman_shaderdemo_lib_ShaderLib_init();
JNIEXPORT void JNICALL
Java_com_lyman_shaderdemo_lib_ShaderLib_resize(jint width, jint height);
JNIEXPORT void JNICALL
Java_com_lyman_shaderdemo_lib_ShaderLib_draw();
}


GLuint program;

JNIEXPORT jboolean

JNICALL
Java_com_lyman_shaderdemo_lib_ShaderLib_init() {
    program = createProgram(VERTEX_SHADER, FRAGMENT_SHADER);
    if (program == 0) {
        ALOGE("Init failed");
        return JNI_FALSE;
    }
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    GLint vertexSize;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS,&vertexSize);
    ALOGE("Vertext size is %d",vertexSize);
    return JNI_TRUE;
}

JNIEXPORT void JNICALL
Java_com_lyman_shaderdemo_lib_ShaderLib_resize(jint width, jint height) {
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);
}

JNIEXPORT void JNICALL
Java_com_lyman_shaderdemo_lib_ShaderLib_draw() {
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
