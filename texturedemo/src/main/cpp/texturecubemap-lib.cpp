//
// Created by 叶立民 on 2017/10/17.
#include "esUtil.h"

static const char VERTEX_SHADER[] =
        "#version 300 es                            \n"
                "layout(location = 0) in vec4 a_position;   \n"
                "layout(location = 1) in vec3 a_normal;     \n"
                "out vec3 v_normal;                         \n"
                "void main()                                \n"
                "{                                          \n"
                "   gl_Position = a_position;               \n"
                "   v_normal = a_normal;                    \n"
                "}                                          \n";


static const char FRAGMENT_SHADER[] =
        "#version 300 es                                     \n"
                "precision mediump float;                            \n"
                "in vec3 v_normal;                                   \n"
                "layout(location = 0) out vec4 outColor;             \n"
                "uniform samplerCube s_texture;                      \n"
                "void main()                                         \n"
                "{                                                   \n"
                "   outColor = texture( s_texture, v_normal );       \n"
                "}                                                   \n";

typedef struct {
    // Handle to a program object
    GLuint programObject;

    // Sampler location
    GLint samplerLoc;

    // Texture handle
    GLuint textureId;

    // Vertex data
    int numIndices;
    GLfloat *vertices;
    GLfloat *normals;
    GLuint *indices;

} UserData;

// Create a simple cubemap with a 1x1 face with a different
// color for each face
GLuint CreateSimpleTextureCubemap() {
    GLuint textureId;
    // Six 1x1 RGB faces
    GLubyte cubePixels[6][3] =
            {
                    // Face 0 - Red
                    255, 0, 0,
                    // Face 1 - Green,
                    0, 255, 0,
                    // Face 2 - Blue
                    0, 0, 255,
                    // Face 3 - Yellow
                    255, 255, 0,
                    // Face 4 - Purple
                    255, 0, 255,
                    // Face 5 - White
                    255, 255, 255
            };

    // Generate a texture object
    glGenTextures(1, &textureId);

    // Bind the texture object
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

    // Load the cube face - Positive X
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, 1, 1, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, &cubePixels[0]);

    // Load the cube face - Negative X
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, 1, 1, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, &cubePixels[1]);

    // Load the cube face - Positive Y
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, 1, 1, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, &cubePixels[2]);

    // Load the cube face - Negative Y
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, 1, 1, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, &cubePixels[3]);

    // Load the cube face - Positive Z
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, 1, 1, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, &cubePixels[4]);

    // Load the cube face - Negative Z
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, 1, 1, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, &cubePixels[5]);

    // Set the filtering mode
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

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

    // Load the texture
    userData.textureId = CreateSimpleTextureCubemap();

    // Generate the vertex data
    userData.numIndices = esGenSphere(20, 0.75f, &userData.vertices, &userData.normals,
                                      NULL, &userData.indices);

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

    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    // Use the program object
    glUseProgram(userData.programObject);

    // Load the vertex position
    glVertexAttribPointer(0, 3, GL_FLOAT,
                          GL_FALSE, 0, userData.vertices);
    // Load the normal
    glVertexAttribPointer(1, 3, GL_FLOAT,
                          GL_FALSE, 0, userData.normals);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // Bind the texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, userData.textureId);

    // Set the sampler texture unit to 0
    glUniform1i(userData.samplerLoc, 0);

    glDrawElements(GL_TRIANGLES, userData.numIndices,
                   GL_UNSIGNED_INT, userData.indices);
}
