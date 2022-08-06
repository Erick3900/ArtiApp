//
// Created by Alcachofa
//

#include <app.hpp>

#include <imgui.hpp>

#include <utils/utils.hpp>
#include <utils/logger.hpp>

namespace arti {

    app::app()
            : isRunning(false),
              isInitialized(false),
              exitRequested(false),
              deltaTime(0.0f),
              appName("Artichaut App"),
              graphics(nullptr) {
        input = std::make_unique<input_manager>(this);
        graphics = std::make_unique<renderer>(this);
    }

    app::~app() {
        pExit();
    }

    bool app::init(std::string_view name, math::vec2di windowSize, sf::Uint32 style) {
        this->appName = name;
        window.create(sf::VideoMode(windowSize.x, windowSize.y), appName, style);

        if (isInitialized) return true;

        if (! ImGui::SFML::Init(window, false)) {
            logger::error("Failed to init ImGui");
            return false;
        }

        if (! graphics->init()) {
            logger::error("Couldn't initialize renderer");
            return false;
        }

        input->update();

        if (! onInit()) {
            return false;
        }

        auto& io = ImGui::GetIO();

        if (io.Fonts->Fonts.empty()) {
            logger::debug("Loading default font");
            io.Fonts->AddFontDefault();
            io.Fonts->Build();
            if (! ImGui::SFML::UpdateFontTexture()) {
                logger::error("Failed to load default ImGui font");
                return false;
            }
        }

        isInitialized = true;
        return true;
    }

    int app::run() {
        if (! isInitialized) {
            logger::error("App was not initialized, maybe you forgot to call onInit?");
            return EXIT_FAILURE;
        }

        isRunning = true;

        sf::Clock timer;

        float fAccTime = 0.0f;
        int fps = 0;

        while (isRunning) {
            auto elapsed = timer.restart();

            if (! exitRequested && pUpdate(elapsed)) {
                pRender();
                deltaTime = to<float>(elapsed.asMicroseconds()) / 1000000.0f;

#ifdef ARTI_MEASURE_FPS
                ++fps;
                    fAccTime += deltaTime;
                    if (fAccTime >= 1.0f) {
                        window.setTitle(sf::String(fmt::format("{} - FPS: {}", appName, fps)));
                        fps = 0;
                        fAccTime -= 1.0f;
                    }
#endif
            }
            else {
                pExit();
            }
        }

        return EXIT_SUCCESS;
    }

    bool app::onInit() { return true; }
    bool app::onExit() { return true; }

    void app::onResize(math::vec2di newSize) { }
    void app::onPollEvent(const sf::Event &event) { }

    int32_t app::getWidth() const {
        return to<int32_t>(window.getSize().x);
    }

    int32_t app::getHeight() const {
        return to<int32_t>(window.getSize().y);
    }

    math::vec2di app::getSize() const {
        auto size = window.getSize();
        return {to<int32_t>(size.x), to<int32_t>(size.y)};
    }

    std::string_view app::getName() const {
        return appName;
    }

    void app::setName(std::string_view name) {
        this->appName = name;
        window.setTitle(appName);
    }

    void app::setSize(math::vec2du windowSize) {
        window.setSize(windowSize);
    }

    void app::setIcon(std::string_view iconFile) {
        sf::Image icon;
        icon.loadFromFile(std::string(iconFile));
        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    }



    void app::onSFMLEvent(const sf::Event& event) {
        ImGui::SFML::ProcessEvent(window, event);

        switch (event.type) {
            case sf::Event::Closed:
                exitRequested = true;
                break;

            case sf::Event::KeyPressed:
                if (event.key.code != sf::Keyboard::Unknown) {
                    input->registerKeyState(to<key_t>(event.key.code), true);
                    input->registerKeyState(key_t::Alt, event.key.alt);
                    input->registerKeyState(key_t::Ctrl, event.key.control);
                    input->registerKeyState(key_t::Shift, event.key.shift);
                    input->registerKeyState(key_t::System, event.key.system);
                }
                break;

            case sf::Event::KeyReleased:
                if (event.key.code != sf::Keyboard::Unknown) {
                    input->registerKeyState(to<key_t>(event.key.code), false);
                    input->registerKeyState(key_t::Alt, event.key.alt);
                    input->registerKeyState(key_t::Ctrl, event.key.control);
                    input->registerKeyState(key_t::Shift, event.key.shift);
                    input->registerKeyState(key_t::System, event.key.system);
                }
                break;

            case sf::Event::MouseButtonPressed:
                input->registerButtonState(to<button_t>(event.mouseButton.button), true);
                break;

            case sf::Event::MouseButtonReleased:
                input->registerButtonState(to<button_t>(event.mouseButton.button), false);
                break;

            case sf::Event::MouseWheelScrolled:
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
                    input->registerVScroll(event.mouseWheelScroll.delta);
                else if (event.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel)
                    input->registerHScroll(event.mouseWheelScroll.delta);
                break;

            case sf::Event::Resized:
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
                onResize(window.getSize());
                break;
        }
    }

    bool app::pUpdate(const sf::Time& elapsed) {
        sf::Event event;
        while (window.pollEvent(event)) {
            onPollEvent(event);
            onSFMLEvent(event);
        }
        input->update();

        if (exitRequested) {
            return false;
        }

        ImGui::SFML::Update(window, elapsed);

        if (! onUpdate(deltaTime)) {
            return false;
        }

        return true;
    }

    bool app::pRender() {
        graphics->render();
        window.display();
        return true;
    }

    void app::pExit() {
        if (! isRunning) return;

        isRunning = false;

        if (! onExit()) {
            logger::warning("User onExit returned false");
        }

        window.close();
        ImGui::SFML::Shutdown(window);
    }

    sf::RenderWindow& app::getWindow() {
        return this->window;
    }
}