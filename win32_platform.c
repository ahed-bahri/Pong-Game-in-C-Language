#include <windows.h>

#include "platform_common.c"
#include "game.c"

global_variable b32 running;

//Window and Render

struct {

    BITMAPINFO info;
    void      *memory;
    int        width;
    int        height;
    int        pitch;
    int        bytes_per_pixel;

} typedef Win32_Offscreen_Buffer;

global_variable Win32_Offscreen_Buffer global_backbuffer;
global_variable WINDOWPLACEMENT window_position = {sizeof (window_position)};

#define WINDOW_FLAGS (WS_OVERLAPPEDWINDOW|WS_VISIBLE)

internal void
toggle_fullscreen(HWND window) {


    DWORD style = GetWindowLong(window, GWL_STYLE);
    if (style & WS_OVERLAPPEDWINDOW) {
        MONITORINFO mi = { sizeof(mi) };
        if (GetWindowPlacement(window, &window_position) &&
            GetMonitorInfo(MonitorFromWindow(window,
                                             MONITOR_DEFAULTTOPRIMARY), &mi)) {
            SetWindowLong(window, GWL_STYLE,
                          style & ~WS_OVERLAPPEDWINDOW);
            SetWindowPos(window, HWND_TOP,
                         mi.rcMonitor.left, mi.rcMonitor.top,
                         mi.rcMonitor.right - mi.rcMonitor.left,
                         mi.rcMonitor.bottom - mi.rcMonitor.top,
                         SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        }
    } else {
        SetWindowLong(window, GWL_STYLE, WINDOW_FLAGS);
        SetWindowPlacement(window, &window_position);
        SetWindowPos(window, NULL, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
                     SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
    }
}


struct {
    int width;
    int height;
} typedef Win32_Window_Dimention;


internal Win32_Window_Dimention
win32_get_window_dimention(HWND window) {

    RECT client_rect;
    GetClientRect(window, &client_rect);
    Win32_Window_Dimention result;
    result.width  = client_rect.right - client_rect.left;
    result.height = client_rect.bottom - client_rect.top;

    return result;
}


internal void
win32_resize_dib(Win32_Offscreen_Buffer *buffer, int width, int height) {

    if (buffer->memory) {

        VirtualFree(buffer->memory, 0, MEM_RELEASE);
    }

    buffer->width  = width;
    buffer->height = height;
    buffer->bytes_per_pixel = 4;
    buffer->pitch = buffer->bytes_per_pixel * width;

    buffer->info.bmiHeader.biSize = sizeof(buffer->info.bmiHeader);
    buffer->info.bmiHeader.biWidth = buffer->width;
    buffer->info.bmiHeader.biHeight = buffer->height;
    buffer->info.bmiHeader.biPlanes = 1;
    buffer->info.bmiHeader.biBitCount = 32;
    buffer->info.bmiHeader.biCompression = BI_RGB;

    u32 size = (buffer->pitch * buffer->height);

    buffer->memory = VirtualAlloc(0, size, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);

}

internal void
win32_draw_buffer_to_window(Win32_Offscreen_Buffer *buffer, HDC device_context,
                            Win32_Window_Dimention window_dimention) {

    int offset_x = 0;
    int offset_y = 0;

    StretchDIBits(device_context,
                  offset_x, offset_y, buffer->width, buffer->height,
                  0, 0, buffer->width, buffer->height,
                  buffer->memory, &buffer->info, DIB_RGB_COLORS, SRCCOPY);
}

LRESULT CALLBACK main_window_callback(
HWND   window,
UINT   message,
WPARAM w_param,
LPARAM l_param) {

    LRESULT result = 0;

    switch(message) {
        case WM_SIZE: {

            Win32_Window_Dimention dim = win32_get_window_dimention(window);
            win32_resize_dib(&global_backbuffer, dim.width, dim.height);

        } break;

        case WM_DESTROY: {
            running = 0;
        } break;

        case WM_CLOSE: {
            running = 0;
        } break;

        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYDOWN:
        case WM_KEYUP: {

            assert(!"Keyboard input came from a non-dispatch message!");

        } break;

        case WM_PAINT: {

            PAINTSTRUCT paint_struct;
            HDC device_context = BeginPaint(window, &paint_struct);

            win32_draw_buffer_to_window(&global_backbuffer, device_context, win32_get_window_dimention(window));

            EndPaint(window, &paint_struct);

        } break;

        default: {
            result = DefWindowProcA(window, message, w_param, l_param);
        }
    }

    return result;
}




//Input

internal void
win32_process_keyboard_message(Game_Button_State *new_state, b32 is_down) {

    if (new_state->ended_down != is_down) {

        new_state->ended_down = is_down;
        new_state->half_transition_count += 1;
    }
}



internal void
win32_process_keyboard_input (Game_Input *keyboard_controller) {

    MSG message;

    while (PeekMessage(&message, 0, 0, 0, PM_REMOVE)) {

        switch(message.message) {
            case WM_QUIT: {

                running = 0;
            } break;

            case WM_SYSKEYDOWN:
            case WM_SYSKEYUP:
            case WM_KEYDOWN:
            case WM_KEYUP: {

                u32 vk_code = (u32)message.wParam;
                b32 was_down = ((message.lParam & (1 << 30)) != 0);
                b32 is_down  = ((message.lParam & (1 << 31)) == 0);

                if (is_down == was_down) break;

                if (vk_code == 'W') {
                    win32_process_keyboard_message(&keyboard_controller->buttons[B_MOVE_UP], is_down);
                } else if (vk_code == 'S') {
                    win32_process_keyboard_message(&keyboard_controller->buttons[B_MOVE_DOWN], is_down);
                } else if (vk_code == VK_SHIFT) {
                    win32_process_keyboard_message(&keyboard_controller->buttons[B_SPEED_UP], is_down);
                } else if (vk_code == VK_ESCAPE) {
                    running = 0;
                } else if (vk_code == VK_F4) {
                    if (message.lParam & (1 << 29)) running = 0;

                } else if (vk_code == VK_RETURN &&(message.lParam & (1 << 29))) {
                    if (message.hwnd && is_down) {
                        toggle_fullscreen(message.hwnd);
                    }

                }
            } break;

            default: {

                TranslateMessage(&message);
                DispatchMessage(&message);
            }
        }
    }
}


//Time

global_variable u64 perf_counter_frequency;

inline u64
win32_get_wall_clock() {

    LARGE_INTEGER result;
    QueryPerformanceCounter(&result);
    return result.QuadPart;
}

inline f32
win32_get_seconds_elapsed (u64 start, u64 end) {

    return (f32)(end - start) / (f32)perf_counter_frequency;
}



int CALLBACK WinMain(
HINSTANCE instance,
HINSTANCE prev_nstance,
LPSTR     command_line,
int       command_show) {

    {
        LARGE_INTEGER perf_counter_frequency_result;
        QueryPerformanceFrequency(&perf_counter_frequency_result);
        perf_counter_frequency = perf_counter_frequency_result.QuadPart;
    }

    WNDCLASS window_class;

    window_class.cbClsExtra = 0;
    window_class.cbWndExtra = 0;
    window_class.hbrBackground = 0;
    window_class.lpszMenuName = 0;

    window_class.style = CS_HREDRAW|CS_VREDRAW;
    window_class.lpfnWndProc = main_window_callback;
    window_class.hInstance = instance;
    window_class.hCursor   = 0;
    SetCursor(0);
    window_class.hIcon = 0;
    window_class.lpszClassName = "Pong Window Class";

    RegisterClass(&window_class);

    HWND window;
#if DEVELOPMENT
    window = CreateWindowEx(0, window_class.lpszClassName, "Pong in C", WINDOW_FLAGS, 1610, 910, 950, 534, 0, 0, instance, 0);
#else
    window = CreateWindowEx(0, window_class.lpszClassName, "Pong in C", WINDOW_FLAGS, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, instance, 0);
#endif
    if (window) {

        toggle_fullscreen(window);
        running = 1;

        Game_Input input;
        zero_struct(input);

        input.last_dt = 0.0166f;
        u64 last_counter = win32_get_wall_clock();

        while (running) {

            win32_process_keyboard_input(&input);

            {
                Game_Offscreen_Buffer game_buffer;
                game_buffer.memory = global_backbuffer.memory;
                game_buffer.width  = global_backbuffer.width;
                game_buffer.height = global_backbuffer.height;
                game_buffer.pitch  = global_backbuffer.pitch;

                game_update_and_render(&input, &game_buffer);
            }

            HDC device_context = GetDC(window);
            win32_draw_buffer_to_window(&global_backbuffer, device_context, win32_get_window_dimention(window));
            ReleaseDC(window, device_context);

            u64 end_counter = win32_get_wall_clock();
            input.last_dt = win32_get_seconds_elapsed(last_counter, end_counter);
            last_counter = end_counter;
        }

    }

}
