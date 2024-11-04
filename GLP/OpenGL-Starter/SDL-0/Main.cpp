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
	int width = 400;
	int height = 300;
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
	

	// ---------Datas------------
	// Vertices
	float vertices[] = {
	-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,
	 0.0f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f
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
			default:
				break;
			}
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen

		glUseProgram(shaderProgram);

		// Change Color over Time
		/*float speed = 4.0f;
		float redValue = (sin(timeValue * speed) / 2.0f) + 0.5f;
		float greenValue = (sin(timeValue * speed + 2) / 2.0f) + 0.5f;
		float blueValue = (sin(timeValue * speed + 4) / 2.0f) + 0.5f;
		int colorLocation = glGetUniformLocation(shaderProgram, "shift");
		glUniform3f(colorLocation, redValue, greenValue, blueValue);*/

		float speed = 5.0f;
		float timeValue = ((float)SDL_GetTicks() / 1000);
		speed += (sin(timeValue) * speed / 2);
		float offset = (sin(timeValue * speed) / 2.0f);
		int offsetLocation = glGetUniformLocation(shaderProgram, "offset");
		glUniform1f(offsetLocation, offset);

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

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
