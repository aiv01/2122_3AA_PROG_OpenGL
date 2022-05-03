#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main() {
	std::cout << "It' Working" << std::endl;

	if (!glfwInit()) {
		std::cout << "Error: Init GLFW\n";
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	int Width = 600;
	int Height = 400;
	GLFWwindow* Window = glfwCreateWindow(Width, Height, "OpenglApp", NULL, NULL);

	if (!Window) {
		std::cout << "Error: Opening Window\n";
	}

	glfwMakeContextCurrent(Window);
	glfwSwapInterval(0);

	if (!gladLoadGL()) {
		std::cout << "Error: Glad Wrapper\n";
	}

	printf("GL VERSION: %s\n", glGetString(GL_VERSION));
	printf("GL RENDERER: %s\n", glGetString(GL_RENDERER));
	printf("GL VENDOR: %s\n", glGetString(GL_VENDOR));
	printf("GLSL VERSION: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	float DeltaTime = 0.f;
	float LastTime = glfwGetTime();
	float TimeElapsed = 0.f;

	while(!glfwWindowShouldClose(Window)) {
		//operazione di disegno
		float CurrTime = glfwGetTime();
		DeltaTime = CurrTime - LastTime;
		LastTime = CurrTime;

		TimeElapsed += DeltaTime;
		if (TimeElapsed >= 1.f) {
			TimeElapsed -= 1.f;

			int Fps = 1.f / DeltaTime;
			char Title[50];
			sprintf_s(Title, sizeof(Title), "Delta Time: %f - Fps: %d", DeltaTime, Fps);
			glfwSetWindowTitle(Window, Title);
		}

		glfwSwapBuffers(Window);
		glfwPollEvents();
	}

	glfwDestroyWindow(Window);
	glfwTerminate();
	return 0;	
}