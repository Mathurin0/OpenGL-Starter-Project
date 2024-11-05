#include <iostream>
#include <fstream>
#include <string>
#include <SDL.h>
#include <glew.h>
using namespace std;
//#define GLEW_STATIC

string LoadShader(string fileName);

int main(int argc, char* argv[])
{
	// -----------Init-----------
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		cout << "SDL initialization failed. SDL Error: " << SDL_GetError();
	}
	else
	{
		cout << "SDL initialization succeeded!\n";
	}

	// --------SETTING UP SDL--------
	//Create a simple window
	int width = 1080;
	int height = 1080;
	unsigned int center = SDL_WINDOWPOS_CENTERED;
	SDL_Window* Window = SDL_CreateWindow("My window", center, center, width, height, SDL_WINDOW_OPENGL);

	//Create an OpenGL compatible context to let glew draw on it
	SDL_GLContext Context = SDL_GL_CreateContext(Window);

	// -----SETTING UP OPENGL WITH GLEW-----
	//Initialize glew
	glewExperimental = GL_TRUE;
	if (glewInit() == GLEW_OK) {
		cout << "Glew initialized successfully\n";
	}

	glViewport(0, 0, width, height);
	glClearColor(0.0f, 0.2f, 0.2f, 1.0f);


	// ---------Shaders-----------
	// Recover Shaders Files
	string vertexShaderString = LoadShader("simpleVertex.shader");
	string fragmentShaderString = LoadShader("simpleFragment.shader");

	const char* vertexShaderSource = vertexShaderString.c_str();
	const char* fragmentShaderSource = fragmentShaderString.c_str();

	// Vertex
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Fragment
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	// Static Shader
	vertexShaderString = LoadShader("staticVertex.shader");
	fragmentShaderString = LoadShader("blinkFragment.shader");

	const char* staticVertexShaderSource = vertexShaderString.c_str();
	const char* blinkFragmentShaderSource = fragmentShaderString.c_str();

	unsigned int staticVertexShader;
	staticVertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(staticVertexShader, 1, &staticVertexShaderSource, NULL);
	glCompileShader(staticVertexShader);

	unsigned int blinkFragmentShader;
	blinkFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(blinkFragmentShader, 1, &blinkFragmentShaderSource, NULL);
	glCompileShader(blinkFragmentShader);

	unsigned int staticShaderProgram;
	staticShaderProgram = glCreateProgram();
	glAttachShader(staticShaderProgram, staticVertexShader);
	glAttachShader(staticShaderProgram, blinkFragmentShader);
	glLinkProgram(staticShaderProgram);
	glUseProgram(staticShaderProgram);

	

	// ---------Datas------------
	// Vertices
	float vertices[] = {
	// Top
	0.0f, 0.3f, 0.0f,		0.969f, 1.0f, 0.459f,
	-1.0f, 0.4f, 0.0f,		1.0f, 0.82f, 0.463f,
	-0.6f, 0.0f, 0.0f,		0.459f, 1.0f, 0.463f,
	0.6f, 0.0f, 0.0f,		0.459f, 0.808f, 1.0f,
	1.0f, 0.4f, 0.0f,		1.0f, 0.459f, 0.992f,
	// Bottom
	0.0f, -0.2f, 0.0f,		0.969f, 1.0f, 0.459f,
	-0.1f, -0.6f, 0.0f,		1.0f, 0.82f, 0.463f,
	-0.3f, -0.4f, 0.0f,		0.459f, 1.0f, 0.463f,
	-0.4f, 0.0f, 0.0f,		0.459f, 0.808f, 1.0f,
	0.4f, 0.0f, 0.0f,		1.0f, 0.459f, 0.992f,
	0.3f, -0.4f, 0.0f,		0.459f, 1.0f, 0.98f,
	0.1f, -0.6f, 0.0f,		1.0f, 0.459f, 0.459f,

	// Square
	0.5f, 0.5f,-1.0f,		0.0f, 0.0f, 1.0f,
	0.5f, -0.5f, -1.0f,		1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -1.0f,	0.0f, 1.0f, 0.0f,
	-0.5f, 0.5f, -1.0f,		1.0f, 1.0f, 1.0f,
	};

	// VAO
	unsigned int vao;
	glGenBuffers(1, &vao);
	glBindVertexArray(vao);

	// VBO
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	float offsetX = 0.0f;
	float offsetY = 0.0f;
	float speedX = 0.5f;
	float speedY = 0.5f;
	float lastTime = 0.0f;
	float deltaTime;
	
	bool isRunning = true;
	while (isRunning)
	{
		// Inputs
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				isRunning = false;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE)
					isRunning = false;
				break;
			default:
				break;
			}
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen

		glUseProgram(staticShaderProgram);
		glDrawArrays(GL_TRIANGLE_FAN, 12, 4);

		glUseProgram(shaderProgram);

		// Change Color over Time
		/*float speed = 4.0f;
		float redValue = (sin(timeValue * speed) / 2.0f) + 0.5f;
		float greenValue = (sin(timeValue * speed + 2) / 2.0f) + 0.5f;
		float blueValue = (sin(timeValue * speed + 4) / 2.0f) + 0.5f;
		int colorLocation = glGetUniformLocation(shaderProgram, "shift");
		glUniform3f(colorLocation, redValue, greenValue, blueValue);*/

		float timeValue = (float)SDL_GetTicks() / 1000;
		deltaTime = timeValue - lastTime;
		lastTime = timeValue;

		// X Offset
		offsetX += deltaTime * speedX;
		if (offsetX >= 0.5 || offsetX <= -0.5f)
		{
			speedX = -speedX;
		}
		int offsetXLocation = glGetUniformLocation(shaderProgram, "offsetX");
		glUniform1f(offsetXLocation, offsetX);

		// Y Offset
		offsetY += deltaTime * speedY;
		if (offsetY >= 0.8 || offsetY <= -0.7f) {
			speedY = -speedY;
		}
		int offsetYLocation = glGetUniformLocation(shaderProgram, "offsetY");
		glUniform1f(offsetYLocation, offsetY);

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 5);
		glDrawArrays(GL_TRIANGLE_FAN, 5, 7);

		SDL_GL_SwapWindow(Window); // Swapbuffer
	}

	SDL_DestroyWindow(Window);
	SDL_GL_DeleteContext(Context);

	cin.get();
	return 0;
}

string LoadShader(string fileName) {
	ifstream myFile;
	myFile.open(fileName);
	if (myFile.fail()) {
		cerr << "Error - failed to open " << fileName << endl;
	}
	string fileText = "";
	string line = "";
	while (getline(myFile, line)) {
		fileText += line + '\n';
	}

	myFile.close();
	return fileText;
}
