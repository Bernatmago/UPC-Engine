#include "ModuleInput.h"

#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"

#include "imgui_impl_sdl.h"

#include <string>

ModuleInput::ModuleInput()
{}

// Destructor
ModuleInput::~ModuleInput()
{}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

update_status ModuleInput::PreUpdate(const float delta)
{
    SDL_Event event;
    // TODO: Improve delta management
    mouse_delta_x = 0;
    mouse_delta_y = 0;
    scroll_delta = 0;
    while (SDL_PollEvent(&event) != 0)
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
        switch (event.type)
        {
        case SDL_QUIT:
            return UPDATE_STOP;
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                App->window->WindowResized();
            break;
        case SDL_MOUSEMOTION:
            mouse_delta_x = event.motion.xrel;
            mouse_delta_y = event.motion.yrel;
            break;
        case SDL_MOUSEWHEEL:
            scroll_delta = event.wheel.y;
            break;
        case SDL_DROPFILE:
            // TODO: should be freed with SDL_free()
            App->renderer->model->Load(event.drop.file);
            break;
        }
    }
    keyboard = SDL_GetKeyboardState(NULL);
    keymods = SDL_GetModState();
    mouse = SDL_GetMouseState(&mouse_x, &mouse_y);

    return UPDATE_CONTINUE;
}

bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

const unsigned ModuleInput::GetKey(SDL_Scancode key) const
{
    return keyboard[key];
}

const bool ModuleInput::GetKeyMod(SDL_Keymod modifier) const
{
    return (keymods & modifier);
}

const bool ModuleInput::GetMouseButton(int button) const
{
    return (mouse & SDL_BUTTON(button));
}

const void ModuleInput::GetMouseDelta(int& x, int& y) const
{
    x = mouse_delta_x;
    y = mouse_delta_y;
}

int ModuleInput::GetScrollDelta() const
{
    return scroll_delta;
}
