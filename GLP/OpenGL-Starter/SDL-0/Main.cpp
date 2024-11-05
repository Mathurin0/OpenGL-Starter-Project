#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <SDL.h>
#include <glew.h>
using namespace std;
//#define GLEW_STATIC

string LoadShader(string fileName);

int main(int argc, char* argv[])
{
	std::srand(std::time(nullptr));
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
	string appleVertexShaderString = LoadShader("appleVertex.shader");
	string appleFragmentShaderString = LoadShader("appleFragment.shader");

	const char* appleVertexShaderSource = appleVertexShaderString.c_str();
	const char* appleFragmentShaderSource = appleFragmentShaderString.c_str();

	// Vertex
	unsigned int appleVertexShader, appleFragmentShader, snakeVertexShader, snakeFragmentShader;
	
	appleVertexShader = glCreateShader(GL_VERTEX_SHADER);
	appleFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(appleVertexShader, 1, &appleVertexShaderSource, NULL);
	glShaderSource(appleFragmentShader, 1, &appleFragmentShaderSource, NULL);

	glCompileShader(appleVertexShader);
	glCompileShader(appleFragmentShader);

	// Program
	unsigned int appleShaderProgram, snakeShaderProgram;

	appleShaderProgram = glCreateProgram();

	glAttachShader(appleShaderProgram, appleVertexShader);
	glAttachShader(appleShaderProgram, appleFragmentShader);

	glLinkProgram(appleShaderProgram);
	glUseProgram(appleShaderProgram);

	string snakeVertexShaderString = LoadShader("snakeVertex.shader");
	string snakeFragmentShaderString = LoadShader("snakeFragment.shader");
	const char* snakeVertexShaderSource = snakeVertexShaderString.c_str();
	const char* snakeFragmentShaderSource = snakeFragmentShaderString.c_str();
	snakeVertexShader = glCreateShader(GL_VERTEX_SHADER);
	snakeFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(snakeVertexShader, 1, &snakeVertexShaderSource, NULL);
	glShaderSource(snakeFragmentShader, 1, &snakeFragmentShaderSource, NULL);
	glCompileShader(snakeVertexShader);
	glCompileShader(snakeFragmentShader);
	snakeShaderProgram = glCreateProgram();
	glAttachShader(snakeShaderProgram, snakeVertexShader);
	glAttachShader(snakeShaderProgram, snakeFragmentShader);
	glLinkProgram(snakeShaderProgram);
	glUseProgram(snakeShaderProgram);

	// ---------Datas------------
	// Vertices
	float vertices[] = {
	0.05f, 0.05f, 0.0f,
	0.05f, -0.05f, 0.0f,
	-0.05f, -0.05f, 0.0f,
	-0.05f, 0.05f, 0.0f,
	};

	vector<float> chunksDirections = {
		0.0f, 0.5f,
	};

	vector<float> chunksPositions = {
		0.0f, 0.0f, 0.0f
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	float snakeOffsetX = 0.0f;
	float snakeOffsetY = 0.0f;
	float appleOffsetX = 0.0f;
	float appleOffsetY = 0.0f;
	float baseSpeedX = 0.5f;
	float baseSpeedY = 0.5f;
	float snakeSpeedX = 0.0f;
	float snakeSpeedY = baseSpeedY;
	float lastTime = 0.0f;
	float deltaTime;
	
	bool isRunning = true;
	bool newAppleNeeded = true;
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
				if (event.key.keysym.sym == SDLK_z)
				{
					snakeSpeedX = 0.0f;
					snakeSpeedY = baseSpeedY;
				}
				if (event.key.keysym.sym == SDLK_q)
				{
					snakeSpeedX = -baseSpeedX;
					snakeSpeedY = 0.0f;
				}
				if (event.key.keysym.sym == SDLK_s)
				{
					snakeSpeedX = 0.0f;
					snakeSpeedY = -baseSpeedY;
				}
				if (event.key.keysym.sym == SDLK_d)
				{
					snakeSpeedX = baseSpeedX;
					snakeSpeedY = 0.0f;
				}
				break;
			default:
				break;
			}
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen

		float timeValue = (float)SDL_GetTicks() / 1000;

		glUseProgram(appleShaderProgram);

		if (newAppleNeeded)
		{
			appleOffsetX = ((std::rand() % 200) - 95.0f) / 100;
			appleOffsetY = ((std::rand() % 200) - 95.0f) / 100;
			newAppleNeeded = false;
		}

		int appleOffsetLocation = glGetUniformLocation(appleShaderProgram, "offset");
		glUniform2f(appleOffsetLocation, appleOffsetX, appleOffsetY);

		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		glUseProgram(snakeShaderProgram);

		deltaTime = timeValue - lastTime;
		lastTime = timeValue;

		// X Offset
		snakeOffsetX += deltaTime * snakeSpeedX;
		chunksPositions[0] = snakeOffsetX;

		// Y Offset
		snakeOffsetY += deltaTime * snakeSpeedY;
		chunksPositions[1] = snakeOffsetY;
		if (snakeOffsetY >= 0.95 || snakeOffsetY <= -0.95f || snakeOffsetX >= 0.95 || snakeOffsetX <= -0.95f)
		{
			isRunning = false;
		}

		if (abs(snakeOffsetX - appleOffsetX) < 0.1f && abs(snakeOffsetY - appleOffsetY) < 0.1f)
		{
			newAppleNeeded = true;

			int lastPosition = chunksPositions.size() - 1;
			chunksPositions.push_back(chunksPositions[lastPosition - 2] + (-snakeSpeedX) / 5);
			chunksPositions.push_back(chunksPositions[lastPosition - 1] + (-snakeSpeedY) / 5);
			chunksPositions.push_back(0.0f);
			int lastDirection = chunksDirections.size() - 1;
			chunksDirections.push_back(chunksDirections[lastDirection - 1]);
			chunksDirections.push_back(chunksDirections[lastDirection]);
		}

		int offsetLocation = glGetUniformLocation(snakeShaderProgram, "offset");
		glUniform2f(offsetLocation, snakeOffsetX, snakeOffsetY);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		for (int i = 0; i < chunksDirections.size() / 2; i++)
		{
			if (i == 0)
			{
				chunksDirections[i] = snakeSpeedX;
				chunksDirections[i + 1] = snakeSpeedY;
			}
			else
			{
				if (chunksDirections[i * 2] != 0 && chunksDirections[i * 2 - 2] == 0)
				{
					std::cout << "turn to move y" << std::endl;
					if (abs(chunksPositions[i * 3 + 1] - chunksPositions[i * 3 - 2]) <= 0.03f) {
						chunksPositions[i * 3] = chunksPositions[i * 3 - 3];
						chunksPositions[i * 3 + 1] = chunksPositions[i * 3 - 2] - snakeSpeedY / 5;
						chunksDirections[i * 2] = chunksDirections[i * 2 - 2];
						chunksDirections[i * 2 + 1] = chunksDirections[i * 2 - 1];
					}
				}
				if (chunksDirections[i * 2 + 1] != 0 && chunksDirections[i * 2 - 1] == 0)
				{
					std::cout << "turn to move x" << std::endl;
					if (abs(chunksPositions[i * 3] - chunksPositions[i * 3 - 3]) <= 0.03f) {
						chunksPositions[i * 3] = chunksPositions[i * 3 - 3] - snakeSpeedX / 5;
						chunksPositions[i * 3 + 1] = chunksPositions[i * 3 - 2];
						chunksDirections[i * 2] = chunksDirections[i * 2 - 2];
						chunksDirections[i * 2 + 1] = chunksDirections[i * 2 - 1];
					}
				}

				chunksPositions[i * 3] += deltaTime * chunksDirections[i * 2];
				chunksPositions[i * 3 + 1] += deltaTime * chunksDirections[i * 2 + 1];

				glUniform2f(offsetLocation, chunksPositions[i * 3], chunksPositions[i * 3 + 1]);
				glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			}

			for (int i = 0; i < chunksDirections.size() / 2; i++)
			{
				if (i != 0)
				{
					std::cout << "x : " << chunksPositions[i * 3] << " / y : " << chunksPositions[i * 3 + 1] << std::endl;
				}
				
				if (i != 0 && abs(snakeOffsetX - chunksPositions[i * 3]) < 0.01f && abs(snakeOffsetY - chunksPositions[i * 3 + 1]) < 0.01f)
				{
					isRunning = false;
				}
			}
		}

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
