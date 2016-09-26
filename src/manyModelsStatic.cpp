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
#include "CelestialBody.hpp"
#include "MoveableEntity.hpp"
#include "BaseEntity.hpp"
#include "Model.hpp"
#include <time.h>

const int X = 0, Y = 1, Z = 2, START = 0, STOP = 1;
// constants for models:  file names, vertex count, model display size
const int nModels = 4;  // number of models in this scene
Model* models[nModels];
const int nEntities = 3;
const int nUpdateable = 3;
BaseEntity* entities[nEntities];
MoveableEntity* updateableEntities[nUpdateable];
bool showAxis = false;
bool snapToForward = false; //when true, the models should be facing forward, away from the camera view
char * modelFile [nModels] = {"src/axes-r100.tri", "src/obelisk-10-20-10.tri", "src/spaceShip-bs100.tri", "src/sphere-r50.tri"};
float modelBR[nModels];       // model's bounding radius
float scaleValue[nModels];    // model's scaling "size" value
const int nVertices[nModels] = { 120 * 3, 14 * 3, 996 * 3, 264 * 3};
char * vertexShaderFile   = "src/simpleVertex.glsl";     
char * fragmentShaderFile = "src/simpleFragment.glsl";    
GLuint shaderProgram; 
GLuint VAO[nModels];      // Vertex Array Objects
GLuint buffer[nModels];   // Vertex Buffer Objects
int timerDelay = 40, frameCount = 0;
double currentTime, lastTime, timeInterval;

// Shader handles, matrices, etc
GLuint MVP ;  // Model View Projection matrix's handle
GLuint vPosition[nModels], vColor[nModels], vNormal[nModels];   // vPosition, vColor, vNormal handles for models
// model, view, projection matrices and values to create modelMatrix.
float modelSize[nModels] = { 100.0f, 25.0f, 50.0f, 50.0f };   // size of model
glm::vec3 scale[nModels];       // set in init()
glm::vec3 translate[nModels] = {glm::vec3(10,0,0), glm::vec3(50, -50, 0), glm::vec3(-150, -50, -50), glm::vec3(0,0,0)};
glm::mat4 modelMatrix;          // set in display()
glm::mat4 viewMatrix;           // set in init()
glm::mat4 projectionMatrix;     // set in reshape()
glm::mat4 ModelViewProjectionMatrix; // set in display();

// window title string
char titleStr [50]= "465 manyModelsStatic Example ";

void reshape(int width, int height) {
    float aspectRatio = (float) width / (float) height;
    float FOVY = glm::radians(60.0f);
    glViewport(0, 0, width, height);
    printf("reshape: FOVY = %5.2f, width = %4d height = %4d aspect = %5.2f \n", 
            FOVY, width, height, aspectRatio);
    projectionMatrix = glm::perspective(FOVY, aspectRatio, 1.0f, 100000.0f); 
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 033: case 'q':  case 'Q': exit(EXIT_SUCCESS); break;
        case 't': showAxis = !showAxis; glutPostRedisplay(); break;
        case 'a': viewMatrix = glm::lookAt(
                          glm::vec3(50.0f, 50.0f, 500.0f),  // eye position
                          glm::vec3(0),                   // look at position
                          glm::vec3(0.0f, 1.0f, 0.0f)); // up vect0r
                  glutPostRedisplay(); 
                  break;
        case 's': viewMatrix = glm::lookAt(
                          glm::vec3(500.0f, 0.0f, 0.0f),  // eye position
                          glm::vec3(0),                   // look at position
                          glm::vec3(0.0f, 1.0f, 0.0f)); // up vect0r
                  glutPostRedisplay(); 
                  break;
        case 'd': viewMatrix = glm::lookAt(
                          glm::vec3(0.0f, 0.0f, 500.0f),  // eye position
                          glm::vec3(0),                   // look at position
                          glm::vec3(0.0f, 1.0f, 0.0f)); // up vect0r
                  glutPostRedisplay(); 
                  break;
        case 'z': viewMatrix = glm::lookAt(
                          glm::vec3(-500.0f, 0.0f, 0.0f),  // eye position
                          glm::vec3(0),                   // look at position
                          glm::vec3(0.0f, 1.0f, 0.0f)); // up vect0r
                  glutPostRedisplay(); 
                  break;

        case 'x': viewMatrix = glm::lookAt(
                          glm::vec3(0.0f, 0.0f, -500.0f),  // eye position
                          glm::vec3(0),                   // look at position
                          glm::vec3(0.0f, 1.0f, 0.0f)); // up vect0r
                  glutPostRedisplay(); 
                  break;
    }
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
	printf("\n");
    for (int e = 0; e < nUpdateable; e++){
        updateableEntities[e] -> Update();
		printf("Sphere %d\n", e);
    }
	glutPostRedisplay();
	glutTimerFunc(timerDelay, update, 1);
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

    printf("\tSun drawn\n");
    target = glm::vec3(rand() , rand(), rand());
	glm::vec3 up = glm::vec3(0, 1, 0);
	if (colinear(target, up, 0.1)) // These can't be colinear
	{
		up = glm::vec3(-1, 0, 0);
	}
    updateableEntities[0] = new CelestialBody(models[3], NULL, glm::vec3(0.0f), glm::vec3(modelSize[3]), target, 
		up, 60.0f);
	entities[0] = updateableEntities[0];

	printf("\tPlanet drawn\n");
	target = glm::vec3(rand(), rand(), rand());
	up = glm::vec3(0, 1, 0);
	if (colinear(target, up, 0.1))
	{
		up = glm::vec3(-1, 0, 0);
	}
	updateableEntities[1] = new CelestialBody(models[3], (CelestialBody*)entities[0], glm::vec3(0.0f),
		glm::vec3(modelSize[3] / 2), target, up, 8.0f, 30.0f);
	entities[1] = updateableEntities[1];
	pos = glm::vec3(100 + entities[0]->BoundingRadius() + entities[1]->BoundingRadius(), 100.0f, 0.0f);
	entities[1]->SetPosition(pos);

	printf("\tMoon drawn\n");
	target = glm::vec3(rand(), rand(), rand());
	up = glm::vec3(0, 1, 0);
	if (colinear(target, up, 0.1))
	{
		up = glm::vec3(-1, 0, 0);
	}
	updateableEntities[2] = new CelestialBody(models[3], (CelestialBody*)entities[1], glm::vec3(0.0f),
		glm::vec3(modelSize[3] / 4), target, up, 8.0f, 8.0f);
	entities[2] = updateableEntities[2];
	pos = glm::vec3(25 + entities[1]->BoundingRadius() + entities[2]->BoundingRadius(), 0.0f, 0.0f);
	entities[2]->SetPosition(pos);

    for (int i = nUpdateable; i < nEntities; i++) {
        printf("init i:%d\n", i);
        pos = glm::vec3((rand() % (max+1)) - max/2, (rand() % (max + 1)) - max / 2, (rand() % (max + 1)) - max / 2);
        target = glm::vec3((rand() % (max + 1)) - max / 2, (rand() % (max + 1)) - max / 2, (rand() % (max + 1)) - max / 2);
        entities[i] = new BaseEntity(models[i % 2 + 1], pos, glm::vec3(modelSize[i % 2 + 1]), target, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    lastTime = glutGet(GLUT_ELAPSED_TIME);
    MVP = glGetUniformLocation(shaderProgram, "ModelViewProjection");

    viewMatrix = glm::lookAt(
            glm::vec3(0.0f, 0.0f, 500.0f),  // eye position
            glm::vec3(0),                   // look at position
            glm::vec3(0.0f, 1.0f, 0.0f)); // up vect0r

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
    glutCreateWindow("465 manyModelsStatic Example: t - Show Axis - cameras: a s d z x");
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
    glutTimerFunc(timerDelay, update, 1);
    //glutIdleFunc(display);
    glutMainLoop();
    printf("done\n");
    return 0;
}
