#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shaders.h"
#include "Model.h"
#include "Texture.h"

const int PLANET_SKINS = 4;

// ===================================================

void configScene();
void renderScene();
void setLights (glm::mat4 P, glm::mat4 V);
void drawObjectMat(Model model, Material material, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawObjectTex(Model model, Textures textures, glm::mat4 P, glm::mat4 V, glm::mat4 M);

void drawOrbes(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawSoporte(glm::mat4 P, glm::mat4 V, glm::mat4 M);

void applyMatrices();

void funFramebufferSize(GLFWwindow* window, int width, int height);
void funKey            (GLFWwindow* window, int key  , int scancode, int action, int mods);
void funScroll         (GLFWwindow* window, double xoffset, double yoffset);
void funCursorPos      (GLFWwindow* window, double xpos, double ypos);

void funPlanetStyle    ();

// #####################################################################################################################

// Shaders
    Shaders shaders;

// Modelos
    Model sphere;

    Model capsula;
    Model cuerpo_sup;
    Model cuerpo_inf;
    Model circle;
    Model orbe;
    Model pieza1_pata;
    Model pieza2_pata;

    Model planeta;
    Model background;

    Model suzanne;

// ===================================================
// Imagenes (texturas)
    Texture img1; // IMG Emisiva Nula
    Texture img8;
    Texture imgWindowEMIS;

    //Cuerpo Superior Ovni
    Texture imgCSDIFF;
    Texture imgCSSPEC;
    Texture imgCSEMIS;

    //Cuerpo Inferior Ovni
    Texture imgCIDIFF;
    Texture imgCISPEC;
    Texture imgCINORM;
    Texture imgCIEMIS;

    //Base Cuerpo Superior Ovni
    Texture imgCircleDIFF;
    Texture imgCircleSPEC;
    Texture imgCircleNORM;
    Texture imgCircleEMIS;

    //Planeta
    Texture imgPlanetDIFF;
    Texture imgPlanetSPEC;
    Texture imgPlanetNORM;
    Texture imgPlanetDIFF2;
    Texture imgPlanetSPEC2;
    Texture imgPlanetEMIS2;
    Texture imgPlanetDIFF3;
    Texture imgPlanetNORM3;
    Texture imgPlanetDIFF4;

    //Fondo
    Texture imgStarMapDIFF;
    Texture imgStarMapEMIS;

// ===================================================
// Luces y materiales
    #define   NLD 1
    #define   NLP 6
    #define   NLF 1
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

// Acumuladores, contadores, etc
    //Giro modelo Ovni sobre el eje X
    float rotX = 0.0;
    //Giro modelo Ovni sobre el eje Y
    float rotY = 0.0;
    //Movilidad tren de aterrizaje Ovni sobre el eje X
    float movX = 0.0;
    //Movilidad tren de aterrizaje Ovni sobre el eje Y
    float movY = 0.0;

    //Movimiento del modelo Ovni sobre el planeta respecto al eje X
    float ws_mov = 0.0;
    //Movimiento del modelo Ovni sobre el planeta respecto al eje Z
    float ad_mov = 0.0;
    //Altura del modelo Ovni sobre el planeta respecto al eje Y
    float altura_Ovni = 0.0;

    //Angulo de rotación 1 : Velocidad = Alta
    float angle = 0.0;
    //Angulo de rotación 2 : Velocidad = Media
    float angle2 = 0.0;
    //Angulo de rotación 3 : Velocidad = Baja
    float angle3 = 0.0;

    float angleX_ovni = 0.0;
    float lastAngleX_ovni = 0.0;
    float angleZ_ovni = 0.0;
    float lastAngleZ_ovni = 0.0;
    
    //Acumulador que almacena el ultimo angulo de rotación 1
    float lastAngle = 0.0;
    //Acumulador que almacena el ultimo angulo de rotación 2
    float lastAngle2 = 0.0f;

    //Angulo de rotación de la capsula
    float ang_caps = 0.0;

// Tiempo actual
    float time = glfwGetTime();
// Tiempo anterior
    static float lastTime = 0.0;
    static float lastTime_ovniX = 0.0;
    static float lastTime_ovniZ = 0.0;

// Movimiento de camara
    //Campo de vision
    float fovy   = 60.0;
    //Distancia de la camara al origen sobre el eje Z
    float dist   = 30.0;
    //Ángulo de rotación de la camara sobre el eje X
    static float alphaX =  0.0;
    //Ángulo de rotación de la camara sobre el eje Y
    static float alphaY =  0.0;


//Selectores
    //Activar o desactivar arranque del motor del ovni
    bool turn_ovniEngine = false;
    //Activar o desactivar modo primera persona
    bool turn_firstP = false;
    //Activar o desactivar mapa emisivo de los modelos seleccionados
    bool turn_emiss = false;
    //Activar o desactivarr visibilidad piloto del ovni
    bool turn_invisible = true;
    //Activar o desactivar renderizacion de texturas
    bool texturasoff= false;

    bool forwback_mov = false;
    bool leftright_mov = false;
    bool turn_ovniMovX = false;
    bool turn_ovniMovZ = false;

    bool luzp5off=false;

    //OPCIONES PLANETA
    int select_planet = 1;

    //Intensidad de la luz
    float incLight = 1.0;
    // Apagar o Encender las luces
    float onoff = 0.0;

    float onoffP5 = 1.0;
    float rotPY = 0.0;

    // Modo para cambiar entre solido y alambre
    unsigned int mode = GL_FILL;

// Matrices y vectores
    //Eje de coordenadas X del mundo
    glm::vec3 X_axis = glm::vec3(1, 0, 0);
    //Eje de coordenadas Y del mundo
    glm::vec3 Y_axis = glm::vec3(0, 1, 0);
    //Eje de coordenadas Z del mundo
    glm::vec3 Z_axis = glm::vec3(0, 0, 1);

    //Centro del planeta según las coordenadas del mundo
    glm::vec3 center_p = glm::vec3(0, -13.8, 0);
    //Eje fisico local para rotar capsula del ovni
    glm::vec3 bisel_caps = glm::vec3(1, 2.8, 0);
    

    //Rotación de 90 grados sobre el eje Y
    glm::mat4 RY_90 = glm::rotate(I, glm::radians(90.0f), Y_axis);

    //Declaración de matrices
    glm::mat4 M_capsula, M_cuerpo_sup, M_cuerpo_inf, M_circle, M_planeta, M_background, M_orbs, M_pata;

    //Tamaño del fondo constante
    glm::mat4 S_bg = glm::scale (I, glm::vec3(50.0, 50.0, 50.0));
    //Posicion del planeta constante
    glm::mat4 T_planeta = glm::translate(I, glm::vec3(0.0,0.2,0.0));

    glm::mat4 M_ovni;


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
    window = glfwCreateWindow(w, h, "Project UFO", nullptr, nullptr);
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
        // Obtenemos el tiempo actual
        time = glfwGetTime();
        applyMatrices();
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
    cuerpo_sup.initModel("resources/models/cuerpo_sup.obj");
    cuerpo_inf.initModel("resources/models/cuerpo_inf_newUVmapping.obj");
    circle.initModel("resources/models/circle.obj");
    orbe.initModel("resources/models/orbe.obj");
    pieza1_pata.initModel("resources/models/pieza1_pata.obj");
    pieza2_pata.initModel("resources/models/pieza2_pata.obj");

    planeta.initModel("resources/models/planeta.obj");
    background.initModel("resources/models/background.obj");

    suzanne.initModel("resources/models/suzanne.obj");

 // Imagenes (texturas)
    img1.initTexture("resources/textures/img1.png");
    img8.initTexture("resources/textures/imgUfo_window3.png");
    imgWindowEMIS.initTexture("resources/textures/imgUfo_window4.png");

    imgCSDIFF.initTexture("resources/textures/cuerpo_supS.png");
    imgCSSPEC.initTexture("resources/textures/imgCS_spec4.png"); //specularAO1
    imgCSEMIS.initTexture("resources/textures/cuerpo_sup_emissive_y3.png");

    imgCIDIFF.initTexture("resources/textures/cuerpo_inf.jpg");//texture.png //cuerpo_inf_metal //6842-v7 //cuerpo_inf.jpg
    imgCISPEC.initTexture("resources/textures/cuerpo_inf_spec.png");
    imgCINORM.initTexture("resources/textures/imgCI_spec.png");
    imgCIEMIS.initTexture("resources/textures/cuerpo_inf_emis2.jpg");

    imgCircleDIFF.initTexture("resources/textures/circle.jpg");
    imgCircleSPEC.initTexture("resources/textures/circle_spec.png");
    imgCircleNORM.initTexture("resources/textures/circle_normal.png");
    imgCircleEMIS.initTexture("resources/textures/circle_emiss_purple.jpg");

    imgPlanetDIFF.initTexture("resources/textures/imgMars.jpg");
    imgPlanetNORM.initTexture("resources/textures/imgMars_norm2.png");
    imgPlanetDIFF2.initTexture("resources/textures/tierra.jpg");
    imgPlanetSPEC2.initTexture("resources/textures/tierra_spec2.png");
    imgPlanetEMIS2.initTexture("resources/textures/tierra_citylights.jpg");
    imgPlanetDIFF3.initTexture("resources/textures/imgMoon.jpg");
    imgPlanetNORM3.initTexture("resources/textures/imgMoon_norm.jpg");
    imgPlanetDIFF4.initTexture("resources/textures/imgSun.jpg");

    imgStarMapDIFF.initTexture("resources/textures/starmap_4k.jpg");
    imgStarMapEMIS.initTexture("resources/textures/starmap_4k_emiss.jpg");

 // Luz ambiental global
    lightG.ambient = glm::vec3(0.5, 0.5, 0.5);

 // Luces direccionales
    lightD[0].direction = glm::vec3(0.0, -1.0, 0.0);
    lightD[0].ambient   = incLight * glm::vec3(0.1,  0.1, 0.1);
    lightD[0].diffuse   = incLight * glm::vec3(0.7,  0.7, 0.7);
    lightD[0].specular  = incLight * glm::vec3(0.7,  0.7, 0.7);

    // Luces posicionales

    lightP[0].position    = glm::vec3(1, 1, 2);
    lightP[0].ambient     = onoff * glm::vec3(0, 0, 1);
    lightP[0].diffuse     = onoff * glm::vec3(0, 0, 1);
    lightP[0].specular    = onoff * glm::vec3(0, 0, 1);
    lightP[0].c0          = 1.00;
    lightP[0].c1          = 0.22;
    lightP[0].c2          = 0.20;


    lightP[1].position    = glm::vec3(-1.5, 1, 2.5);
    lightP[1].ambient     = onoff * glm::vec3(0, 1, 0);
    lightP[1].diffuse     = onoff * glm::vec3(0, 1, 0);
    lightP[1].specular    = onoff * glm::vec3(0, 1, 0);
    lightP[1].c0          = 1.00;
    lightP[1].c1          = 0.22;
    lightP[1].c2          = 0.20;


    lightP[2].position    = glm::vec3(-2, 1, -2.5);
    lightP[2].ambient     = onoff * glm::vec3(1, 0, 0);
    lightP[2].diffuse     = onoff * glm::vec3(1, 0, 0);
    lightP[2].specular    = onoff * glm::vec3(1, 0, 0);
    lightP[2].c0          = 1.00;
    lightP[2].c1          = 0.22;
    lightP[2].c2          = 0.20;



    lightP[3].position    = glm::vec3(2, 1, -0.5);
    lightP[3].ambient     = onoff * glm::vec3(1, 0, 1);
    lightP[3].diffuse     = onoff * glm::vec3(1, 0, 1);
    lightP[3].specular    = onoff * glm::vec3(1, 0, 1);
    lightP[3].c0          = 1.00;
    lightP[3].c1          = 0.22;
    lightP[3].c2          = 0.20;


    lightP[4].position    = glm::vec3(-1.1, 2.2, -1.1);
    lightP[4].ambient     = onoff * glm::vec3(0, 1, 1);
    lightP[4].diffuse     = onoff * glm::vec3(0, 1, 1);
    lightP[4].specular    = onoff * glm::vec3(0, 1, 1);
    lightP[4].c0          = 1.00;
    lightP[4].c1          = 0.22;
    lightP[4].c2          = 0.20;


    lightP[5].position    =  glm::vec3(0.0, 3.0, 3.0);
    lightP[5].ambient     = onoffP5 * glm::vec3(0.2, 0.2, 0.2);
    lightP[5].diffuse     = onoffP5 * glm::vec3(0.9, 0.9, 0.9);
    lightP[5].specular    = onoffP5 * glm::vec3(0.9, 0.9, 0.9);
    lightP[5].c0          = 1.00;
    lightP[5].c1          = 0.22;
    lightP[5].c2          = 0.20;



    // Luces focales
    lightF[0].position    = glm::vec3(0.0, 1.0, 0.0);  // Cambia la posición en el eje y
    lightF[0].direction   = glm::vec3(0.0, -1.0, 0.0);  // Cambia la dirección si es necesario
    lightF[0].ambient     = onoff * glm::vec4(0.7, 0.7, 0.0, 1.0);  // Amarillo
    lightF[0].diffuse     = onoff * glm::vec4(0.7, 0.7, 0.0, 1.0);
    lightF[0].specular    = onoff * glm::vec4(0.7, 0.7, 0.0, 1.0);
    lightF[0].innerCutOff = 25.0;
    lightF[0].outerCutOff = lightF[0].innerCutOff + 0.1;
    lightF[0].c0          = 1.0;
    lightF[0].c1          = 0.03;
    lightF[0].c2          = 0.05;




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
    texCuerpoSup.shininess = 1.0; //10.0; //32.0; //64.0;

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
    texPlanet.specular  = img1.getTexture();
    texPlanet.emissive  = img1.getTexture(); //Reaccion a tecla + -
    texPlanet.normal    = imgPlanetNORM.getTexture();
    texPlanet.shininess = 32.0;

    texStars.diffuse   = imgStarMapDIFF.getTexture();
    texStars.specular  = imgStarMapEMIS.getTexture();
    texStars.emissive  = imgStarMapEMIS.getTexture();
    texStars.normal    = 0;
    texStars.shininess = 10.0;

}

void applyMatrices() {

    // Uso y manipulación de matrices
    glm::mat4 Rx_mLocal = glm::rotate   (I, glm::radians(rotX), X_axis);
    glm::mat4 Ry_mLocal = glm::rotate   (I, glm::radians(rotY), Y_axis);
    // Trasladar el sistema de coordenadas
    glm::mat4 translate = glm::translate(I, -center_p);
    // Trasladar el sistema de coordenadas de vuelta
    glm::mat4 translateBack = glm::translate(I, center_p);
    // Trasladar el ovni a la altura deseada
    glm::mat4 T_alturaOvni = glm::translate(I, glm::vec3(0.0,altura_Ovni,0.0));

    // Realizar la rotación
    if (forwback_mov)
        angleX_ovni = lastAngleX_ovni + (time - lastTime_ovniX) * glm::radians(2.0f * 50.0f);
    else 
        angleX_ovni = lastAngleX_ovni - (time - lastTime_ovniX) * glm::radians(2.0f * 50.0f);
    if (leftright_mov)
        angleZ_ovni = lastAngleZ_ovni + (time - lastTime_ovniZ) * glm::radians(2.0f * 50.0f);
    else
        angleZ_ovni = lastAngleZ_ovni - (time - lastTime_ovniZ) * glm::radians(2.0f * 50.0f);

    glm::mat4 Rx_mGlobal = glm::rotate (I, turn_ovniMovX ? angleX_ovni : lastAngleX_ovni, X_axis);
    glm::mat4 Rz_mGlobal = glm::rotate(I, turn_ovniMovZ ? angleZ_ovni : lastAngleZ_ovni, Z_axis);
    // Combinar las transformaciones
    glm::mat4 M_transform = translateBack * Rz_mGlobal * Rx_mGlobal * translate;

    //Matriz general de transformación del ovni
    M_ovni = M_transform * T_alturaOvni * Ry_mLocal * Rx_mLocal;

    glm::mat4 rotateCaps = glm::rotate(I, glm::radians(ang_caps), Z_axis);
    glm::mat4 translateC = glm::translate(I, -bisel_caps);
    glm::mat4 translateBackC = glm::translate(I, bisel_caps);

    glm::mat4 M_op_caps = translateBackC * rotateCaps * translateC;

    // Calcula el ángulo de rotación basado en el tiempo
    angle = lastAngle + (time - lastTime) * glm::radians(2.0f * 80.0f);
    angle2 = lastAngle2 + (time - lastTime) * glm::radians(2.0f * 50.0f);

    angle3 = time * glm::radians(1.0f * 1.0f);

    glm::mat4 Ry_fast = glm::rotate   (I, turn_ovniEngine ? angle : lastAngle, Y_axis);
    glm::mat4 Ry_medium = glm::rotate   (I, turn_ovniEngine ? -angle2 : -lastAngle2, Y_axis);
    glm::mat4 Ry_medium2 = glm::rotate   (I, turn_ovniEngine ? angle2*0.1f : lastAngle2*0.1f, Y_axis);

    glm::mat4 Rz_slow = glm::rotate   (I, -angle3, Z_axis);

    M_capsula = M_ovni * M_op_caps;
    M_cuerpo_sup = M_ovni * Ry_fast;
    M_cuerpo_inf = M_ovni * Ry_medium;
    M_circle = M_ovni * Ry_medium2;
    M_orbs = M_ovni * Ry_medium;
    M_pata = M_ovni * Ry_medium;

    M_planeta = T_planeta;
    M_background = Rz_slow * S_bg;
}

// #####################################################################################################################

void renderScene() {

 // Borramos el buffer de color
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 // Indicamos los shaders a utilizar
    shaders.useShaders();


// Matriz P
    float nplane =   0.1;
    float fplane = 150.0;
    float aspect = (float)w/(float)h;
    glm::mat4 P = glm::perspective(glm::radians(fovy), aspect, nplane, fplane);

// Matriz V
    float x = dist*glm::cos(glm::radians(alphaY))*glm::sin(glm::radians(alphaX));
    float y = dist*glm::sin(glm::radians(alphaY));
    float z = dist*glm::cos(glm::radians(alphaY))*glm::cos(glm::radians(alphaX));

    glm::vec3 direction (glm::radians(alphaY)*glm::cos(glm::radians(alphaX)),
                         glm::radians(alphaY),
                         glm::radians(alphaY)*glm::sin(glm::radians(alphaX)));



    glm::vec3 eye   (  x,   y,   z);  //Posición de la cámara en el mundo
    glm::vec3 center(0.0, 0.0,  0.0); //Dirección donde la cámara mira
    glm::vec3 up    (0.0, 1.0,  0.0);

    glm::vec3 eye_firstPerson   (  0,   3.5,   0.0);  //Posición de la cámara en el mundo


    glm::vec3 center_firstPerson = M_ovni * glm::vec4(eye, 1.0);

    // Calcular el vector "up_firstPerson"
    glm::vec3 up_firstPerson = glm::vec3(M_ovni * glm::vec4(up, 1.0));

    eye_firstPerson = glm::vec3(M_ovni * glm::vec4(eye_firstPerson, 1.0));


    glm::mat4 V = turn_firstP ? glm::lookAt(eye_firstPerson, center_firstPerson, up_firstPerson) : glm::lookAt(eye, center, up);
    shaders.setVec3("ueye",eye);

// Fijamos las luces
    setLights(P,V);

// Dibujamos la escena
    if (texturasoff == false) {

        // Enable back face culling //////////////////////////////////////////////////////
        glEnable(GL_CULL_FACE); // Enable culling
        glCullFace(GL_BACK);

        drawObjectTex(cuerpo_sup, texCuerpoSup, P, V, M_cuerpo_sup);

        drawObjectTex(cuerpo_inf, texCuerpoInf, P, V, M_cuerpo_inf);

        drawOrbes(P, V, M_orbs);

        drawSoporte(P, V, M_pata);

        drawObjectTex(planeta, texPlanet, P, V, M_planeta);

        if (!turn_firstP && !turn_invisible) drawObjectMat(suzanne, cromo, P, V, M_ovni);

        // Disable back face culling
        glDisable(GL_CULL_FACE);

        // Enable front face culling //////////////////////////////////////////////////////
        glEnable(GL_CULL_FACE); // Enable culling
        glCullFace(GL_FRONT); // Specify that front faces should be culled

        drawObjectTex(background, texStars, P, V, M_background);

        drawObjectTex(circle, texCircle, P, V, M_circle);

        // Disable front face culling
        glDisable(GL_CULL_FACE);


        //Objetos transparentes //////////////////////////////////////////////////////

        // Limpiar el buffer de stencil
        glClear(GL_STENCIL_BUFFER_BIT);

        // Habilitar el buffer de stencil
        glEnable(GL_STENCIL_TEST);

        // Configurar el buffer de stencil para marcar los píxeles donde se dibuja la cápsula
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

        glDepthMask(GL_FALSE);
        drawObjectTex(capsula, texWindow, P, V, M_capsula);
        glDepthMask(GL_TRUE);

        // Configurar el buffer de stencil para descartar los píxeles marcados
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

        // Dibujar a Suzanne solo en los píxeles no marcados
        if (!turn_firstP && turn_invisible) drawObjectMat(suzanne, cromo, P, V, M_ovni);

        // Deshabilitar el buffer de stencil
        glDisable(GL_STENCIL_TEST);

    }else{
        // Enable back face culling //////////////////////////////////////////////////////
        glEnable(GL_CULL_FACE); // Enable culling
        glCullFace(GL_BACK);

        drawObjectMat(cuerpo_sup, cromo, P, V, M_cuerpo_sup);

        drawObjectMat(cuerpo_inf, cromo, P, V, M_cuerpo_inf);

        drawOrbes(P, V, M_orbs);

        drawSoporte(P, V, M_pata);

        drawObjectMat(planeta, cromo, P, V, M_planeta);

        // Disable back face culling
        glDisable(GL_CULL_FACE);

        // Enable front face culling //////////////////////////////////////////////////////
        glEnable(GL_CULL_FACE); // Enable culling
        glCullFace(GL_FRONT); // Specify that front faces should be culled

        drawObjectMat(background, cromo, P, V, M_background);

        drawObjectMat(circle, cromo, P, V, M_circle);

        // Disable front face culling
        glDisable(GL_CULL_FACE);


        //Objetos transparentes //////////////////////////////////////////////////////

        drawObjectMat(capsula, cromo, P, V, M_capsula);
    }
}

// #####################################################################################################################

void drawSoporte(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 T_act = glm::translate(I, glm::vec3(-movX,movY,0.0));

    drawObjectMat(pieza1_pata, turquesa, P, V, M*T_act);
    drawObjectMat(pieza2_pata, cromo, P, V, M*T_act);

    drawObjectMat(pieza1_pata, turquesa, P, V, M * RY_90 * T_act);
    drawObjectMat(pieza2_pata, cromo, P, V, M * RY_90 * T_act);
    drawObjectMat(pieza1_pata, turquesa, P, V, M * RY_90 * RY_90 * T_act);
    drawObjectMat(pieza2_pata, cromo, P, V, M * RY_90 * RY_90 * T_act);
    drawObjectMat(pieza1_pata, turquesa, P, V, M * RY_90 * RY_90 * RY_90 * T_act);
    drawObjectMat(pieza2_pata, cromo, P, V, M * RY_90 * RY_90 * RY_90 * T_act);

}

void drawOrbes(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    drawObjectMat(orbe, onoff == 1? mluz : cromo, P, V, M);
    drawObjectMat(orbe, onoff == 1? mluz : cromo, P, V, M * RY_90);
    drawObjectMat(orbe, onoff == 1? mluz : cromo, P, V, M * RY_90 * RY_90);
    drawObjectMat(orbe, onoff == 1? mluz : cromo, P, V, M * RY_90 * RY_90 * RY_90);

}

void setLights(glm::mat4 P, glm::mat4 V) {

    shaders.setLight("ulightG",lightG);
    for(int i=0; i<NLD; i++) shaders.setLight("ulightD["+toString(i)+"]",lightD[i]);
    for(int i=0; i<NLP; i++) shaders.setLight("ulightP["+toString(i)+"]",lightP[i]);
    for(int i=0; i<NLF; i++) shaders.setLight("ulightF["+toString(i)+"]",lightF[i]);

    if(!luzp5off) {
        glm::mat4 Mp = glm::translate(I, lightP[5].position) * glm::scale(I, glm::vec3(0.1));
        drawObjectMat(sphere, mluz, P, V, Mp);
    }

    lightF[0].position  = glm::vec3(M_ovni * glm::vec4(0.0, 1.0, 0.0, 1.0));
    lightF[0].direction = glm::vec3(M_ovni * glm::vec4(0.0, -1.0, 0.0, 0.0));


    lightP[0].position = glm::vec3(M_orbs * glm::vec4(1, 1, 2,1));
    lightP[1].position = glm::vec3(M_orbs * glm::vec4(-1.5, 1, 2.5,1.0));
    lightP[2].position = glm::vec3(M_orbs * glm::vec4(-2, 1, -2.5,1.0));
    lightP[3].position = glm::vec3(M_orbs * glm::vec4(2, 1, -0.5,1));
    lightP[4].position = glm::vec3(M_ovni * glm::vec4(-1.1, 2.2, -1.1,1.0));
}

// #####################################################################################################################

void drawObjectMat(Model model, Material material, glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    shaders.setMat4("uN"  ,glm::transpose(glm::inverse(M)));
    shaders.setMat4("uM"  ,M);
    shaders.setMat4("uPVM",P*V*M);
    shaders.setBool("uWithMaterials",true);
    shaders.setMaterial("umaterial",material);
    model.renderModel(mode);

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
    model.renderModel(mode);

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

void funKey(GLFWwindow* window, int key, int scancode, int action, int mods) {

    switch(key) {
        case GLFW_KEY_1:
            if (action == GLFW_PRESS) {
                turn_firstP = !turn_firstP;
            }
            break;

        case GLFW_KEY_2:
            if (action == GLFW_PRESS) {
                fovy += 10;
                if (fovy > 170) fovy = 20;
            }
            break;

        case GLFW_KEY_3: if (action==GLFW_PRESS) funPlanetStyle(); break;

        case GLFW_KEY_4:
            if (action==GLFW_PRESS) {
                if (!texturasoff) {
                    texturasoff=true;
                } else {
                    texturasoff=false;
                }
            }
            break;

        case GLFW_KEY_5:
            if (action==GLFW_PRESS) {
                if (mode == GL_FILL) {
                    mode = GL_LINE;
                } else {
                    mode = GL_FILL;
                }
            }
            break;

        case GLFW_KEY_6:
            if (action==GLFW_PRESS) {
                turn_invisible = !turn_invisible;
            }
            break;
        case GLFW_KEY_KP_ADD:
            if (action == GLFW_PRESS || action == GLFW_REPEAT) {
                incLight += incLight < 10.0 ? 0.1 : 0.0;
                if (incLight >= 0.2 && select_planet==2) texPlanet.emissive  = img1.getTexture();

                lightD[0].ambient   = incLight * glm::vec3(0.1,  0.1, 0.1);
                lightD[0].diffuse   = incLight * glm::vec3(0.7,  0.7, 0.7);
                lightD[0].specular  = incLight * glm::vec3(0.7,  0.7, 0.7);
            }
            break;
        case GLFW_KEY_KP_SUBTRACT:
            if (action == GLFW_PRESS || action == GLFW_REPEAT) {
                incLight -= 0.1;
                if (incLight < 0.2 && select_planet==2) texPlanet.emissive  = imgPlanetEMIS2.getTexture();

                lightD[0].ambient   = incLight * glm::vec3(0.1,  0.1, 0.1);
                lightD[0].diffuse   = incLight * glm::vec3(0.7,  0.7, 0.7);
                lightD[0].specular  = incLight * glm::vec3(0.7,  0.7, 0.7);
            }
            break;

        case GLFW_KEY_UP:
            if (action==GLFW_PRESS || action == GLFW_REPEAT) {
                rotX -= 5.0;
                if (rotX < 0) rotX = 355;
            }
            break;
        case GLFW_KEY_DOWN:
            if (action==GLFW_PRESS || action == GLFW_REPEAT) {
                rotX += 5.0;
                if (rotX > 355) rotX = 0;
            }
            break;
        case GLFW_KEY_LEFT:
            if (action==GLFW_PRESS || action == GLFW_REPEAT) {
                rotY -= 5.0;
                if (rotY < 0) rotY = 355;
            }
            break;
        case GLFW_KEY_RIGHT:
            if (action==GLFW_PRESS || action == GLFW_REPEAT) {
                rotY += 5.0;
                if (rotY > 355) rotY = 0;
            }
            break;
        case GLFW_KEY_S:
            if (action == GLFW_PRESS || action == GLFW_REPEAT)
                if (!turn_ovniMovX) {
                    forwback_mov = true;
                    turn_ovniMovX = true;
                    lastTime_ovniX = time; // Guardamos el tiempo cuando la tecla es presionada
                }
            if (action == GLFW_RELEASE)
                if (turn_ovniMovX) {
                    turn_ovniMovX = false;
                    lastAngleX_ovni = angleX_ovni;
                }
            break;
        case GLFW_KEY_W:
            if (action == GLFW_PRESS || action == GLFW_REPEAT)
                if (!turn_ovniMovX) {
                    forwback_mov = false;
                    turn_ovniMovX = true;
                    lastTime_ovniX = time; // Guardamos el tiempo cuando la tecla es presionada
                }
            if (action == GLFW_RELEASE)
                if (turn_ovniMovX) {
                    turn_ovniMovX = false;
                    lastAngleX_ovni = angleX_ovni;
                }
            break;
        case GLFW_KEY_A:
            if (action == GLFW_PRESS || action == GLFW_REPEAT)
                if (!turn_ovniMovZ) {
                    leftright_mov = true;
                    turn_ovniMovZ = true;
                    lastTime_ovniZ = time; // Guardamos el tiempo cuando la tecla es presionada
                }
            if (action == GLFW_RELEASE)
                if (turn_ovniMovZ) {
                    turn_ovniMovZ = false;
                    lastAngleZ_ovni = angleZ_ovni;
                }
            break;
        case GLFW_KEY_D:
            if (action == GLFW_PRESS || action == GLFW_REPEAT)
                if (!turn_ovniMovZ) {
                    leftright_mov = false;
                    turn_ovniMovZ = true;
                    lastTime_ovniZ = time; // Guardamos el tiempo cuando la tecla es presionada
                }
            if (action == GLFW_RELEASE)
                if (turn_ovniMovZ) {
                    turn_ovniMovZ = false;
                    lastAngleZ_ovni = angleZ_ovni;
                }
            break;
        case GLFW_KEY_Q:
            if (action==GLFW_PRESS || action == GLFW_REPEAT) {
                altura_Ovni += 0.1;
            }
            break;
        case GLFW_KEY_E:
            if (action==GLFW_PRESS || action == GLFW_REPEAT) {
                altura_Ovni -= altura_Ovni > 0.0 ? 0.1 : 0.0;
            }
            break;
        case GLFW_KEY_Z:
            if (mods == GLFW_MOD_SHIFT) {
                if (action == GLFW_REPEAT) {
                    movX -= movX < 0.025 ? 0.0 : 0.025;
                    movY -= movY < 0.025 ? 0.0 : 0.025;
                }
            } else {
                if (action == GLFW_REPEAT) {
                    movX += movX > 0.875 ? 0.0 : 0.025;
                    movY += movY > 0.875 ? 0.0 : 0.025;
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
                texCuerpoInf.emissive = turn_emiss ? imgCIEMIS.getTexture() : img1.getTexture();
                texCircle.emissive = turn_emiss ? imgCircleEMIS.getTexture() : img1.getTexture();
                texWindow.emissive = turn_emiss ? imgWindowEMIS.getTexture() : img1.getTexture();
            }
            break;
        case GLFW_KEY_F:
            if (action == GLFW_PRESS) {
                if(onoff==0.0)
                    onoff=1.0;
                else
                    onoff=0.0;

                lightP[0].ambient     = onoff * glm::vec3(0, 0, 1);
                lightP[0].diffuse     = onoff * glm::vec3(0, 0, 1);
                lightP[0].specular    = onoff * glm::vec3(0, 0, 1);

                lightP[1].ambient     = onoff * glm::vec3(0, 1, 0);
                lightP[1].diffuse     = onoff * glm::vec3(0, 1, 0);
                lightP[1].specular    = onoff * glm::vec3(0, 1, 0);

                lightP[2].ambient     = onoff * glm::vec3(1, 0, 0);
                lightP[2].diffuse     = onoff * glm::vec3(1, 0, 0);
                lightP[2].specular    = onoff * glm::vec3(1, 0, 0);

                lightP[3].ambient     = onoff * glm::vec3(1, 0, 1);
                lightP[3].diffuse     = onoff * glm::vec3(1, 0, 1);
                lightP[3].specular    = onoff * glm::vec3(1, 0, 1);

                lightP[4].ambient     = onoff * glm::vec3(0, 1, 1);
                lightP[4].diffuse     = onoff * glm::vec3(0, 1, 1);
                lightP[4].specular    = onoff * glm::vec3(0, 1, 1);

                lightF[0].ambient     = onoff * glm::vec4(0.7, 0.7, 0.0, 1.0);
                lightF[0].diffuse     = onoff * glm::vec4(0.7, 0.7, 0.0, 1.0);
                lightF[0].specular    = onoff * glm::vec4(0.7, 0.7, 0.0, 1.0);
            }
            break;
        case GLFW_KEY_M:
            if (action==GLFW_PRESS || action == GLFW_REPEAT) {
                ang_caps -= ang_caps > -100 ? 5.0 : 0.0;
            }
            break;
        case GLFW_KEY_N:
            if (action==GLFW_PRESS || action == GLFW_REPEAT) {
                ang_caps += ang_caps < 0 ? 5.0 : 0.0;
            }
            break;
        case GLFW_KEY_P:
            if (mods == GLFW_MOD_SHIFT) {
                if (action==GLFW_PRESS || action == GLFW_REPEAT) {
                    rotPY+=5.0;
                    glm::mat4 Ry = glm::rotate   (I, glm::radians(rotPY), Y_axis);
                    lightP[5].position = glm::vec3(Ry * glm::vec4(0.0, 3.0, 3.0,1.0));

                }
            } else {
                if (action==GLFW_PRESS || action == GLFW_REPEAT) {
                    rotPY-=5.0;
                    glm::mat4 Ry = glm::rotate   (I, glm::radians(rotPY), Y_axis);
                    lightP[5].position = glm::vec3(Ry * glm::vec4(0.0, 3.0, 3.0,1.0));
                }
            }
            break;

        case GLFW_KEY_T:
            if (action == GLFW_PRESS) {
                if (onoffP5 == 0.0) {
                    onoffP5 = 1.0;
                    luzp5off=false;
                }else {
                    onoffP5 = 0.0;
                    luzp5off= true;
                }

                lightP[5].ambient     =  onoffP5 * glm::vec3(0.2, 0.2, 0.2);
                lightP[5].diffuse     =  onoffP5 * glm::vec3(0.9, 0.9, 0.9);
                lightP[5].specular    =  onoffP5 * glm::vec3(0.9, 0.9, 0.9);

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

    if (turn_firstP) return;

    if(yoffset>0)
        dist -= dist>10.0 ? 5.0  : 0.0;
    if(yoffset<0)
        dist += dist<100.0   ? 5.0  : 0.0;
}

// #####################################################################################################################

void funCursorPos(GLFWwindow* window, double xpos, double ypos) {

    if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT)==GLFW_RELEASE && !turn_firstP) {

        return;
    }

    float limY = 89.0;
    alphaX = turn_firstP ? 360.0*(1.0 - 2.0*(xpos)/(float)w) : 360.0*(2.0*(xpos)/(float)w - 1.0);
    alphaY = 90.0*(1.0 - 2.0*(ypos)/(float)h);
    if(alphaY<-limY) alphaY = -limY;
    if(alphaY> limY) alphaY =  limY;

}

// #####################################################################################################################

void funPlanetStyle() {

    select_planet++;
    if (select_planet > PLANET_SKINS) select_planet = 1;

    switch (select_planet) {
        case 1:
            texPlanet.diffuse   = imgPlanetDIFF.getTexture();
            texPlanet.specular  = imgPlanetSPEC.getTexture();
            texPlanet.normal    = imgPlanetNORM.getTexture();
            texPlanet.emissive  = img1.getTexture();
            break;
        case 2:
            texPlanet.diffuse   = imgPlanetDIFF2.getTexture();
            texPlanet.specular  = imgPlanetSPEC2.getTexture();
            texPlanet.normal    = 0;
            texPlanet.emissive  = incLight < 0.2 ? imgPlanetEMIS2.getTexture() : img1.getTexture();
            break;
        case 3:
            texPlanet.diffuse   = imgPlanetDIFF3.getTexture();
            texPlanet.specular  = img1.getTexture();
            texPlanet.normal    = imgPlanetNORM3.getTexture();
            texPlanet.emissive  = img1.getTexture();
            break;
        case 4:
            texPlanet.diffuse   = imgPlanetDIFF4.getTexture();
            texPlanet.specular  = img1.getTexture();
            texPlanet.normal    = 0;
            texPlanet.emissive  = imgPlanetDIFF4.getTexture();
            break;
    }
}
