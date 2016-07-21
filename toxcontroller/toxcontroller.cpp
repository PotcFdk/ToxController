/*
	Copyright PotcFdk, 2016

    This file is part of ToxController.

    ToxController is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ToxController is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ToxController.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "toxcontroller.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstring>

#ifdef WIN32
    #define PLATFORM_WINDOWS

    #include <windows.h>
#else
    #define PLATFORM_NIX
#endif


uint8_t char2int (char input)
{
  if (input >= '0' && input <= '9')
    return input - '0';
  if (input >= 'A' && input <= 'F')
    return input - 'A' + 10;
  if (input >= 'a' && input <= 'f')
    return input - 'a' + 10;
  throw std::invalid_argument ("Invalid input string");
}


void hex_string_to_bin (const char *hex_string, uint8_t *out)
{
    size_t i, len = strlen (hex_string) / 2;

    for (i = 0; i < len; ++i)
        out[i] = char2int (hex_string[i*2]) << 4 | char2int (hex_string[i*2+1]);
}

ToxController::ToxController (std::string bootstrap_address, uint16_t bootstrap_port, std::string bootstrap_key, std::string nickname, std::string status)
{
    uint8_t *bootstrap_pub_key = new uint8_t[TOX_PUBLIC_KEY_SIZE];
    hex_string_to_bin (bootstrap_key.c_str(), bootstrap_pub_key);
    tox = std::unique_ptr<Tox, decltype(&tox_kill)> (tox_new (NULL, NULL), tox_kill);

    tox_self_set_name (tox.get(), (uint8_t *) nickname.c_str(), nickname.length(), NULL);
    tox_self_set_status_message (tox.get(), (uint8_t *) status.c_str(), status.length(), NULL);

    tox_self_set_status (tox.get(), TOX_USER_STATUS_NONE);

    tox_bootstrap (tox.get(), bootstrap_address.c_str(), bootstrap_port, bootstrap_pub_key, NULL);
}

void ToxController::setupCallbacks()
{
    // The following basically does this, but in a way that actually works.
    // Improve this, if possible.
    //  tox_callback_friend_request (tox.get(), (tox_friend_request_cb*) &evtOnRequest, NULL);
    //  tox_callback_friend_message (tox.get(), (tox_friend_message_cb*) &evtOnMessage, NULL);

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wpmf-conversions"
    void(ToxController::*_evtOnRequest)(Tox*, const uint8_t*, const uint8_t*, size_t , void*) = &ToxController::evtOnRequest;
    tox_callback_friend_request (tox.get(), (tox_friend_request_cb*) _evtOnRequest, NULL);

    void(ToxController::*_evtOnMessage)(Tox*, uint32_t, TOX_MESSAGE_TYPE, const uint8_t*, size_t, void*) = &ToxController::evtOnMessage;
    tox_callback_friend_message (tox.get(), (tox_friend_message_cb*) _evtOnMessage, NULL);
    #pragma GCC diagnostic pop
}

void ToxController::think()
{
    tox_iterate (tox.get());

    #ifdef PLATFORM_WINDOWS
    Sleep (tox_iteration_interval (tox));
    #elifdef PLATFORM_NIX
    usleep (tox_iteration_interval (tox) * 1000);
    #endif
}

std::string ToxController::convPublicKeyToHexString (const uint8_t *public_key)
{
    std::stringstream ss;
    ss << std::setfill('0');
    for (int i = 0; i < TOX_PUBLIC_KEY_SIZE; ++i)
        ss << std::hex << std::uppercase << std::setw(2) << static_cast<unsigned short>(public_key[i]);
    return ss.str();
}

std::string ToxController::convAddressToHexString (const uint8_t *address)
{
    std::stringstream ss;
    ss << std::setfill('0');
    for (size_t i = 0; i < TOX_ADDRESS_SIZE; ++i)
        ss << std::hex << std::uppercase << std::setw(2) << static_cast<unsigned short>(address[i]);
    return ss.str();
}

uint8_t *ToxController::convPublicKeyToAddress (const uint8_t *public_key)
{
    uint8_t *address = new uint8_t[TOX_ADDRESS_SIZE];
    std::memcpy (address, public_key, TOX_PUBLIC_KEY_SIZE);
    address [TOX_PUBLIC_KEY_SIZE+5] = 0;
    address [TOX_PUBLIC_KEY_SIZE+6] = 0;
    for (int i = 0; i < TOX_PUBLIC_KEY_SIZE+4; ++i)
    {
        if (i >= TOX_PUBLIC_KEY_SIZE)
            address[i] = 0;
        if (!(i % 2))
            address [TOX_PUBLIC_KEY_SIZE+5] ^= address[i];
        else
            address [TOX_PUBLIC_KEY_SIZE+6] ^= address[i];
    }
    return address;
}

std::string ToxController::getAddress()
{
    uint8_t *address = new uint8_t[TOX_ADDRESS_SIZE];
	tox_self_get_address (tox.get(), address);
	std::string out = convAddressToHexString (address);
	delete[] address;
	return out;
}

