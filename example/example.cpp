#include "example.hpp"

void ExampleClass::evtOnRequest (Tox *tox, const uint8_t *public_key, const uint8_t *message, size_t length, void *user_data)
{
	std::cout << "Incoming friend request, auto-accepting..." << std::endl;
	tox_friend_add_norequest (tox, public_key, NULL);
	std::cout << "Accepted new friend with public key: " << convPublicKeyToHexString (public_key) << std::endl;
}

void ExampleClass::evtOnMessage (Tox *tox, uint32_t friend_number, TOX_MESSAGE_TYPE type, const uint8_t *message, size_t length, void *user_data)
{
	std::cout << "Received message: " << message << std::endl;
	tox_friend_send_message (tox, friend_number, type, message, length, NULL);
}
