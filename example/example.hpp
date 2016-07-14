#include "../toxcontroller/toxcontroller.hpp"

#include <string>
#include <cstdint>
#include <iostream>

class ExampleClass : public ToxController
{
public:
    // using ToxController::ToxController;
    ExampleClass (std::string bootstrap_address,
                  uint16_t bootstrap_port,
                  std::string bootstrap_key,
                  std::string nickname,
                  std::string status
                 ) : ToxController (bootstrap_address, bootstrap_port, bootstrap_key, nickname, status)
    {
        setupCallbacks();
    };

private:
    void evtOnRequest(Tox *tox, const uint8_t *public_key, const uint8_t *message, size_t length, void *user_data) override;
    void evtOnMessage (Tox *tox, uint32_t friend_number, TOX_MESSAGE_TYPE type, const uint8_t *message, size_t length, void *user_data) override;
};
