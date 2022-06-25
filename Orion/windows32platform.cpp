#include <windows.h>
#include <chrono>
#include <cstdio>
#include <iostream>

#include "constants.h"
#include "Game.h"
#include "InputHandler.h"

bool running = true;

void* buffer_memory;
int buffer_width = constants::width;
int buffer_height = constants::height;

BITMAPINFO buffer_bitmap_info;

LRESULT CALLBACK window_callback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	// Process the messages in the queue:
	switch (uMsg) 
	{
		case WM_CLOSE:
		case WM_DESTROY:
		{
			running = false;
		} break;

		case WM_SIZE: 
		{
			// In case the user resizes the window (we're porbably going to not allow this!)
			// We need to make sure that the new buffer_memory has enough space to handle the new pixels
			RECT rect;
			GetClientRect(hWnd, &rect);
			buffer_width = (rect.right - rect.left);
			buffer_height = (rect.bottom - rect.top);

			int buffer_size = (buffer_width * buffer_height * sizeof(unsigned int));

			if (buffer_memory) VirtualFree(buffer_memory, 0, MEM_RELEASE);
			buffer_memory = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

			buffer_bitmap_info.bmiHeader.biSize = sizeof(buffer_bitmap_info);
			buffer_bitmap_info.bmiHeader.biWidth = buffer_width;
			buffer_bitmap_info.bmiHeader.biHeight = buffer_height;
			buffer_bitmap_info.bmiHeader.biPlanes = 1;
			buffer_bitmap_info.bmiHeader.biBitCount = 32;
			buffer_bitmap_info.bmiHeader.biCompression = BI_RGB;

		} break;

		default:
		{
			result = DefWindowProc(hWnd, uMsg, wParam, lParam);
		} break;


	}
	return result;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	// TO SEE STATUS OUTPUT IN CONSOLE!
	AllocConsole();
	FILE* fpstdin = stdin, * fpstdout = stdout, * fpstderr = stderr;

	freopen_s(&fpstdin, "CONIN$", "r", stdin);
	freopen_s(&fpstdout, "CONOUT$", "w", stdout);
	freopen_s(&fpstderr, "CONOUT$", "w", stderr);

	// Create a Window Class
	WNDCLASS window_class = {};
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpszClassName = L"Orion Window Class";
	window_class.lpfnWndProc = window_callback;

	// Register the previously created Window Class with the OS
	RegisterClass(&window_class);

	// Calling the CreateWindow function 
	HWND hWnd = CreateWindow(window_class.lpszClassName, L"Orion Engine", (WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME) | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, constants::width, constants::height, 0, 0, hInstance, 0);
	HDC hdc = GetDC(hWnd);

	auto display_last_time = std::chrono::steady_clock::now();

	auto last_time = std::chrono::high_resolution_clock::now();
	double ns = 1000000000 / 60.0;
	double delta = 0.0;

	unsigned int color = 0;

	int ticks_per_second = 0;
	int frames_per_second = 0;

	//std::cout << "buffer_width: " << buffer_width << '\n';
	//std::cout << "buffer_height: " << buffer_height << '\n';

	Game* game = new Game(buffer_width, buffer_height);
	Input game_input = {};

	while (running)
	{
		for (int i = 0; i < KEY::KEY_COUNT; ++i)
		{
			game_input.key_input[i].is_changed = false;
		}

		// Gather Input
		MSG message;
		while (PeekMessage(&message, hWnd, 0, 0, PM_REMOVE)) {

			switch (message.message)
			{
				case WM_KEYUP:
				case WM_KEYDOWN:
				{
					unsigned int vk_code = static_cast<unsigned int>(message.wParam);
					bool is_down = ((message.lParam & (1 << 31)) == 0);

					switch (vk_code)
					{
						case 0x57:	// W KEY
						{
							game_input.key_input[KEY::KEY_W].is_down = is_down;
							game_input.key_input[KEY::KEY_W].is_changed = true;
						} break;

						case 0x41:	// A KEY
						{
							game_input.key_input[KEY::KEY_A].is_down = is_down;
							game_input.key_input[KEY::KEY_A].is_changed = true;
						} break;

						case 0x53:	// S KEY
						{
							game_input.key_input[KEY::KEY_S].is_down = is_down;
							game_input.key_input[KEY::KEY_S].is_changed = true;
						} break;

						case 0x44:	// D KEY
						{
							game_input.key_input[KEY::KEY_D].is_down = is_down;
							game_input.key_input[KEY::KEY_D].is_changed = true;
						} break;

						case VK_LEFT:
						{
							game_input.key_input[KEY::KEY_LEFT].is_down = is_down;
							game_input.key_input[KEY::KEY_LEFT].is_changed = true;
						} break;

						case VK_RIGHT:
						{
							game_input.key_input[KEY::KEY_RIGHT].is_down = is_down;
							game_input.key_input[KEY::KEY_RIGHT].is_changed = true;
						} break;

						case VK_SPACE:
						{
							game_input.key_input[KEY::KEY_SPACE].is_down = is_down;
							game_input.key_input[KEY::KEY_SPACE].is_changed = true;
						} break;
							
					}
				} break;
					
			}
			TranslateMessage(&message);
			DispatchMessage(&message);

		}

		auto current_time = std::chrono::high_resolution_clock::now();
		auto display_current_time = std::chrono::steady_clock::now();

		if (((std::chrono::duration_cast<std::chrono::nanoseconds>(current_time - last_time).count()) / ns) > 1.0) {
			last_time = current_time;
			color += 0x000001;
			game->tick(&game_input);
			++ticks_per_second;
		}

		if (std::chrono::duration_cast<std::chrono::milliseconds>(display_current_time - display_last_time).count() > 1000)
		{
			display_last_time = display_current_time;
			std::cout << "Ticks: " << ticks_per_second << " | Frames: " << frames_per_second << '\n';
			ticks_per_second = 0;
			frames_per_second = 0;
		}

		// Call the render() function here
		game->render(buffer_memory, buffer_width, buffer_height);
		
		// render the minimap (using the console; only for debugging)
		//game->render_console();

		// Render the window
		++frames_per_second;

		// Telling Windows to render according to the pixel data in buffer_memory
		StretchDIBits(hdc, 0, 0, buffer_width, buffer_height, 0, 0, buffer_width, buffer_height, buffer_memory, &buffer_bitmap_info, DIB_RGB_COLORS, SRCCOPY);
	}
}