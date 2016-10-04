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


#include "../includes465/include465.hpp"

#ifdef _WIN32
#include <Windows.h>
#include <gl/wglext.h>
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
#elif defined __linux__
#include <gl\glxext.h>
	bool checkGLXExtension(const char* extName)
	{
		/*
		Search for extName in the extensions string.  Use of strstr()
		is not sufficient because extension names can be prefixes of
		other extension names.  Could use strtok() but the constant
		string returned by glGetString can be in read-only memory.
		*/
		Display* dpy = glXGetCurrentDisplay();
		int screen = glXGetCurrentDrawable();
		char* list = (char*)glXQueryExtensionsString(dpy, screen);
		char* end;
		int extNameLen;

		extNameLen = strlen(extName);
		end = list + strlen(list);

		while (list < end)
		{
			int n = strcspn(list, " ");

			if ((extNameLen == n) && (strncmp(extName, list, n) == 0))
				return true;

			list += (n + 1);
		};
		return false;
	};

	void(*)(int) glXSwapIntervalEXT = NULL;
#endif


#include "Scene.hpp"
#include "CelestialBody.hpp"
#include "Ship.hpp"
#include "DynamicCamera.hpp"
#include <time.h>

// constants for models:  file names, vertex count, model display size
const int nModels = 8;  // number of models in this scene
Model* models[nModels];
const int nEntities = 6;
const int nUpdateable = 6;
BaseEntity* entities[nEntities];
MoveableEntity* updateableEntities[nUpdateable];
bool showAxis = false, idleTimerFlag = false;
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
int currentCamera = 0;
const int nCameras = 5, nDynamicCameras = 3;
glm::mat4 viewMatrix;           // set in init()
StaticCamera* viewingCamera;
StaticCamera* availableCameras[nCameras] = {
    new StaticCamera("Front", glm::lookAt(
                glm::vec3(0.0f, 10000.0f, 20000.0f),
                glm::vec3(0),
                glm::vec3(0.0f, 1.0f, 0.0f))),
    new StaticCamera("Top", glm::lookAt(
                glm::vec3(0.0f, 20000.0f, 0.0f),
                glm::vec3(0),
                glm::vec3(0.0f, 0.0f, -1.0f)))
};

DynamicCamera* dynamicCameras[nDynamicCameras];

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
char titleStr[110];
char baseStr [32] = "Warbird Simulation {x, v, t, u}";
char shipCountStr[12] = "  Warbird ?";
char unumCountStr[9] = "  Unum ?";
char secundusCountStr[13] = "  Secundus ?";
char upsStr[10] = "  U/S 200";
char fpsStr[11] = "  F/S ????";
char viewStr[13] = "  View Front";


void reshape(int width, int height) {
    windowWidth = width;
    windowHeight = height;
    float aspectRatio = (float) width / (float) height;
    glViewport(0, 0, width, height);
    printf("reshape: FOVY = %5.2f, width = %4d height = %4d aspect = %5.2f \n + nearclip = %5f farclip = %5f \n", 
		viewingCamera->FOVY(), width, height, aspectRatio, viewingCamera->NearClip(), viewingCamera->FarClip());
    projectionMatrix = viewingCamera->updateProjectionMatrix(width, height);
}

void updateTitle()
{
	strcpy(titleStr, baseStr);
	strcat(titleStr, shipCountStr);
	strcat(titleStr, unumCountStr);
	strcat(titleStr, secundusCountStr);
	strcat(titleStr, upsStr);
	strcat(titleStr, fpsStr);
	strcat(titleStr, viewStr);
	glutSetWindowTitle(titleStr);
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

    if (timeInterval >= 1000)
	{
		sprintf(fpsStr, "  F/S %4d", (int)(frameCount / (timeInterval / 1000.0f)));
        lastTime = currentTime;
        frameCount = 0;
		updateTitle();
    }
}

void update()
{
	//glutTimerFunc(scene->TimerDelay(), update, 1);

    for (int e = 0; e < nUpdateable; e++)
	{
        updateableEntities[e]->Update();
    }

	for (int i = 0; i < nDynamicCameras; i++)
	{
		dynamicCameras[i]->Update();
	}

	viewMatrix = viewingCamera->ViewMatrix();
    glutPostRedisplay();
}

void timerFunc(int value)
{
	glutTimerFunc(scene->TimerDelay(), timerFunc, 1);
	if (!idleTimerFlag) update();
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
    updateableEntities[0] = new CelestialBody(models[3], NULL, glm::vec3(0.0f), glm::vec3(2000), pos + target,
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
		glm::vec3(200), pos + target, up, 5.0f, 8.0f);
	entities[1] = updateableEntities[1];

	printf("\tDuo drawn\n");
	target = glm::vec3(rand(), rand(), rand());
	up = glm::vec3(0, 1, 0);
	if (colinear(target, up, 0.1))
	{
		up = glm::vec3(-1, 0, 0);
	}
	updateableEntities[2] = new CelestialBody(models[5], (CelestialBody*)entities[0], glm::vec3(9000.0f, 0.0f, 0.0f),
		glm::vec3(400), pos + target, up, 5.0f, 16.0f);
	entities[2] = updateableEntities[2];

	printf("\tPrimus drawn\n");
	target = glm::vec3(rand(), rand(), rand());
	up = glm::vec3(0, 1, 0);
	if (colinear(target, up, 0.1))
	{
		up = glm::vec3(-1, 0, 0);
	}
	updateableEntities[3] = new CelestialBody(models[6], (CelestialBody*)entities[2], glm::vec3(-900.0f, 0.0f, 0.0f),
		glm::vec3(100), pos + target, up, 5.0f, 8.0f);
	entities[3] = updateableEntities[3];

	printf("\tSecundus drawn\n");
	target = glm::vec3(rand(), rand(), rand());
	up = glm::vec3(0, 1, 0);
	if (colinear(target, up, 0.1))
	{
		up = glm::vec3(-1, 0, 0);
	}
	updateableEntities[4] = new CelestialBody(models[7], (CelestialBody*)entities[2], glm::vec3(-1750.0f, 0.0f, 0.0f),
		glm::vec3(150), pos + target, up, 5.0f, 16.0f);
	entities[4] = updateableEntities[4];

	printf("\tWarbird drawn\n");
	pos = glm::vec3(5000.0f, 1000.0f, 5000.0f);
	updateableEntities[5] = new Ship(models[2], pos, glm::vec3(100.0f), pos + glm::vec3(0.0f, 0.0f, -1.0f));
	entities[5] = updateableEntities[5];

	Scene::Instance()->SetEntities(entities, nEntities);
	Scene::Instance()->SetMoveables(updateableEntities, nUpdateable);

	dynamicCameras[0] = new DynamicCamera("Ship", (MoveableEntity*)updateableEntities[5], false, 0.0f,
		glm::vec3(0.0f, 300.0f, 1000.0f), glm::vec3(0.0f, 300.0f, 0.0f));
	availableCameras[2] = dynamicCameras[0];

	dynamicCameras[1] = new DynamicCamera("Unum", (MoveableEntity*)updateableEntities[1], true, 8000.0f);
	availableCameras[3] = dynamicCameras[1];

	dynamicCameras[2] = new DynamicCamera("Duo", (MoveableEntity*)updateableEntities[2], true, 8000.0f);
	availableCameras[4] = dynamicCameras[2];

    lastTime = glutGet(GLUT_ELAPSED_TIME);
    MVP = glGetUniformLocation(shaderProgram, "ModelViewProjection");
    viewingCamera = availableCameras[0];
    viewMatrix = viewingCamera->ViewMatrix();

    // set render state values
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 033: case 'q':  case 'Q':
		exit(EXIT_SUCCESS);
		break;
	case 'a': case 'A':  // change animation timer
						 // printf("%s   %s\n", timerStr, fpsStr);
		if (idleTimerFlag) // switch to interval timer  
		{
			glutIdleFunc(NULL);
			idleTimerFlag = false;
		}
		else // switch to idle timer
		{
			glutIdleFunc(update);
			idleTimerFlag = true;
		}
		break;
	case 't': case 'T':
		showAxis = !showAxis;
		printf("Show Axis: %d\n", showAxis);
		break;
	case 'v': case 'V':
		currentCamera = (currentCamera + 1) % nCameras;
		viewingCamera = availableCameras[currentCamera];
		viewMatrix = viewingCamera->ViewMatrix();
		sprintf(viewStr, "  View %s", viewingCamera->Name());
		break;
	case 'x': case 'X':
		currentCamera = (nCameras + currentCamera - 1) % nCameras;
		viewingCamera = availableCameras[currentCamera];
		viewMatrix = viewingCamera->ViewMatrix();
		sprintf(viewStr, "  View %s", viewingCamera->Name());
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
		sprintf(upsStr, "  U/S %4d", 1000 / scene->TimerDelay());
		break;
}

	updateTitle();
	glutPostRedisplay();
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
    glutCreateWindow("");
	updateTitle();
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
	if (checkGLXExtension("GLX_EXT_swap_control"))
	{
		glXSwapIntervalEXT = (void(*)(int))glXGetProcAddress((const GLubyte*) "glXSwapIntervalEXT");
	}

	glXSwapIntervalEXT(0);
#endif

    // initialize scene
    init();
    // set glut callback functions
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(scene->TimerDelay(), timerFunc, 1);
    glutIdleFunc(NULL);
    glutMainLoop();
    printf("done\n");
    return 0;
}
