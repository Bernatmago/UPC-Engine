#include "Globals.h"
#include "Application.h"
#include "ModuleGui.h"
#include "ModuleWindow.h"
#include "ModuleShader.h"
#include "ModuleRender.h"
#include "SDL.h"
#include "Console.h"

#include "glew.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

ModuleGui::ModuleGui()
{
}

// Destructor
ModuleGui::~ModuleGui()
{
}

// Called before render is available
bool ModuleGui::Init()
{
	LOG("Creating Imgui ")

	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;

	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	// io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	ImGui::StyleColorsDark();
	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->context);
	ImGui_ImplOpenGL3_Init("#version 330");

	return true;
}

void ModuleGui::RenderMenu() {
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

void ModuleGui::RenderSidebar() {
	//Sidebar
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_MenuBar;
	//window_flags |= ImGuiWindowFlags_NoMove;
	//window_flags |= ImGuiWindowFlags_NoResize;
	ImGui::Begin("Sidebar", NULL, window_flags);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Tools")) {
			// TODO: Investigate shortcut null
			ImGui::MenuItem("Console", NULL ,&show_console);
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	if (ImGui::CollapsingHeader("Performance")) {
		ImGui::Text("Fps Graph Here");
	}
	if (ImGui::CollapsingHeader("About")) {
		ImGui::Text("This is the about section");
	}

	ImGui::End();
}

void RenderConsole(bool* p_open) {
	ImGui::Begin("Console", p_open);
	ImGui::Text("Logs Here");
	ImGui::End();
}

update_status ModuleGui::PreUpdate()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	bool demo = false;
	if (demo) {
		ImGui::ShowDemoWindow(&demo);
	}
	else
	{
		if (show_console) {
			Logger->Draw(&show_console);
		}
		RenderMenu();
		RenderSidebar();
	}

	// It does literally nothing
	
	

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleGui::Update()
{	
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return UPDATE_CONTINUE;
}

update_status ModuleGui::PostUpdate()
{
	
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

