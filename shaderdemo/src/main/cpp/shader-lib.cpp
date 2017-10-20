#include <jni.h>
#include <android/log.h>
#include <GLES3/gl3.h>
#include <stdlib.h>

#define LOG_TA "SHADER-LIB"
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TA, __VA_ARGS__)

namespace shader {


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
                    "uniform vec4 ourColor;\n"
                    "void main()\n"
                    "{\n"
                    "fragColor = ourColor;"
                    "}\n"
    };
    static float vertices[]{
            -0.5f, -0.5, 0,
            0.5f, -0.5f, 0,
            0, 0.5f, 0
    };

    GLuint loadShader(GLenum type, const char *shaderSrc) {
        GLuint shader;
        GLint compiled = GL_FALSE;

        //Create Shader Object
        shader = glCreateShader(type);
        if (shader == 0) {
            return 0;
        }

        //Load the shader source
        glShaderSource(shader, 1, &shaderSrc, NULL);

        //Compile Shader
        glCompileShader(shader);

        //Check the compile status
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

        //ALOGE("compile result is%d",compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen > 1) {
                char *infoLog = (char *) malloc(sizeof(char) * infoLen);
                glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
                ALOGE("Could not compile %s shader:\n%s\n",
                      type == GL_VERTEX_SHADER ? "vertex" : "fragment",
                      infoLog);
                free(infoLog);
            }
        }
        return shader;
    }

    GLint createProgram(const char *vertexShader, const char *fragmentShader) {
        GLuint program, vertex, fragment;

        program = glCreateProgram();
        if (program == 0) {
            return 0;
        }
        vertex = loadShader(GL_VERTEX_SHADER, vertexShader);
        fragment = loadShader(GL_FRAGMENT_SHADER, fragmentShader);

        glAttachShader(program, vertex);
        glAttachShader(program, fragment);

        //link the program
        glLinkProgram(program);
        GLint linked;
        //check link
        glGetProgramiv(program, GL_LINK_STATUS, &linked);

        if (!linked) {
            GLint infoLen = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen > 0) {
                char *logInfo = (char *) malloc(sizeof(char) * infoLen);
                glGetProgramInfoLog(program, infoLen, NULL, logInfo);
                ALOGE("error in create program %s\n", logInfo);
                free(logInfo);
            }
        }
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        //glDeleteProgram(program);
        return program;
    }

    extern "C" {
    JNIEXPORT jboolean JNICALL
    Java_com_lyman_shaderdemo_lib_ShaderLib_init();
    JNIEXPORT void JNICALL
    Java_com_lyman_shaderdemo_lib_ShaderLib_resize(JNIEnv *env, jobject obj, jint width,
                                                   jint height);
    JNIEXPORT void JNICALL
    Java_com_lyman_shaderdemo_lib_ShaderLib_draw(JNIEnv *env, jobject obj);
    }


    GLint program;

    JNIEXPORT jboolean JNICALL
    Java_com_lyman_shaderdemo_lib_ShaderLib_init() {
//    ALOGE("程序开始初始化");
        ALOGE("start init");
        program = createProgram(VERTEX_SHADER, FRAGMENT_SHADER);
        if (!program) {
            ALOGE("init program fail");
        }
        glClearColor(1.0, 1.0, 1.0, 1.0);
        GLint nrAttributes;
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
        ALOGE("Vertex attribute maximum is %d", nrAttributes);
        ALOGE("end init");
        return JNI_TRUE;
    }

    JNIEXPORT void JNICALL
    Java_com_lyman_shaderdemo_lib_ShaderLib_resize(JNIEnv *env, jobject obj, jint wight,
                                                   jint height) {
        glViewport(0, 0, wight, height);
    }

    JNIEXPORT void JNICALL
    Java_com_lyman_shaderdemo_lib_ShaderLib_draw(JNIEnv *env, jobject obj) {
        //清楚颜色缓冲
        glClear(GL_COLOR_BUFFER_BIT);

        //激活着色器
        glUseProgram(program);

        //update uniform color data
        int vertexColorLocation = glGetUniformLocation(program, "ourColor");
        glUniform4f(vertexColorLocation, 0.5f, 0.5f, 0.0f, 0.5f);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
        glEnableVertexAttribArray(0);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
}