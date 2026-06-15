#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <GL/glew.h>

class Model {
public:
    Model() : VAO(0), VBO(0), uvVBO(0), normalsVBO(0), numVertexs(0) {}
    Model(const std::vector<float>& vertexs, const std::vector<float>& uvs, const std::vector<float>& normals);
    void Render() const;

private:
    GLuint VAO, VBO, uvVBO, normalsVBO;
    unsigned int numVertexs;
};

#endif
