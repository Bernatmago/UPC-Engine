#include "Globals.h"
#include "Application.h"
#include "ModuleGui.h"
#include "ModuleWindow.h"
#include "ModuleProgram.h"
#include "ModuleRender.h"
#include "ModuleCamera.h"
#include "SDL.h"
#include "Console.h"

#include "glew.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include <shellapi.h>

ModuleGui::ModuleGui()
{
}

ModuleGui::~ModuleGui()
{
}

// Called before render is available
bool ModuleGui::Init()
{
	LOG("Creating Imgui ")
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->context);
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGui::StyleColorsDark();

	 SDL_GetVersion(&about.sdl_version);
	 about.n_cpu = SDL_GetCPUCount();
	 about.ram_gb = SDL_GetSystemRAM() / 1024.0f;

	 about.gpu = (unsigned char*)glGetString(GL_RENDERER);
	 about.gpu_brand = (unsigned char*)glGetString(GL_VENDOR);
	 glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &about.vram_budget);
	 glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &about.vram_free);

	return true;
}

void ModuleGui::Menu() {
	// Main Menu
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("Help")) {
			ImGui::MenuItem("Gui Demo", NULL, &show_demo);
			if (ImGui::MenuItem("About"))
				OpenBrowser("https://github.com/Bernatmago/UPC-Engine");			
			if (ImGui::MenuItem("Bug Report"))
				OpenBrowser("https://github.com/Bernatmago/UPC-Engine/issues");
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Windows")) {
			ImGui::MenuItem("Sidebar", NULL, &show_sidebar);
			ImGui::MenuItem("Model Properties", NULL, &show_model_properties); // TODO: Add properties menu
			ImGui::MenuItem("Console", NULL, &show_console);
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void ModuleGui::Sidebar() {
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_MenuBar;
	//window_flags |= ImGuiWindowFlags_NoMove;
	//window_flags |= ImGuiWindowFlags_NoResize;
	ImGui::Begin("Sidebar", &show_sidebar, window_flags);
	if (ImGui::CollapsingHeader("Window")) App->window->OptionsMenu();
	if (ImGui::CollapsingHeader("Camera")) App->camera->OptionsMenu();
	if (ImGui::CollapsingHeader("Model")) App->renderer->model->OptionsMenu();
	if (ImGui::CollapsingHeader("Performance")) Performance();
	if (ImGui::CollapsingHeader("About")) About();
	ImGui::End();
}

void ModuleGui::Performance() {
	static const float vram_budget_mb = about.vram_budget / 1024.0f;
	glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &about.vram_free);
	float vram_free_mb = about.vram_free / 1024.0f;
	float vram_usage_mb = vram_budget_mb - vram_free_mb;
	ImGui::Text("VRAM Budget: %.1f Mb", vram_budget_mb);
	ImGui::Text("Vram Usage:  %.1f Mb", vram_usage_mb);
	ImGui::Text("Vram Avaliable:  %.1f Mb", vram_free_mb);
}

void ModuleGui::About() {
	static SDL_version version;
	ImGui::Text("SDL Version: %d.%d.%d", about.sdl_version.major,
		about.sdl_version.minor, about.sdl_version.patch);
	ImGui::Separator();
	ImGui::Text("CPUs: %d", about.n_cpu);
	ImGui::Text("System RAM: %.1f Gb", about.ram_gb);
	ImGui::Separator();
	ImGui::Text("GPU: %s", about.gpu);
	ImGui::Text("Brand: %s", about.gpu_brand);

	
}

void ModuleGui::OpenBrowser(const char* url)
{
	// Handle parent window (none)
	// Verb to be executed (0 uses default)
	// Object to execute with verb (url)
	// Executable file (none)
	// Workdir for the action (default)
	// Show the window
	ShellExecute(0, 0, url, 0, 0, SW_SHOW);
}

update_status ModuleGui::PreUpdate()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
		
	Menu();
	if (show_sidebar) Sidebar(); // TODO: Make all vatiables editable for renderer, window, input and textures
	if (show_console) Logger->Draw(&show_console);
	if (show_demo) ImGui::ShowDemoWindow(&show_demo);
	if (show_model_properties)
		App->renderer->model->PropertiesWindow(&show_model_properties);

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleGui::Update(const float delta)
{	
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleGui::CleanUp()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}

