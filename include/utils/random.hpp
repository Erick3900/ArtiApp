//
// Created by Alcachofa
//

#pragma once

#include <random>

namespace arti {

    class random_generator {

    protected:
        std::random_device rd;
        std::mt19937 gen;

        random_generator() : gen(rd()) {

        }

    };

    template <typename RealType_>
    class f_random : public random_generator {

    private:
        std::uniform_real_distribution<RealType_> distribution;

    public:
        f_random() : random_generator(), distribution(0, 1.0) {

        }

        f_random(RealType_ max) : random_generator(), distribution(0, max) {

        }

        f_random(RealType_ min, RealType_ max) : random_generator(), distribution(min, max) {

        }

        f_random(const f_random& other) : random_generator(), distribution(other.distribution) {

        }

        RealType_ get() {
            return distribution(gen);
        }

        static RealType_ Get() {
            static f_random sZeroToOne;
            return sZeroToOne.get();
        }

        static RealType_ GetRange(RealType_ min, RealType_ max) {
            return min + Get() * (max - min);
        }

        static RealType_ GetUnder(RealType_ max) {
            return Get() * max;
        }

    };


    template <typename IntegerType_>
    class i_random : public random_generator {

    private:
        std::uniform_int_distribution<IntegerType_> distribution;

    public:
        i_random() : random_generator(), distribution(0, std::numeric_limits<IntegerType_>::max()) {

        }

        i_random(IntegerType_ max) : random_generator(), distribution(0, max) {

        }

        i_random(IntegerType_ min, IntegerType_ max) : random_generator(), distribution(min, max) {

        }

        i_random(const i_random& other) : random_generator(), distribution(other.distribution) {

        }

        IntegerType_ get() {
            return distribution(gen);
        }

        static IntegerType_ Get() {
            static i_random sZeroToMax;
            return sZeroToMax.get();
        }

        static IntegerType_ GetUnder(IntegerType_ max) {
            return Get() % max;
        }

        static IntegerType_ GetRange(IntegerType_ min, IntegerType_ max) {
            return min + Get() % (max - min);
        }

    };

}