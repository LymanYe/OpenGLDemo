//
// Created by 叶立民 on 2017/9/26.
//
//使用顶点常量属性glVertexAttrib*和顶点数组glVertexAttrib[I]Pointer函数指定顶点属性。
#include "esUtil.h"

static const char VERTEX_SHADER[] = {
        "#version 300 es\n"
                "layout(location = 0) in vec4 a_color;\n"
                "layout(location = 1) in vec4 a_position;\n"
                "out vec4 v_color;\n"
                "void main()\n"
                "{\n"
                "v_color = a_color;\n"
                "gl_Position = a_position;\n"
                "}\n"
};

static const char FRAGMENT_SHADER[] = {
        "#version 300 es\n"
                "precision mediump float;\n"
                "in vec4 v_color;\n"
                "out vec4 fragColor;\n"
                "void main()\n"
                "{\n"
                "fragColor = v_color;"
                "}\n"
};

static float vertices[]{
        -0.5f, -0.5, 0,
        0.5f, -0.5f, 0,
        0, 0.5f, 0,
};


extern "C" {
JNIEXPORT jboolean JNICALL
Java_com_lyman_vertexdemo_lib_VertexLib_init();
JNIEXPORT void JNICALL
Java_com_lyman_vertexdemo_lib_VertexLib_resize(jint width, jint height);
JNIEXPORT void JNICALL
Java_com_lyman_vertexdemo_lib_VertexLib_draw();
}


GLuint program;

JNIEXPORT jboolean

JNICALL
Java_com_lyman_vertexdemo_lib_VertexLib_init() {
    program = createProgram(VERTEX_SHADER, FRAGMENT_SHADER);
    if (program == 0) {
        ALOGE("Init failed");
        return JNI_FALSE;
    }
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    GLint vertexSize;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS,&vertexSize);
    ALOGE("Vertext size is %d",vertexSize);
    GLint location = glGetAttribLocation(program,"a_position");
    return JNI_TRUE;
}

JNIEXPORT void JNICALL
Java_com_lyman_vertexdemo_lib_VertexLib_resize(jint width, jint height) {
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);
}

JNIEXPORT void JNICALL
Java_com_lyman_vertexdemo_lib_VertexLib_draw() {
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program);
    GLfloat color[4] = {1.0f,0.5f,0.0f,1.0f};
    glVertexAttrib4fv(0,color);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    glEnableVertexAttribArray(1);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(1);
}
