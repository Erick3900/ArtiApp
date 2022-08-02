//
// Created by Alcachofa
//

#pragma once

#include <array>
#include <cstdint>
#include <unordered_map>

#include <constants/keys.hpp>

#include <math/vec2d.hpp>

namespace arti {

    class app;

    class input_manager {

        friend class app;

        struct input_state {
            enum : int8_t {
                pressed = 0b010,
                released = 0b001,
                held = 0b100,
                none = 0
            };

            int8_t state;
        };

    public:
        input_manager(app* appInstance);
        ~input_manager();

        void update();

        bool isKeyHeld(key_t keyId) const;
        bool isKeyPressed(key_t keyId) const;
        bool isKeyReleased(key_t keyId) const;

        bool isButtonHeld(button_t buttonId) const;
        bool isButtonPressed(button_t buttonId) const;
        bool isButtonReleased(button_t buttonId) const;

        int32_t getVScroll() const;
        int32_t getHScroll() const;

        math::vec2di getMousePos() const;

        int32_t getMouseX() const;
        int32_t getMouseY() const;

    private:
        void registerKeyState(key_t keyId, bool state);

        void registerButtonState(button_t buttonId, bool state);

        void registerVScroll(int32_t val);
        void registerHScroll(int32_t val);

        bool scrollUpdated;

        int32_t vScroll;
        int32_t hScroll;

        math::vec2di mousePos;

        app* appInstance;

        std::array<bool, static_cast<int16_t>(key_t::KeyCount)> actKeyState;
        std::array<bool, static_cast<int16_t>(key_t::KeyCount)> prevKeyState;
        std::array<input_state, static_cast<int16_t>(key_t::KeyCount)> keyStates;

        std::array<bool, static_cast<int16_t>(button_t::ButtonCount)> actButtonState;
        std::array<bool, static_cast<int16_t>(button_t::ButtonCount)> prevButtonState;
        std::array<input_state, static_cast<int16_t>(button_t::ButtonCount)> buttonStates;
    };

}