//
// Created by Alcachofa
//

#include <input.hpp>

#include <app.hpp>

namespace arti {

    input_manager::input_manager(app* appInstance)
            : scrollUpdated(true),
              vScroll(0),
              hScroll(0),
              mousePos(0, 0),
              appInstance(appInstance) {
        for (auto& state : actKeyState) {
            state = false;
        }

        for (auto& state : actButtonState) {
            state = false;
        }

        for (auto& state : keyStates) {
            state.state = input_state::none;
        }

        for (auto& state : buttonStates) {
            state.state = input_state::none;
        }
    }

    input_manager::~input_manager() = default;


    void input_manager::update() {
        for (key_t id = key_t::A; id < key_t::KeyCount; id = to<key_t>(id + 1)) {
            keyStates[id].state &= input_state::held;

            if (actKeyState[id] != prevKeyState[id]) {
                if (actKeyState[id]) {
                    if (! (input_state::held & keyStates[id].state)) {
                        keyStates[id].state = input_state::pressed;
                    }
                    keyStates[id].state |= input_state::held;
                }
                else {
                    keyStates[id].state = input_state::released;
                }
            }

            prevKeyState[id] = actKeyState[id];
        }

        for (button_t id = button_t::mLeft; id < button_t::ButtonCount; id = to<button_t>(id + 1)) {
            buttonStates[id].state &= input_state::held;

            if (actButtonState[id] != prevButtonState[id]) {
                if (actButtonState[id]) {
                    if (! (input_state::held & buttonStates[id].state)) {
                        buttonStates[id].state = input_state::pressed;
                    }
                    buttonStates[id].state |= input_state::held;
                }
                else {
                    buttonStates[id].state = input_state::released;
                }
            }

            prevButtonState[id] = actButtonState[id];
        }

        if (! scrollUpdated) {
            vScroll = 0;
            hScroll = 0;
        }

        scrollUpdated = false;
    }


    bool input_manager::isKeyHeld(key_t keyId) const {
        return (keyStates[keyId].state & input_state::held);
    }

    bool input_manager::isKeyPressed(key_t keyId) const {
        return (keyStates[keyId].state & input_state::pressed);
    }

    bool input_manager::isKeyReleased(key_t keyId) const {
        return (keyStates[keyId].state & input_state::released);
    }


    bool input_manager::isButtonHeld(button_t buttonId) const {
        return (buttonStates[buttonId].state & input_state::held);
    }

    bool input_manager::isButtonPressed(button_t buttonId) const {
        return (buttonStates[buttonId].state & input_state::pressed);
    }

    bool input_manager::isButtonReleased(button_t buttonId) const {
        return (buttonStates[buttonId].state & input_state::released);
    }


    int32_t input_manager::getVScroll() const {
        return vScroll;
    }

    int32_t input_manager::getHScroll() const {
        return hScroll;
    }


    math::vec2di input_manager::getMousePos() const {
        return sf::Mouse::getPosition(appInstance->getWindow());
    }


    int32_t input_manager::getMouseX() const {
        return sf::Mouse::getPosition(appInstance->getWindow()).x;
    }

    int32_t input_manager::getMouseY() const {
        return sf::Mouse::getPosition(appInstance->getWindow()).y;
    }

    void input_manager::registerKeyState(key_t keyId, bool state) {
        actKeyState[keyId] = state;
    }

    void input_manager::registerButtonState(button_t buttonId, bool state) {
        actButtonState[buttonId] = state;
    }

    void input_manager::registerVScroll(int32_t val) {
        vScroll = val;
        scrollUpdated = true;
    }

    void input_manager::registerHScroll(int32_t val) {
        hScroll = val;
        scrollUpdated = true;
    }

}