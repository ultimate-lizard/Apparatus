//#include <iostream>
//#include <string>
//#include <fstream>
//#include <sstream>
//#include <vector>
//
//#include <SDL2/SDL.h>
//#undef main
//#include <SDL2/SDL_image.h>
//#include <glad/glad.h>
//#include <glm/glm.hpp>
//#include <freetype/freetype.h>

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include "Apparatus/Apparatus.h"
#include "Apparatus/Rendering/Mesh.h"



int main(int argc, char** argv)
{	
	Apparatus app("Apparatus");
	return app.launch(argc, argv);

	//SDL_Init(SDL_INIT_VIDEO);
	//IMG_Init(IMG_INIT_PNG);

	//FT_Library ft;
	//FT_Init_FreeType(&ft);

	//FT_Face face;
	//FT_New_Face(ft, "../Fonts/Arial.ttf", 0, &face);
	//FT_ULong a;
	//FT_Load_Char(face, a, 0);

	//SDL_Window* sdlWindow = nullptr;
	//sdlWindow = SDL_CreateWindow(
	//	"TITLE",
	//	SDL_WINDOWPOS_CENTERED,
	//	SDL_WINDOWPOS_CENTERED,
	//	800,
	//	600,
	//	SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
	//);

	//const std::string shaderFolder = "../Shaders/";
	//Shader testShader(shaderFolder + "Test.vert", shaderFolder + "Test.frag");

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//float data[] = {
	//	-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
	//	-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
	//	0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
	//	0.5f, 0.5f, 0.0f, 1.0f, 1.0f
	//};

	//unsigned int vbo = 0;
	//glGenBuffers(1, &vbo);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

	//// EBO
	//unsigned int elements[6] {
	//	0, 1, 2,
	//	1, 3, 2
	//};

	//unsigned int ebo = 0;
	//glGenBuffers(1, &ebo);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	//// VAO
	//unsigned int vao = 0;
	//glCreateVertexArrays(1, &vao);
	//glBindVertexArray(vao);

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);

	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	//// TEXTURE
	//Texture tex;
	//tex.load("../Textures/container.png");

	//// CLEAR
	//glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);

	//// DRAW
	//glBindVertexArray(vao);
	//tex.bind();
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	//testShader.bind();
	//// glDrawArrays(GL_TRIANGLES, 0, 3);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//SDL_GL_SwapWindow(sdlWindow);

	//SDL_Delay(10000);

	//// glDeleteVertexArrays(1, &vao);
	//SDL_Quit();
}
