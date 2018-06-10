//////////////////////////////////////////////////////////////////////////////
// This file is part of the LibreMaple MMORPG client                        //
// Copyright © 2015-2016 Daniel Allendorf, 2018-2019 LibreMaple Team        //
//                                                                          //
// This program is free software: you can redistribute it and/or modify     //
// it under the terms of the GNU Affero General Public License as           //
// published by the Free Software Foundation, either version 3 of the       //
// License, or (at your option) any later version.                          //
//                                                                          //
// This program is distributed in the hope that it will be useful,          //
// but WITHOUT ANY WARRANTY; without even the implied warranty of           //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
// GNU Affero General Public License for more details.                      //
//                                                                          //
// You should have received a copy of the GNU Affero General Public License //
// along with this program.  If not, see <http://www.gnu.org/licenses/>.    //
//////////////////////////////////////////////////////////////////////////////
#include "Window.h"

#include "../Configuration.h"
#include "../Console.h"
#include "../Constants.h"
#include "../Graphics/GraphicsGL.h"
#include "../Util/Misc.h"
#include "UI.h"

#include <string_view>

namespace jrc
{
Window::Window()
{
    context = nullptr;
    glwnd = nullptr;
    opacity = 1.0f;
    opcstep = 0.0f;
    width = Constants::VIEW_WIDTH;
    height = Constants::VIEW_HEIGHT;
}

Window::~Window()
{
    glfwTerminate();
}

void error_callback(int no, const char* description)
{
    Console::get().print(str::concat("GLFW error: ",
                                     std::string_view{description},
                                     " (",
                                     std::to_string(no),
                                     ')'));
}

void key_callback(GLFWwindow*, int key, int, int action, int)
{
    UI::get().send_key(key, action != GLFW_RELEASE);
}

void mousekey_callback(GLFWwindow*, int button, int action, int)
{
    switch (button) {
    case GLFW_MOUSE_BUTTON_LEFT:
        switch (action) {
        case GLFW_PRESS:
            UI::get().send_cursor(true);
            break;
        case GLFW_RELEASE:
            UI::get().send_cursor(false);
            break;
        default:
            break;
        }
        break;
    case GLFW_MOUSE_BUTTON_RIGHT:
        switch (action) {
        case GLFW_PRESS:
            UI::get().doubleclick();
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

void cursor_callback(GLFWwindow*, double xpos, double ypos)
{
    UI::get().send_cursor(
        {static_cast<std::int16_t>(xpos), static_cast<std::int16_t>(ypos)});
}

Error Window::init()
{
    full_screen = Setting<Fullscreen>::get().load();

    if (!glfwInit()) {
        return Error::GLFW;
    }

    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
    context = glfwCreateWindow(1, 1, "", nullptr, nullptr);
    glfwMakeContextCurrent(context);
    glfwSetErrorCallback(error_callback);
    glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    if (Error error = GraphicsGL::get().init()) {
        return error;
    }

    return init_window();
}

Error Window::init_window()
{
    if (glwnd) {
        glfwDestroyWindow(glwnd);
    }

    glwnd = glfwCreateWindow(Constants::VIEW_WIDTH,
                             Constants::VIEW_HEIGHT,
                             "LibreMaple",
                             full_screen ? glfwGetPrimaryMonitor() : nullptr,
                             context);

    if (!glwnd) {
        return Error::WINDOW;
    }

    glfwMakeContextCurrent(glwnd);

    const bool vsync = Setting<VSync>::get().load();
    glfwSwapInterval(vsync ? 1 : 0);

    glViewport(0, 0, Constants::VIEW_WIDTH, Constants::VIEW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glfwSetInputMode(glwnd, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glfwSetInputMode(glwnd, GLFW_STICKY_KEYS, 1);
    glfwSetKeyCallback(glwnd, key_callback);
    glfwSetMouseButtonCallback(glwnd, mousekey_callback);
    glfwSetCursorPosCallback(glwnd, cursor_callback);

    GraphicsGL::get().reinit();

    return Error::NONE;
}

bool Window::not_closed() const
{
    return glfwWindowShouldClose(glwnd) == 0;
}

void Window::update()
{
    update_opc();
}

void Window::update_opc()
{
    if (opcstep != 0.0f) {
        opacity += opcstep;

        if (opacity >= 1.0f) {
            opacity = 1.0f;
            opcstep = 0.0f;
        } else if (opacity <= 0.0f) {
            opacity = 0.0f;
            opcstep = -opcstep;

            fade_procedure();
        }
    }
}

void Window::check_events()
{
    std::int32_t tabstate = glfwGetKey(glwnd, GLFW_KEY_F11);
    if (tabstate == GLFW_PRESS) {
        full_screen = !full_screen;
        init_window();
    }
    glfwPollEvents();
}

void Window::begin() const
{
    GraphicsGL::get().clearscene();
}

void Window::end() const
{
    GraphicsGL::get().flush(opacity);
    glfwSwapBuffers(glwnd);
}

void Window::fadeout(float step, std::function<void()> fade_proc)
{
    opcstep = -step;
    fade_procedure = std::move(fade_proc);
}

void Window::set_clipboard(const char* text) const
{
    glfwSetClipboardString(glwnd, text);
}
void Window::set_clipboard(const std::string& text) const
{
    glfwSetClipboardString(glwnd, text.c_str());
}

std::string Window::get_clipboard() const
{
    const char* text = glfwGetClipboardString(glwnd);
    return text ? text : "";
}

void Window::resize(bool in_game) noexcept
{
    if (in_game) {
        width = Constants::GAME_VIEW_WIDTH;
        height = Constants::GAME_VIEW_HEIGHT;
    } else {
        width = Constants::VIEW_WIDTH;
        height = Constants::VIEW_HEIGHT;
    }

    glfwSetWindowSize(glwnd, width, height);
    glViewport(0, 0, width, height);
    GraphicsGL::set_screen(0,
                           width,
                           -Constants::VIEW_Y_OFFSET,
                           -Constants::VIEW_Y_OFFSET + height);

    GraphicsGL::get().reinit();
}

std::int16_t Window::get_width() const noexcept
{
    return width;
}

std::int16_t Window::get_height() const noexcept
{
    return height;
}
} // namespace jrc
