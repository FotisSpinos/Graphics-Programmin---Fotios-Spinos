#include "MainGame.h"
#include "Camera.h"
#include <iostream>
#include <string>


Transform transform;

MainGame::MainGame()
{
	_gameState = GameState::PLAY;
	Display* _gameDisplay = new Display(); //new display
}

MainGame::~MainGame() 
{
}

void MainGame::run()
{
	initSystems(); 
	gameLoop();
}

void MainGame::initSystems()
{
	_gameDisplay.initDisplay(); 
	texture.init("..\\res\\bricks.jpg"); //load texture
	texture1.init("..\\res\\water.jpg"); //load texture

	shaderSkybox.init("..\\res\\shaderSkybox.vert", "..\\res\\shaderSkybox.frag");

	shaderReflection.init("..\\res\\shaderReflection.vert", "..\\res\\shaderReflection.frag");

	shaderLight.init("..\\res\\shaderLighting.vert", "..\\res\\shaderLighting.frag", "..\\res\\geomShader.geom");//"..\\res\\geomShader.geom");
	shaderExplosion.init("..\\res\\shaderExplosion.vert", "..\\res\\shaderExplosion.frag", "..\\res\\shaderExplosion.geom");

	overlay.init("..\\res\\bricks.jpg");

	mesh1.loadModel("..\\res\\monkey3.obj");
	mesh2.loadModel("..\\res\\Cone.obj");
	mesh3.loadModel("..\\res\\Sphere.obj");
	
	myCamera.initCamera(glm::vec3(0, 0, -10.0), 70.0f, (float)_gameDisplay.getWidth()/_gameDisplay.getHeight(), 0.01f, 1000.0f);

	counter = 1.0f;

	vector<std::string> faces
	{
		"..\\res\\skybox\\right.jpg",
		"..\\res\\skybox\\left.jpg",
		"..\\res\\skybox\\top.jpg",
		"..\\res\\skybox\\bottom.jpg",
		"..\\res\\skybox\\front.jpg",
		"..\\res\\skybox\\back.jpg"
	};
	cubemapTexture = skybox.loadCubemap(faces); //Load the cube map using "faces" into cubemapTextures

	float skyboxVertices[] = {
		// positions          
		-6.0f,  6.0f, -6.0f,
		-6.0f, -6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,
		6.0f,  6.0f, -6.0f,
		-6.0f,  6.0f, -6.0f,

		-6.0f, -6.0f,  6.0f,
		-6.0f, -6.0f, -6.0f,
		-6.0f,  6.0f, -6.0f,
		-6.0f,  6.0f, -6.0f,
		-6.0f,  6.0f,  6.0f,
		-6.0f, -6.0f,  6.0f,

		6.0f, -6.0f, -6.0f,
		6.0f, -6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,

		-6.0f, -6.0f,  6.0f,
		-6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f, -6.0f,  6.0f,
		-6.0f, -6.0f,  6.0f,

		-6.0f,  6.0f, -6.0f,
		6.0f,  6.0f, -6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		-6.0f,  6.0f,  6.0f,
		-6.0f,  6.0f, -6.0f,

		-6.0f, -6.0f, -6.0f,
		-6.0f, -6.0f,  6.0f,
		6.0f, -6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,
		-6.0f, -6.0f,  6.0f,
		6.0f, -6.0f,  6.0f
	};

	//use openGL functionality to generate & bind data into buffers
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		processInput();
		drawGame();
	}
}

void MainGame::processInput()
{
	SDL_Event evnt;

	while(SDL_PollEvent(&evnt)) //get and process events
	{
		switch (evnt.type)
		{
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;
		}
	}	
}


void MainGame::Skybox()
{
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	shaderSkybox.Bind();
	shaderSkybox.setInt("skybox", 0);
	//view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
	shaderSkybox.setMat4("view", myCamera.GetView());
	shaderSkybox.setMat4("projection", myCamera.GetProjection());
	// skybox cube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default
}

void MainGame::updateReflectionShader()
{
	shaderReflection.setMat4("model", transform.GetModel());
	shaderReflection.setMat4("view", myCamera.GetView());
	shaderReflection.setMat4("projection", myCamera.GetProjection());

	shaderReflection.setVec3("cameraPos", myCamera.getPos());
	shaderReflection.setInt("skybox", 0);
}

void MainGame::updateExplosionShader(glm::mat4 model)
{
	shaderExplosion.setMat4("transform", transform.GetModel());	
	shaderExplosion.setSampler("texture", 0);
	shaderExplosion.setFloat("texCont", 0.4f);	
	shaderExplosion.setFloat("time", 0.0f + counter * 2.0f);
}

void MainGame::updateLightingShader(glm::mat4 model)
{
	shaderLight.setMat4("transform", model);

	shaderLight.setVec3("aColor", glm::vec3(1.0, 0.0, 0.0));	// (1.0, 0.0, 0.0)	
	shaderLight.setVec3("sColor", glm::vec3(0.3, 0.7, 0.0));	// (0.5, 0.5, 0.0)	
	shaderLight.setVec3("dColor", glm::vec3(0.7, 0.0, 0.3));	// (0.7, 0.0, 0.3)	

	shaderLight.setVec3("cameraPos", myCamera.getPos());
	shaderLight.setVec3("lightPos", glm::vec3(0, -5.0, -5.0));

	shaderLight.setFloat("aIntensity", 0.15f);	//0.15f);	
	shaderLight.setFloat("sIntensity", 0.5f);	//0.5f);
	shaderLight.setFloat("dIntensity", 6.2f);	//6.2f);	 

	shaderLight.setFloat("sRoughness", 35.0);

	shaderLight.setSampler("texture1", 0);
	shaderLight.setSampler("texture2", 1);

	shaderLight.setFloat("texCont1", 0.3f);	//0.3f
	shaderLight.setFloat("texCont2", 0.7f);	//0.7f
}

void MainGame::drawGame()
{
	_gameDisplay.clearDisplay(0.0f, 0.0f, 0.0f, 1.0f);

	Skybox();

	texture.Bind(0);
	texture1.Bind(1);

	// object 1
	transform.SetPos(glm::vec3(sinf(counter), 0.7, -0.8));
	transform.SetRot(glm::vec3(0.0, 0.2, 0.0));
	transform.SetScale(glm::vec3(0.3, 0.3, 0.3));

	shaderExplosion.Bind();
	updateExplosionShader(transform.GetModel());

	mesh1.draw();

	// object 2
	transform.SetPos(glm::vec3(sinf(counter), 0.2, 0.0));
	transform.SetRot(glm::vec3(0.0, counter * 5, 0.0));
	transform.SetScale(glm::vec3(0.2, 0.2, 0.2));

	shaderReflection.Bind();
	updateReflectionShader();

	mesh2.draw();

	// object 3
	transform.SetPos(glm::vec3(-0.2, 0.2, 0.2));	//sinf(counter)
	transform.SetRot(glm::vec3(0.0, counter * 1.01, 0.0));
	transform.SetScale(glm::vec3(0.4, 0.4, 0.4));

	shaderLight.Bind();
	updateLightingShader(transform.GetModel());

	mesh3.draw();

	counter = counter + 0.01f;

	glEnableClientState(GL_COLOR_ARRAY); 
	glEnd();

	_gameDisplay.swapBuffer();
} 