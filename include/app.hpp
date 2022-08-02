//
// Created by Alcachofa
//

#pragma once

#include <string>
#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include <input.hpp>
#include <renderer.hpp>

#include <math/vec2d.hpp>

namespace arti {

    class app {

    public:
        app();
        virtual ~app();

        bool init(std::string_view name, math::vec2di windowSize, sf::Uint32 style = sf::Style::Default);

        int run();

        virtual bool onInit();
        virtual bool onUpdate(float) = 0;
        virtual bool onExit();

        virtual void onResize(math::vec2di newSize);
        virtual void onPollEvent(const sf::Event& event);

        int32_t getWidth() const;
        int32_t getHeight() const;
        math::vec2di getSize() const;
        std::string_view getName() const;

        void setName(std::string_view name);
        void setSize(math::vec2du windowSize);
        void setIcon(std::string_view iconFile);

        sf::RenderWindow& getWindow();

    protected:
        std::unique_ptr<renderer> graphics;
        std::unique_ptr<input_manager> input;

    private:
        void onSFMLEvent(const sf::Event& event);

        bool pUpdate(const sf::Time& elapsed);
        bool pRender();
        void pExit();

        bool isRunning;
        bool isInitialized;
        bool exitRequested;
        float deltaTime;
        std::string appName;

        sf::RenderWindow window;
    };

}