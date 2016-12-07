/*
Kory Dondzila
Garret Richardson

WarbirdSimulation.cpp
10/05/2016

Main file that initializes the scene, loads model files, scene entities,
performs update and display methods. Takes user input for changing cameras,
update speed, toggle axes, and toggle idle function.

*/

#include "../includes465/include465.hpp"
#include "../includes465/texture.hpp"
#include <string>
#include <vector>

// Initial gl includes required before wglext.h/glxext.h include
#ifdef _WIN32
#include <Windows.h>
#include "../includes465/wglext.h"
bool WGLExtensionSupported(const char *extension_name)
{
    // this is pointer to function which returns pointer to string with list of all wgl extensions
    PFNWGLGETEXTENSIONSSTRINGEXTPROC _wglGetExtensionsStringEXT = NULL;

    // determine pointer to wglGetExtensionsStringEXT function
    _wglGetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC)wglGetProcAddress("wglGetExtensionsStringEXT");

    if (strstr(_wglGetExtensionsStringEXT(), extension_name) == NULL)
    {
        // string was not found
        return false;
    }

    // extension is supported
    return true;
}

PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;
#endif


#include "Scene.hpp"
#include "CelestialBody.hpp"
#include "Ship.hpp"
#include "MissileBattery.hpp"
#include "Missile.hpp"
#include "DynamicCamera.hpp"
#include "Skybox.hpp"

// constants for models:  file names, vertex count, model display size
const int nModels = 9;  // number of models in this scene
char * modelFile [nModels] = {"src/axes-r100.tri", "src/Missile.tri", "src/Warbird.tri",
                              "src/Ruber.tri", "src/Unum.tri", "src/Duo.tri", "src/Primus.tri", "src/Secundus.tri",
                              "src/MissileBattery.tri"
                             };
const int nVertices[nModels] = { 120 * 3, 928 * 3, 4914 * 3, 760 * 3, 760 * 3, 760 * 3, 760 * 3, 760 * 3, 112 * 3};
char * vertexShaderFile   = "src/simpleVertex.glsl";
char * fragmentShaderFile = "src/simpleFragment.glsl";


// Shader handles, matrices, etc
GLuint shaderProgram;
GLuint skyboxShaderProgram;
GLuint MVP, NormalMatrix, ModelView;  // Model View Projection matrix's handle
GLuint VAO[nModels], buffer[nModels];

//skybox stuff
char * skyboxVertexFile = "src/skyboxVertex.glsl";
char * skyboxFragmentFile = "src/skyboxFragment.glsl";
GLuint skyboxVAO, skyboxVBO;
GLfloat skyboxVertices[] =
{
    // Positions
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f,  1.0f
};

// Lights and textures
glm::vec3 lightDir[2];
glm::vec4 shipPos;
GLuint texture, Texture, showTexture, light[2], shipLightPos;  // texture id, shader, light handles
GLuint ruberLight, headLight, shipLight, ambient, noLighting, debug;
bool ruberLightOn = true, headLightOn = true, shipLightOn = true, ambientOn = true, debugOn = false;

//skybox cubemap
std::vector<const GLchar*> faces;
GLuint skyboxTexture;

// model, view, projection matrices and values to create modelMatrix.
glm::mat3 normalMatrix;
glm::mat4 modelMatrix;          // set in display()
glm::mat4 modelViewMatrix;
glm::mat4 projectionMatrix;     // set in reshape()
glm::mat4 ModelViewProjectionMatrix; // set in display();

// flags
bool showAxesFlag = false, idleTimerFlag = true, mb1 = true, mb2 = true, ship = true;

// Constants for scene
Scene* scene = Scene::Instance();  // Scene object
int tq = 0, frameCount = 0, updateCount = 0;
double currentTime, lastTime, timeInterval, ulastTime, utimeInterval;
int shipID;

// Constants for cameras
glm::mat4 viewMatrix;  // Current view matrix
StaticCamera* viewingCamera;  // Current camera
DynamicCamera* warpCamera; //current camera to warp to

// window title string
char titleStr[160];
char baseStr [76] = "Warbird Simulation {a - Idle, x - Prev Cam, v - Next Cam, u - Axes, t - TQ}";
char shipCountStr[12] = "  Warbird ?";
char unumCountStr[9] = "  Unum ?";
char secundusCountStr[13] = "  Secundus ?";
char upsStr[10] = "  U/S ???";
char fpsStr[11] = "  F/S ????";
char viewStr[13] = "  View Front";

// Update window display and projection matrix
void reshape(int width, int height)
{
    projectionMatrix = viewingCamera->UpdateProjectionMatrix(width, height);
    float aspectRatio = (float)width / (float)height;
    glViewport(0, 0, width, height);
    printf("reshape: FOVY = %5.2f, width = %4d height = %4d aspect = %5.2f \n + nearclip = %5f farclip = %5f \n",
           viewingCamera->FOVY(), width, height, aspectRatio, viewingCamera->NearClip(), viewingCamera->FarClip());
}

// Update window title
void updateTitle()
{
    if (!ship)
    {
        strcpy(titleStr, "Cadet resigns from War College");
    }
    else if (!mb1 && !mb2)
    {
        strcpy(titleStr, "Cadet passes flight training");
    }
    else
    {
        strcpy(titleStr, baseStr);
        strcat(titleStr, shipCountStr);
        strcat(titleStr, unumCountStr);
        strcat(titleStr, secundusCountStr);
        strcat(titleStr, upsStr);
        strcat(titleStr, fpsStr);
        strcat(titleStr, viewStr);
    }

    glutSetWindowTitle(titleStr);
}

void setUniform(const char *name, float x, GLuint shader = shaderProgram)
{
	GLint loc = glGetUniformLocation(shader, name);
	glUniform1f(loc, x);
}

void setUniform(const char *name, const glm::vec3& v, GLuint shader = shaderProgram)
{
	GLint loc = glGetUniformLocation(shader, name);
	glUniform3fv(loc, 1, glm::value_ptr(v));
}

void setUniform(const char *name, const glm::mat4& m, GLuint shader = shaderProgram)
{
	GLint loc = glGetUniformLocation(shader, name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m));
}

void display()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //draw skybox first
    glDepthMask(GL_FALSE);
    glUseProgram(skyboxShaderProgram);
    setUniform("view", glm::mat4(glm::mat3(viewMatrix)), skyboxShaderProgram);
    setUniform("projection", projectionMatrix, skyboxShaderProgram);
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(skyboxShaderProgram, "skybox"), 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthMask(GL_TRUE);

    glUseProgram(shaderProgram);
	glUniform1f(showTexture, 0);
	glUniform1f(noLighting, 1);

    // update model matrix
    for (int id : *scene->DrawableObjects())
    {
        StaticEntity* entity = (StaticEntity*)scene->GetEntityFromID(id);

		if ("Ship" == entity->GetType() || "Missile" == entity->GetType() || "MissileBattery" == entity->GetType())
		{
			setUniform("Material.Ka", 0.05f);
			setUniform("Material.Kd", 1.0f);
			setUniform("Material.Ks", 1.0f);
			setUniform("Material.Shininess", 180.0f);
		}
		else
		{
			setUniform("Material.Ka", 0.05f);
			setUniform("Material.Kd", 1.0f);
			setUniform("Material.Ks", 0.01f);
			setUniform("Material.Shininess", 1.0f);
		}

		modelMatrix = entity->ObjectMatrix();
		modelViewMatrix = viewMatrix * modelMatrix;
		glUniformMatrix4fv(ModelView, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));

		normalMatrix = glm::mat3(modelViewMatrix);
		glUniformMatrix3fv(NormalMatrix, 1, GL_FALSE, glm::value_ptr(normalMatrix));

        ModelViewProjectionMatrix = projectionMatrix * modelViewMatrix;
        glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix));
        glBindVertexArray(*(entity->ModelFile()->VAO()));
        glDrawArrays(GL_TRIANGLES, 0, entity->ModelFile()->Vertices());

		if (id == 0)
		{
			glUniform1f(noLighting, 0);
		}
    }

    if (showAxesFlag)
    {
		glUniform1f(noLighting, 1);
        Model* axis = scene->GetModel("axes-r100");
        // Local axis for each entity
        for (int id : *scene->DrawableObjects())
        {
            StaticEntity* entity = (StaticEntity*)scene->GetEntityFromID(id);
            modelMatrix = glm::translate(glm::mat4(), entity->Position()) *
				glm::rotate(glm::mat4(), glm::pi<float>(), entity->Up()) *
				entity->RotationMatrix() *
				glm::scale(glm::mat4(), entity->Scale() * entity->ModelFile()->BoundingRadius() * 1.5f / axis->BoundingRadius());
            ModelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;
            glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix));
            glBindVertexArray(*(axis->VAO()));
            glDrawArrays(GL_TRIANGLES, 0, axis->Vertices());
        }
    }

    glutSwapBuffers();
    frameCount++;
    currentTime = glutGet(GLUT_ELAPSED_TIME);
    timeInterval = currentTime - lastTime;

    // Update fps
    if (timeInterval >= 1000)
    {
        sprintf(fpsStr, "  F/S %4d", (int)(frameCount / (timeInterval / 1000.0f)));
        lastTime = currentTime;
        frameCount = 0;
        updateTitle();
    }
}

void update(int value)
{
    glutTimerFunc(scene->TimerDelay(), update, 1);

    scene->Update();

    if (mb1)
    {
        auto e = (MissileBattery*)scene->GetEntityFromID(6);
        if (e)
        {
            sprintf(unumCountStr, " Unum %d", e->NumMissiles());
        }
        else
        {
			sprintf(unumCountStr, " Unum ?");
            mb1 = false;
        }
    }

    if (mb2)
    {
        auto e = (MissileBattery*)scene->GetEntityFromID(7);
        if (e)
        {
            sprintf(secundusCountStr, " Secundus %d", e->NumMissiles());
        }
        else
        {
			sprintf(secundusCountStr, " Secundus ?");
            mb2 = false;
        }
    }

    if (ship)
    {
        auto e = (Ship*)scene->GetEntityFromID(5);
        if (e)
        {
            sprintf(shipCountStr, " Warbird %d", e->NumMissiles());
			shipPos = viewMatrix * glm::vec4(e->Position() + (100 + e->BoundingRadius()) * e->Forward(), 1.0f);
			glUniform4fv(shipLightPos, 1, glm::value_ptr(shipPos));  // update shipLightPos value
			lightDir[1] =  glm::mat3(viewMatrix) * e->Forward();
			glUniform3fv(light[1], 1, glm::value_ptr(lightDir[1]));  // update shipLightDir value
        }
        else
        {
			setUniform("shipLightIntensity", glm::vec3(0.0));
            ship = false;
        }
    }

    viewingCamera = scene->ViewingCamera();
    viewMatrix = viewingCamera->ViewMatrix();
	setUniform("ruberLightPos", glm::vec3(viewMatrix * glm::vec4(0.0, 0.0, 0.0, 1.0)));
	lightDir[0] = glm::mat3(viewMatrix) * (viewingCamera->Eye() - viewingCamera->At());
	glUniform3fv(light[0], 1, glm::value_ptr(lightDir[0]));  // update headLight value

    updateCount++;
    currentTime = glutGet(GLUT_ELAPSED_TIME);
    utimeInterval = currentTime - ulastTime;

    // Update ups
    if (utimeInterval >= 1000)
    {
        sprintf(upsStr, "  U/S %3d", (int)(updateCount / (utimeInterval / 1000.0f)));
        ulastTime = currentTime;
        updateCount = 0;
        updateTitle();
    }

    if (!idleTimerFlag) glutPostRedisplay(); // Redisplay if no idle function
}

// load the shader programs, vertex data from model files, create the solids, set initial view
void init()
{
    // load the shader programs
    shaderProgram = loadShaders(vertexShaderFile,fragmentShaderFile);
    skyboxShaderProgram = loadShaders(skyboxVertexFile, skyboxFragmentFile);
    glUseProgram(shaderProgram);
    // generate VAOs and VBOs
    glGenVertexArrays( nModels, VAO );
    glGenBuffers( nModels, buffer );

    // Load models
    for (int i = 0; i < nModels; i++)
    {
        new Model(modelFile[i], nVertices[i], &VAO[i], &buffer[i], &shaderProgram);
    }

    srand((unsigned int)time(NULL));
    glm::vec3 pos; // Position of entity
    glm::vec3 target; // Direction vector
    glm::vec3 up; // Up vector

    // Create each entity

    printf("\tRuber drawn\n");
    pos = glm::vec3(0.0f);
    target = glm::vec3(rand(), rand(), rand());
    up = glm::vec3(0, 1, 0);
    if (colinear(target, up, 0.1)) // Up and target can't be colinear
    {
        up = glm::vec3(-1, 0, 0);
    }
    new CelestialBody(scene->GetModel("Ruber"), NULL, pos, glm::vec3(2000), target,
                      up, 60.0f);

    printf("\tUnum drawn\n");
    pos = glm::vec3(4000.0f, 0.0f, 0.0f);
    target = glm::vec3(rand(), rand(), rand());
    up = glm::vec3(0, 1, 0);
    if (colinear(target, up, 0.1))
    {
        up = glm::vec3(-1, 0, 0);
    }
    new CelestialBody(scene->GetModel("Unum"), (CelestialBody*)scene->GetEntityFromID(0), pos,
                      glm::vec3(200), pos + target, up, 5.0f, 8.0f);

    printf("\tDuo drawn\n");
    pos = glm::vec3(9000.0f, 0.0f, 0.0f);
    target = glm::vec3(rand(), rand(), rand());
    up = glm::vec3(0, 1, 0);
    if (colinear(target, up, 0.1))
    {
        up = glm::vec3(-1, 0, 0);
    }
    new CelestialBody(scene->GetModel("Duo"), (CelestialBody*)scene->GetEntityFromID(0), pos,
                      glm::vec3(400), pos + target, up, 5.0f, 16.0f);

    printf("\tPrimus drawn\n");
    pos = glm::vec3(-900.0f, 0.0f, 0.0f);
    target = glm::vec3(rand(), rand(), rand());
    up = glm::vec3(0, 1, 0);
    if (colinear(target, up, 0.1))
    {
        up = glm::vec3(-1, 0, 0);
    }
    new CelestialBody(scene->GetModel("Primus"), (CelestialBody*)scene->GetEntityFromID(2), pos,
                      glm::vec3(100), pos + target, up, 5.0f, 8.0f);

    printf("\tSecundus drawn\n");
    pos = glm::vec3(-1750.0f, 0.0f, 0.0f);
    target = glm::vec3(rand(), rand(), rand());
    up = glm::vec3(0, 1, 0);
    if (colinear(target, up, 0.1))
    {
        up = glm::vec3(-1, 0, 0);
    }
    new CelestialBody(scene->GetModel("Secundus"), (CelestialBody*)scene->GetEntityFromID(2), pos,
                      glm::vec3(150), pos + target, up, 5.0f, 16.0f);

    printf("\tWarbird drawn\n");
    pos = glm::vec3(5000.0f, 1000.0f, 5000.0f);
    target = glm::vec3(0.0f, 0.0f, -1.0f);
    Ship* s = new Ship(scene->GetModel("Warbird"), pos, glm::vec3(100.0f), pos + target);

    pos = glm::vec3(0.0f, 0.0f, -((StaticEntity*)scene->GetEntityFromID(1))->BoundingRadius());
    MissileBattery* m = new MissileBattery(scene->GetModel("MissileBattery"), (CelestialBody*)scene->GetEntityFromID(1),
                                           pos, glm::vec3(30.0f), pos + target);
    m->SetTargets("Ship");
    sprintf(unumCountStr, " Unum %d", m->NumMissiles());

    pos = glm::vec3(0.0f, 0.0f, -((StaticEntity*)scene->GetEntityFromID(4))->BoundingRadius());
    m = new MissileBattery(scene->GetModel("MissileBattery"), (CelestialBody*)scene->GetEntityFromID(4),
                           pos, glm::vec3(30.0f), pos + target);
    m->SetTargets("Ship");
    sprintf(secundusCountStr, " Secundus %d", m->NumMissiles());
    s->SetTargets("MissileBattery");

    // Create cameras
    new StaticCamera("Front", glm::vec3(0.0f, 10000.0f, 20000.0f), glm::vec3(0), glm::vec3(0.0f, 1.0f, 0.0f));
    new StaticCamera("Top",  glm::vec3(0.0f, 20000.0f, 0.0f), glm::vec3(0), glm::vec3(0.0f, 0.0f, -1.0f));
    new DynamicCamera("Ship", (MoveableEntity*)scene->GetEntityFromID(5), false, 0.0f, glm::vec3(0.0f, 300.0f, 1000.0f),
                      glm::vec3(0.0f, 300.0f, 0.0f));
    new DynamicCamera("Unum", (MoveableEntity*)scene->GetEntityFromID(1), true, 8000.0f, true);
    new DynamicCamera("Duo", (MoveableEntity*)scene->GetEntityFromID(2), true, 8000.0f, true);

	// Adds warp points for ship
	scene->AddWarpPoint(11);
	scene->AddWarpPoint(12);

    // Initialize display info
    lastTime = glutGet(GLUT_ELAPSED_TIME);
    ulastTime = lastTime;

	// Get shader program locations
    MVP = glGetUniformLocation(shaderProgram, "MVP");
	light[0] = glGetUniformLocation(shaderProgram, "headLightDir");
	light[1] = glGetUniformLocation(shaderProgram, "shipLightDir");
	shipLightPos = glGetUniformLocation(shaderProgram, "shipLightPos");
	showTexture = glGetUniformLocation(shaderProgram, "IsTexture");
	NormalMatrix = glGetUniformLocation(shaderProgram, "NormalMatrix");
	ModelView = glGetUniformLocation(shaderProgram, "ModelView");
	ruberLight = glGetUniformLocation(shaderProgram, "ruberLightOn");
	headLight = glGetUniformLocation(shaderProgram, "headLightOn");
	shipLight = glGetUniformLocation(shaderProgram, "shipLightOn");
	ambient = glGetUniformLocation(shaderProgram, "ambientOn");
	noLighting = glGetUniformLocation(shaderProgram, "noLighting");
	debug = glGetUniformLocation(shaderProgram, "debugOn");

	// Set camera
    viewingCamera = scene->ViewingCamera();
    viewMatrix = viewingCamera->ViewMatrix();

	// Initialize lights
	lightDir[0] = glm::vec3(0.0f, -1.0f, 0.0f);
	lightDir[1] =  glm::mat3(viewMatrix) * glm::vec3(0.0f, 0.0f, -1.0f);
	shipPos = viewMatrix * glm::vec4(5000.0f, 1000.0f, 5000.0f, 1.0f);


	//initialize skybox
    glUseProgram(skyboxShaderProgram);
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glBindVertexArray(0);

    faces.push_back("src/skybox/right.data");
    faces.push_back("src/skybox/left.data");
    faces.push_back("src/skybox/up.data");
    faces.push_back("src/skybox/down.data");
    faces.push_back("src/skybox/back.data");
    faces.push_back("src/skybox/front.data");

	skyboxTexture = loadSkyboxTexture(faces);

    // set render state values
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Finalize scene
    scene->InitDone();
}

// Keyboard input
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 033:
    case 'q':
    case 'Q':
        exit(EXIT_SUCCESS);
        break;

    case 'a':
    case 'A':  // Toggle ambient light
		ambientOn = !ambientOn;
		glUniform1f(ambient, ambientOn);
        break;

	case 'd':
	case 'D':  // Toggle ambient light
		debugOn = !debugOn;
		glUniform1f(debug, debugOn);
		break;

    case 'f':
    case 'F':
        MessageDispatcher::Instance()->DispatchMsg(0, -1, 5, Msg_ShipFireMissile, NULL);
        break;

	case 'p':
	case 'P':  // Toggle ambient light
		ruberLightOn = !ruberLightOn;
		glUniform1f(ruberLight, ruberLightOn);
		break;

	case 'h':
	case 'H':  // Toggle ambient light
		headLightOn = !headLightOn;
		glUniform1f(headLight, headLightOn);
		break;

	case 'l':
	case 'L':  // Toggle ambient light
		shipLightOn = !shipLightOn;
		glUniform1f(shipLight, shipLightOn);
		break;

    case 'g':
    case 'G':
        MessageDispatcher::Instance()->DispatchMsg(0, -1, 5, Msg_ToggleGravity, NULL);
        break;

    case 's':
    case 'S':
        MessageDispatcher::Instance()->DispatchMsg(0, -1, 5, Msg_ShipSpeedChange, NULL);
        break;

    case 't':
    case 'T':  // Change time quantum
        tq = (tq + 1) % 4;
        switch (tq)
        {
        case 0:
            scene->SetTimerDelay(5);
            break;
        case 1:
            scene->SetTimerDelay(40);
            break;
        case 2:
            scene->SetTimerDelay(100);
            break;
        case 3:
            scene->SetTimerDelay(500);
            break;
        }
        break;

    case 'u':
    case 'U':  // Toggle axes
        showAxesFlag = !showAxesFlag;
        break;

    case 'w':
    case 'W':
        warpCamera = scene->NextWarpCamera();
        MessageDispatcher::Instance()->DispatchMsg(0, -1, 5, Msg_ShipWarp, warpCamera);
        printf("warping to camera at %s\n", warpCamera->Name());
        break;

    case 'v':
    case 'V':  // Next camera
        viewingCamera = scene->NextCamera();
        viewMatrix = viewingCamera->ViewMatrix();
        sprintf(viewStr, "  View %s", viewingCamera->Name());
        break;

    case 'x':
    case 'X':  // Prev camera
        viewingCamera = scene->PrevCamera();
        viewMatrix = viewingCamera->ViewMatrix();
        sprintf(viewStr, "  View %s", viewingCamera->Name());
        break;
    }

    updateTitle();
    glutPostRedisplay();
}
void specialKeys(int key, int x, int y)
{
    int modifiers = glutGetModifiers();
    switch (modifiers)
    {
    case GLUT_ACTIVE_CTRL:
        MessageDispatcher::Instance()->DispatchMsg(0, -1, 5, Msg_CtrlMod_SpecialKeyPress, &key);
        break;
    default:
        MessageDispatcher::Instance()->DispatchMsg(0, -1, 5, Msg_SpecialKeyPress, &key);
        break;
    }

}

void specialUpFunc(int key, int x, int y)
{
    int modifiers = glutGetModifiers();
    switch (modifiers)
    {
    case GLUT_ACTIVE_CTRL:
        MessageDispatcher::Instance()->DispatchMsg(0, -1, 5, Msg_CtrlMod_SpecialKeyRelease, &key);
        break;
    default:
        MessageDispatcher::Instance()->DispatchMsg(0, -1, 5, Msg_SpecialKeyRelease, &key);
        break;
    }
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
# ifdef __Mac__
    // Can't change the version in the GLUT_3_2_CORE_PROFILE
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_3_2_CORE_PROFILE);
# endif
# ifndef __Mac__
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
# endif
    glutInitWindowSize(800, 600);
    // set OpenGL and GLSL versions to 3.3 for Comp 465/L, comment to see highest versions
# ifndef __Mac__
    glutInitContextVersion(3, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
# endif
    glutCreateWindow("");
    updateTitle();
    // initialize and verify glew
    glewExperimental = GL_TRUE;  // needed my home system
    GLenum err = glewInit();
    if (GLEW_OK != err)
        printf("GLEW Error: %s \n", glewGetErrorString(err));
    else
    {
        printf("Using GLEW %s \n", glewGetString(GLEW_VERSION));
        printf("OpenGL %s, GLSL %s\n",
               glGetString(GL_VERSION),
               glGetString(GL_SHADING_LANGUAGE_VERSION));
    }

    // Disable vsync
#ifdef _WIN32
    printf("WINDOWS\n");
    if (WGLExtensionSupported("WGL_EXT_swap_control"))
    {
        // Extension is supported, init pointer
        wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
    }

    wglSwapIntervalEXT(0);
#elif defined __linux__
    printf("LINUX\n");
#endif

    // initialize scene
    init();
    // set glut callback functions
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutSpecialUpFunc(specialUpFunc);
    glutTimerFunc(scene->TimerDelay(), update, 1);
    glutIdleFunc(display);
    glutMainLoop();
    printf("done\n");
    delete scene;
    return 0;
}
