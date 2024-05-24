#include "VertexArrayObject.h"

//SpriteMesh::SpriteMesh(size_t size)
//{
//    glCreateBuffers(1, &vbo);
//    glBindBuffer(GL_ARRAY_BUFFER, vbo);
//    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
//
//    vao.addAttribute(3);
//    vao.addAttribute(2);
//    vao.setStride(5 * sizeof(float));
//    vao.finalize();
//
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//}
//
//void SpriteMesh::setSubData(const std::vector<float>& vertices)
//{
//    this->vertices = vertices;
//    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * vertices.size(), vertices.data());
//}
//
//void SpriteMesh::bind()
//{
//    vao.bind();
//    glBindBuffer(GL_ARRAY_BUFFER, vbo);
//}
//
//const std::vector<float>& SpriteMesh::getVertices() const
//{
//    return vertices;
//}
