//
// Created by 叶立民 on 2017/9/26.
//
//使用顶点数组对象进行封装
#include "esUtil.h"

#define VERTEX_POS_SIZE       3 // x, y and z
#define VERTEX_COLOR_SIZE     4 // r, g, b, and a

#define VERTEX_POS_INDX       0
#define VERTEX_COLOR_INDX     1

#define VERTEX_STRIDE         (sizeof(GLfloat) *(VERTEX_POS_SIZE +VERTEX_COLOR_SIZE))


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

static GLfloat vertices[3 * (VERTEX_POS_SIZE + VERTEX_COLOR_SIZE)] = {
        0.0f, 0.5f, 0.0f,        // v0
        1.0f, 0.0f, 0.0f, 1.0f,  // c0
        -0.5f, -0.5f, 0.0f,        // v1
        0.0f, 1.0f, 0.0f, 1.0f,  // c1
        0.5f, -0.5f, 0.0f,        // v2
        0.0f, 0.0f, 1.0f, 1.0f,  // c2
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
static GLuint vboIds[2] = {
        0, 0
};
GLuint vaoId = 0;


JNIEXPORT jboolean
JNICALL
Java_com_lyman_vertexdemo_lib_VertexLib_init() {
    program = createProgram(VERTEX_SHADER, FRAGMENT_SHADER);
    if (program == 0) {
        ALOGE("Init failed");
        return JNI_FALSE;
    }
    //Index element
    GLushort indices[3] = {0, 1, 2};
    glGenBuffers(2, vboIds);

    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
                 vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),
                 indices, GL_STATIC_DRAW);

    //Generate VAO Id
    glGenVertexArrays(1, &vaoId);
    //Bind the VAO and then setup the vertex
    glBindVertexArray(vaoId);

    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);

    glEnableVertexAttribArray(VERTEX_POS_INDX);
    glEnableVertexAttribArray(VERTEX_COLOR_INDX);

    glVertexAttribPointer(VERTEX_POS_INDX, VERTEX_POS_SIZE,
                          GL_FLOAT, GL_FALSE, VERTEX_STRIDE,0);
    glVertexAttribPointer(VERTEX_COLOR_INDX, VERTEX_COLOR_SIZE,
                          GL_FLOAT, GL_FALSE, VERTEX_STRIDE,
                          (const void *) (VERTEX_POS_SIZE * sizeof(GLfloat)));

    // Reset to the default VAO
    glBindVertexArray(0);

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
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
    // Bind the VAO
    glBindVertexArray(vaoId);
    // Draw with the VAO settings
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, (const void *) 0);
    // Return to the default VAO
    glBindVertexArray(0);
}
