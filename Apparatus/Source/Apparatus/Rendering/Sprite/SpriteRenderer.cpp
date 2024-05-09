#include "SpriteRenderer.h"

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
    commands.push(sprite);
}

void SpriteRenderer::render()
{
    glClear(GL_DEPTH_BUFFER_BIT);

    if (!activeCamera)
    {
        return;
    }

    while (!commands.empty())
    {
        Drawable* sprite = commands.front();
        commands.pop();

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

                if (SpriteMesh* spriteMesh = sprite->getSpriteMesh())
                {
                    spriteMesh->bind();
                    glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(spriteMesh->getVertices().size()));
                }
            }
        }
    }
}
