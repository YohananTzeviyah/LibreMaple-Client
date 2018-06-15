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
#pragma once
#include "../Journey.h"
#ifdef JOURNEY_USE_ASIO
#    include "NetConstants.h"

#    define BOOST_DATE_TIME_NO_LIB
#    define BOOST_REGEX_NO_LIB
#    include "asio.hpp"

namespace jrc
{
#    ifndef JOURNEY_USE_CRYPTO
constexpr std::size_t HANDSHAKE_LEN = 2;
#    else
constexpr std::size_t HANDSHAKE_LEN = 16;
#    endif

using asio::error_code;
using asio::io_service;
using asio::ip::tcp;

//! Class that wraps an ASIO socket.
class SocketAsio
{
public:
    SocketAsio();
    ~SocketAsio();

    bool open(const char* address, const char* port);
    bool close() noexcept;
    std::size_t receive(bool* connected);
    const std::int8_t* get_buffer() const;
    bool dispatch(const std::int8_t* bytes, std::size_t length) noexcept;

private:
    io_service ioservice;
    tcp::resolver resolver;
    tcp::socket socket;
    std::int8_t buffer[MAX_PACKET_LENGTH];
};
} // namespace jrc
#endif
