//"Copyright 2016 Intel Corporation.
//
//The source code, information and material("Material") contained herein is owned by Intel Corporation or its suppliers or licensors, and title to such Material 
//remains with Intel Corporation or its suppliers or licensors.The Material contains proprietary information of Intel or its suppliers and licensors.
//The Material is protected by worldwide copyright laws and treaty provisions.
//No part of the Material may be used, copied, reproduced, modified, published, uploaded, posted, transmitted,distributed or disclosed in any way without Intel's prior express written permission. 
//No license under any patent, copyright or other intellectual property rights in the Material is granted to or conferred upon you, either expressly, by implication, inducement, estoppel or otherwise. Any license under such intellectual property rights must be express and approved by Intel in writing.
//Unless otherwise agreed by Intel in writing, you may not remove or alter this notice or any other notice embedded in 
//Materials by Intel or Intel's suppliers or licensors in any way."


#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/glut.h>
#include <lodepng.h>

#include <vector>

#include <string>

#include <intrin.h>

// This example uses attribute-less rendering

// Vertex shader specifies vertex position in clip space
static std::string vertexShader =
    "#version 430 core\n"
    "\n"
    "const vec2 Position[4] = vec2[]\n"
    "(\n"
    "    vec2(-1,  1),\n"
    "    vec2(-1, -1),\n"
    "    vec2( 1,  1),\n"
    "    vec2( 1, -1) \n"
    ");"
    "\n"
    "uniform float offset;\n"
    "\n"
    "smooth out vec2 texcoord;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    vec2 pos = Position[ gl_VertexID ];\n"
    "    pos.x += offset * -sign(pos.x);\n"
    "    gl_Position = vec4(pos * 0.5, 0.0, 1.0);\n"
    "    texcoord = pos * vec2(0.5, -0.5) + 0.5;\n"
    "}\n"
;

// Fragment shader gets output color from a texture
static std::string fragmentShader =
    "#version 430 core\n"
    "\n"
    "uniform sampler2D texUnit;\n"
    "\n"
    "smooth in vec2 texcoord;\n"
    "\n"
    "layout(location = 0) out vec4 fragColor;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    fragColor = texture(texUnit, texcoord);\n"
    "}\n"
;

// Static variables, program state
static GLenum err;
static GLuint vShader;
static GLuint fShader;
static GLuint program;
static GLuint texture;
static GLuint fbo[3];
static GLuint rb[2];
static GLint offset, texUnit;
static GLfloat animation;
static unsigned selector, w, h;
static bool swap, animating;

// Array of structures, one item for each option we're testing
#define I(x) { options:: ## x, #x }
static struct options {
    enum  { FBO, SURFACE, nOPTS } option;
    const char* optionStr;
} options[]
{
    I(FBO),
    I(SURFACE),
};

// Debug build performs OpenGL error checking, Release does not
#ifdef _DEBUG
#define GLCHK { if (GL_NO_ERROR != (err=glGetError())) __debugbreak(); }
#else
#define GLCHK
#define __debugbreak() {}
#endif

// Static function to compile an OpenGL shader, check and report errors
static GLuint compileShader(const std::string& src, GLenum type)
{
    GLuint shader = glCreateShader(type);														GLCHK;
    const GLchar* str = src.c_str();  glShaderSource(shader, 1, &str, NULL);					GLCHK;
    glCompileShader(shader);                                                                    GLCHK;
    GLint status; glGetShaderiv(shader, GL_COMPILE_STATUS, &status); if (GL_FALSE == status) {  GLCHK;
        GLint sz; glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &sz);                               GLCHK;
        std::vector<GLchar> v(sz); glGetShaderInfoLog(shader, sz, &sz, &v[0]);                  GLCHK;
        const char* msg = &v[0];  __debugbreak();
        glDeleteShader(shader);                                                                 GLCHK;
        return 0;
    }
    return shader;
}

// Static function to link a set of OpenGL shaders into a program, check and report errors
static GLuint createProgram(std::initializer_list<GLuint> shaders)
{
    GLuint program = glCreateProgram();                                                         GLCHK;
    for (auto shader : shaders) glAttachShader(program, shader);                                GLCHK;
    glLinkProgram(program);                                                                     GLCHK;
    GLint status; glGetProgramiv(program, GL_LINK_STATUS, &status); if (GL_FALSE == status) {   GLCHK;
        GLint sz; glGetProgramiv(program, GL_INFO_LOG_LENGTH, &sz);                             GLCHK;
        std::vector<GLchar> v(sz); glGetProgramInfoLog(program, sz, &sz, &v[0]);                GLCHK;
        const char* msg = &v[0];  __debugbreak();
        glDeleteProgram(program);                                                               GLCHK;
        for (auto shader : shaders) glDeleteShader(shader);                                     GLCHK;
        return 0;
    }
    for (auto shader : shaders) glDetachShader(program, shader);                                GLCHK;
    return program;
}

// Static function to check for minimum OpenGL version (which is 4.3 for now)
static void versionCheck()
{
    const char* s = (const char *)glGetString(GL_VERSION);
    int v[2]; sscanf_s(s, "%d.%d", &v[0], &v[1]);
    if (v[0] < 4 || v[1] < 3) {
        char msg[512]; sprintf_s(msg,
            "Error, Inadequate OpenGL Version!\n\n"
            "This lesson requires OpenGL version 4.3 or better.\n\n"
            "Your version is: %s\n\n"
            "Press Ok to exit the application.", s);
        MessageBox(NULL, msg, "Bad OpenGL Version", MB_OK | MB_ICONERROR);
        exit(0);
    }
}

// GLUT initialization function.   Initialize program state as defined in static variables.
void init()
{
    versionCheck();

    // turn off vsync
    if (!wglSwapIntervalEXT(0))                                                                 __debugbreak();

    // compile and link the shaders into a program, make it active
    vShader = compileShader(vertexShader, GL_VERTEX_SHADER);
    fShader = compileShader(fragmentShader, GL_FRAGMENT_SHADER);
    program = createProgram({ vShader, fShader });
    offset = glGetUniformLocation(program, "offset");                                           GLCHK;
    texUnit = glGetUniformLocation(program, "texUnit");                                         GLCHK;
    glUseProgram(program);                                                                      GLCHK;

    // configure texture unit
    glActiveTexture(GL_TEXTURE0);                                                               GLCHK;
    glUniform1i(texUnit, 0);                                                                    GLCHK;

    // create and configure the textures
    glGenTextures(1, &texture);                                                                 GLCHK;
    glBindTexture(GL_TEXTURE_2D, texture);                                                      GLCHK;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);                               GLCHK;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);                               GLCHK;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);                          GLCHK;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);                          GLCHK;

    // load texture image
    GLuint w, h;  std::vector<GLubyte> img; if (lodepng::decode(img, w, h, "sample.png"))         __debugbreak();

    // upload the image to vram
    glBindTexture(GL_TEXTURE_2D, texture);                                                      GLCHK;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, &img[0]);      GLCHK;

    // create the fbos
    glGenFramebuffers(3, fbo);                                                                  GLCHK;
    glGenRenderbuffers(2, rb);                                                                  GLCHK;
    for (int i = 0; i < 2; ++i) {
        glBindRenderbuffer(GL_RENDERBUFFER, rb[i]);                                             GLCHK;
        glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, 640, 480);                             GLCHK;
    }
    glViewport(0, 0, 640, 480);                                                                 GLCHK;
    glBindRenderbuffer(GL_RENDERBUFFER, 0);                                                     GLCHK;
    for (int i = 0; i < 3; ++i) {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo[i]);                                              GLCHK;
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rb[i % 2]); GLCHK;
        if (GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER))                __debugbreak();
    }

    // restore default framebuffer a.k.a backbuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);                                                       GLCHK;
}

// GLUT display function.   Draw one frame's worth of imagery.
void display()
{
    static unsigned cnt; ++cnt;

    // attributeless rendering
    glClear(GL_COLOR_BUFFER_BIT);                                                               GLCHK;
    glBindTexture(GL_TEXTURE_2D, texture);                                                      GLCHK;
    if (!animating) {
        glViewport(0, 0, 640, 480);                                                             GLCHK;
        glUniform1f(offset, 0.f);                                                               GLCHK;
        if (options::FBO == options[selector].option) {
            glBindFramebuffer(GL_FRAMEBUFFER, fbo[cnt % 2]);                                    GLCHK;
        } else if (options::SURFACE == options[selector].option) {
            glBindFramebuffer(GL_FRAMEBUFFER, fbo[2]);                                          GLCHK;
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rb[cnt % 2]); GLCHK;
        }
    } else {
        glViewport(0, 0, w, h);                                                                 GLCHK;
        glUniform1f(offset, animation);                                                         GLCHK;
    }
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);                                                      GLCHK;
    if (!animating) {
        glReadBuffer(GL_COLOR_ATTACHMENT0);                                                     GLCHK;
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);                                              GLCHK;
        glBlitFramebuffer(0, 0, 640, 480, 0, 0, w, h, GL_COLOR_BUFFER_BIT, GL_LINEAR);          GLCHK;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);                                                   GLCHK;
    }
    glutSwapBuffers();
}

// GLUT reshape function.   Remember the windows width and height
void reshape(int w, int h)
{
    ::w = w; ::h = h;
}

// GLUT keyboard function.  Exit on <esc>, advance to next test item on <space>
void keyboard(unsigned char key, int, int)
{
    // end on <esc> keypress
    if (key == 27) exit(0);
    if (key == ' ') swap = true;
}

// Static function to print currently selected test item's state.  Called every time the user presses <space>.
void print()
{
    printf("\nmeasuring the swapping of %s ...\n", options[selector].optionStr);
}

// Static function.  Calculates elapsed time in microseconds.
static unsigned __int64 elapsedUS(unsigned __int64 now, unsigned __int64 start)
{
    unsigned __int64 freq; if (!QueryPerformanceFrequency((PLARGE_INTEGER)&freq))                               __debugbreak();
    unsigned __int64 elapsed = now >= start ? now - start : _UI64_MAX - start + now;
    unsigned __int64 us = elapsed * 1000000ui64 / freq, sec = elapsed / freq;
    return us;
}

// GLUT idle function.  Called once per video frame.  Calculate and print timing reports and handle console input.
void idle()
{
    // Calculate performance
    static unsigned __int64 skip;  if (++skip < 512) return;
    static unsigned __int64 start; if (!start && !QueryPerformanceCounter((PLARGE_INTEGER)&start))              __debugbreak();
    unsigned __int64 now;  if (!QueryPerformanceCounter((PLARGE_INTEGER)&now))                                  __debugbreak();
    unsigned __int64 us = elapsedUS(now, start), sec = us / 1000000;
    static unsigned __int64 animationStart;
    static unsigned __int64 cnt; ++cnt;

    // We're either animating
    if (animating)
    {
        float sec = elapsedUS(now, animationStart) / 1000000.f; if (sec < 1.f) {
            animation = (sec < 0.5f ? sec : 1.f - sec) / 0.5f;
        } else {
            animating = false;
            selector = (selector + 1) % options::nOPTS; skip = 0;
            cnt = start = 0;
            print();
        }
    }

    // Or measuring
    else if (sec >= 2)
    {
        printf("frames rendered = %I64u, uS = %I64u, fps = %f,  milliseconds-per-frame = %f\n", cnt, us, cnt * 1000000. / us, us / (cnt * 1000.));
        if (swap) {
            animating = true; animationStart = now; swap = false;
        } else {
            cnt = start = 0;
        }
    }

    // Get input from the console too.
    HANDLE h = GetStdHandle(STD_INPUT_HANDLE); INPUT_RECORD r[128]; DWORD n;
    if (PeekConsoleInput(h, r, 128, &n) && n)
        if (ReadConsoleInput(h, r, n, &n))
            for (DWORD i = 0; i < n; ++i)
                if (r[i].EventType == KEY_EVENT && r[i].Event.KeyEvent.bKeyDown)
                    keyboard(r[i].Event.KeyEvent.uChar.AsciiChar, 0, 0);

    // Ask for another frame
    glutPostRedisplay();
}

// Main function, program entry.  Configure, initialize then run application.  Catch and report all unexpected errors
int main(int argc, char** argv)
{
    __try {
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
        glutInitWindowSize(640, 480);
        glutInitWindowPosition(0, 480);
        glutCreateWindow(argv[0]);
        GLenum err = glewInit(); if (GLEW_OK != err)                                                            __debugbreak();
        init();
        glutDisplayFunc(display);
        glutReshapeFunc(reshape);
        glutKeyboardFunc(keyboard);
        glutIdleFunc(idle);
        SetWindowPos(GetConsoleWindow(), NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
        printf("OpenGL vendor string: %s\n", glGetString(GL_VENDOR));
        printf("OpenGL renderer string: %s\n", glGetString(GL_RENDERER));
        printf("OpenGL version string: %s\n\n", glGetString(GL_VERSION));
        puts("This lesson compares the rendering performance between swapping entire FBOs or swapping the surface in a single FBO.");
        puts("Press <esc> to exit; <space bar> to switch between swapping states ...\n");
        print();
        glutMainLoop();
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        MessageBox(NULL,
            "Unhandled Exception!\n\n"
            "An unknown error occurred.\n\n"
            "Press OK to exit.", "Unknown Error", MB_OK | MB_ICONERROR);
    }
    return 0;
}
