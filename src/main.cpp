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

void drawOrbes(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawSoporte(glm::mat4 P, glm::mat4 V, glm::mat4 M);

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
    Model aro; //////
    Model cuerpo_sup;
    Model cuerpo_inf;
    Model circle;
    Model orbe;
    Model pieza1_pata;
    Model pieza2_pata;

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
    #define   NLP 5
    #define   NLF 2
    Light     lightG;
    Light     lightD[NLD];
    Light     lightP[NLP];
    Light     lightF[NLF];

    Material  mluz;
    Material  turquesa;
    Material  cromo;

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
    float movX = 0.0;
    float movY = 0.0;

    float angle = 0.0;
    float angle2 = 0.0;
    float angle3 = 0.0;
    static float lastAngle = 0.0f;
    static float lastAngle2 = 0.0f;

// Tiempo actual
    float time = glfwGetTime();
    static double lastTime = 0.0f;

// Movimiento de camara
    float fovy   = 60.0;
    static float alphaX =  0.0;
    static float alphaY =  0.0;
    float incLight = 1.0;


//Selectores
    bool turn_ovniEngine = false;
    bool turn_firstP = false;

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
    orbe.initModel("resources/models/orbe.obj");
    pieza1_pata.initModel("resources/models/pieza1_pata.obj");
    pieza2_pata.initModel("resources/models/pieza2_pata.obj");

    planeta.initModel("resources/models/planeta.obj");
    background.initModel("resources/models/background.obj");

 // Imagenes (texturas)
    img1.initTexture("resources/textures/img1.png");
    img8.initTexture("resources/textures/img8.png");

    imgCSDIFF.initTexture("resources/textures/cuerpo_sup.png");
    imgCSSPEC.initTexture("resources/textures/s.png"); //specularAO1
    imgCSEMIS.initTexture("resources/textures/cuerpo_sup_emissive_y3.png");

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
    // lightP[0].position    = glm::vec3(0, 0, 0)
    lightP[0].position    = glm::vec3(1, 1, 2);
    lightP[0].ambient     = glm::vec3(0, 0, 1);
    lightP[0].diffuse     = glm::vec3(0, 0, 1);
    lightP[0].specular    = glm::vec3(0, 0, 1);
    lightP[0].c0          = 1.00;
    lightP[0].c1          = 0.22;
    lightP[0].c2          = 0.20;


    // lightP[1].position    = glm::vec3(-2.2, 0, 0);
    lightP[1].position    = glm::vec3(-1.5, 1, 2.5);
    lightP[1].ambient     = glm::vec3(0, 1, 0);
    lightP[1].diffuse     = glm::vec3(0, 1, 0);
    lightP[1].specular    = glm::vec3(0, 1, 0);
    lightP[1].c0          = 1.00;
    lightP[1].c1          = 0.22;
    lightP[1].c2          = 0.20;

    // lightP[2].position    = glm::vec3(-2.2, 0, -2.2);
    lightP[2].position    = glm::vec3(-2, 1, -2.5);
    lightP[2].ambient     = glm::vec3(1, 0, 0);
    lightP[2].diffuse     = glm::vec3(1, 0, 0);
    lightP[2].specular    = glm::vec3(1, 0, 0);
    lightP[2].c0          = 1.00;
    lightP[2].c1          = 0.22;
    lightP[2].c2          = 0.20;


    //lightP[3].position    = glm::vec3(0.0, 0.0, -2.2);
    lightP[3].position    = glm::vec3(2, 1, -0.5);
    lightP[3].ambient     = glm::vec3(1, 0, 1);
    lightP[3].diffuse     = glm::vec3(1, 0, 1);
    lightP[3].specular    = glm::vec3(1, 0, 1);
    lightP[3].c0          = 1.00;
    lightP[3].c1          = 0.22;
    lightP[3].c2          = 0.20;


    lightP[4].position    = glm::vec3(-1.1, 2.2, -1.1);
    lightP[4].ambient     = glm::vec3(0, 1, 1);
    lightP[4].diffuse     = glm::vec3(0, 1, 1);
    lightP[4].specular    = glm::vec3(0, 1, 1);
    lightP[4].c0          = 1.00;
    lightP[4].c1          = 0.22;
    lightP[4].c2          = 0.20;


    // Luces focales
    lightF[0].position    = glm::vec3(0.0, 1.0, 0.0);  // Cambia la posición en el eje y
    lightF[0].direction   = glm::vec3(0.0, -1.0, 0.0);  // Cambia la dirección si es necesario

    lightF[0].ambient  = glm::vec4(0.7, 0.7, 0.0, 1.0);  // Amarillo con componente azul apagada
    lightF[0].diffuse  = glm::vec4(0.7, 0.7, 0.0, 1.0);
    lightF[0].specular = glm::vec4(0.7, 0.7, 0.0, 1.0);

    lightF[0].innerCutOff = 20.0;
    lightF[0].outerCutOff = lightF[0].innerCutOff + 10.0;
    lightF[0].c0          = 1.0;
    lightF[0].c1          = 0.09;
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

    turquesa.ambient   = glm::vec4(0.1,	0.18725,	0.1745, 1.0);
    turquesa.diffuse   = glm::vec4(0.396,0.74151,	0.69102, 1.0);
    turquesa.specular  = glm::vec4(0.297254, 0.30829,	0.306678, 1.0);
    turquesa.emissive  = glm::vec4(0.0, 0.0, 0.0, 0.0);
    turquesa.shininess = 0.1;

    cromo.ambient   = glm::vec4(0.25, 0.25, 0.25, 1.0);
    cromo.diffuse   = glm::vec4(0.4, 0.4, 0.4, 1.0);
    cromo.specular  = glm::vec4(0.774597, 0.774597, 0.774597, 1.0);
    cromo.emissive  = glm::vec4(0.0, 0.0, 0.0, 0.0);
    cromo.shininess = 0.6;


 // Texturas

    texWindow.diffuse   = img8.getTexture();
    texWindow.specular  = img8.getTexture();
    texWindow.emissive  = img1.getTexture();
    texWindow.normal    = 0;
    texWindow.shininess = 10.0;

    texCuerpoSup.diffuse   = imgCSDIFF.getTexture();
    texCuerpoSup.specular  = imgCSSPEC.getTexture();
    texCuerpoSup.emissive  = img1.getTexture(); //Modificable con tecla C
    texCuerpoSup.normal    = 0; //imgCSNORM.getTexture();
    texCuerpoSup.shininess = 10; //32.0; //64.0;

    texCuerpoInf.diffuse   = imgCIDIFF.getTexture();
    texCuerpoInf.specular  = imgCISPEC.getTexture();
    texCuerpoInf.emissive  = img1.getTexture();
    texCuerpoInf.normal    = imgCINORM.getTexture();
    texCuerpoInf.shininess = 32.0;

    texCircle.diffuse   = imgCircleDIFF.getTexture();
    texCircle.specular  = imgCircleSPEC.getTexture();
    texCircle.emissive  = img1.getTexture(); //Modificable con tecla C
    texCircle.normal    = imgCircleNORM.getTexture();
    texCircle.shininess = 10.0;

    texPlanet.diffuse   = imgPlanetDIFF.getTexture();
    texPlanet.specular  = imgPlanetSPEC.getTexture();
    texPlanet.emissive  = img1.getTexture(); //Reaccion a tecla + -
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

// Obtén el tiempo actual
    time = glfwGetTime();

 // Uso y manipulación de matrices
    glm::mat4 M_camara, M_cuerpo_sup, M_cuerpo_inf, M_circle, M_planeta, M_background, M_orbs, M_pata;


    glm::mat4 Rx = glm::rotate   (I, glm::radians(rotX), glm::vec3(1,0,0));
    glm::mat4 Ry = glm::rotate   (I, glm::radians(rotY), glm::vec3(0,1,0));

    glm::mat4 S_bg = glm::scale    (I, glm::vec3(40.0, 40.0, 40.0));

    glm::mat4 T_planeta = glm::translate(I, glm::vec3(0.0,0.2,0.0));

    glm::mat4 M1 = Ry * Rx;

 // Matriz P
    float nplane =  0.1;
    float fplane = 64.0; //42.0;
    float aspect = (float)w/(float)h;
    glm::mat4 P = glm::perspective(glm::radians(fovy), aspect, nplane, fplane);

 // Matriz V
    float x = 10.0f*glm::cos(glm::radians(alphaY))*glm::sin(glm::radians(alphaX));
    float y = 10.0f*glm::sin(glm::radians(alphaY));
    float z = 10.0f*glm::cos(glm::radians(alphaY))*glm::cos(glm::radians(alphaX));
    float x_fp = glm::cos(glm::radians(alphaY))*glm::sin(glm::radians(alphaX));
    float y_fp = glm::sin(glm::radians(alphaY));
    float z_fp = glm::cos(glm::radians(alphaY))*glm::cos(glm::radians(alphaX));

    glm::vec3 direction (glm::radians(alphaY)*glm::cos(glm::radians(alphaX)),
                         glm::radians(alphaY),
                         glm::radians(alphaY)*glm::sin(glm::radians(alphaX)));

    direction = glm::normalize(direction);

    glm::vec3 eye   (  x,   y,   z);  //Posición de la cámara en el mundo
    glm::vec3 center(0.0, 0.0,  0.0); //Dirección donde la cámara mira
    glm::vec3 up    (0.0, 1.0,  0.0);

    glm::vec3 eye_firstPerson   (  0,   3.0,   0.0);  //Posición de la cámara en el mundo
    //glm::vec3 center_firstPerson(0.0, 2.0,  1.0); //Dirección donde la cámara mira
    //glm::vec3 up_firstPerson    (0.0, 1.0,  0.0);

    glm::vec3 center_firstPerson = eye + direction;

    // Calcular el vector "right"
    glm::vec3 right = glm::cross(direction, glm::vec3(0.0f, 1.0f, 0.0f));

    // Calcular el vector "up_firstPerson"
    glm::vec3 up_firstPerson = glm::vec3(Ry * Rx * glm::vec4(up, 1.0));

    eye_firstPerson = glm::vec3(Ry * Rx * glm::vec4(eye_firstPerson, 1.0));


    glm::mat4 V = turn_firstP ? glm::lookAt(eye_firstPerson, center_firstPerson, up_firstPerson) : glm::lookAt(eye, center, up);
    shaders.setVec3("ueye",eye);

 // Fijamos las luces
    setLights(P,V);

 // Dibujamos la escena





    // Calcula el ángulo de rotación basado en el tiempo
    angle = lastAngle + (time - lastTime) * glm::radians(2.0f * 80.0f);
    angle2 = lastAngle2 + (time - lastTime) * glm::radians(2.0f * 50.0f);

    angle3 = time * glm::radians(1.0f * 1.0f);

    glm::mat4 Ry_fast = glm::rotate   (I, turn_ovniEngine ? angle : lastAngle, glm::vec3(0,1,0));
    glm::mat4 Ry_medium = glm::rotate   (I, turn_ovniEngine ? -angle2 : -lastAngle2, glm::vec3(0, 1, 0));
    glm::mat4 Ry_medium2 = glm::rotate   (I, turn_ovniEngine ? angle2*0.1f : lastAngle2*0.1f, glm::vec3(0, 1, 0));

    glm::mat4 Rz_slow = glm::rotate   (I, -angle3, glm::vec3(0, 0, 1));

    M_cuerpo_sup = M1 * Ry_fast;
    M_cuerpo_inf = M1 * Ry_medium;
    M_circle = M1 * Ry_medium2;
    M_orbs = M1 * Ry_medium;

    M_pata = M1 * Ry_medium;

    M_planeta = T_planeta;

    M_background = Rz_slow * S_bg;


    // Enable back face culling //////////////////////////////////////////////////////
    glEnable(GL_CULL_FACE); // Enable culling
    glCullFace(GL_BACK);

    drawObjectTex(cuerpo_sup, texCuerpoSup, P, V, M_cuerpo_sup);

    drawObjectTex(cuerpo_inf, texCuerpoInf, P, V, M_cuerpo_inf);

    drawSoporte(P, V, M_pata);

    drawObjectTex(planeta, texPlanet, P, V, M_planeta);

    // Disable back face culling
    glDisable(GL_CULL_FACE);

    // Enable front face culling //////////////////////////////////////////////////////
    glEnable(GL_CULL_FACE); // Enable culling
    glCullFace(GL_FRONT); // Specify that front faces should be culled

    drawObjectTex(background, texStars, P, V, M_background);

    drawObjectTex(circle, texCircle, P, V, M_circle);

    // Disable front face culling
    glDisable(GL_CULL_FACE);

    drawOrbes(P, V, M_orbs );

    //Objetos transparentes //////////////////////////////////////////////////////
    glDepthMask(GL_FALSE);
    drawObjectTex(capsula, texWindow, P, V, Ry * Rx);
    glDepthMask(GL_TRUE);
    
}

// #####################################################################################################################

void drawSoporte(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 R90 = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 T_act = glm::translate(I, glm::vec3(-movX,movY,0.0));

    drawObjectMat(pieza1_pata, turquesa, P, V, M*T_act);
    drawObjectMat(pieza2_pata, cromo, P, V, M*T_act);

    drawObjectMat(pieza1_pata, turquesa, P, V, M*R90*T_act);
    drawObjectMat(pieza2_pata, cromo, P, V, M*R90*T_act);
    drawObjectMat(pieza1_pata, turquesa, P, V, M*R90*R90*T_act);
    drawObjectMat(pieza2_pata, cromo, P, V, M*R90*R90*T_act);
    drawObjectMat(pieza1_pata, turquesa, P, V, M*R90*R90*R90*T_act);
    drawObjectMat(pieza2_pata, cromo, P, V, M*R90*R90*R90*T_act);

}

void drawOrbes(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 R90 = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    drawObjectMat(orbe, mluz, P, V, M);
    drawObjectMat(orbe, mluz, P, V, M*R90);
    drawObjectMat(orbe, mluz, P, V, M*R90*R90);
    drawObjectMat(orbe, mluz, P, V, M*R90*R90*R90);


}

void setLights(glm::mat4 P, glm::mat4 V) {

    lightD[0].ambient   = incLight * glm::vec3(0.1,  0.1, 0.1);
    lightD[0].diffuse   = incLight * glm::vec3(0.7,  0.7, 0.7);
    lightD[0].specular  = incLight * glm::vec3(0.7,  0.7, 0.7);

    shaders.setLight("ulightG",lightG);
    for(int i=0; i<NLD; i++) shaders.setLight("ulightD["+toString(i)+"]",lightD[i]);
    for(int i=0; i<NLP; i++) shaders.setLight("ulightP["+toString(i)+"]",lightP[i]);
    for(int i=0; i<NLF; i++) shaders.setLight("ulightF["+toString(i)+"]",lightF[i]);

    for(int i=0; i<NLP; i++) {
        glm::mat4 M = glm::translate(I,lightP[i].position) /* glm::scale(I,glm::vec3(0.1))*/;
        drawObjectMat(orbe, mluz, P, V, M);
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
        case GLFW_KEY_4:
            if (action==GLFW_PRESS) {
                funPlanetStyle(select_planet);
                select_planet++;
                if (select_planet > PLANET_TYPES) select_planet = 0;
            }
            break;

        case GLFW_KEY_3:
            if (action==GLFW_PRESS) {
                turn_firstP = !turn_firstP;
            }
            break;


        case GLFW_KEY_KP_ADD:
            if (action == GLFW_PRESS) {
                incLight += incLight < 10.0f ? 0.1f : 0.0f;
                if (incLight >= 0.2) texPlanet.emissive  = img1.getTexture();
            }
            break;
        case GLFW_KEY_KP_SUBTRACT:
            if (action == GLFW_PRESS) {
                incLight -= 0.1f;
                if (incLight < 0.2) texPlanet.emissive  = imgPlanetEMIS.getTexture();
            }
            break;

        case GLFW_KEY_UP:
            if (action==GLFW_PRESS || action == GLFW_REPEAT) {
                rotX -= 5.0f;
                glm::mat4 Rx2 = glm::rotate(I, glm::radians(rotX), glm::vec3(1, 0, 0));
                glm::mat4 Ry2 = glm::rotate   (I, glm::radians(rotY), glm::vec3(0,1,0));

                lightF[0].position = glm::vec3(Ry2  * Rx2 * glm::vec4(0.0, 1.0, 0.0, 1.0));
                lightF[0].direction = glm::vec3(Ry2 * Rx2 * glm::vec4(0.0, -1.0, 0.0, 0.0));


                lightP[0].position = glm::vec3( Ry2 * Rx2 * glm::vec4(1, 1, 2,1));
                lightP[1].position = glm::vec3( Ry2 * Rx2 * glm::vec4(-1.5, 1, 2.5,1.0));
                lightP[2].position = glm::vec3( Ry2 * Rx2 *  glm::vec4(-2, 1, -2.5,1.0));
                lightP[3].position = glm::vec3( Ry2 * Rx2 *  glm::vec4(2, 1, -0.5,1));
                lightP[4].position = glm::vec3( Ry2 * Rx2 *  glm::vec4(-1.1, 2.2, -1.1,1.0));

            }
            break;
        case GLFW_KEY_DOWN:
            if (action==GLFW_PRESS || action == GLFW_REPEAT) {
                rotX += 5.0f;
                glm::mat4 Rx2 = glm::rotate(I, glm::radians(rotX), glm::vec3(1, 0, 0));
                glm::mat4 Ry2 = glm::rotate   (I, glm::radians(rotY), glm::vec3(0,1,0));

                lightF[0].position = glm::vec3(Ry2 * Rx2* glm::vec4(0.0, 1.0, 0.0, 1.0));
                lightF[0].direction = glm::vec3(Ry2 * Rx2* glm::vec4(0.0, -1.0, 0.0, 0.0));

                lightP[0].position = glm::vec3( Ry2 * Rx2 * glm::vec4(1, 1, 2,1));
                lightP[1].position = glm::vec3( Ry2 * Rx2 *  glm::vec4(-1.5, 1, 2.5,1.0));
                lightP[2].position = glm::vec3( Ry2 * Rx2 *  glm::vec4(-2, 1, -2.5,1.0));
                lightP[3].position = glm::vec3( Ry2 * Rx2 *  glm::vec4(2, 1, -0.5,1));
                lightP[4].position = glm::vec3( Ry2 * Rx2 * glm::vec4(-1.1, 2.2, -1.1,1.0));


            }
            break;
        case GLFW_KEY_LEFT:
            if (action==GLFW_PRESS || action == GLFW_REPEAT) {
                rotY -= 5.0f;
                glm::mat4 Rx2 = glm::rotate(I, glm::radians(rotX), glm::vec3(1, 0, 0));
                glm::mat4 Ry2 = glm::rotate   (I, glm::radians(rotY), glm::vec3(0,1,0));
                lightF[0].position = glm::vec3(Ry2 * Rx2 * glm::vec4(0.0, 1.0, 0.0, 1.0));
                lightF[0].direction = glm::vec3(Ry2 * Rx2*  glm::vec4(0.0, -1.0, 0.0, 0.0));


                lightP[0].position = glm::vec3( Ry2 * Rx2 * glm::vec4(1, 1, 2,1));
                lightP[1].position = glm::vec3( Ry2 * Rx2* glm::vec4(-1.5, 1, 2.5,1.0));
                lightP[2].position = glm::vec3( Ry2 * Rx2* glm::vec4(-2, 1, -2.5,1.0));
                lightP[3].position = glm::vec3( Ry2 * Rx2* glm::vec4(2, 1, -0.5,1));
                lightP[4].position = glm::vec3( Ry2 * Rx2* glm::vec4(-1.1, 2.2, -1.1,1.0));


            }
            break;
        case GLFW_KEY_RIGHT:
            if (action==GLFW_PRESS || action == GLFW_REPEAT) {
                rotY += 5.0f;
                glm::mat4 Rx2 = glm::rotate(I, glm::radians(rotX), glm::vec3(1, 0, 0));
                glm::mat4 Ry2 = glm::rotate   (I, glm::radians(rotY), glm::vec3(0,1,0));
                lightF[0].position = glm::vec3(Ry2 * Rx2* glm::vec4(0.0, 1.0, 0.0, 1.0));
                lightF[0].direction = glm::vec3(Ry2 * Rx2* glm::vec4(0.0, -1.0, 0.0, 0.0));


                lightP[0].position = glm::vec3( Ry2 * Rx2* glm::vec4(1, 1, 2,1));
                lightP[1].position = glm::vec3( Ry2 * Rx2* glm::vec4(-1.5, 1, 2.5,1.0));
                lightP[2].position = glm::vec3( Ry2 * Rx2* glm::vec4(-2, 1, -2.5,1.0));
                lightP[3].position = glm::vec3( Ry2 * Rx2* glm::vec4(2, 1, -0.5,1));
                lightP[4].position = glm::vec3( Ry2 * Rx2* glm::vec4(-1.1, 2.2, -1.1,1.0));


            }
            break;
        case GLFW_KEY_M:
            if(mods==GLFW_MOD_SHIFT) desZ -= desZ > -24.0f ? 0.1f : 0.0f;
            else                     desZ += desZ <   5.0f ? 0.1f : 0.0f;
            break;
        case GLFW_KEY_Z:
            if (mods==GLFW_MOD_SHIFT) {
                if (mods==GLFW_MOD_SHIFT && action == GLFW_REPEAT) {
                    movY -= movX <= 0 ? 0.0 : 0.025;
                    movX -= movX <= 0 ? 0.0 : 0.025;
                }
            } else {
                if (action == GLFW_REPEAT) {
                    movX += movX >= 0.9 ? 0.0 : 0.025;
                    movY += movX >= 0.9 ? 0.0 : 0.025;
                }
            }
            break;
        case GLFW_KEY_X:
            if (action==GLFW_PRESS) {
                turn_ovniEngine = !turn_ovniEngine;
                if (!turn_ovniEngine) {
                    lastAngle = angle;
                    lastAngle2 = angle2;
                }
                lastTime = glfwGetTime();
            }
            break;
        case GLFW_KEY_C:
            if (action==GLFW_PRESS) {
                turn_emiss = !turn_emiss;
                texCuerpoSup.emissive = turn_emiss ? imgCSEMIS.getTexture() : img1.getTexture();
                texCircle.emissive = turn_emiss ? imgCircleEMIS.getTexture() : img1.getTexture();
            }
            break;
        case GLFW_KEY_0:
            rotX = 0.0f;
            rotY = 0.0f;
            break;
        default:

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

    float limY = 180.0;
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
