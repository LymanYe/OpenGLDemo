//
// Created by 叶立民 on 2017/9/26.
//加载颜色组成的简单的纹理
#include "esUtil.h"

static const char VERTEX_SHADER[] =
        "#version 300 es                            \n"
                "layout(location = 0) in vec4 a_position;   \n"
                "layout(location = 1) in vec2 a_texCoord;   \n"
                "out vec2 v_texCoord;                       \n"
                "void main()                                \n"
                "{                                          \n"
                "   gl_Position = a_position;               \n"
                "   v_texCoord = a_texCoord;                \n"
                "}                                          \n";


static const char FRAGMENT_SHADER[] =
        "#version 300 es                                     \n"
                "precision mediump float;                            \n"
                "in vec2 v_texCoord;                                 \n"
                "layout(location = 0) out vec4 outColor;             \n"
                "uniform sampler2D s_texture;                        \n"
                "void main()                                         \n"
                "{                                                   \n"
                "  outColor = texture( s_texture, v_texCoord );      \n"
                "}                                                   \n";


// 2x2 Image, 3 bytes per pixel (R, G, B)
static const GLubyte pixels[4 * 3] =
        {
                255, 0, 0, // Red
                0, 255, 0, // Green
                0, 0, 255, // Blue
                255, 255, 0  // Yellow
        };

static const GLfloat vVertices[] = {-0.5f, 0.5f, 0.0f,  // Position 0
                                    0.0f, 0.0f,        // TexCoord 0
                                    -0.5f, -0.5f, 0.0f,  // Position 1
                                    0.0f, 1.0f,        // TexCoord 1
                                    0.5f, -0.5f, 0.0f,  // Position 2
                                    1.0f, 1.0f,        // TexCoord 2
                                    0.5f, 0.5f, 0.0f,  // Position 3
                                    1.0f, 0.0f         // TexCoord 3
};


typedef struct {
    // Handle to a program object
    GLuint programObject;

    // Sampler location
    GLint samplerLoc;

    // Texture handle
    GLuint textureId;

} UserData;

GLuint CreateSimpleTexture2D() {
    GLuint textureId;

    //Use tightly packed data
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    //Generate a texture object
    glGenTextures(1, &textureId);

    //Bind the texture object
    glBindTexture(GL_TEXTURE_2D, textureId);

    //Load the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    //set the filtering mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    return textureId;
}

extern "C" {
JNIEXPORT jboolean JNICALL
Java_com_lyman_texturedemo_lib_TextureLib_init();
JNIEXPORT void JNICALL
Java_com_lyman_texturedemo_lib_TextureLib_resize(jint width, jint height);
JNIEXPORT void JNICALL
Java_com_lyman_texturedemo_lib_TextureLib_draw();
}

UserData userData;

JNIEXPORT jboolean
JNICALL
Java_com_lyman_texturedemo_lib_TextureLib_init() {
    userData.programObject = createProgram(VERTEX_SHADER, FRAGMENT_SHADER);
    if (userData.programObject == 0) {
        ALOGE("Init failed");
        return JNI_FALSE;
    }
    userData.samplerLoc = glGetUniformLocation(userData.programObject, "s_texture");
    userData.textureId = CreateSimpleTexture2D();

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    return JNI_TRUE;
}

JNIEXPORT void JNICALL
Java_com_lyman_texturedemo_lib_TextureLib_resize(jint width, jint height) {
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);
}

JNIEXPORT void JNICALL
Java_com_lyman_texturedemo_lib_TextureLib_draw() {
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(userData.programObject);

    GLushort indices[] = {0, 2, 1, 0, 3, 2};

    // Load the vertex position
    glVertexAttribPointer(0, 3, GL_FLOAT,
                          GL_FALSE, 5 * sizeof(GLfloat), vVertices);
    // Load the texture coordinate
    glVertexAttribPointer(1, 2, GL_FLOAT,
                          GL_FALSE, 5 * sizeof(GLfloat), &vVertices[3]);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // Bind the texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, userData.textureId);

    //Set the sampler texture unit to 0
    glUniform1i(userData.samplerLoc, 0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
}
