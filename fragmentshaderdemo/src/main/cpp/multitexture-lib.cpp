//
// Created by 叶立民 on 2017/10/17.
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
                "uniform sampler2D s_baseMap;                        \n"
                "uniform sampler2D s_lightMap;                       \n"
                "void main()                                         \n"
                "{                                                   \n"
                "  vec4 baseColor;                                   \n"
                "  vec4 lightColor;                                  \n"
                "                                                    \n"
                "  baseColor = texture( s_baseMap, v_texCoord );     \n"
                "  lightColor = texture( s_lightMap, v_texCoord );   \n"
                "  outColor = baseColor * (lightColor + 0.25);       \n"
                "}                                                   \n";

static const GLfloat vVertices[] = {
        -0.5f, 0.5f, 0.0f,  // Position 0
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

    // Sampler locations
    GLint baseMapLoc;
    GLint lightMapLoc;

    // Texture handle
    GLuint baseMapTexId;
    GLuint lightMapTexId;

} UserData;

// Load texture from disk
//
GLuint LoadTexture(void *ioContext, char *fileName) {
    int width, height;

    char *buffer = esLoadTGA(ioContext, fileName, &width, &height);
    GLuint texId;

    if (buffer == NULL) {
        ALOGE("Error loading (%s) image.\n", fileName);
        return 0;
    }

    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    free(buffer);

    return texId;
}


extern "C" {
JNIEXPORT jboolean JNICALL
Java_com_lyman_fragmentshaderdemo_lib_FragmentShaderLib_init(JNIEnv *env, jclass cls,
                                                             jobject assetManager);
JNIEXPORT void JNICALL
Java_com_lyman_fragmentshaderdemo_lib_FragmentShaderLib_resize(jint width, jint height);
JNIEXPORT void JNICALL
Java_com_lyman_fragmentshaderdemo_lib_FragmentShaderLib_draw();
}

UserData userData;

JNIEXPORT jboolean
JNICALL
Java_com_lyman_fragmentshaderdemo_lib_FragmentShaderLib_init(JNIEnv *env, jclass cls,
                                                             jobject assetManager) {
    userData.programObject = createProgram(VERTEX_SHADER, FRAGMENT_SHADER);
    if (userData.programObject == 0) {
        ALOGE("Init failed");
        return JNI_FALSE;
    }
    // Get the sampler location
    userData.baseMapLoc = glGetUniformLocation(userData.programObject, "s_baseMap");
    userData.lightMapLoc = glGetUniformLocation(userData.programObject, "s_lightMap");

    // Load the textures
    AAssetManager *myAAsetManager = AAssetManager_fromJava(env, assetManager);
    userData.baseMapTexId = LoadTexture(myAAsetManager, "basemap.tga");
    userData.lightMapTexId = LoadTexture(myAAsetManager, "lightmap.tga");

    if (userData.baseMapTexId == 0 || userData.lightMapTexId == 0) {
        return GL_FALSE;
    }

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    return JNI_TRUE;
}

JNIEXPORT void JNICALL
Java_com_lyman_fragmentshaderdemo_lib_FragmentShaderLib_resize(jint width, jint height) {
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);
}

JNIEXPORT void JNICALL
Java_com_lyman_fragmentshaderdemo_lib_FragmentShaderLib_draw() {
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(userData.programObject);

    GLushort indices[] = {0, 1, 2, 0, 2, 3};
    // Use the program object
    glUseProgram(userData.programObject);

    // Load the vertex position
    glVertexAttribPointer(0, 3, GL_FLOAT,
                          GL_FALSE, 5 * sizeof(GLfloat), vVertices);
    // Load the texture coordinate
    glVertexAttribPointer(1, 2, GL_FLOAT,
                          GL_FALSE, 5 * sizeof(GLfloat), &vVertices[3]);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // Bind the base map
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, userData.baseMapTexId);

    // Set the base map sampler to texture unit to 0
    glUniform1i(userData.baseMapLoc, 0);

    // Bind the light map
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, userData.lightMapTexId);

    // Set the light map sampler to texture unit 1
    glUniform1i(userData.lightMapLoc, 1);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);

}
