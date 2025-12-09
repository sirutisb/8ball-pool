#pragma once
#include <SFML/Graphics.hpp>
#include "ball.h"

class Cue : public sf::Drawable {
public:
    Cue()
        : cueSprite_(sf::Vector2f{150, 25})
        , strength_(minStrength_)
    {
        cueSprite_.setFillColor(sf::Color{100, 108, 105});
        cueSprite_.setOutlineColor(sf::Color(139, 69, 19));
        cueSprite_.setOutlineThickness(3.f);
        cueSprite_.setOrigin(sf::Vector2f{0, cueSprite_.getSize().y / 2.f});
        // cueSprite_.setRotation();
    }

    void update(Ball& cueBall, const sf::Vector2f& mouseWorldPos) {
        sf::Vector2f delta = mouseWorldPos - cueBall.getPosition();
        if (delta.x == 0 && delta.y == 0) return;
        sf::Vector2f dir = delta.normalized();

        float cueDist = (maxDist_ - minDist_) * strength_ + minDist_;
        cueDist += Constants::ballRadius;

        sf::Vector2f cueDeltaPos = dir * cueDist;

        float cueRot = std::atan2(delta.y, delta.x);
        cueSprite_.setPosition(cueBall.getPosition() + cueDeltaPos);
        cueSprite_.setRotation(sf::radians(cueRot));
    }

    void increaseStength() { strength_ = std::min(strength_ + strengthIncrement_, 1.0f); }
    void decreaseStength() { strength_ = std::max(strength_ - strengthIncrement_, minStrength_); }

    float getStrength() const { return strength_; }

private:

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        target.draw(cueSprite_);
    }

    float minStrength_ = 0.1f;
    float strengthIncrement_ = 0.1f;
    
    float minDist_ = 0;
    float maxDist_ = 300.f;
    
    float strength_;

    sf::RectangleShape cueSprite_;
};