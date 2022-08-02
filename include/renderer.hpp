//
// Created by Alcachofa
//

#pragma once

#include <list>
#include <optional>

#include <SFML/Graphics.hpp>

#include <math/vec2d.hpp>

#include <utils/logger.hpp>

#include <pixel.hpp>

namespace arti {

    class app;
    class transformed_view;

    class renderer {

        friend class app;
        friend class transformed_view;

    public:
        using layer_id = uint16_t;
        using texture_id = uint16_t;

    protected:
        struct layer_t {
            layer_id id;
            bool enabled;
            float scale;
            math::vec2df offset;
            sf::RenderTexture texture;

            float viewScale;
            math::vec2df viewOffset;

            sf::View view;

            void updateView() {
                view.reset(sf::FloatRect(
                   viewOffset,
                   math::vec2df{texture.getSize()} / viewScale
                   )
                );

                texture.setView(view);
            }

            inline void render(sf::Sprite& spr, sf::RenderWindow& wind) {
                spr.setTexture(texture.getTexture(), true);
                spr.setPosition(offset.x, offset.y + to<float>(texture.getSize().y) * scale);
                spr.setScale(scale, -scale);

                wind.draw(spr);
            }
        };

    public:
        renderer(app* appInstance);

        ~renderer();

        void clear(const pixel& color = sf::Color::Transparent);

        layer_id createLayer();

        layer_id createLayer(const math::vec2di& size);

        void offsetLayer(const math::vec2df& offset);
        void scaleLayerAt(float scale, const math::vec2df& screenCenter);

        void offsetView(const math::vec2df& offset);
        void scaleViewAt(float scale, const math::vec2df& screenCenter);

        float getLayerScale() const;
        math::vec2df getLayerOffset() const;
        math::vec2di getLayerSize() const;

        void resizeLayer(const math::vec2di& newSize);

        bool enableLayer(const layer_id& id, bool enabled);

        bool isLayerEnabled(const layer_id& id);

        bool setTargetedLayer(const layer_id& id);
        layer_id getTargetedLayer() const;

        bool targetDefaultLayer();

        void renderCircle(const math::vec2df& coords, float radius, const pixel& fillColor);
        void renderCircle(const math::vec2df& coords, float radius, float borderThickness, const pixel& fillColor, const pixel& borderColor);

        void renderRectangle(const math::vec2df& coords, const math::vec2df& size, const pixel& fillColor, float rotation = 0.0f);
        void renderRectangle(const math::vec2df& coords, const math::vec2df& size, float borderThickness, const pixel& fillColor, const pixel& borderColor, float rotation = 0.0f);

        void renderSquare(const math::vec2df& coords, float sideSize, const pixel& fillColor, float rotation = 0.0f);
        void renderSquare(const math::vec2df& coords, float sideSize, float borderThickness, const pixel& fillColor, const pixel& borderColor, float rotation = 0.0f);

        void renderLine(const math::vec2df& pointA, const math::vec2df& pointB, const pixel& color);
        void renderLine(const math::vec2df& pointA, const math::vec2df& pointB, float thickness, const pixel& color);

        void render(const sf::Drawable& drawable) {
            layersList.at(targetedLayer).texture.draw(drawable);
        }

        bool isVisible(const math::vec2df& world_pos, float radius = 0.0f) {
            auto sup_left = screenToView(layerToScreen({0, 0}));
            auto inf_right = screenToView(layerToScreen(to<math::vec2df>(getLayerSize())));
            auto window_limit = screenToView(window.getSize());
            if (inf_right.y > window_limit.y) {
                inf_right = window_limit;
            }
            auto box = (math::vec2df{radius, radius});
            return world_pos >= (sup_left - box) && world_pos <= (inf_right + box);
        }

        math::vec2df screenToLayer(const math::vec2df& coord);
        math::vec2df layerToScreen(const math::vec2df& coord);

        math::vec2df screenToView(const math::vec2df& coord);
        math::vec2df viewToScreen(const math::vec2df& coord);

    protected:
        bool init();
        bool render();



        app* appInstance;

        layer_id layerCount;
        layer_id defaultLayer;
        layer_id targetedLayer;

        texture_id textureCount;

        std::map<layer_id, layer_t> layersList;

        sf::RenderWindow& window;
    };

}