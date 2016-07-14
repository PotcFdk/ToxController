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

#include <tox/tox.h>

/* // Can be useful depending on the style of the deriving class.
template <typename T> T initialize_controller (std::string bootstrap_address, uint16_t bootstrap_port, std::string bootstrap_key)
{
  T o (bootstrap_address, bootstrap_port, bootstrap_key);
  o.setupCallbacks ();
  return o;
}
*/

class ToxController
{
public:
    ToxController (std::string bootstrap_address,
                   uint16_t bootstrap_port,
                   std::string bootstrap_key,
                   std::string nickname,
                   std::string status);
    ~ToxController();

    void think();
    std::string getAddress();

protected:
    void setupCallbacks();
    uint8_t *convPublicKeyToAddress (const uint8_t *public_key);
    std::string convPublicKeyToHexString (const uint8_t *address);
    std::string convAddressToHexString (const uint8_t *address);

private:
    Tox *tox;
    uint8_t *bootstrap_pub_key;
    uint8_t *address;

    virtual void evtOnRequest (Tox *tox, const uint8_t *public_key, const uint8_t *message, size_t length, void *user_data) = 0;
    virtual void evtOnMessage (Tox *tox, uint32_t friend_number, TOX_MESSAGE_TYPE type, const uint8_t *message, size_t length, void *user_data) = 0;
};
