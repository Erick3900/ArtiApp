//
// Created by Alcachofa
//

#pragma once

#include <cstdint>

#include <SFML/Graphics/Color.hpp>

// TODO credits to David Barr, oneLoneCoder, code from olc::PixelGameEngine

namespace arti {

    struct pixel {

        union {
            uint32_t n = 0xFF;
            struct {
                uint8_t r;
                uint8_t g;
                uint8_t b;
                uint8_t a;
            };
        };

        pixel();
        pixel(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF);
        pixel(uint32_t color);
        pixel(const sf::Color& color);

        operator sf::Color() const;

        pixel&  operator=   (const pixel &v ) = default;
        bool    operator==  (const pixel &p ) const;
        bool    operator!=  (const pixel &p ) const;
        pixel   operator*   (const float &i ) const;
        pixel   operator/   (const float &i ) const;
        pixel&  operator*=  (const float &i );
        pixel&  operator/=  (const float &i );
        pixel   operator+   (const pixel &p ) const;
        pixel   operator-   (const pixel &p ) const;
        pixel&  operator+=  (const pixel &p );
        pixel&  operator-=  (const pixel &p );
        pixel   inv() const;

    };

    pixel pixelF(float r, float g, float b, float a = 1.0f);
    pixel pixelLerp(const pixel &p1, const pixel &p2, float t);

}