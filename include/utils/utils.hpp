//
// Created by Alcachofa
//

#pragma once

namespace arti {

    template <typename T, typename U>
    inline T to(const U& val) {
        return static_cast<T>(val);
    }

}