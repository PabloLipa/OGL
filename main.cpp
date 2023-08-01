#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <iostream>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <GLFW/glfw3.h>

#include"dep/shader.hpp"
#include"dep/CamContr.hpp"
#include"dep/objLoader.hpp"

GLFWwindow* window;

int main(void)
{
    

    /* Initialize the library */
    glfwInit();
    //glewInit();
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(700, 600, "OpenGLAPP", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    


    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Erro ao inicializar o GLEW" << std::endl;
        glfwTerminate();
        return 1;
    }
    std::cout << "OpenGL Vendor   : " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "OpenGL Renderer : " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL Version  : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL Version    : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //aplica o cursor para o centro
    glfwPollEvents();
    glfwSetCursorPos(window, 700 / 2, 600 / 2);

    glClearColor(0.01f, 0.05f, 0.01f, 0);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // criar o programa e compilar os shaders apartir dos arquivos GLSL
    GLuint ProgramID = LoadShaders("ShadersF/vertex.glsl", "ShadersF/fragment.glsl");

    GLuint MatrixP = glGetUniformLocation(ProgramID, "MVP");

    //ler o modelo
    std::vector<glm::vec3> vertice;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    bool valobj = loadOBJ("cow.obj", vertice, uvs, normals);
    //carregar o modelo para o VBO
    GLuint vb;
    glGenBuffers(1, &vb);
    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glBufferData(GL_ARRAY_BUFFER, vertice.size() * sizeof(glm::vec3), &vertice[0], GL_STATIC_DRAW);

    GLuint uvsb;
    glGenBuffers(1, &uvsb);
    glBindBuffer(GL_ARRAY_BUFFER, uvsb);
    glBufferData(GL_ARRAY_BUFFER, vertice.size() * sizeof(glm::vec3), &uvs[0], GL_STATIC_DRAW);

    

    /* Loop until the user closes the window */
    do {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(ProgramID);

        computeMatricesFromInputs();
        glm::mat4 ProjectionMatrix = getProjectionMatrix();
        glm::mat4 ViewMatrix = getViewMatrix();
        glm::mat4 ModelMatrix = glm::mat4(1.0);
        glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

        glUniformMatrix4fv(MatrixP, 1, GL_FALSE, &MVP[0][0]);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vb);
        glVertexAttribPointer(
            0,                  
            3,                  
            GL_FLOAT,           
            GL_FALSE,           
            0,                  
            (void*)0            
        );

        //UVs
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvsb);
        glVertexAttribPointer(
            1,                                
            2,                                
            GL_FLOAT,                         
            GL_FALSE,                         
            0,                                
            (void*)0                          
        );

        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, vertice.size());

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    while ( glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

    // limpeza VBO e Shaders
    glDeleteBuffers(1, &vb);
    glDeleteBuffers(1, &uvsb);
    glDeleteProgram(ProgramID);
    glDeleteVertexArrays(1, &VertexArrayID);
    glfwTerminate();
    return 0;
}
