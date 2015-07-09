#include "model_component.hpp"

namespace game
{
  struct ModelComponent::ModelComponentImpl
  {
    GLuint vao;
    GLuint vertex_vbo;
    GLuint texture;
    std::vector<float> vertices;
  };
  ModelComponent::ModelComponent(EntityWPtr entityWPtr) :
    Component(entityWPtr),
    pimpl(std::unique_ptr<ModelComponentImpl>(new ModelComponentImpl()))
  {
    glGenVertexArrays(1, &pimpl->vao);
  }
  void ModelComponent::setVertices( std::vector<float>& vertices )
  {
    glBindVertexArray(pimpl->vao);
    if(vertices.size() == 0)
    {
      return;
    }
    glGenBuffers(1, &pimpl->vertex_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, pimpl->vertex_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);
    pimpl->vertices = std::move(vertices);
  }

  void ModelComponent::setTexture( GLuint texture )
  {
    pimpl->texture = texture;
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }

  GLuint ModelComponent::getTexture() const
  {
    return pimpl->texture;
  }

  GLuint ModelComponent::getArrayObject() const
  {
    return pimpl->vao;
  }

  size_t ModelComponent::vertexCount() const
  {
    return pimpl->vertices.size();
  }

  ComponentType ModelComponent::getComponentType()
  {
    return ComponentType::MODEL_COMPONENT;
  }

  void ModelComponent::init() {}

  ModelComponent::~ModelComponent()
  {}
}

