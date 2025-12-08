#pragma once
#include <SFML/System/Vector2.hpp>

namespace Constants {
    constexpr sf::Vector2f tableSize{1500.f, 750.f};

    constexpr sf::Vector2u windowSize{1920u, 1080u};

    constexpr float tableOutlineThickness{50.f};
    constexpr float ballRadius{25.f};
    constexpr float pocketRadius{30.f};

    constexpr unsigned int frameLimit = 30;
};