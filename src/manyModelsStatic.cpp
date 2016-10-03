/*

   manyModelsStatic.cpp

   465 utility include files:  shader465.hpp, triModel465.hpp  

Shaders:  simpleVertex.glsl and simpleFragment.glsl
provide flat shading with a fixed light position

C OpenGL Core 3.3 example that loads "nModels" *.tri model files 
and displays them in at static locations with a static view.
Demonstrates use of triModel465.hpp functions for loading *.tri models.

display() updated to comment out unneeded statements like:
glEnableVertexAttribArray( vPosition[m] );

These lines are not needed even when the models are moving.

Mike Barnes
10/23/2014
*/


#ifdef _WIN32
#include <Windows.h>
#endif

# include "../includes465/include465.hpp"
#include "Scene.hpp"
#include "Model.hpp"
#include "BaseEntity.hpp"
#include "MoveableEntity.hpp"
#include "CelestialBody.hpp"
#include "StaticCamera.hpp"
#include <time.h>


const int X = 0, Y = 1, Z = 2, START = 0, STOP = 1;
// constants for models:  file names, vertex count, model display size
const int nModels = 8;  // number of models in this scene
Model* models[nModels];
const int nEntities = 6;
const int nUpdateable = 5;
BaseEntity* entities[nEntities];
MoveableEntity* updateableEntities[nUpdateable];
bool showAxis = false;
bool snapToForward = false; //when true, the models should be facing forward, away from the camera view
char * modelFile [nModels] = {"src/axes-r100.tri", "src/obelisk-10-20-10.tri", "src/Warbird.tri",
    "src/Ruber.tri", "src/Unum.tri", "src/Duo.tri", "src/Primus.tri", "src/Secundus.tri"};
float modelBR[nModels];       // model's bounding radius
float scaleValue[nModels];    // model's scaling "size" value
const int nVertices[nModels] = { 120 * 3, 14 * 3, 4914 * 3, 760 * 3, 760 * 3, 760 * 3, 760 * 3, 760 * 3};
char * vertexShaderFile   = "src/simpleVertex.glsl";     
char * fragmentShaderFile = "src/simpleFragment.glsl";    
GLuint shaderProgram; 
GLuint VAO[nModels];      // Vertex Array Objects
GLuint buffer[nModels];   // Vertex Buffer Objects
Scene* Scene::s_pInstance = NULL;
Scene* scene = new Scene();
int tq = 0, frameCount = 0;
double currentTime, lastTime, timeInterval;
int currentCamera = 0, nCameras = 6;
glm::mat4 viewMatrix;           // set in init()
StaticCamera* viewingCamera;
StaticCamera* availableCameras[6] = {
    new StaticCamera(glm::lookAt(
                glm::vec3(0.0f, 10000.0f, 20000.0f),
                glm::vec3(0),
                glm::vec3(0.0f, 1.0f, 0.0f))),
    new StaticCamera(glm::lookAt(
                glm::vec3(0.0f, 20000.0f, 0.0f),
                glm::vec3(0),
                glm::vec3(0.0f, 0.0f, -1.0f))),
    new StaticCamera(glm::lookAt(
                glm::vec3(5000.0f, 1300.0f, 6000.0f),
                glm::vec3(5000.0f, 1300.0f, 5000.0f),
                glm::vec3(0.0f, 1.0f, 0.0f))),
    new StaticCamera(glm::lookAt(
                glm::vec3(5250.0f, 1000.0f, 5000.0f),
                glm::vec3(5000.0f, 1000.0f, 5000.0f),
                glm::vec3(0.0f, 1.0f, 0.0f))),
    new StaticCamera(glm::lookAt(
                glm::vec3(5100.0f, 1100.0f, 5100.0f),
                glm::vec3(5000.0f, 1000.0f, 5000.0f),
                glm::vec3(0.0f, 1.0f, 0.0f))),
    new StaticCamera(glm::lookAt(
                glm::vec3(-20000.0f, 0.0f, 0.0f),
                glm::vec3(0),
                glm::vec3(0.0f, 1.0f, 0.0f)))
};

// Shader handles, matrices, etc
GLuint MVP ;  // Model View Projection matrix's handle
GLuint vPosition[nModels], vColor[nModels], vNormal[nModels];   // vPosition, vColor, vNormal handles for models
// model, view, projection matrices and values to create modelMatrix.
glm::mat4 modelMatrix;          // set in display()

glm::mat4 projectionMatrix;     // set in reshape()
glm::mat4 ModelViewProjectionMatrix; // set in display();

int windowWidth = 800;
int windowHeight = 600;

// window title string
char titleStr [50]= "Warbird Simulation ";

void reshape(int width, int height) {
    windowWidth = width;
    windowHeight = height;
    float aspectRatio = (float) width / (float) height;
    float FOVY = viewingCamera->getFOVY();
    glViewport(0, 0, width, height);
    printf("reshape: FOVY = %5.2f, width = %4d height = %4d aspect = %5.2f \n + nearclip = %5f farclip = %5f \n", 
            FOVY, width, height, aspectRatio, viewingCamera->NearClip(), viewingCamera->FarClip());
    projectionMatrix = viewingCamera->updateProjectionMatrix(width, height);
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 033: case 'q':  case 'Q':
            exit(EXIT_SUCCESS);
            break;
        case 't': case 'T':
            showAxis = !showAxis;
            glutPostRedisplay();
            break;
        case 'v': case 'V':
            currentCamera = (currentCamera + 1) % nCameras;
            viewingCamera = availableCameras[currentCamera];
            viewMatrix = viewingCamera->getViewMatrix();
            break;
        case 'x': case 'X':
            currentCamera = (nCameras + currentCamera - 1) % nCameras;
            viewingCamera = availableCameras[currentCamera];
            viewMatrix = viewingCamera->getViewMatrix();
            break;
        case 'u': case 'U':
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
    }

    glutPostRedisplay();
}

void display() 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // update model matrix
    for(int e = 0; e < nEntities; e++) 
    {
        ModelViewProjectionMatrix = projectionMatrix * viewMatrix * entities[e]->ModelMatrix(); 
        glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix));
        glBindVertexArray(*(entities[e]->ModelFile()->VAO()));
        glDrawArrays(GL_TRIANGLES, 0, entities[e]->ModelFile()->Vertices() ); 
    }

    if (showAxis)
    {
        // Local axis for each entity
        for (int e = 0; e < nEntities; e++) 
        {
            modelMatrix = glm::translate(glm::mat4(), entities[e]->Position()) *
                glm::rotate(glm::mat4(), glm::pi<float>(), entities[e]->Up()) *
                entities[e]->RotateToForward() *
                glm::scale(glm::mat4(), entities[e]->Scale() * entities[e]->ModelFile()->BoundingRadius() * 1.5f / models[0]->BoundingRadius());
            ModelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;
            glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(ModelViewProjectionMatrix));
            glBindVertexArray(*(models[0]->VAO()));
            glDrawArrays(GL_TRIANGLES, 0, models[0]->Vertices());
        }
    }
    glutSwapBuffers();
    frameCount++;
    currentTime = glutGet(GLUT_ELAPSED_TIME);
    timeInterval = currentTime - lastTime;

    if (timeInterval >= 1000){
        lastTime = currentTime;
        frameCount = 0;
    }
}

void update(int value)
{
    for (int e = 0; e < nUpdateable; e++){
        updateableEntities[e] -> Update();
    }
    glutPostRedisplay();
    glutTimerFunc(scene->TimerDelay(), update, 1);
}
// load the shader programs, vertex data from model files, create the solids, set initial view
void init() {
    // load the shader programs
    shaderProgram = loadShaders(vertexShaderFile,fragmentShaderFile);
    glUseProgram(shaderProgram);

    // generate VAOs and VBOs
    glGenVertexArrays( nModels, VAO );
    glGenBuffers( nModels, buffer );
    // load the buffers from the model files
    for (int i = 0; i < nModels; i++) {
        models[i] = new Model(modelFile[i], nVertices[i], &VAO[i], &buffer[i], &shaderProgram,
                &vPosition[i], &vColor[i], &vNormal[i]);
        models[i]->Init();
    }

    srand(time(NULL));
    int max = 250;

    glm::vec3 pos;
    glm::vec3 target;

    printf("\tRuber drawn\n");
    target = glm::vec3(rand() , rand(), rand());
    glm::vec3 up = glm::vec3(0, 1, 0);
    if (colinear(target, up, 0.1)) // These can't be colinear
    {
        up = glm::vec3(-1, 0, 0);
    }
    updateableEntities[0] = new CelestialBody(models[3], NULL, glm::vec3(0.0f), glm::vec3(2000), target, 
		up, 60.0f);
	entities[0] = updateableEntities[0];

	printf("\tUnum drawn\n");
	target = glm::vec3(rand(), rand(), rand());
	up = glm::vec3(0, 1, 0);
	if (colinear(target, up, 0.1))
	{
		up = glm::vec3(-1, 0, 0);
	}
	updateableEntities[1] = new CelestialBody(models[4], (CelestialBody*)entities[0], glm::vec3(4000.0f, 0.0f, 0.0f),
		glm::vec3(200), target, up, 5.0f, 8.0f);
	entities[1] = updateableEntities[1];

	printf("\tDuo drawn\n");
	target = glm::vec3(rand(), rand(), rand());
	up = glm::vec3(0, 1, 0);
	if (colinear(target, up, 0.1))
	{
		up = glm::vec3(-1, 0, 0);
	}
	updateableEntities[2] = new CelestialBody(models[5], (CelestialBody*)entities[0], glm::vec3(9000.0f, 0.0f, 0.0f),
		glm::vec3(400), target, up, 5.0f, 16.0f);
	entities[2] = updateableEntities[2];

	printf("\tPrimus drawn\n");
	target = glm::vec3(rand(), rand(), rand());
	up = glm::vec3(0, 1, 0);
	if (colinear(target, up, 0.1))
	{
		up = glm::vec3(-1, 0, 0);
	}
	updateableEntities[3] = new CelestialBody(models[6], (CelestialBody*)entities[2], glm::vec3(-900.0f, 0.0f, 0.0f),
		glm::vec3(100), target, up, 5.0f, 8.0f);
	entities[3] = updateableEntities[3];

	printf("\tSecundus drawn\n");
	target = glm::vec3(rand(), rand(), rand());
	up = glm::vec3(0, 1, 0);
	if (colinear(target, up, 0.1))
	{
		up = glm::vec3(-1, 0, 0);
	}
	updateableEntities[4] = new CelestialBody(models[7], (CelestialBody*)entities[2], glm::vec3(-1750.0f, 0.0f, 0.0f),
		glm::vec3(150), target, up, 5.0f, 16.0f);
	entities[4] = updateableEntities[4];

	printf("\tWarbird drawn\n");
	entities[5] = new BaseEntity(models[2], glm::vec3(5000.0f, 1000.0f, 5000.0f), glm::vec3(100.0f));

	Scene::Instance()->SetEntities(entities, nEntities);
	Scene::Instance()->SetMoveables(updateableEntities, nUpdateable);

    lastTime = glutGet(GLUT_ELAPSED_TIME);
    MVP = glGetUniformLocation(shaderProgram, "ModelViewProjection");
    viewingCamera = availableCameras[0];
    viewMatrix = viewingCamera->getViewMatrix();

    // set render state values
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
}

int main(int argc, char* argv[]) {
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
#ifdef _WIN32
    printf("WINDOWS\n");
#else
    printf("LINUX\n");
#endif
    glutCreateWindow("Warbird Simulation: t - Show Axis, Camera - c");
    // initialize and verify glew
    glewExperimental = GL_TRUE;  // needed my home system 
    GLenum err = glewInit();  
    if (GLEW_OK != err) 
        printf("GLEW Error: %s \n", glewGetErrorString(err));      
    else {
        printf("Using GLEW %s \n", glewGetString(GLEW_VERSION));
        printf("OpenGL %s, GLSL %s\n", 
                glGetString(GL_VERSION),
                glGetString(GL_SHADING_LANGUAGE_VERSION));
    }
    // initialize scene
    init();
    // set glut callback functions
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(scene->TimerDelay(), update, 1);
    //glutIdleFunc(display);
    glutMainLoop();
    printf("done\n");
    return 0;
}
