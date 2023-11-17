#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION 
#include <learnopengl/stb_image.h>


/*Grupo 3
   Christian Manuel Agila Vivanco
   Giulian David Albuja Salazar
   David Alexander Averos Otavalo
   Edwin Ricardo Pillajo Nasimba
*/


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// configuracion pantalla
const unsigned int SCR_WIDTH = 1100;
const unsigned int SCR_HEIGHT = 900;
bool linternaEncendida = true;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// tiempo actulizacion frame
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    // glfw: inicialización y configuración
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw creacion de la ventana
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Examen Bimestral 2: Scene Modeling and Implementation - Grupo 3", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // funcion de GLFW que oculta el cursor del mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: carga los punteros a las funciones de GLFW
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // le dice a stb_image.h to cambiar el sentido de las coordenadas de textura en el eje y (antes de cargar el modelo).
    stbi_set_flip_vertically_on_load(false);

    // Habilitar la profundidad Z-Buffer
    glEnable(GL_DEPTH_TEST);

    Shader skyBoxShader("shaders/skyShader.vs", "shaders/skyShader.fs");
    Shader lightingShader("shaders/lightingShaderProy.vs", "shaders/lightingShaderProy.fs");

    // Carga de los modelos a ocupar en la escena.
    Model escena("model/escena/voxel.obj");
    Model casa("model/casa/casa.obj");
    Model tren("model/tren/tren.obj");
    Model trump("model/trump/trump.obj");
    Model white("model/white/white.obj");
    Model rick("model/rick/rick.obj");
    Model capybara("model/capybara/capybara.obj");
  

    //------------------------------------------CARGA SKYBOX-----------------------------------------------
    // Carga del skybox para simular el cielo
    float verticesSkyBox[] = {
        // posicion          // texture coords
        //Cara trasera
        -0.5f, -0.5f, -0.5f,  0.75f, 0.33f, //Inf izquierda    
         0.5f, -0.5f, -0.5f,  1.0f,  0.33f, //Inf derecha
         0.5f,  0.5f, -0.5f,  1.0f,  0.66f, //Sup derecha
         0.5f,  0.5f, -0.5f,  1.0f,  0.66f, //Sup derecha
        -0.5f,  0.5f, -0.5f,  0.75f, 0.66f, //Sup izquierda
        -0.5f, -0.5f, -0.5f,  0.75f, 0.33f, //Inf izquierda
        //Cara frontal 
        -0.5f, -0.5f,  0.5f,  0.25f, 0.33f, //Inf izquierda
         0.5f, -0.5f,  0.5f,  0.5f,  0.33f, //Inf derecha
         0.5f,  0.5f,  0.5f,  0.5f,  0.66f, //Sup derecha
         0.5f,  0.5f,  0.5f,  0.5f,  0.66f, //Sup derecha
        -0.5f,  0.5f,  0.5f,  0.25f, 0.66f, //Sup izquierda
        -0.5f, -0.5f,  0.5f,  0.25f, 0.33f, //Inf izquierda
        //Cara Izquierda
        -0.5f,  0.5f,  0.5f,  0.25f, 0.66f, //Sup derecha
        -0.5f,  0.5f, -0.5f,  0.0f,  0.66f, //Sup izquierda
        -0.5f, -0.5f, -0.5f,  0.0f,  0.33f, //Inf izquierda
        -0.5f, -0.5f, -0.5f,  0.0f,  0.33f, //Inf izquierda
        -0.5f, -0.5f,  0.5f,  0.25f, 0.33f, //Inf derecha
        -0.5f,  0.5f,  0.5f,  0.25f, 0.66f, //Sup derecha
        //Cara derecha
         0.5f,  0.5f,  0.5f,  0.5f,  0.66f,  //Sup izquierda
         0.5f,  0.5f, -0.5f,  0.75f, 0.66f,  //Sup derecha
         0.5f, -0.5f, -0.5f,  0.75f, 0.33f,  //Inf derecha
         0.5f, -0.5f, -0.5f,  0.75f, 0.33f,  //Inf derecha
         0.5f, -0.5f,  0.5f,  0.5f,  0.33f,  //Inf izquierda
         0.5f,  0.5f,  0.5f,  0.5f,  0.66f,  //Sup izquierda
         //Cara inferior
        -0.5f, -0.5f, -0.5f,  0.25f, 0.33f,   //Sup izquierda
         0.5f, -0.5f, -0.5f,  0.5f,  0.33f,   //Sup derecha 
         0.5f, -0.5f,  0.5f,  0.5f,  0.0f,   //Inf derecha
         0.5f, -0.5f,  0.5f,  0.5f,  0.0f,   //Inf derecha
        -0.5f, -0.5f,  0.5f,  0.25f, 0.0f,   //Inf izquierda
        -0.5f, -0.5f, -0.5f,  0.25f, 0.33f,   //Sup izquierda
        //Cara Superior
        -0.5f,  0.5f, -0.5f,  0.25f, 1.0f,   //Sup izquierda
         0.5f,  0.5f, -0.5f,  0.5f,  1.0f,   //Sup derecha 
         0.5f,  0.5f,  0.5f,  0.5f,  0.66f,   //Inf derecha
         0.5f,  0.5f,  0.5f,  0.5f,  0.66f,   //Inf derecha
        -0.5f,  0.5f,  0.5f,  0.25f, 0.66f,   //Inf izquierda
        -0.5f,  0.5f, -0.5f,  0.25f, 1.0f    //Sup izquierda
    };
    // Creacion y configuracion del VBO y VAO para el skybox
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesSkyBox), verticesSkyBox, GL_STATIC_DRAW);
    // Indicar al buffer como leer los datos de vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Indicar al buffer como leer los datos de coordenadas de texturas
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // creacion y carga de textura de skybox
    unsigned int textura1;
    // textura 1
    glGenTextures(1, &textura1);
    glBindTexture(GL_TEXTURE_2D, textura1);
    // Asignacion de los parametros de wrapping a la textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Asignacion de los parametros de filtering a la textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // carga de la imagen, paso de datos de la CPU a la GPU y generacion de mipmap.
    int width, height, nrChannels;
    //stbi_set_flip_vertically_on_load(true); // Voltea la textura en el eje y
    unsigned char* data = stbi_load("textures/nightsky.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // Enlaza las unidades de textura a su respectivo sampler en el fg skyshader.fs
    skyBoxShader.use();
    skyBoxShader.setInt("texture1", 0);
    //---------------------------------------------------------------------------------------------------------


    // loop de renderizacion
    while (!glfwWindowShouldClose(window))
    {
        // Tiempo para el movimiento por frame
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window);

        // render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightingShader.use();
        // Se activa activar el shader para configurar las variables uniformes/dibujar objetos
        lightingShader.setVec3("viewPos", camera.Position);
        lightingShader.setFloat("material.shininess", 16.0f);

        //Configuracion choqueluz objetos
        // directional light
        lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        lightingShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        //cabeza de Trump: LUZ BLANCA -- > Punto de luz 1
        lightingShader.setVec3("pointLights[0].position", glm::vec3(15.0f, 7.0f, -10.0f));
        lightingShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointLights[0].diffuse", 0.7f, 0.7f, 0.7f);
        lightingShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights[0].constant", 1.0f);
        lightingShader.setFloat("pointLights[0].linear", 0.022f);
        lightingShader.setFloat("pointLights[0].quadratic", 0.0019f);
        //atardecer derecha -- > Punto de luz 2
        lightingShader.setVec3("pointLights[1].position", glm::vec3(25.0f, -2.0f, 0.0f));
        lightingShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointLights[1].diffuse", 1.0f, 0.2f, 0.0f);
        lightingShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights[1].constant", 1.0f);
        lightingShader.setFloat("pointLights[1].linear", 0.027);
        lightingShader.setFloat("pointLights[1].quadratic", 0.0028);
        //atardecer izquierda --> Punto de luz 3
        lightingShader.setVec3("pointLights[2].position", glm::vec3(0.0f, -2.0f, 0.0f));
        lightingShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointLights[2].diffuse", 1.0f, 0.2f, 0.0f);
        lightingShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights[2].constant", 1.0f);
        lightingShader.setFloat("pointLights[2].linear", 0.027);
        lightingShader.setFloat("pointLights[2].quadratic", 0.0028);

        //luz de tren --> Punto de luz 4
        lightingShader.setVec3("pointLights[3].position", glm::vec3((glfwGetTime() * 0.07) + 7.0, 5.0f, -9.0f));
        lightingShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointLights[3].diffuse", 1.0f, 1.0f, 0.0f);
        lightingShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights[3].constant", 1.0f);
        lightingShader.setFloat("pointLights[3].linear", 0.027);
        lightingShader.setFloat("pointLights[3].quadratic", 0.0028);
        //Spotlight linterna
        if(linternaEncendida){
            lightingShader.setVec3("spotLight.position", camera.Position);
            lightingShader.setVec3("spotLight.direction", camera.Front);
            lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
            lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
            lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
            lightingShader.setFloat("spotLight.constant", 1.0f);
            lightingShader.setFloat("spotLight.linear", 0.09);
            lightingShader.setFloat("spotLight.quadratic", 0.032);
            lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
            lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
        }

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
        glm::mat4 view = camera.GetViewMatrix();


        lightingShader.use();
        // view/projection transformations
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);
        

        //-------------------------ESCENA---------------------------------------------------
        // render del modelo escena
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(.2f, .2f, .2f));	
        lightingShader.setMat4("model", model);

        escena.Draw(lightingShader);
        //---------------------------------------------------------------------------------


        //-------------------------CASA---------------------------------------------------
        // render del modelo casa
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); 
        model = glm::scale(model, glm::vec3(.2f, .2f, .2f));
        lightingShader.setMat4("model", model);

        casa.Draw(lightingShader);
        //--------------------------------------------------------------------------------



        //-------------------------TREN---------------------------------------------------
        // render del modelo tren
        model = glm::mat4(1.0f);
        float movimiento = glfwGetTime() * 0.07;
        model = glm::translate(model, glm::vec3(movimiento, 0.0f, 0.0f)); 
        model = glm::scale(model, glm::vec3(.2f, .2f, .2f));
        lightingShader.setMat4("model", model);

        tren.Draw(lightingShader);
        //--------------------------------------------------------------------------------



        //-------------------------WHITE---------------------------------------------------
        // render del modelo white
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(5.0 + movimiento, 5.0f, -9.0f)); 
        model = glm::scale(model, glm::vec3(.2f, .2f, .2f));	
        lightingShader.setMat4("model", model);
        white.Draw(lightingShader);
        //---------------------------------------------------------------------------------



        //-------------------------CABEZAS TRUMP-------------------------------------------
        // render del modelo trump(cabeza 1)
        model = glm::mat4(1.0f);
        float crecimiento = (sin(glfwGetTime()) / 2.0f + 5.0f) * 5.0f;
        model = glm::translate(model, glm::vec3(15.0f, 10.0f, -17.0f)); 
        model = glm::rotate(model, glm::radians(crecimiento), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(.5f, .5f, .5f));	
        lightingShader.setMat4("model", model);

        trump.Draw(lightingShader);

        // render del modelo trump(cabeza 2)
        float lado = (sin(glfwGetTime()) / 2.0f + 1.0f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.0f + lado, 10.0f, -20.0f)); 
        model = glm::rotate(model, glm::radians(crecimiento), glm::vec3(crecimiento, crecimiento, 0.0f));
        model = glm::scale(model, glm::vec3(.2f, .2f, .2f));	
        lightingShader.setMat4("model", model);

        trump.Draw(lightingShader);

        // render del modelo trump(cabeza 3)
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(20.0f - lado, 10.0f, -20.0f));
        model = glm::rotate(model, glm::radians(crecimiento), glm::vec3(crecimiento, -crecimiento, 0.0f));
        model = glm::scale(model, glm::vec3(.3f, .3f, .3f));
        lightingShader.setMat4("model", model);

        trump.Draw(lightingShader);
        //-------------------------------------------------------------------------------------------


        //------------------------------------RICK----------------------------------------------------
        // render del modelo rick
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(19.0f, -0.06f, -1.8f));
        model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
        model = glm::rotate(model, glm::radians(-152.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        lightingShader.setMat4("model", model);

        rick.Draw(lightingShader);
        //------------------------------------------------------------------------------------------------



        //------------------------------------CAPYBARA----------------------------------------------------
        // render del modelo capybara
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(12.0f, -0.07f, -2.0f));
        model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
        model = glm::rotate(model, glm::radians(160.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        lightingShader.setMat4("model", model);

        capybara.Draw(lightingShader);
        //------------------------------------------------------------------------------------------------



        //------------------------------------SKYBOX----------------------------------------------------
        // Dibujar el skybox
        skyBoxShader.use();
        model = glm::mat4(1.0f);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textura1);
        glBindVertexArray(VAO);
        skyBoxShader.setMat4("projection", projection);
        skyBoxShader.setMat4("view", view);
        model = glm::translate(model, glm::vec3(12.0f,15.0f,-10.0f));
        model = glm::rotate(model, (float)glfwGetTime()/50, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(40.0f, 40.0f, 40.0f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        skyBoxShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //------------------------------------------------------------------------------------------------
      
     
        // glfw: intercambia los buffers y escanea eventos de entrada y salida(I/O)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // deshabilita todos los recursos una vez que hayan cumplido con su proposito:
    glfwTerminate();
    return 0;
}

//Proceso para elementos de entrada: consulta a GLFW si... 
//se han pulsado o soltado las teclas pertinentes en este... 
//fotograma y reacciona en consecuencia.
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    //BLOQUEAR Y DESBLOQUEAR LINTERNA
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
        linternaEncendida = false;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        linternaEncendida = true;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}