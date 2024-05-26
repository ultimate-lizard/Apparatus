#include "Drawable.h"

#include "../Material.h"

Drawable::Drawable() :
    material(material),
    position(0),
    size(0),
    depth(0.0f),
    color(0.0f)
{

}

void Drawable::setMaterial(Material* material)
{
    if (material)
    {
        this->material = material->createInstance();
    }
}

Material* Drawable::getMaterial()
{
    return material;
}

void Drawable::rebuildMesh()
{
    if (material)
    {
        MaterialParameters& params = material->getParameters();
        params.setVec4("spriteColor", color);
    }
}

void Drawable::setPosition(const glm::ivec2& position)
{
    this->position = position;
}

const glm::ivec2& Drawable::getPosition() const
{
    return position;
}

void Drawable::setSize(const glm::ivec2& size)
{
    this->size = size;
}

const glm::ivec2& Drawable::getSize() const
{
    return size;
}

void Drawable::setDepth(float depth)
{
    this->depth = depth;
}

float Drawable::getDepth() const
{
    return depth;
}

void Drawable::setColor(const glm::vec4& color)
{
    this->color = color;
}

const glm::vec4& Drawable::getColor() const
{
    return color;
}

Mesh* Drawable::getSpriteMesh()
{
    return spriteMesh.get();
}
