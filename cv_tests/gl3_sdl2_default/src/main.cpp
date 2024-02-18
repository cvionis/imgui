#include <assert.h>
#include <stdio.h>
#include <stdint.h>

#include "gl32.h"
#include "SDL.h"

#include "imgui.h" 
#include "imgui_impl_sdl2.h" 
#include "imgui_impl_opengl3.h" 

#undef main // Temporary SDL-related fix. Figure out how to make this not required.

typedef uint32_t u32;
typedef uint32_t b32;
typedef uint16_t u16;
typedef uint8_t  u8;

const u32 SCREEN_WIDTH  = 1280;
const u32 SCREEN_HEIGHT = 720;

SDL_Window    *window;
SDL_GLContext  gl_context;
b32 		   should_quit;

ImGuiIO io; 

int Init() {
	int window_flags;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_LogError(0, "Failed to initialize SDL. SDL_Error: %s", 
			SDL_GetError());
		return 0;
	}

	window_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL;
	window = SDL_CreateWindow("imgui demo", SDL_WINDOWPOS_UNDEFINED, 
		SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, window_flags);
	if (!window) {
		SDL_LogError(0, "Failed to create SDL Window. SDL_Error: %s",
			SDL_GetError());
		return 0;
	}

	gl_context = SDL_GL_CreateContext(window);
	if (!gl_context) {
		SDL_LogError(0, "Failed to create SDL OpenGL context. SDL_Error: %s",
			SDL_GetError());
		return 0;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetSwapInterval(1);

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	io = ImGui::GetIO(); 
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; 
	io.Fonts->AddFontFromFileTTF("../imgui/misc/fonts/DroidSans.ttf", 16);

	ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
	ImGui_ImplOpenGL3_Init();

	return 1;
}

void ProcessInput() {
	SDL_Event event; 

	while (SDL_PollEvent(&event)) {
		ImGui_ImplSDL2_ProcessEvent(&event); 
		if (event.type == SDL_QUIT) should_quit = 1; 
	}
}

int main(int argc, char **argv) {
	should_quit = 0;

	if (!Init()) should_quit = 1;

	while (!should_quit) {
		ProcessInput(); 

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame(); 

		ImGui::ShowDemoWindow();

		glClearColor(0.08f, 0.08f, 0.08f, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui::Render(); 
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); 

		SDL_GL_SwapWindow(window); 
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown(); 
	ImGui::DestroyContext();

	return 0;
}
