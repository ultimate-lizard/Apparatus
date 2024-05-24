#include "SpriteRenderer.h"

#include <algorithm>

#include <glad/glad.h>

#include "Sprite.h"
#include "../Shader.h"
#include "../Texture.h"
#include "../Material.h"
#include "../Camera.h"

void SpriteRenderer::setActiveCamera(Camera* camera)
{
    activeCamera = camera;
}

void SpriteRenderer::push(Drawable* sprite)
{
    commands.push_back(sprite);
}

void SpriteRenderer::render()
{
    glClear(GL_DEPTH_BUFFER_BIT);
    glDepthMask(GL_FALSE);

    if (!activeCamera)
    {
        return;
    }

    std::sort(commands.begin(), commands.end(), [](Drawable* a, Drawable* b) {
        return (a && b && a->getDepth() < b->getDepth());
    });

    while (!commands.empty())
    {
        Drawable* sprite = commands.front();
        commands.pop_front();

        if (!sprite)
        {
            continue;
        }

        if (Material* spriteMaterial = sprite->getMaterial())
        {
            spriteMaterial->submitUniforms();

            if (Shader* spriteShader = spriteMaterial->getShader())
            {
                spriteShader->bind();
                spriteShader->setUniform("projection", activeCamera->getProjection());

                if (Mesh* spriteMesh = sprite->getSpriteMesh())
                {
                    spriteMesh->bind();

                    std::shared_ptr<VertexBuffer<float>> vertexBuffer = spriteMesh->getVertexBuffer<VertexBuffer<float>>();
                    assert(vertexBuffer);
                    glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(vertexBuffer->vertices.size()));
                }
            }
        }
    }

    glDepthMask(GL_TRUE);
}
