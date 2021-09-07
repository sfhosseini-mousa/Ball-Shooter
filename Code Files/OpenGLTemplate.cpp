#include <glad/glad.h>
#include <GLFW/glfw3.h>



#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "shader.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void applyTexture1(unsigned int& texture1);
void applyTexture2(unsigned int& texture2);
void applyTexture3(unsigned int& texture3);
void applyTexture4(unsigned int& texture4);
void applyTexture5(unsigned int& texture5);
void applyTexture6(unsigned int& texture6);


void processInput(GLFWwindow* window);
void init(void);
void render();

void tranformations(Shader& ourShader, Shader& lightCubeShader, Shader& twoTextureShader);

// variables
const unsigned int screen_width = 1024;
const unsigned int screen_height = 768;
const GLuint NumVertices = 12;

GLuint VBO[6];
GLuint VAO[6];
GLuint EBO[6];

GLuint VBORect1, VBORect2, VBORect3;
GLuint VAORect1, VAORect2, VAORect3;
GLuint EBORect1, EBORect2, EBORect3;

unsigned int texture1, texture2, texture3, texture4, texture5, texture6, texture7, texture8;

bool canDoIt = true;
bool moveBool = false;

bool isZero = true;

glm::vec3 lightPos(0.0f, 0.0f, 2.0f);


glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -5.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

void texture1Rendering();
void texture2Rendering();



int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window;
    window = glfwCreateWindow(screen_width, screen_height, "Midterm", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    Shader ourShader("shader.vs", "shader.fs");
    Shader lightCubeShader("lightCube.vs", "lightCube.fs");
    Shader twoTextureShader("twoTexture.vs", "twoTexture.fs");

    glEnable(GL_BLEND);// enable the blending function
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    init();

    ourShader.use(); // activate the shader
// pass the uniform variables
    glUniform1i(glGetUniformLocation(lightCubeShader.ID, "texture4"), 0);
  
    // render loop

    while (!glfwWindowShouldClose(window))
    {
        
        lightPos.x = (sin(glfwGetTime()) * 1.0f);
        lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;
        processInput(window);
        render();

        tranformations(ourShader, lightCubeShader, twoTextureShader);



        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glDeleteVertexArrays(1, &VAO[0]);
    glDeleteBuffers(1, &VBO[0]);

    glDeleteVertexArrays(1, &VAO[1]);
    glDeleteBuffers(1, &VBO[1]);

    glDeleteVertexArrays(1, &VAO[2]);
    glDeleteBuffers(1, &VBO[2]);

    glDeleteVertexArrays(1, &VAO[3]);
    glDeleteBuffers(1, &VBO[3]);

    glDeleteVertexArrays(1, &VAO[4]);
    glDeleteBuffers(1, &VBO[4]);

    glDeleteVertexArrays(1, &VAO[5]);
    glDeleteBuffers(1, &VBO[5]);


    //////     //////     //////     //////     //////    


    glDeleteVertexArrays(1, &VAORect1);
    glDeleteBuffers(1, &VBORect1);

    glDeleteVertexArrays(1, &VAORect2);
    glDeleteBuffers(1, &VBORect2);

    glDeleteVertexArrays(1, &VAORect3);
    glDeleteBuffers(1, &VBORect3);
    glfwTerminate();
    return 0;
}

void tranformations(Shader& ourShader, Shader& lightCubeShader, Shader& twoTextureShader)
{

    //////     //////     //////    SHADER    //////     //////     //////      

    ourShader.use();
    ourShader.setVec3("light.position", lightPos);
    ourShader.setVec3("viewPos", cameraPos);

    // light properties
    glm::vec3 lightColor;
    lightColor.x = 1.0f;
    lightColor.y = 1.0f;
    lightColor.z = 1.0f;

    glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
    glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);
    ourShader.setVec3("light.ambient", ambientColor);
    ourShader.setVec3("light.diffuse", diffuseColor);
    ourShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

    ourShader.setVec3("material.ambient", 0.25f, 0.25f, 0.25f);
    ourShader.setVec3("material.diffuse", 0.4f, 0.4f, 0.4f);
    ourShader.setVec3("material.specular", 0.774597f, 0.774597f, 0.774597f);
    ourShader.setFloat("material.shininess", 76.8f);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    view = glm::rotate(view, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    // view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    projection = glm::perspective(glm::radians(55.0f), (float)screen_width / (float)screen_height, 0.1f, 100.0f);

    ourShader.setMat4("projection", projection);
    ourShader.setMat4("view", view);
    ourShader.setMat4("model", model);

    //////     //////     //////   LIGHTCUBE SHADER    //////     //////     //////      

    lightCubeShader.use();
    glm::vec3 lightColor2;
    lightColor2.x = sin(glfwGetTime() * 2.0f);
    lightColor2.y = sin(glfwGetTime() * 0.7f);
    lightColor2.z = sin(glfwGetTime() * 1.3f);

    glm::vec3 diffuseColor2 = lightColor2 * glm::vec3(1.0f);
    glm::vec3 ambientColor2 = diffuseColor2 * glm::vec3(0.5f);
    lightCubeShader.setVec3("light.ambient", ambientColor2);
    lightCubeShader.setVec3("light.diffuse", diffuseColor2);
    lightCubeShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

    lightCubeShader.setVec3("material.ambient", 0.25f, 0.25f, 0.25f);
    lightCubeShader.setVec3("material.diffuse", 0.4f, 0.4f, 0.4f);
    lightCubeShader.setVec3("material.specular", 0.774597f, 0.774597f, 0.774597f);
    lightCubeShader.setFloat("material.shininess", 76.8f);
    lightCubeShader.setMat4("projection", projection);
    lightCubeShader.setMat4("view", view);
    lightCubeShader.setMat4("model", model);


    twoTextureShader.use();
    glm::vec3 diffuseColor3 = lightColor * glm::vec3(1.0f);
    glm::vec3 ambientColor3 = diffuseColor3 * glm::vec3(0.5f);
    twoTextureShader.setVec3("light.ambient", ambientColor3);
    twoTextureShader.setVec3("light.diffuse", diffuseColor3);
    twoTextureShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

    twoTextureShader.setVec3("material.ambient", 0.25f, 0.25f, 0.25f);
    twoTextureShader.setVec3("material.diffuse", 0.4f, 0.4f, 0.4f);
    twoTextureShader.setVec3("material.specular", 0.774597f, 0.774597f, 0.774597f);
    twoTextureShader.setFloat("material.shininess", 76.8f);
    twoTextureShader.setMat4("projection", projection);
    twoTextureShader.setMat4("view", view);
    twoTextureShader.setMat4("model", model);

    glUniform1i(glGetUniformLocation(twoTextureShader.ID, "texture1"), 0);
    twoTextureShader.setInt("texture2", 1);

    //////     //////     //////    BLOCKS    //////    //////     //////   

    applyTexture1(texture1);

    ourShader.use();
    glBindVertexArray(VAO[0]);
    for (unsigned int i = 0; i < 7; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.3f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(0.0 + i - i * 0.69f, 0.6f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.2f, 0.1f));
        ourShader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    applyTexture2(texture2);

    glBindVertexArray(VAO[1]);
    for (unsigned int i = 0; i < 7; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.3f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(0.0 + i - i * 0.69f, 0.4f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.2f, 0.1f));
        ourShader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
    applyTexture3(texture3);

    glBindVertexArray(VAO[2]);
    for (unsigned int i = 0; i < 7; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.3f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(0.0 + i - i * 0.69f, 0.2f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.2f, 0.1f));
        ourShader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    applyTexture1(texture1);

    glBindVertexArray(VAO[3]);
    for (unsigned int i = 0; i < 7; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.3f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(0.0 + i - i * 0.69f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.2f, 0.1f));
        ourShader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
    
    applyTexture2(texture2);

    glBindVertexArray(VAO[4]);
    for (unsigned int i = 0; i < 7; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.3f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(0.0 + i - i * 0.69f, -0.2f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.2f, 0.1f));
        ourShader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
    applyTexture3(texture3);

    glBindVertexArray(VAO[5]);
    for (unsigned int i = 0; i < 7; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.3f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(0.0 + i - i * 0.69f, -0.4f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.2f, 0.1f));
        ourShader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    twoTextureShader.use();
    texture1Rendering();
    texture2Rendering();

    //////     //////     //////     PADDLE       //////    //////     //////   
    applyTexture5(texture5);

    glBindVertexArray(VAORect1);
    for (unsigned int i = 0; i < 1; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.3f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(0.78f, -1.4f, 0.0f));
        model = glm::scale(model, glm::vec3(0.7f, 0.2f, 0.1f));
        ourShader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
    //////     //////     //////     BALL       //////    //////     //////   
    applyTexture6(texture6);
  

    glBindVertexArray(VAORect2);
   

    for (unsigned int i = 0; i < 1; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.3f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(1.15f, -1.2f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.1f));

        if (canDoIt) {
          if (moveBool) {
            if (isZero) {
              glfwSetTime(0);

              isZero = false;
            }

            model = glm::translate(model, glm::vec3(0.0, glfwGetTime() * 0.6f, 0.0f));
          }
        }
        ourShader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    //////     //////     //////     LIGHTCUBE    //////    //////     //////   
    applyTexture4(texture4);

    lightCubeShader.use();
    glBindVertexArray(VAORect3);
    for (unsigned int i = 0; i < 1; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(-1.3f, 0.0f, 1.0f));
        model = glm::translate(model, lightPos);
        //model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.1f));
        lightCubeShader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }




    ourShader.use();

    unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
    unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

    ourShader.setMat4("projection", projection);


}

void render()
{
    static const float black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    glClearBufferfv(GL_COLOR, 0, black);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


}

void init(void)
{


    float verticesCube[] =
    {
          0.5f, -0.5f, -0.5f,             1.0f, 0.0f, 0.0f,         1.0f, 1.0f,
          1.0f, -0.5f, -0.5f,             1.0f, 0.0f, 0.0f,         1.0f, 0.0f,
          1.0f, -1.0f, -0.5f,             1.0f, 0.0f, 0.0f,         0.0f, 0.0f,
          0.5f, -1.0f, -0.5f,             1.0f, 0.0f, 0.0f,         0.0f, 1.0f,
          0.5f, -0.5f,  0.0f,             1.0f, 0.0f, 0.0f,         1.0f, 1.0f,
          1.0f, -0.5f,  0.0f,             1.0f, 0.0f, 0.0f,         1.0f, 0.0f,
          1.0f, -1.0f,  0.0f,             1.0f, 0.0f, 0.0f,         0.0f, 0.0f,
          0.5f, -1.0f,  0.0f,             1.0f, 0.0f, 0.0f,         0.0f, 1.0f
    };



    float verticesLight[] =
    {
          0.1f, -0.1f, -0.1f,             1.0f, 1.0f, 1.0f,         1.0f, 1.0f,
          0.2f, -0.1f, -0.1f,             1.0f, 1.0f, 1.0f,         1.0f, 0.0f,
          0.2f, -0.2f, -0.1f,             1.0f, 1.0f, 1.0f,         0.0f, 0.0f,
          0.1f, -0.2f, -0.1f,             1.0f, 1.0f, 1.0f,         0.0f, 1.0f,
          0.1f, -0.1f,  0.0f,             1.0f, 1.0f, 1.0f,         1.0f, 1.0f,
          0.2f, -0.1f,  0.0f,             1.0f, 1.0f, 1.0f,         1.0f, 0.0f,
          0.2f, -0.2f,  0.0f,             1.0f, 1.0f, 1.0f,         0.0f, 0.0f,
          0.1f, -0.2f,  0.0f,             1.0f, 1.0f, 1.0f,         0.0f, 1.0f
    };

    int indicesCube[] = {

           5, 4, 0,
           1, 5, 0,
           6, 5, 1,
           2, 6, 1,
           7, 6, 2,
           3, 7, 2,
           4, 7, 3,
           0, 4, 3,
           6, 7, 4,
           5, 6, 4,
           1, 0, 3,
           2, 1, 3
    };
    /////// /////// /////     /////// First Set /////// /////    /////// /////// /////
    //first set
    glGenVertexArrays(1, &VAO[0]);
    glGenBuffers(1, &VBO[0]);
    glGenBuffers(1, &EBO[0]);

    glBindVertexArray(VAO[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCube), verticesCube, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesCube), indicesCube, GL_STATIC_DRAW);


    // position attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attributes
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute information
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
    /////// /////// /////     /////// Second /////// /////    /////// /////// /////
    //second set
    glGenVertexArrays(1, &VAO[1]);
    glGenBuffers(1, &VBO[1]);
    glGenBuffers(1, &EBO[1]);

    glBindVertexArray(VAO[1]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCube), verticesCube, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesCube), indicesCube, GL_STATIC_DRAW);


    // position attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attributes
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute information
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    /////// /////// /////     /////// Third /////// /////    /////// /////// /////
    //third set
    glGenVertexArrays(1, &VAO[2]);
    glGenBuffers(1, &VBO[2]);
    glGenBuffers(1, &EBO[2]);

    glBindVertexArray(VAO[2]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCube), verticesCube, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesCube), indicesCube, GL_STATIC_DRAW);


    // position attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attributes
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute information
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    /////// /////// /////     /////// Fourth /////// /////    /////// /////// /////
    //fourth set
    glGenVertexArrays(1, &VAO[3]);
    glGenBuffers(1, &VBO[3]);
    glGenBuffers(1, &EBO[3]);

    glBindVertexArray(VAO[3]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCube), verticesCube, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesCube), indicesCube, GL_STATIC_DRAW);


    // position attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attributes
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute information
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    /////// /////// /////     /////// Fifth /////// /////    /////// /////// /////
    //fifth set
    glGenVertexArrays(1, &VAO[4]);
    glGenBuffers(1, &VBO[4]);
    glGenBuffers(1, &EBO[4]);

    glBindVertexArray(VAO[4]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCube), verticesCube, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[4]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesCube), indicesCube, GL_STATIC_DRAW);


    // position attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attributes
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute information
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    /////// /////// /////     /////// Six /////// /////    /////// /////// /////
    //sixth set
    glGenVertexArrays(1, &VAO[5]);
    glGenBuffers(1, &VBO[5]);
    glGenBuffers(1, &EBO[5]);

    glBindVertexArray(VAO[5]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCube), verticesCube, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[5]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesCube), indicesCube, GL_STATIC_DRAW);


    // position attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attributes
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute information
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    /////// /////// /////     /////// PADDLE /////// /////    /////// /////// /////

    glGenVertexArrays(1, &VAORect1);
    glGenBuffers(1, &VBORect1);
    glGenBuffers(1, &EBORect1);
    glBindVertexArray(VAORect1);

    glBindBuffer(GL_ARRAY_BUFFER, VBORect1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCube), verticesCube, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBORect1);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesCube), indicesCube, GL_STATIC_DRAW);


    // position attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attributes
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute information
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    /////// /////// /////     /////// BALL  /////// /////    /////// /////// /////


    glGenVertexArrays(1, &VAORect2);
    glGenBuffers(1, &VBORect2);
    glGenBuffers(1, &EBORect2);
    glBindVertexArray(VAORect2);

    glBindBuffer(GL_ARRAY_BUFFER, VBORect2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCube), verticesCube, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBORect2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesCube), indicesCube, GL_STATIC_DRAW);


    // position attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attributes
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute information
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    /////// /////// /////     /////// LIGHT SOURCE  /////// /////    /////// /////// /////


    glGenVertexArrays(1, &VAORect3);
    glGenBuffers(1, &VBORect3);
    glGenBuffers(1, &EBORect3);
    glBindVertexArray(VAORect3);

    glBindBuffer(GL_ARRAY_BUFFER, VBORect3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesLight), verticesLight, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBORect3);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesCube), indicesCube, GL_STATIC_DRAW);


    // position attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attributes
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute information
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

}


void applyTexture1(unsigned int& texture1)
{
    glGenTextures(1, &texture1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1); // the texture object is applied with all the texture operations
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set GL_REPEAT as the wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // load image (mybox.png) and create the texture 
    int width, height, nrChannels;
    unsigned char* data = stbi_load("assets/Blue.jpg", &width, &height, &nrChannels, 0);
    // Generate mipmaps
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}



void applyTexture2(unsigned int& texture2)
{
    glGenTextures(1, &texture2);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture2); // the texture object is applied with all the texture operations
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set GL_REPEAT as the wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // load image (mybox.png) and create the texture 
    int width, height, nrChannels;
    unsigned char* data = stbi_load("assets/Green.jpg", &width, &height, &nrChannels, 0);
    // Generate mipmaps
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

void applyTexture3(unsigned int& texture3)
{
    glGenTextures(1, &texture3);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture3); // the texture object is applied with all the texture operations
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set GL_REPEAT as the wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // load image (mybox.png) and create the texture 
    int width, height, nrChannels;
    unsigned char* data = stbi_load("assets/Orange.jpg", &width, &height, &nrChannels, 0);
    // Generate mipmaps
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}



void applyTexture4(unsigned int& texture4)
{
    glGenTextures(1, &texture4);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture3); // the texture object is applied with all the texture operations
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set GL_REPEAT as the wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // load image (mybox.png) and create the texture 
    int width, height, nrChannels;
    unsigned char* data = stbi_load("assets/ghostWhite.jpg", &width, &height, &nrChannels, 0);
    // Generate mipmaps
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}



void applyTexture5(unsigned int& texture5)
{
    glGenTextures(1, &texture5);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture3); // the texture object is applied with all the texture operations
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set GL_REPEAT as the wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // load image (mybox.png) and create the texture 
    int width, height, nrChannels;
    unsigned char* data = stbi_load("assets/Purple.jpg", &width, &height, &nrChannels, 0);
    // Generate mipmaps
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}




void applyTexture6(unsigned int& texture6)
{
    glGenTextures(1, &texture6);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture6); // the texture object is applied with all the texture operations
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set GL_REPEAT as the wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // load image (mybox.png) and create the texture 
    int width, height, nrChannels;
    unsigned char* data = stbi_load("assets/Red.jpg", &width, &height, &nrChannels, 0);
    // Generate mipmaps
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

void texture2Rendering()
{
  glGenTextures(1, &texture2);
  glActiveTexture(GL_TEXTURE1);

  glBindTexture(GL_TEXTURE_2D, texture2);
  // texture wrapping params
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set wrapping to GL_REPEAR
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // filtering params
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load image (smilie.png) and create the texture 
  int width, height, nrChannels;

  unsigned char* data1 = stbi_load("assets/smilie.png", &width, &height, &nrChannels, 0);
  if (data1)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data1);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data1);
}

void texture1Rendering()
{
  // texture 1
  // ---------
  glGenTextures(1, &texture1);
  glActiveTexture(GL_TEXTURE0);

  glBindTexture(GL_TEXTURE_2D, texture1);
  // set the texture wrapping parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load image (smilie.png) and create the texture 
  int width, height, nrChannels;

  unsigned char* data = stbi_load("assets/mybox.png", &width, &height, &nrChannels, 0);
  if (data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);
}

void processInput(GLFWwindow* window)
{
    //TIME DELAY FOR GAME
    if (glfwGetTime() > 2.5) {

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            moveBool = true;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}