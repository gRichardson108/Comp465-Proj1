/*
Kory Dondzila
Garret Richardson

Model.hpp
10/08/2016

Model class, used for keeping track of models used in the
scene, entities have a pointer to a model to know what it
will render.

*/

#ifndef MODEL_H
#define MODEL_H

#include "../includes465/include465.hpp"

class Model
{
private:
    char* m_cModelFile; // Model file name
    int m_iVertices; // Number of vertices
    GLuint* m_gliVAO;
    GLuint* m_gliBuffer;
    GLuint* m_gliShaderProgram;
    float m_fBoundingRadius; // Bounding radius of model

    // Initialize and load model
    void Init();

public:
    Model(char* modelFile, int vertices, GLuint* VAO, GLuint* buffer, GLuint* shaderProgram);

    ~Model()
    {
        delete []m_cModelFile;
    }

    char* ModelFile() const { return m_cModelFile; }
    int Vertices() const { return m_iVertices; }
    GLuint* VAO() const { return m_gliVAO; }
    GLuint* Buffer() const { return m_gliBuffer; }
    GLuint* ShaderProgram() const { return m_gliShaderProgram; }
    float BoundingRadius() const { return m_fBoundingRadius; }
};

#endif
