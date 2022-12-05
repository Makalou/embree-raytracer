//
// Created by 王泽远 on 2022/4/14.
//

#include "rend/viewer.h"
#include "core/color.h"
#include <thread>

NAMESPACE_BEGIN(ert)
    NAMESPACE_BEGIN(viewer)
        NAMESPACE_BEGIN()
            const char *vertexShaderSource = "#version 330 core\n"
                                             "layout (location = 0) in vec3 aPos;\n"
                                             "layout (location = 1) in vec2 aTexCoords;\n"
                                             "out vec2 TexCoords;\n"
                                             "void main()\n"
                                             "{\n"
                                             "   TexCoords = aTexCoords;\n"
                                             "   gl_Position = vec4(aPos.x, -aPos.y, 0.0, 1.0); \n"
                                             "}\0";
            const char *fragmentShaderSource = "#version 330 core\n"
                                               "out vec4 color;\n"
                                               "in vec2 TexCoords;\n"
                                               "uniform sampler2D Texture;\n"
                                               "void main()\n"
                                               "{\n"
                                               "   vec3 col = texture(Texture, TexCoords).rgb;\n"
                                               "   color = vec4(col, 1.0f);\n"
                                               "}\n\0";

        NAMESPACE_END()

        std::unique_ptr<viewer> viewer::create(std::string_view name) {
            auto v = new viewer;
            if (v->init(name) == 0)
                return std::unique_ptr<viewer>(v);
            return nullptr;
        }

        int viewer::init(std::string_view name) {
            glfwInit();
            if (!glfwInit())
                return -1;

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

            window = glfwCreateWindow(img::image_width, img::image_height, name.data(), nullptr, nullptr);
            if (!window) {

                glfwTerminate();
                return -1;
            }

            glfwMakeContextCurrent(window);

            if (glewInit() != GLEW_OK)
                std::cout << "[GLEW Error][ GLEW could not be initialized ]\n";

            unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
            glCompileShader(vertexShader);

            int success;
            char infoLog[512];
            glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
                std::cout << "[OpenGL Error][ Vertez Shader compilation failed ]\n" << infoLog << '\n';
            }

            unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
            glCompileShader(fragmentShader);

            glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
                std::cout << "[OpenGL Error][ Fragment Shader compilation failed ]\n" << infoLog << '\n';
            }

            shaderProgram = glCreateProgram();
            glAttachShader(shaderProgram, vertexShader);
            glAttachShader(shaderProgram, fragmentShader);
            glLinkProgram(shaderProgram);

            glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
                std::cout << "[OpenGL Error][ Shader Program linking failed ]\n" << infoLog << '\n';
            }
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
                    // positions   // texCoords
                    -1.0f, 1.0f, 0.0f, 1.0f,
                    -1.0f, -1.0f, 0.0f, 0.0f,
                    1.0f, -1.0f, 1.0f, 0.0f,

                    -1.0f, 1.0f, 0.0f, 1.0f,
                    1.0f, -1.0f, 1.0f, 0.0f,
                    1.0f, 1.0f, 1.0f, 1.0f
            };

            // screen quad VAO
            glGenVertexArrays(1, &quadVAO);
            glGenBuffers(1, &quadVBO);
            glBindVertexArray(quadVAO);
            glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) (2 * sizeof(float)));

            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            // 为当前绑定的纹理对象设置环绕、过滤方式
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            return 0;
        }

        void viewer::mainloop() {
            while (!glfwWindowShouldClose(window)) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img::image_width, img::image_height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                             img::pixels);
                glClearColor(0, 0, 0, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                glUseProgram(shaderProgram);

                glBindVertexArray(quadVAO);
                glDrawArrays(GL_TRIANGLES, 0, 6);

                glfwSwapBuffers(window);
                glfwPollEvents();
                //std::this_thread::sleep_for(std::chrono::seconds(5));
                //std::this_thread::yield();
            }
            glDeleteVertexArrays(1, &quadVAO);
            glDeleteBuffers(1, &quadVBO);
            glDeleteProgram(shaderProgram);

            glfwTerminate();
        }
    NAMESPACE_END(viewer)
NAMESPACE_END(ert)

