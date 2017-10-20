//
// Created by 叶立民 on 2017/10/17.
#include "esUtil.h"

static const char VERTEX_SHADER[] =
        "#version 300 es                            \n"
                "uniform float u_offset;                    \n"
                "layout(location = 0) in vec4 a_position;   \n"
                "layout(location = 1) in vec2 a_texCoord;   \n"
                "out vec2 v_texCoord;                       \n"
                "void main()                                \n"
                "{                                          \n"
                "   gl_Position = a_position;               \n"
                "   gl_Position.x += u_offset;              \n"
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
                "   outColor = texture( s_texture, v_texCoord );     \n"
                "}                                                   \n";

static const GLfloat vVertices[] = {
        -0.3f, 0.3f, 0.0f, 1.0f,  // Position 0
        -1.0f, -1.0f,              // TexCoord 0
        -0.3f, -0.3f, 0.0f, 1.0f, // Position 1
        -1.0f, 2.0f,              // TexCoord 1
        0.3f, -0.3f, 0.0f, 1.0f, // Position 2
        2.0f, 2.0f,              // TexCoord 2
        0.3f, 0.3f, 0.0f, 1.0f,  // Position 3
        2.0f, -1.0f               // TexCoord 3
};


typedef struct {
    // Handle to a program object
    GLuint programObject;

    // Sampler location
    GLint samplerLoc;

    // Offset location
    GLint offsetLoc;

    // Texture handle
    GLuint textureId;

} UserData;

//  Generate an RGB8 checkerboard image
//
GLubyte *GenCheckImage(int width, int height, int checkSize) {
    int x, y;
    GLubyte *pixels = (GLubyte *) malloc(width * height * 3);

    if (pixels == NULL) {
        return NULL;
    }

    for (y = 0; y < height; y++)
        for (x = 0; x < width; x++) {
            GLubyte rColor = 0;
            GLubyte bColor = 0;

            if ((x / checkSize) % 2 == 0) {
                rColor = 255 * ((y / checkSize) % 2);
                bColor = 255 * (1 - ((y / checkSize) % 2));
            } else {
                bColor = 255 * ((y / checkSize) % 2);
                rColor = 255 * (1 - ((y / checkSize) % 2));
            }

            pixels[(y * width + x) * 3] = rColor;
            pixels[(y * width + x) * 3 + 1] = 0;
            pixels[(y * width + x) * 3 + 2] = bColor;
        }

    return pixels;
}

// Create a mipmapped 2D texture image
GLuint CreateTexture2D() {
    // Texture object handle
    GLuint textureId;
    int width = 256, height = 256;
    GLubyte *pixels;

    pixels = GenCheckImage(width, height, 64);

    if (pixels == NULL) {
        return 0;
    }

    // Generate a texture object
    glGenTextures(1, &textureId);

    // Bind the texture object
    glBindTexture(GL_TEXTURE_2D, textureId);

    // Load mipmap level 0
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
                 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    // Set the filtering mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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
    // Get the sampler location
    userData.samplerLoc = glGetUniformLocation(userData.programObject, "s_texture");

    // Get the offset location
    userData.offsetLoc = glGetUniformLocation(userData.programObject, "u_offset");

    // Load the texture
    userData.textureId = CreateTexture2D();

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

    GLushort indices[] = {0, 1, 2, 0, 2, 3};

    // Load the vertex position
    glVertexAttribPointer(0, 4, GL_FLOAT,
                          GL_FALSE, 6 * sizeof(GLfloat), vVertices);
    // Load the texture coordinate
    glVertexAttribPointer(1, 2, GL_FLOAT,
                          GL_FALSE, 6 * sizeof(GLfloat), &vVertices[4]);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // Bind the texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, userData.textureId);

    // Set the sampler texture unit to 0
    glUniform1i(userData.samplerLoc, 0);

    // Draw quad with repeat wrap mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glUniform1f(userData.offsetLoc, -0.7f);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);

    // Draw quad with clamp to edge wrap mode
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glUniform1f(userData.offsetLoc, 0.0f);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);

    // Draw quad with mirrored repeat
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glUniform1f(userData.offsetLoc, 0.7f);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
}
