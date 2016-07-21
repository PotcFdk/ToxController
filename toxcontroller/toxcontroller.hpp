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

#include <string>
#include <cstdint>
#include <memory>

#include <tox/tox.h>

/* // Can be useful depending on the style of the deriving class.
template <typename T> T initialize_controller (std::string bootstrap_address, uint16_t bootstrap_port, std::string bootstrap_key)
{
  T o (bootstrap_address, bootstrap_port, bootstrap_key);
  o.setupCallbacks ();
  return o;
}
*/

typedef uint32_t message_id;
typedef uint32_t friend_number;

class ToxController
{
public:
    ToxController (std::string bootstrap_address,
                   uint16_t bootstrap_port,
                   std::string bootstrap_key,
                   std::string nickname,
                   std::string status);

    void think();
    std::string getAddress();

    message_id sendMessage (friend_number, std::string message, TOX_MESSAGE_TYPE type = TOX_MESSAGE_TYPE_NORMAL);
    friend_number addFriend (const uint8_t *address, std::string message);
    friend_number addFriendNoRequest (const uint8_t *public_key);

protected:
    uint8_t *convPublicKeyToAddress (const uint8_t *public_key);
    std::string convPublicKeyToHexString (const uint8_t *address);
    std::string convAddressToHexString (const uint8_t *address);

private:
    std::unique_ptr<Tox, std::function<decltype(tox_kill)>> tox;
    uint8_t *bootstrap_pub_key;
    uint8_t *address;

    static void __ctxCallEventOnRequest (Tox *tox, const uint8_t *public_key, const uint8_t *message, size_t length, void *user_data);
    static void __ctxCallEventOnMessage (Tox *tox, uint32_t friend_number, TOX_MESSAGE_TYPE type, const uint8_t *message, size_t length, void *user_data);

    virtual void onRequest (Tox *tox, const uint8_t *public_key, const uint8_t *message, size_t length, void *user_data) = 0;
    virtual void onMessage (Tox *tox, uint32_t friend_number, TOX_MESSAGE_TYPE type, const uint8_t *message, size_t length, void *user_data) = 0;
};
