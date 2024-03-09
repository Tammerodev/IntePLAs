#pragma once
#include <SFML/OpenGL.hpp>
#include <GL/glu.h>
#include "Type.hpp"

namespace dep {
    namespace opengl {

        struct OpenGLSettings {
            GLdouble fov = 90.f;
            GLdouble zfar = 1000.f;
            dep::Color clearColor = {0.f, 0.f, 0.f, 0.f};
        };

        void initOpenGL(OpenGLSettings settings) {
            glClearDepth(1.f);
            glClearColor(settings.clearColor.r,
                         settings.clearColor.g,
                         settings.clearColor.b,
                         settings.clearColor.a);

            // Enable Z-buffer read and write
            glEnable(GL_DEPTH_TEST);
            glDepthMask(GL_TRUE);

            // Setup a perspective projection
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluPerspective(settings.fov, 1.f, 1.f, settings.zfar);
        }
    }
}