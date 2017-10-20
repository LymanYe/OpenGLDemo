//
// Created by 叶立民 on 2017/9/26.
//

#include "esUtil.h"

#define VERTEX_POS_SIZE       3 // x, y and z
#define VERTEX_COLOR_SIZE     4 // r, g, b, and a

#define VERTEX_POS_INDX       0
#define VERTEX_COLOR_INDX     1


static const char VERTEX_SHADER[] = {
        "#version 300 es\n"
                "layout(location = 0) in vec4 a_position;\n"
                "layout(location = 1) in vec4 a_color;\n"
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

static GLfloat vertexPos[3 * VERTEX_POS_SIZE] =
        {
                0.0f, 0.5f, 0.0f,        // v0
                -0.5f, -0.5f, 0.0f,        // v1
                0.5f, -0.5f, 0.0f         // v2
        };
static GLfloat color[4 * VERTEX_COLOR_SIZE] =
        {
                1.0f, 0.0f, 0.0f, 1.0f,   // c0
                0.0f, 1.0f, 0.0f, 1.0f,   // c1
                0.0f, 0.0f, 1.0f, 1.0f    // c2
        };
static GLint vtxStrides[2] =
        {
                VERTEX_POS_SIZE * sizeof(GLfloat),
                VERTEX_COLOR_SIZE * sizeof(GLfloat)
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
static GLuint vboIds[3] = {
        0, 0, 0
};

/**
 * 使用在顶点缓冲区对象中创建和存储顶点属性以及元素数据
 * @param numVertices 顶点个数
 * @param vtxBuf 顶点属性数据
 * @param vtxStrides
 * @param numIndices
 * @param indices
 */
void DrawPrimitiveWithVBOs(GLint numVertices, GLfloat **vtxBuf,
                           GLint *vtxStrides, GLint numIndices,
                           GLushort *indices) {
    if (vboIds[0] == 0 && vboIds[1] == 0 && vboIds[2] == 0) {
        glGenBuffers(3, vboIds);

        glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
        glBufferData(GL_ARRAY_BUFFER, vtxStrides[0] * numVertices,
                     vtxBuf[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, vboIds[1]);
        glBufferData(GL_ARRAY_BUFFER, vboIds[1] * numVertices,
                     vtxBuf[1], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[2]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(GLushort) * numIndices, indices, GL_STATIC_DRAW);
    }

    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glEnableVertexAttribArray(VERTEX_POS_INDX);
    glVertexAttribPointer(VERTEX_POS_INDX, VERTEX_POS_SIZE,
                          GL_FLOAT, GL_FALSE, vtxStrides[0], 0);

    glBindBuffer(GL_ARRAY_BUFFER, vboIds[1]);
    glEnableVertexAttribArray(VERTEX_COLOR_INDX);
    glVertexAttribPointer(VERTEX_COLOR_INDX,
                          VERTEX_COLOR_SIZE,
                          GL_FLOAT, GL_FALSE, vtxStrides[1], 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[2]);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);

    glDisableVertexAttribArray(VERTEX_POS_INDX);
    glDisableVertexAttribArray(VERTEX_COLOR_INDX);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


JNIEXPORT jboolean
JNICALL
Java_com_lyman_vertexdemo_lib_VertexLib_init() {
    program = createProgram(VERTEX_SHADER, FRAGMENT_SHADER);
    if (program == 0) {
        ALOGE("Init failed");
        return JNI_FALSE;
    }
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
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
    // Index buffer data
    GLushort indices[3] = {0, 1, 2};
    GLfloat *vtxBuf[2] = {vertexPos, color};
    DrawPrimitiveWithVBOs(3, vtxBuf, vtxStrides, 3, indices);
}
