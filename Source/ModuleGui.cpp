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
		if (ImGui::BeginMenu("File")) {
			ImGui::MenuItem("Save");
			ImGui::MenuItem("Load");
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit")) {
			if (ImGui::MenuItem("Do")) {}
			if (ImGui::MenuItem("Undo")) {}
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
	ImGui::Begin("Sidebar", NULL, window_flags);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Tools")) {
			ImGui::MenuItem("Console", NULL ,&show_console);
			ImGui::MenuItem("Demo", NULL, &show_demo);
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	if (ImGui::CollapsingHeader("Window")) App->window->OptionsMenu();
	if (ImGui::CollapsingHeader("Camera")) App->camera->OptionsMenu();
	if (ImGui::CollapsingHeader("Model")) App->renderer->model->OptionsMenu();
	if (ImGui::CollapsingHeader("Performance")) Performance();
	if (ImGui::CollapsingHeader("About")) About();

	ImGui::End();
}

void ModuleGui::Performance() {

}

void ModuleGui::About() {
	static SDL_version version;
	static const float vram_budget_mb = about.vram_budget / 1024.0f;
	glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &about.vram_free);
	float vram_free_mb = about.vram_free / 1024.0f;
	float vram_usage_mb = vram_budget_mb - vram_free_mb;

	ImGui::Text("SDL Version: %d.%d.%d", about.sdl_version.major,
		about.sdl_version.minor, about.sdl_version.patch);
	ImGui::Separator();
	ImGui::Text("CPUs: %d", about.n_cpu);
	ImGui::Text("System RAM: %.1f Gb", about.ram_gb);
	ImGui::Separator();
	ImGui::Text("GPU: %s", about.gpu);
	ImGui::Text("Brand: %s", about.gpu_brand);
	ImGui::Text("VRAM Budget: %.1f Mb", vram_budget_mb);
	ImGui::Text("Vram Usage:  %.1f Mb", vram_usage_mb);
	ImGui::Text("Vram Avaliable:  %.1f Mb", vram_free_mb);
}

update_status ModuleGui::PreUpdate()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	if (show_console) Logger->Draw(&show_console);
	if (show_demo) ImGui::ShowDemoWindow(&show_demo);
	Menu();
	Sidebar();


	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleGui::Update()
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

