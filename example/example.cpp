#include "example.hpp"

void ExampleClass::onRequest (Tox *tox, const uint8_t *public_key, const uint8_t *message, size_t length, void *user_data)
{
	std::cout << "Incoming friend request, auto-accepting..." << std::endl;
	addFriendNoRequest (public_key);
	std::cout << "Accepted new friend with public key: " << convPublicKeyToHexString (public_key) << std::endl;
}

void ExampleClass::onMessage (Tox *tox, uint32_t friend_number, TOX_MESSAGE_TYPE type, const uint8_t *message, size_t length, void *user_data)
{
	std::cout << "Received message: " << message << std::endl;
	sendMessage(friend_number, (char *) message);
}
