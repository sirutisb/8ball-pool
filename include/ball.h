#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include "gameConstants.h"

#include <iostream>

enum class BallType {
    Cue,
    Solid,
    Stripe,
    EightBall
};

class Ball : public sf::Drawable {
public:
    Ball(BallType type, std::optional<int> number, const sf::Vector2f& position, const sf::Font& font)
        : type_(type),
        number_(number),
        position_(position),
        velocity_(),
        pocketed_(false),
        shape_(Constants::ballRadius),
        numberText_(font)
    {
        float ballRadius = Constants::ballRadius;
        shape_.setOrigin(sf::Vector2f{ballRadius, ballRadius});
        shape_.setPosition(position);

        if (number) {
            numberText_.setString(std::to_string(*number));
            numberText_.setFillColor(sf::Color::Black);
            numberText_.setPosition(position_);

            const sf::FloatRect bounds = numberText_.getLocalBounds();
            numberText_.setOrigin(bounds.position + bounds.size / 2.f);
        }
    }

    void update(float dt) {
        position_ += velocity_ * dt;


        shape_.setPosition(position_);
        numberText_.setPosition(position_);
    }

    void setVelocity(sf::Vector2f vel) {
        velocity_ = vel;
    }

    BallType getType() const { return type_; }

    sf::Vector2f& getPosition() { return position_; }
    sf::Vector2f& getVelocity() { return velocity_; }

    bool isPocketed() const { return pocketed_; }



private:

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        target.draw(shape_);
        if (number_) {
            target.draw(numberText_);
        }
    }

    BallType type_;
    std::optional<int> number_;

    sf::Vector2f position_;
    sf::Vector2f velocity_;

    bool pocketed_;

    sf::CircleShape shape_;

    sf::Text numberText_;
};