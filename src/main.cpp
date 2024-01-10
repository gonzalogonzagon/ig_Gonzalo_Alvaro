#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shaders.h"
#include "Model.h"
#include "Texture.h"

const int PLANET_TYPES = 1;

// ===================================================

void configScene();
void renderScene();
void setLights (glm::mat4 P, glm::mat4 V);
void drawObjectMat(Model model, Material material, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawObjectTex(Model model, Textures textures, glm::mat4 P, glm::mat4 V, glm::mat4 M);

void funFramebufferSize(GLFWwindow* window, int width, int height);
void funKey            (GLFWwindow* window, int key  , int scancode, int action, int mods);
void funScroll         (GLFWwindow* window, double xoffset, double yoffset);
void funCursorPos      (GLFWwindow* window, double xpos, double ypos);

void funPlanetStyle    (int select);

// #####################################################################################################################

// Shaders
    Shaders shaders;

// Modelos

    Model sphere;

    Model capsula;
    Model aro;
    Model cuerpo_sup;
    Model cuerpo_inf;
    Model circle;
    Model orbs;

    Model planeta;
    Model background;

// ===================================================
// Imagenes (texturas)
    Texture img1; // IMG Emisiva Nula
    Texture img8;

    //Cuerpo Superior Ovni
    Texture imgCSDIFF;
    Texture imgCSSPEC;
    Texture imgCSEMIS;
    Texture imgCSNORM; ////

    //Cuerpo Inferior Ovni
    Texture imgCIDIFF;
    Texture imgCISPEC;
    Texture imgCINORM;

    //Base Cuerpo Superior Ovni
    Texture imgCircleDIFF;
    Texture imgCircleSPEC;
    Texture imgCircleNORM;
    Texture imgCircleEMIS;

    //Planeta
    Texture imgPlanetDIFF;
    Texture imgPlanetSPEC;
    Texture imgPlanetNORM;
    Texture imgPlanetEMIS;

    //Fondo
    Texture imgStarMapDIFF;
    Texture imgStarMapEMIS;


// ===================================================
// Luces y materiales
    #define   NLD 1
    #define   NLP 1
    #define   NLF 2
    Light     lightG;
    Light     lightD[NLD];
    Light     lightP[NLP];
    Light     lightF[NLF];

    Material  mluz;

    Textures  texWindow;
    Textures  texCuerpoSup;
    Textures  texCuerpoInf;
    Textures  texCircle;
    Textures  texPlanet;
    Textures  texStars;


// ===================================================
// Viewport
    int w = 500;
    int h = 500;

// Animaciones
    float rotX = 0.0;
    float rotY = 0.0;
    float desZ = 0.0;

    float angle = 0.0;
    float angle2 = 0.0;

// Tiempo actual
    float time = glfwGetTime();

// Movimiento de camara
    float fovy   = 60.0;
    static float alphaX =  0.0;
    static float alphaY =  0.0;
    float incLight = 1.0;


//Selectores
    int turn_emiss = 0;
    int select_planet = 1;


// #####################################################################################################################

int main() {

 // Inicializamos GLFW
    if(!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

 // Creamos la ventana
    GLFWwindow* window;
    window = glfwCreateWindow(w, h, "Sesion 7", nullptr, nullptr);
    if(!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

 // Inicializamos GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if(GLEW_OK != err) {
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
        return -1;
    }
    std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    const GLubyte *oglVersion = glGetString(GL_VERSION);
    std::cout <<"This system supports OpenGL Version: " << oglVersion << std::endl;

 // Configuramos los CallBacks
    glfwSetFramebufferSizeCallback(window, funFramebufferSize);
    glfwSetKeyCallback      (window, funKey);
    glfwSetScrollCallback   (window, funScroll);
    glfwSetCursorPosCallback(window, funCursorPos);

 // Entramos en el bucle de renderizado
    configScene();
    while(!glfwWindowShouldClose(window)) {
        renderScene();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

// #####################################################################################################################

void configScene() {

 // Test de profundidad
    glEnable(GL_DEPTH_TEST);

 // Transparencias
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

 // Shaders
    shaders.initShaders("resources/shaders/vshader.glsl","resources/shaders/fshader.glsl");

 // Modelos

    sphere.initModel("resources/models/sphere.obj");

    capsula.initModel("resources/models/capsula.obj");
    aro.initModel("resources/models/aro.obj");
    cuerpo_sup.initModel("resources/models/cuerpo_sup.obj");
    cuerpo_inf.initModel("resources/models/cuerpo_inf.obj");
    circle.initModel("resources/models/circle.obj");
    orbs.initModel("resources/models/orbs.obj");

    planeta.initModel("resources/models/planeta.obj");
    background.initModel("resources/models/background.obj");

 // Imagenes (texturas)
    img1.initTexture("resources/textures/img1.png");
    img8.initTexture("resources/textures/img8.png");

    imgCSDIFF.initTexture("resources/textures/cuerpo_sup.png");
    imgCSSPEC.initTexture("resources/textures/s.png"); //specularAO1
    imgCSEMIS.initTexture("resources/textures/cuerpo_sup_emissive_y3.png");
    imgCSNORM.initTexture("resources/textures/normal3.png");

    imgCIDIFF.initTexture("resources/textures/texture.png"); //cuerpo_inf_metal
    imgCISPEC.initTexture("resources/textures/cuerpo_inf_spec.png");
    imgCINORM.initTexture("resources/textures/cuerpo_inf_norm.png");

    imgCircleDIFF.initTexture("resources/textures/circle.jpg");
    imgCircleSPEC.initTexture("resources/textures/circle_spec.png");
    imgCircleNORM.initTexture("resources/textures/circle_normal.png");
    imgCircleEMIS.initTexture("resources/textures/circle_emiss_purple.jpg");

    imgPlanetDIFF.initTexture("resources/textures/tierra.jpg");
    imgPlanetSPEC.initTexture("resources/textures/tierra_spec2.png");
    imgPlanetNORM.initTexture("resources/textures/tierra_normal.png");
    imgPlanetEMIS.initTexture("resources/textures/tierra_citylights.jpg");

    imgStarMapDIFF.initTexture("resources/textures/starmap_4k.jpg");
    imgStarMapEMIS.initTexture("resources/textures/starmap_4k_emiss.jpg");

 // Luz ambiental global
    lightG.ambient = glm::vec3(0.5, 0.5, 0.5);

 // Luces direccionales
    lightD[0].direction = glm::vec3(0.0, -1.0, 0.0);

 // Luces posicionales
    lightP[0].position    = glm::vec3(0.0, 3.0, 3.0);
    lightP[0].ambient     = glm::vec3(0.2, 0.2, 0.2);
    lightP[0].diffuse     = glm::vec3(0.9, 0.9, 0.9);
    lightP[0].specular    = glm::vec3(0.9, 0.9, 0.9);
    lightP[0].c0          = 1.00;
    lightP[0].c1          = 0.22;
    lightP[0].c2          = 0.20;

 // Luces focales
    lightF[0].position    = glm::vec3(-2.0,  2.0,  5.0);
    lightF[0].direction   = glm::vec3( 2.0, -2.0, -5.0);
    lightF[0].ambient     = glm::vec3( 0.2,  0.2,  0.2);
    lightF[0].diffuse     = glm::vec3( 0.9,  0.9,  0.9);
    lightF[0].specular    = glm::vec3( 0.9,  0.9,  0.9);
    lightF[0].innerCutOff = 10.0;
    lightF[0].outerCutOff = lightF[0].innerCutOff + 5.0;
    lightF[0].c0          = 1.000;
    lightF[0].c1          = 0.090;
    lightF[0].c2          = 0.032;
    lightF[1].position    = glm::vec3( 2.0,  2.0,  5.0);
    lightF[1].direction   = glm::vec3(-2.0, -2.0, -5.0);
    lightF[1].ambient     = glm::vec3( 0.2,  0.2,  0.2);
    lightF[1].diffuse     = glm::vec3( 0.9,  0.9,  0.9);
    lightF[1].specular    = glm::vec3( 0.9,  0.9,  0.9);
    lightF[1].innerCutOff = 5.0;
    lightF[1].outerCutOff = lightF[1].innerCutOff + 1.0;
    lightF[1].c0          = 1.000;
    lightF[1].c1          = 0.090;
    lightF[1].c2          = 0.032;

 // Materiales
    mluz.ambient   = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mluz.diffuse   = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mluz.specular  = glm::vec4(0.0, 0.0, 0.0, 1.0);
    mluz.emissive  = glm::vec4(1.0, 1.0, 1.0, 1.0);
    mluz.shininess = 1.0;


 // Texturas

    texWindow.diffuse   = img8.getTexture();
    texWindow.specular  = img8.getTexture();
    texWindow.emissive  = img1.getTexture();
    texWindow.normal    = 0;
    texWindow.shininess = 10.0;

    texCuerpoSup.diffuse   = imgCSDIFF.getTexture();
    texCuerpoSup.specular  = imgCSSPEC.getTexture();
    //texCuerpoSup.emissive  = imgCSEMIS.getTexture();
    texCuerpoSup.normal    = 0; //imgCSNORM.getTexture();
    texCuerpoSup.shininess = 10; //32.0; //64.0;

    texCuerpoInf.diffuse   = imgCIDIFF.getTexture();
    texCuerpoInf.specular  = imgCISPEC.getTexture();
    texCuerpoInf.emissive  = img1.getTexture();
    texCuerpoInf.normal    = imgCINORM.getTexture();
    texCuerpoInf.shininess = 32.0;

    texCircle.diffuse   = imgCircleDIFF.getTexture();
    texCircle.specular  = imgCircleSPEC.getTexture();
    //texCircle.emissive  = imgCircleEMIS.getTexture();
    texCircle.normal    = imgCircleNORM.getTexture();
    texCircle.shininess = 10.0;

    texPlanet.diffuse   = imgPlanetDIFF.getTexture();
    texPlanet.specular  = imgPlanetSPEC.getTexture();
    texPlanet.emissive  = img1.getTexture();
    texPlanet.normal    = 0; //imgPlanetNORM.getTexture();
    texPlanet.shininess = 32.0;

    texStars.diffuse   = imgStarMapDIFF.getTexture();
    texStars.specular  = imgStarMapEMIS.getTexture();
    texStars.emissive  = imgStarMapEMIS.getTexture();
    texStars.normal    = 0;
    texStars.shininess = 10.0;

}

// #####################################################################################################################

void renderScene() {

 // Borramos el buffer de color
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 // Indicamos los shaders a utilizar
    shaders.useShaders();

 // Matriz P
    float nplane =  0.1;
    float fplane = 64.0; //42.0;
    float aspect = (float)w/(float)h;
    glm::mat4 P = glm::perspective(glm::radians(fovy), aspect, nplane, fplane);

 // Matriz V
    float x = 10.0f*glm::cos(glm::radians(alphaY))*glm::sin(glm::radians(alphaX));
    float y = 10.0f*glm::sin(glm::radians(alphaY));
    float z = 10.0f*glm::cos(glm::radians(alphaY))*glm::cos(glm::radians(alphaX));
    glm::vec3 eye   (  x,   y,   z);
    glm::vec3 center(0.0, 0.0,  0.0);
    glm::vec3 up    (0.0, 1.0,  0.0);
    glm::mat4 V = glm::lookAt(eye, center, up);
    shaders.setVec3("ueye",eye);

 // Fijamos las luces
    setLights(P,V);

 // Dibujamos la escena

    glm::mat4 Ry = glm::rotate   (I, glm::radians(rotY), glm::vec3(0,1,0));
    glm::mat4 Rx = glm::rotate   (I, glm::radians(rotX), glm::vec3(1,0,0));

    glm::mat4 Splanet = glm::scale    (I, glm::vec3(45.0, 45.0, 45.0));

    // Obtén el tiempo actual
    time = glfwGetTime();

    // Calcula el ángulo de rotación basado en el tiempo
    angle = time * glm::radians(2.0f * 100.0f); // 2 grados cada 10 milisegundos
    angle2 = time * glm::radians(2.0f * 10.0f);

    glm::mat4 R_csup = glm::rotate   (I, angle, glm::vec3(0,1,0));
    glm::mat4 R_cinf = glm::rotate   (I, -angle2, glm::vec3(0,1,0));

//    drawObjectTex(cuerpo_sup, texCuerpoSup, P, V, Rx*Ry);
//    texCuerpoSup.emissive  = turn_emiss ? imgCSEMIS.getTexture() : img1.getTexture();

    //drawObjectMat(aro, mluz, P, V, Rx*Ry);

//    drawObjectTex(cuerpo_inf, texCuerpoInf, P, V, Rx*Ry);
//    drawObjectTex(circle, texCircle, P, V, Rx*Ry);
//    texCircle.emissive  = turn_emiss ? imgCircleEMIS.getTexture() : img1.getTexture();

    // Enable back face culling //////////////////////////////////////////////////////
    glEnable(GL_CULL_FACE); // Enable culling
    glCullFace(GL_BACK);

    drawObjectTex(cuerpo_sup, texCuerpoSup, P, V, Rx*Ry*R_csup);
    texCuerpoSup.emissive  = turn_emiss ? imgCSEMIS.getTexture() : img1.getTexture();

    drawObjectTex(cuerpo_inf, texCuerpoInf, P, V, Rx*Ry*R_cinf);

    drawObjectTex(planeta, texPlanet, P, V, I);

    // Disable back face culling
    glDisable(GL_CULL_FACE);

    // Enable front face culling //////////////////////////////////////////////////////
    glEnable(GL_CULL_FACE); // Enable culling
    glCullFace(GL_FRONT); // Specify that front faces should be culled

    drawObjectTex(background, texStars, P, V, Rx*Ry*Splanet);

    drawObjectTex(circle, texCircle, P, V, Rx*Ry);
    texCircle.emissive  = turn_emiss ? imgCircleEMIS.getTexture() : img1.getTexture();

    // Disable front face culling
    glDisable(GL_CULL_FACE);

//    drawObjectTex(background, texStars, P, V, Rx*Ry*Splanet);

    drawObjectMat(orbs, mluz, P, V, Rx*Ry*R_cinf);

    //Objetos transparentes

    glDepthMask(GL_FALSE);
    drawObjectTex(capsula, texWindow, P, V, Rx * Ry);
    glDepthMask(GL_TRUE);
    
}

// #####################################################################################################################

void setLights(glm::mat4 P, glm::mat4 V) {

    lightD[0].ambient   = incLight * glm::vec3(0.1,  0.1, 0.1);
    lightD[0].diffuse   = incLight * glm::vec3(0.7,  0.7, 0.7);
    lightD[0].specular  = incLight * glm::vec3(0.7,  0.7, 0.7);

    shaders.setLight("ulightG",lightG);
    for(int i=0; i<NLD; i++) shaders.setLight("ulightD["+toString(i)+"]",lightD[i]);
    for(int i=0; i<NLP; i++) shaders.setLight("ulightP["+toString(i)+"]",lightP[i]);
    for(int i=0; i<NLF; i++) shaders.setLight("ulightF["+toString(i)+"]",lightF[i]);

    for(int i=0; i<NLP; i++) {
        glm::mat4 M = glm::translate(I,lightP[i].position) * glm::scale(I,glm::vec3(0.1));
        drawObjectMat(sphere, mluz, P, V, M);
    }

    for(int i=0; i<NLF; i++) {
        glm::mat4 M = glm::translate(I,lightF[i].position) * glm::scale(I,glm::vec3(0.025));
        drawObjectMat(sphere, mluz, P, V, M);
    }

}

// #####################################################################################################################

void drawObjectMat(Model model, Material material, glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    shaders.setMat4("uN"  ,glm::transpose(glm::inverse(M)));
    shaders.setMat4("uM"  ,M);
    shaders.setMat4("uPVM",P*V*M);
    shaders.setBool("uWithMaterials",true);
    shaders.setMaterial("umaterial",material);
    model.renderModel(GL_FILL);

}

// #####################################################################################################################

void drawObjectTex(Model model, Textures textures, glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    shaders.setMat4("uN"  ,glm::transpose(glm::inverse(M)));
    shaders.setMat4("uM"  ,M);
    shaders.setMat4("uPVM",P*V*M);
    shaders.setBool("uWithMaterials",false);
    shaders.setTextures("utextures",textures);
    if(textures.normal!=0) shaders.setBool("uWithNormals",true);
    else                   shaders.setBool("uWithNormals",false);
    model.renderModel(GL_FILL);

}

// #####################################################################################################################

void funFramebufferSize(GLFWwindow* window, int width, int height) {

 // Configuracion del Viewport
    glViewport(0, 0, width, height);

 // Actualizacion de w y h
    w = width;
    h = height;

}

// #####################################################################################################################

void funKey(GLFWwindow* window, int key  , int scancode, int action, int mods) {

    switch(key) {
        case GLFW_KEY_1: if (action==GLFW_PRESS) turn_emiss = !turn_emiss; break;
        case GLFW_KEY_2: if (action==GLFW_PRESS) {
            funPlanetStyle(select_planet);
            select_planet++;
            if(select_planet > PLANET_TYPES) select_planet = 0;
            break;
        }


        case GLFW_KEY_KP_ADD: {
            incLight += incLight < 10.0f ? 0.1f : 0.0f; 
            if (incLight >= 0.2) texPlanet.emissive  = img1.getTexture();
            break;
        }   
        case GLFW_KEY_KP_SUBTRACT: {
            incLight -= 0.1f; 
            if (incLight < 0.2) texPlanet.emissive  = imgPlanetEMIS.getTexture();
            break;
        } 


        case GLFW_KEY_UP:    rotX -= 5.0f;   break;
        case GLFW_KEY_DOWN:  rotX += 5.0f;   break;
        case GLFW_KEY_LEFT:  rotY -= 5.0f;   break;
        case GLFW_KEY_RIGHT: rotY += 5.0f;   break;
        case GLFW_KEY_Z:
            if(mods==GLFW_MOD_SHIFT) desZ -= desZ > -24.0f ? 0.1f : 0.0f;
            else                     desZ += desZ <   5.0f ? 0.1f : 0.0f;
            break;
        default:
            rotX = 0.0f;
            rotY = 0.0f;
            break;
    }

}

// #####################################################################################################################

void funScroll(GLFWwindow* window, double xoffset, double yoffset) {

    if(yoffset>0) fovy -= fovy>5.0f ? 5.0f : 0.0f;
    if(yoffset<0) fovy += fovy<180.0f ? 5.0f : 0.0f;

}

// #####################################################################################################################

void funCursorPos(GLFWwindow* window, double xpos, double ypos) {

    if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT)==GLFW_RELEASE) return;

    float limY = 89.0;
    alphaX = 90.0*(2.0*xpos/(float)w - 1.0);
    alphaY = 90.0*(1.0 - 2.0*ypos/(float)h);
    if(alphaY<-limY) alphaY = -limY;
    if(alphaY> limY) alphaY =  limY;

}

// #####################################################################################################################

void funPlanetStyle(int select) {

//    switch (select) {
//        case 1: {
//            imgPlanetDIFF.initTexture("resources/textures/tierra.jpg");
//            imgPlanetSPEC.initTexture("resources/textures/tierra_spec.png");
//            imgPlanetNORM.initTexture("resources/textures/tierra_normal.png");
//            imgPlanetEMIS.initTexture("resources/textures/tierra_citylights.jpg");
//            break;
//        }
//    }

}
