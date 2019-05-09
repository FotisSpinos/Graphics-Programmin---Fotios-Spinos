#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include "Display.h" 
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "transform.h"
#include "Overlay.h"

enum class GameState{PLAY, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:

	void initSystems();
	void processInput();
	void gameLoop();
	void drawGame();
	void updateExplosionShader(glm::mat4 model);
	void updateLightingShader(glm::mat4 model);
	void updateReflectionShader();
	void Skybox();

	GLuint skyboxVAO, skyboxVBO, cubemapTexture;
	vector<std::string> faces;

	Display _gameDisplay;
	GameState _gameState;
	Mesh mesh1;
	Mesh mesh2;
	Mesh mesh3;

	Texture texture;
	Texture texture1;
	Texture skybox;

	Overlay overlay;

	Shader shaderSkybox;
	Shader shaderExplosion;
	Shader shaderLight;
	Shader shaderReflection;

	glm::mat4 modelView;

	Camera myCamera;

	float counter;
	unsigned int whistle;
	unsigned int backGroundMusic;
};

