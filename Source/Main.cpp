#include <stdlib.h>
#include "Application.h"
#include "Globals.h"

#include "SDL.h"
#pragma comment( lib, "SDL2.lib" )
#pragma comment( lib, "SDL2main.lib" )

#include "Console.h"

// Memory leak stuff

#define DEBUG_NEW new( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DEBUG_NEW
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

enum main_states
{
	MAIN_CREATION,  
	MAIN_START,
	MAIN_UPDATE,
	MAIN_FINISH,
	MAIN_EXIT
};

void DumpLeaks(void)
{
	_CrtDumpMemoryLeaks(); // Show leaks with file and line where allocation was made
}

Application* App = NULL;
Console* Logger = NULL;

int main(int argc, char ** argv)
{
	atexit(DumpLeaks);
	Logger = new Console();

	int main_return = EXIT_FAILURE;
	main_states state = MAIN_CREATION;

	while (state != MAIN_EXIT)
	{
		switch (state)
		{
		case MAIN_CREATION:

			LOG("Application Creation --------------");
			App = new Application();
			state = MAIN_START;
			break;

		case MAIN_START:

			LOG("Application Init --------------");
			if (App->Init() == false)
			{
				LOG("Application Init exits with error -----");
				state = MAIN_EXIT;
			}
			else
			{
				state = MAIN_UPDATE;
				LOG("Application Update --------------");
			}

			break;

		case MAIN_UPDATE:
		{
			int update_return = App->Update();

			if (update_return == UPDATE_ERROR)
			{
				LOG("Application Update exits with error -----");
				state = MAIN_EXIT;
			}

			if (update_return == UPDATE_STOP)
				state = MAIN_FINISH;
		}
		break;

		case MAIN_FINISH:

			LOG("Application CleanUp --------------");
			if (App->CleanUp() == false)
			{
				LOG("Application CleanUp exits with error -----");
			}
			else
				main_return = EXIT_SUCCESS;

			state = MAIN_EXIT;

			break;

		}

	}
	delete App;
	LOG("Bye :)\n");
	//delete console;
	return main_return;
}
