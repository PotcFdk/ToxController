#include "../toxcontroller/toxcontroller.hpp"

#include <string>
#include <cstdint>
#include <iostream>

class ExampleClass : public ToxController
{
    using ToxController::ToxController;

private:
    void onRequest (Tox *tox, const uint8_t *public_key, const uint8_t *message, size_t length, void *user_data) override;
    void onMessage (Tox *tox, uint32_t friend_number, TOX_MESSAGE_TYPE type, const uint8_t *message, size_t length, void *user_data) override;
};
