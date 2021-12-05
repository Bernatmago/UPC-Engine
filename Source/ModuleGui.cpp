#include "Globals.h"
#include "Application.h"
#include "ModuleGui.h"
#include "ModuleWindow.h"
#include "ModuleProgram.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
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
	CreateContext();
	SetConfig();
	RetrieveAboutData();
	return true;
}

void ModuleGui::CreateContext()
{
	LOG("Creating Imgui");
	ImGui::CreateContext();	
	ImGui_ImplSDL2_InitForOpenGL(App->window->GetWindow(), App->renderer->GetGLContext());
	ImGui_ImplOpenGL3_Init("#version 330");
}

void ModuleGui::SetConfig()
{
	ImGui::StyleColorsDark();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
}

void ModuleGui::RetrieveAboutData()
{
	SDL_GetVersion(&about.sdl_version);
	about.devil_version = App->textures->GetDevilVersion();
	about.cpu.cores = SDL_GetCPUCount();
	about.cpu.l1_cache_kb = (float)SDL_GetCPUCacheLineSize() / 1024.0f;
	about.ram_gb = (float)SDL_GetSystemRAM() / 1024.0f;

	about.gpu = App->renderer->GetGpuData();
	about.gl = App->renderer->GetGlVersion();
}

update_status ModuleGui::PreUpdate(const float delta)
{
	NewFrame();

	Menu();
	if (show_sidebar)
		Sidebar(delta);
	if (show_console)
		Logger->Draw(&show_console);
	if (show_demo)
		ImGui::ShowDemoWindow(&show_demo);
	if (show_model_properties)
		App->renderer->GetModel()->PropertiesWindow(&show_model_properties);

	return UPDATE_CONTINUE;
}

void ModuleGui::NewFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
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
	DestroyContext();
	return true;
}

void ModuleGui::DestroyContext()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
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
			ImGui::MenuItem("Model Properties", NULL, &show_model_properties);
			ImGui::MenuItem("Console", NULL, &show_console);
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void ModuleGui::Sidebar(const float delta) {
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_MenuBar;
	//window_flags |= ImGuiWindowFlags_NoMove;
	//window_flags |= ImGuiWindowFlags_NoResize;
	ImGui::Begin("Sidebar", &show_sidebar, window_flags);
	if (ImGui::CollapsingHeader("Window")) App->window->OptionsMenu();
	if (ImGui::CollapsingHeader("Camera")) App->camera->OptionsMenu();
	if (ImGui::CollapsingHeader("Render")) App->renderer->OptionsMenu();
	if (ImGui::CollapsingHeader("Model")) App->renderer->GetModel()->OptionsMenu();
	if (ImGui::CollapsingHeader("Textures")) App->textures->OptionsMenu();
	if (ImGui::CollapsingHeader("Performance")) App->renderer->PerformanceMenu(delta);
	if (ImGui::CollapsingHeader("About")) About();
	ImGui::End();
}

void ModuleGui::About() {
	static const ImVec4 cool_blue(0.110f, 0.588f, 0.950f, 1.0f);
	static const ImVec4 yellow(1.0f, 1.0f, 0.0f, 1.0f);
	ImGui::TextColored(yellow, "Libraries");
	ImGui::Text("SDL Version: %d.%d.%d", about.sdl_version.major,
		about.sdl_version.minor, about.sdl_version.patch);	
	ImGui::Text("DevIL Version: %d", about.devil_version);
	ImGui::Text("Glew Version: %s", about.gl.glew);
	ImGui::Text("OpenGL Version: %s", about.gl.opengl);
	ImGui::Text("GLSL Version: %s", about.gl.glsl);

	ImGui::Separator();
	ImGui::TextColored(yellow, "Hardware");
	ImGui::TextColored(cool_blue, "CPU");
	ImGui::Text("Cores: %d", about.cpu.cores);
	ImGui::Text("L1 line size: %f Kb", about.cpu.l1_cache_kb);
	ImGui::Separator();
	ImGui::TextColored(cool_blue, "Memory");
	ImGui::Text("RAM: %.1f Gb", about.ram_gb);
	ImGui::Separator();
	ImGui::TextColored(cool_blue, "GPU");
	ImGui::Text("GPU: %s", about.gpu.name);
	ImGui::Text("Brand: %s", about.gpu.brand);
	ImGui::Text("Vram Budget: %f Mb", about.gpu.vram_budget_mb);
		
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
