#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <memory>

enum class BallType {
    Cue,
    Solid,
    Stripe,
    EightBall
};

class Ball : public sf::Drawable {
public:
    Ball(BallType type, const sf::Vector2f& position) : type_(type), position_(position)
    {
        float ballRadius = 25.f;
        shape_.setOrigin(sf::Vector2f{ballRadius, ballRadius});
    }

protected:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        target.draw(shape_);
    }

    BallType type_;
    sf::Vector2f position_;
    sf::Vector2f velocity_;


    sf::CircleShape shape_;
};

class NumberBall : public Ball {
public:
    NumberBall(BallType type, const sf::Vector2f& position, int number) : Ball(type, position), number_(number) {}


private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        Ball::draw(target, states);
    }


    int number_;
};

class Game {
public:
    Game()
        : window_(sf::VideoMode({1920u, 1080u}), "8 Ball Pool")
        , view_(window_.getDefaultView())
        , table(tableSize)
    {
        window_.setFramerateLimit(30);
        view_.setCenter(sf::Vector2f(0.f, 0.f));
        window_.setView(view_);

        setupBalls();
    }

    void run() {
        while (window_.isOpen()) {
            handleEvents();
            update();
            render();
        }
    }

private:
    void handleEvents() {
        while (const std::optional event = window_.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window_.close();
            }
        }
    }

    void update() {

    }

    void render() {
        table.setFillColor(sf::Color(0, 100, 0));
        table.setOutlineColor(sf::Color(139, 69, 19));
        table.setOutlineThickness(50.f);

        table.setOrigin(tableSize / 2.f);


        static sf::CircleShape potHole(30.f);
        potHole.setOrigin(sf::Vector2f{potHole.getRadius(), potHole.getRadius()});
        potHole.setFillColor(sf::Color::Black);
        
        static std::vector<sf::Vector2f> pot_positions {
            sf::Vector2f{-750.f, -375.f}, sf::Vector2f{0.f, -375.f}, sf::Vector2f{750.f, -375.f},
            sf::Vector2f{-750.f,  375.f}, sf::Vector2f{0.f,  375.f}, sf::Vector2f{750.f,  375.f},
        };

        window_.clear(sf::Color(50, 50, 50));

        window_.draw(table);

        for (const auto& pos : pot_positions) {
            potHole.setPosition(pos);
            window_.draw(potHole);
        }

        for (const auto& ball : balls_) {
            window_.draw(*ball);
        }

        window_.display();
    }

    void setupBalls() {
        float ballRadius = 25.f;
        float diameter = ballRadius * 2.f;
        
        // The vertical distance between rows in a hex packing
        // Formula: Radius * sqrt(3)
        float rowOffset = ballRadius * std::sqrt(3.f);

        // std::vector<sf::CircleShape> balls;
        
        
        // Define the start position (The Apex of the pyramid)
        // We place it on the right side of the table
        sf::Vector2f apexPos = table.getPosition();
        apexPos.x += tableSize.x / 4.f; // Shift to the "foot spot" (right side)
        apexPos.y -= 0.f;               // Centered vertically

        int ballCount = 0; // To track which ball we are creating

        // Loop 5 Columns (Vertical Pyramid Rotated 90 degrees to point Left)
        // col 0 = 1 ball, col 1 = 2 balls, etc.
        for (int col = 0; col < 5; ++col) {
            for (int row = 0; row <= col; ++row) {

                sf::CircleShape ball(ballRadius);
                ball.setOrigin(sf::Vector2f{ballRadius, ballRadius}); // Center the ball origin


                // MATH FOR COORDINATES:
                // X grows as we add columns (moving right)
                float x = col * rowOffset;
                
                // Y is offset based on the row, centered around 0
                // (row - col / 2.0f) centers the row vertically
                float y = (row - col / 2.0f) * diameter;

                // Apply calculated offsets to the Apex position
                // Note: We subtract x to make the pyramid point LEFT (standard TV view)
                sf::Vector2f ballPosition{apexPos.x + x, apexPos.y + y};

                // COLOR LOGIC (Basic 8-Ball Rules)
                if (col == 2 && row == 1) {
                    ball.setFillColor(sf::Color::Black); // The 8-Ball (Center)
                    balls_.push_back(std::make_unique<NumberBall>(BallType::EightBall, ballPosition, 8));
                } 
                else if (col == 4 && row == 0) {
                    //  ball.setFillColor(sf::Color::Yellow); // Corner 1 (Solid)
                    int ballNum = -1;
                    balls_.push_back(std::make_unique<NumberBall>(BallType::Solid, ballPosition, ballNum));
                }
                else if (col == 4 && row == 4) {
                    // ball.setFillColor(sf::Color::Red);    // Corner 2 (Stripe)
                    int ballNum = -1;
                    balls_.push_back(std::make_unique<NumberBall>(BallType::Stripe, ballPosition, ballNum));
                }
                else {
                    ball.setFillColor(sf::Color::Magenta);   // Rest are random
                    int ballNum = -1;
                    BallType type = BallType::Solid;
                    balls_.push_back(std::make_unique<NumberBall>(type, ballPosition, ballNum));
                }

                ballCount++;
            }
        }
    }

    sf::RenderWindow window_;
    sf::View view_;

    sf::Vector2f tableSize{1500.f, 750.f};
    sf::RectangleShape table;

    std::vector<std::unique_ptr<Ball>> balls_;

};