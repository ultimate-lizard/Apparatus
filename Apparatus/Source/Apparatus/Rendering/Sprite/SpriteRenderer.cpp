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

void SpriteRenderer::push(Sprite* sprite)
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
        Sprite* sprite = commands.front();
        commands.pop();

        if (!sprite)
        {
            continue;
        }

        if (Material* spriteMaterial = sprite->getMaterial())
        {
            if (Shader* spriteShader = spriteMaterial->getShader())
            {
                spriteShader->bind();
                spriteShader->setUniform("projection", activeCamera->getProjection());

                MaterialParameters& params = spriteMaterial->getParameters();

                for (auto mapIter : params.getAllBoolParameters())
                {
                    spriteShader->setUniform(mapIter.first, mapIter.second);
                }

                for (auto mapIter : params.getAllFloatParameters())
                {
                    spriteShader->setUniform(mapIter.first, mapIter.second);
                }

                for (auto mapIter : params.getAllVec2Parameters())
                {
                    spriteShader->setUniform(mapIter.first, mapIter.second);
                }

                for (auto mapIter : params.getAllVec3Parameters())
                {
                    spriteShader->setUniform(mapIter.first, mapIter.second);
                }

                for (auto mapIter : params.getAllVec4Parameters())
                {
                    spriteShader->setUniform(mapIter.first, mapIter.second);
                }

                int textureIndex = 0;
                for (auto mapIter : params.getAllTextureParameters())
                {
                    if (Texture* texture = mapIter.second)
                    {
                        glActiveTexture(GL_TEXTURE0 + textureIndex);
                        texture->bind();
                        textureIndex++;
                    }
                }

                if (SpriteMesh* spriteMesh = sprite->getSpriteMesh())
                {
                    spriteMesh->bind();
                    glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(spriteMesh->getVertices().size()));
                }
            }
        }
    }
}