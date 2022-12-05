//
// Created by 王泽远 on 2022/1/27.
//

#ifndef MAIN_CPP_VIEWER_H
#define MAIN_CPP_VIEWER_H

#include "glew/include/GL/glew.h"
#include <GLFW/glfw3.h>

#include "core/utility.h"

NAMESPACE_BEGIN(ert)
    NAMESPACE_BEGIN(viewer)

        class viewer {
        public:
            static std::unique_ptr<viewer> create(std::string_view);

            viewer(const viewer &) = delete;

            viewer &operator=(const viewer &) = delete;

            void mainloop();

        private:
            viewer() = default;

            int init(std::string_view);

            GLFWwindow *window;
            unsigned int shaderProgram;
            unsigned int quadVAO;
            unsigned int quadVBO;
        };

    NAMESPACE_END(viewer)
NAMESPACE_END(ert)
#endif //MAIN_CPP_VIEWER_H
