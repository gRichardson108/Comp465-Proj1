#ifndef MODEL_H
#define MODEL_H

#include <glm/glm.hpp>
#include <gl/glew.h>

class Model
{
private:
	char * m_cModelFile;
	int m_iVertices;
	GLuint * m_gliShaderProgram;
	GLuint * m_gliVAO;
	GLuint * m_gliBuffer;
	float m_fBoundingRadius;
	// Not sure if Scale, vPosition, vColor, and vNormal are needed

public:
	Model(char * modelFile, int vertices,
		GLuint * shaderProgram, GLuint * VAO, GLuint * buffer) :
		m_cModelFile(modelFile),
		m_iVertices(vertices),
		m_gliShaderProgram(shaderProgram),
		m_gliVAO(VAO),
		m_gliBuffer(buffer),
		m_fBoundingRadius(0.0)
	{}

	~Model(){}

	void Init(){}
	float BoundingRadius() { return m_fBoundingRadius; }
};

#endif
