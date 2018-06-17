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
#include "Audio/Audio.h"
#include "Character/Char.h"
#include "Configuration.h"
#include "Constants.h"
#include "Error.h"
#include "Gameplay/Combat/DamageNumber.h"
#include "Gameplay/Stage.h"
#include "IO/UI.h"
#include "IO/Window.h"
#include "Net/Session.h"
#include "Timer.h"
#include "Util/NxFiles.h"

#include <iostream>

namespace jrc
{
Error init()
{
    if (Error error = Session::get().init()) {
        return error;
    }

    if (Error error = NxFiles::init()) {
        return error;
    }

    if (Error error = Window::get().init()) {
        return error;
    }

    if (Error error = Sound::init()) {
        return error;
    }

    Music::init();
    Char::init();
    DamageNumber::init();
    MapPortals::init();
    Stage::get().init();
    UI::get().init();

    return Error::NONE;
}

void update()
{
    Window::get().check_events();
    Window::get().update();
    Stage::get().update();
    UI::get().update();
    Session::get().read();
}

void draw(float alpha)
{
    Window::get().begin();
    Stage::get().draw(alpha);
    UI::get().draw(alpha);
    Window::get().end();
}

bool running()
{
    return Session::get().is_connected() && UI::get().not_quitted()
           && Window::get().not_closed();
}

void loop()
{
    Timer::get().start();
    std::int64_t timestep = Constants::TIMESTEP * 1'000;
    std::int64_t accumulator = timestep;

    std::int64_t period = 0;
    std::int32_t samples = 0;

    while (running()) {
        std::int64_t elapsed = Timer::get().stop();

        // Update game with constant timestep as many times as possible.
        for (accumulator += elapsed; accumulator >= timestep;
             accumulator -= timestep) {
            update();
        }

        // Draw the game. Interpolate to account for remaining time.
        float alpha = static_cast<float>(accumulator) / timestep;
        draw(alpha);

        if (samples < 100) {
            period += elapsed;
            ++samples;
        } else if (period) {
            // std::int64_t fps = (samples * 1000000) / period;
            // std::cout << "FPS: " << fps << '\n';

            period = 0;
            samples = 0;
        }
    }

    Sound::close();
}

void start()
{
    // Initialize and check for errors.
    if (Error error = init()) {
        const char* message = error.get_message();
        const char* args = error.get_args();
        const bool can_retry = error.can_retry();

        std::cerr << "Error: " << message << args << '\n';

        std::string command;
        std::cin >> command;

        if (can_retry && command == "retry") {
            start();
        }
    } else {
        loop();
    }
}
} // namespace jrc

int main()
{
    jrc::start();
    return 0;
}
