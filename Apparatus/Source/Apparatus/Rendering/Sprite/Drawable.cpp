#include "Drawable.h"

#include "../Material.h"

Drawable::Drawable(int bufferSize) :
    material(material),
    position(0),
    size(0),
    depth(0.0f),
    color(0.0f)
{
    auto vao = std::make_unique<VertexArrayObject>();
    assert(vao);
    vao->setStride(5 * sizeof(float));
    vao->addAttribute(3);
    vao->addAttribute(2);

    spriteMesh = std::make_unique<Mesh>(std::move(vao), bufferSize, 0);
    assert(spriteMesh);
    spriteMesh->init();
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

    if (material)
    {
        MaterialParameters& params = material->getParameters();
        params.setVec4("spriteColor", color);
    }
}

const glm::vec4& Drawable::getColor() const
{
    return color;
}

Mesh* Drawable::getSpriteMesh()
{
    return spriteMesh.get();
}
