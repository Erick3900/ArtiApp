//
// Created by Alcachofa
//

#include <renderer.hpp>

#include <app.hpp>
#include <imgui.hpp>

#include <utils/logger.hpp>

namespace arti {

    renderer::renderer(app* appInstance)
            : layerCount(0),
              defaultLayer(0),
              targetedLayer(0),
              textureCount(0),
              appInstance(appInstance),
              window(appInstance->getWindow()) {

    }

    renderer::~renderer() = default;

    void renderer::clear(const pixel& color) {
        layersList.at(targetedLayer).texture.clear(color);
    }

    renderer::layer_id renderer::createLayer() {
        return this->createLayer(window.getSize());
    }

    renderer::layer_id renderer::createLayer(const math::vec2di& size) {
        auto& newLayer = layersList[layerCount + 1];
        sf::RenderTexture& newTexture = newLayer.texture;

        if (!newTexture.create(size.x, size.y)) {
            layersList.erase(layerCount + 1);
            logger::error("Couldn't create layer {} of size {}", layerCount + 1, size.to_string());
            return std::numeric_limits<layer_id>::max();
        }

        newTexture.clear(sf::Color::Transparent);

        newLayer.id = ++layerCount;
        newLayer.enabled = true;
        newLayer.scale = 1.0f;
        newLayer.viewScale = 1.0f;

        return newLayer.id;
    }

    void renderer::offsetLayer(const math::vec2df& offset) {
        layersList.at(targetedLayer).offset += offset;
    }

    void renderer::scaleLayerAt(float scale, const math::vec2df& screenCenter) {
        auto before = screenToLayer(screenCenter);
        layersList.at(targetedLayer).scale = scale;
        auto after = layerToScreen(before);
        layersList.at(targetedLayer).offset -= (after - screenCenter);
    }

    void renderer::offsetView(const math::vec2df& offset) {
        layersList.at(targetedLayer).viewOffset -= ((offset / layersList.at(targetedLayer).viewScale) / layersList.at(targetedLayer).scale);
        layersList.at(targetedLayer).updateView();
    }

    void renderer::scaleViewAt(float scale, const math::vec2df& screenCenter) {
        auto before = screenToView(screenCenter);
        layersList.at(targetedLayer).viewScale = scale;
        auto after = viewToScreen(before);
        layersList.at(targetedLayer).viewOffset += (((after - screenCenter) / scale) / layersList.at(targetedLayer).scale);
        layersList.at(targetedLayer).updateView();
    }


    float renderer::getLayerScale() const {
        return layersList.at(targetedLayer).scale;
    }

    math::vec2df renderer::getLayerOffset() const {
        return layersList.at(targetedLayer).offset;
    }


    math::vec2di renderer::getLayerSize() const {
        return layersList.at(targetedLayer).texture.getSize();
    }

    void renderer::resizeLayer(const math::vec2di& newSize) {
        if (!layersList.at(targetedLayer).texture.create(newSize.x, newSize.y)) {
            logger::error("Couldn't resize texture {} to {}", targetedLayer, newSize.to_string());
            layersList.erase(targetedLayer);
        }
    }

    bool renderer::enableLayer(const layer_id& id, bool enabled) {
        layersList.at(id).enabled = enabled;
        return true;
    }

    bool renderer::isLayerEnabled(const layer_id& id) {
        return layersList.at(id).enabled;
    }

    bool renderer::setTargetedLayer(const layer_id& id) {
        if (layersList.find(id) != layersList.end()) {
            targetedLayer = id;
            return true;
        }
        return false;
    }

    renderer::layer_id renderer::getTargetedLayer() const {
        return targetedLayer;
    }

    bool renderer::targetDefaultLayer() {
        targetedLayer = defaultLayer;
        return true;
    }

    void renderer::renderCircle(const math::vec2df& coords, float radius, const pixel& fillColor) {
        if (isVisible(coords, radius)) {
            sf::CircleShape circ(radius);
            circ.setOrigin(radius, radius);
            circ.setFillColor(fillColor);
            circ.setOutlineThickness(0);
            circ.setPosition(coords);

            layersList.at(targetedLayer).texture.draw(circ);
        }
    }

    void renderer::renderCircle(const math::vec2df& coords, float radius, float borderThickness, const pixel& fillColor, const pixel& borderColor) {
        if (isVisible(coords, radius + borderThickness)) {
            sf::CircleShape circ(radius);
            circ.setOutlineThickness(borderThickness);
            circ.setOutlineColor(borderColor);
            circ.setOrigin(radius, radius);
            circ.setFillColor(fillColor);
            circ.setPosition(coords);

            layersList.at(targetedLayer).texture.draw(circ);
        }
    }

    void renderer::renderRectangle(const math::vec2df& coords, const math::vec2df& size, const pixel& fillColor, float rotation) {
        if (isVisible(coords, std::max(size.x, size.y))) {
            sf::RectangleShape rect(size);
            rect.setFillColor(fillColor);
            rect.setRotation(rotation);
            rect.setPosition(coords);

            layersList.at(targetedLayer).texture.draw(rect);
        }
    }

    void renderer::renderRectangle(const math::vec2df& coords, const math::vec2df& size, float borderThickness, const pixel& fillColor, const pixel& borderColor, float rotation) {
        if (isVisible(coords, std::max(size.x, size.y) + borderThickness)) {
            sf::RectangleShape rect(size);
            rect.setOutlineThickness(borderThickness);
            rect.setOutlineColor(borderColor);
            rect.setFillColor(fillColor);
            rect.setRotation(rotation);
            rect.setPosition(coords);

            layersList.at(targetedLayer).texture.draw(rect);
        }
    }


    void renderer::renderSquare(const math::vec2df& coords, float sideSize, const pixel& fillColor, float rotation) {
        if (isVisible(coords, sideSize)) {
            sf::RectangleShape rect({ sideSize, sideSize });
            rect.setFillColor(fillColor);
            rect.setRotation(rotation);
            rect.setPosition(coords);

            layersList.at(targetedLayer).texture.draw(rect);
        }
    }

    void renderer::renderSquare(const math::vec2df& coords, float sideSize, float borderThickness, const pixel& fillColor, const pixel& borderColor, float rotation) {
        if (isVisible(coords, sideSize + borderThickness)) {
            sf::RectangleShape rect({ sideSize, sideSize });
            rect.setOutlineThickness(borderThickness);
            rect.setOutlineColor(borderColor);
            rect.setFillColor(fillColor);
            rect.setRotation(rotation);
            rect.setPosition(coords);

            layersList.at(targetedLayer).texture.draw(rect);
        }
    }

    void renderer::renderLine(const math::vec2df& pointA, const math::vec2df& pointB, const pixel& color) {
//        if (isVisible(pointA) || isVisible(pointB)) {
            sf::Vertex line[] = {
                    sf::Vertex(sf::Vector2f(pointA), color),
                    sf::Vertex(sf::Vector2f(pointB), color)
            };

            layersList.at(targetedLayer).texture.draw(line, 2, sf::Lines);
//        }
    }

    void renderer::renderLine(const math::vec2df& pointA, const math::vec2df& pointB, float thickness, const pixel& color) {
//        if (isVisible(pointA, thickness) || isVisible(pointB, thickness)) {
            auto perpendicular = (pointB - pointA).perpendicular().normalize() * thickness * 0.5;

            sf::Vertex line[] = {
                    sf::Vertex(sf::Vector2f(pointA + perpendicular), color),
                    sf::Vertex(sf::Vector2f(pointA - perpendicular), color),
                    sf::Vertex(sf::Vector2f(pointB - perpendicular), color),
                    sf::Vertex(sf::Vector2f(pointB + perpendicular), color),
            };

            layersList.at(targetedLayer).texture.draw(line, 4, sf::Quads);
//        }
    }

    bool renderer::init() {
        defaultLayer = this->createLayer();

        this->setTargetedLayer(defaultLayer);

        return true;
    }

    bool renderer::render() {
        window.clear();

        sf::Sprite sprite;
        for (auto& [layerId, layer_data] : layersList) {
            if (layer_data.enabled) {
                layer_data.render(sprite, window);
            }
        }

        ImGui::SFML::Render(window);

        return true;
    }

    math::vec2df renderer::screenToLayer(const math::vec2df &coord) {
        return ((coord - layersList[targetedLayer].offset) / layersList[targetedLayer].scale);
    }

    math::vec2df renderer::layerToScreen(const math::vec2df &coord) {
        return layersList[targetedLayer].offset + (coord * layersList[targetedLayer].scale);
    }

    math::vec2df renderer::screenToView(const math::vec2df& coord) {
        return layersList[targetedLayer].viewOffset + (screenToLayer(coord) / layersList[targetedLayer].viewScale);
    }

    math::vec2df renderer::viewToScreen(const math::vec2df& coord) {
        return layerToScreen((coord - layersList[targetedLayer].viewOffset) * layersList[targetedLayer].viewScale);
    }

}