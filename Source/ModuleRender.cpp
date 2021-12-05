#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleProgram.h"
#include "ModuleCamera.h"
#include "ModuleDebugDraw.h"

#include "SDL.h"
#include "glew.h"

#include "MathGeoLib.h"
#include "il.h"
#include "ilu.h"

ModuleRender::ModuleRender()
{
	fps_log = std::vector<float>(n_bins);
	ms_log = std::vector<float>(n_bins);
}

ModuleRender::~ModuleRender()
{
}

void __stdcall OurOpenGLErrorFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

bool ModuleRender::Init()
{
	CreateContext();

	RetrieveGpuInfo();	
	RetrieveLibVersions();

	SetGLOptions();
	EnableGLDebug();
	
	CreateModel("BakerHouse.fbx");

	return true;
}

void ModuleRender::CreateContext()
{
	LOG("Creating Renderer context");
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4); // desired version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // we want a double buffer
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); // we want to have a depth buffer with 24 bits
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8); // we want to have a stencil buffer with 8 bits
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG); // enable context debug

	context = SDL_GL_CreateContext(App->window->GetWindow());
	GLenum err = glewInit();

	int w, h;
	SDL_GetWindowSize(App->window->GetWindow(), &w, &h);
	glViewport(0, 0, w, h);

	clear_color = float4(0.0f, 0.0f, 0.0f, 1.0f);
}

void ModuleRender::SetGLOptions()
{
	glEnable(GL_DEPTH_TEST); // Enable depth test
	glEnable(GL_CULL_FACE); // Enable cull backward faces
	glFrontFace(GL_CCW); // Front faces will be counter clockwise
}

void ModuleRender::EnableGLDebug()
{
	glEnable(GL_DEBUG_OUTPUT); // Enable output callback
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(&OurOpenGLErrorFunction, nullptr); // Set the callback
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true); // Filter notifications
}

void ModuleRender::CreateModel(const char* path)
{
	model = new Model();
	model->Load(path);
	App->camera->FocusOBB(model->GetOBB());
}

update_status ModuleRender::PreUpdate(const float delta)
{
	glClearColor(clear_color[0], clear_color[1], clear_color[2], clear_color[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return UPDATE_CONTINUE;
}

update_status ModuleRender::Update(const float delta)
{
	SDL_Surface* screen_surface = App->window->GetScreenSurface();
	if(debug_draw)
		App->debug->Draw(App->camera->GetView(), App->camera->GetProjection(), screen_surface->w, screen_surface->h);

	// Note: Debug draw disables blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	model->Draw();

	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate(const float delta)
{	
	SDL_GL_SwapWindow(App->window->GetWindow());
	AddFrame(delta);
	
	return UPDATE_CONTINUE;
}

void ModuleRender::WindowResized(unsigned width, unsigned height)
{
	int w, h;
	SDL_GetWindowSize(App->window->GetWindow(), &w, &h);
	glViewport(0, 0, w, h);
}

void GLOptionCheck(GLenum option, bool enable) {
	if (enable)
		glEnable(option);
	else
		glDisable(option);
}

void ModuleRender::OptionsMenu()
{
	
	ImGuiColorEditFlags flag = ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoLabel;
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Draw Options");
	ImGui::Text("Background Color");
	ImGui::PushItemWidth(150.0f);
	ImGui::ColorPicker3("Clear Color", &clear_color[0], flag);
	ImGui::Checkbox("Debug Draw", &debug_draw);

	static bool line_smooth = false;
	if (ImGui::Checkbox("Line Smooth", &line_smooth))
		GLOptionCheck(GL_LINE_SMOOTH, line_smooth);

	ImGui::SameLine();
	static bool polygon_smooth = false;
	if (ImGui::Checkbox("Polygon Smooth", &polygon_smooth))
		GLOptionCheck(GL_POLYGON_SMOOTH, polygon_smooth);
}

void ModuleRender::PerformanceMenu(const float delta)
{
	glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX, &vram_free);
	float vram_free_mb = vram_free / 1024.0f;
	float vram_usage_mb = gpu.vram_budget_mb - vram_free_mb;
	ImGui::Text("VRAM Budget: %.1f Mb", gpu.vram_budget_mb);
	ImGui::Text("Vram Usage:  %.1f Mb", vram_usage_mb);
	ImGui::Text("Vram Avaliable:  %.1f Mb", vram_free_mb);
	ImGui::Separator();
	FpsGraph();
}

void ModuleRender::FpsGraph()
{
	ImGui::Text("Fps: %.1f", current_fps);

	char title[25];
	sprintf_s(title, 25, "Framerate %.1f", current_fps);
	ImGui::PlotHistogram("##framerate", &fps_log[0], (int)fps_log.size(), 0, title, 0.0f, 1000.f, ImVec2(310, 100));
	sprintf_s(title, 25, "Milliseconds %0.1f", current_ms);
	ImGui::PlotHistogram("##milliseconds", &ms_log[0], (int)ms_log.size(), 0, title, 0.0f, 20.0f, ImVec2(310, 100));
}

void ModuleRender::AddFrame(const float delta)
{
	static const float update_frequency_seconds = 0.5f;
	static int filled_bins = 0;
	static int frames = 0;
	static float time = 0;

	++frames;
	time += delta;

	if (time >= update_frequency_seconds)
	{
		if (filled_bins == n_bins) {
			for (int i = 0; i < n_bins - 1; ++i)
			{
				fps_log[i] = fps_log[i + 1];
				ms_log[i] = ms_log[i + 1];
			}
		}
		else {
			++filled_bins;
		}
		fps_log[filled_bins - 1] = float(frames)/time;
		current_fps = fps_log[filled_bins - 1];
		ms_log[filled_bins - 1] = time * 1000.0f / float(frames);
		current_ms = ms_log[filled_bins - 1];
		time = 0;
		frames = 0;
	}
}

void ModuleRender::RetrieveLibVersions()
{
	gl.glew = (unsigned char*)glewGetString(GLEW_VERSION);
	gl.opengl = (unsigned char*)glGetString(GL_VERSION);
	gl.glsl = (unsigned char*)glGetString(GL_SHADING_LANGUAGE_VERSION);

	LOG("Using Glew %s", gl.glew);
	LOG("OpenGL version supported %s", gl.opengl);
	LOG("GLSL: %s", gl.glsl);
}

void ModuleRender::RetrieveGpuInfo()
{
	gpu.name = (unsigned char*)glGetString(GL_RENDERER);
	gpu.brand = (unsigned char*)glGetString(GL_VENDOR);

	int vram_budget;
	glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX, &vram_budget);
	gpu.vram_budget_mb = (float)vram_budget / 1024.0f;
}

bool ModuleRender::CleanUp()
{
	LOG("Destroying renderer");	
	model->CleanUp();
	delete model;
	SDL_GL_DeleteContext(context);

	return true;
}

void __stdcall OurOpenGLErrorFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	const char* tmp_source = "", * tmp_type = "", * tmp_severity = "";
	switch (source) {
	case GL_DEBUG_SOURCE_API: tmp_source = "API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM: tmp_source = "Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: tmp_source = "Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY: tmp_source = "Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION: tmp_source = "Application"; break;
	case GL_DEBUG_SOURCE_OTHER: tmp_source = "Other"; break;
	};
	switch (type) {
	case GL_DEBUG_TYPE_ERROR: tmp_type = "Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: tmp_type = "Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: tmp_type = "Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY: tmp_type = "Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE: tmp_type = "Performance"; break;
	case GL_DEBUG_TYPE_MARKER: tmp_type = "Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP: tmp_type = "Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP: tmp_type = "Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER: tmp_type = "Other"; break;
	};
	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH: tmp_severity = "high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM: tmp_severity = "medium"; break;
	case GL_DEBUG_SEVERITY_LOW: tmp_severity = "low"; break;
		// case GL_DEBUG_SEVERITY_NOTIFICATION: tmp_severity = "notification"; break;
	default: return;
	};
	LOG("<Source:%s> <Type:%s> <Severity:%s> <ID:%d> <Message:%s>\n", tmp_source, tmp_type, tmp_severity, id, message);
}