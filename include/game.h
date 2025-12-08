#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <memory>

#include "ball.h"
#include "gameConstants.h"

#include <iostream>


class Game {
public:
    Game()
        : window_(sf::VideoMode(Constants::windowSize), "8 Ball Pool")
        , view_(window_.getDefaultView())
        , table_(Constants::tableSize)
        , pocket_(Constants::pocketRadius)
        , cue_(BallType::Cue, std::nullopt, sf::Vector2f{}, font_) // dont need font
    {
        window_.setFramerateLimit(Constants::frameLimit);
        view_.setCenter(sf::Vector2f(0.f, 0.f));
        window_.setView(view_);

        if (!font_.openFromFile("arial.ttf")) {
            std::cout << "error loading font!!!\n";
            throw std::runtime_error("couldnt find font");
        }


        setupTable();
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
            } else if (auto* mouseMove = event->getIf<sf::Event::MouseMoved>()) {
                mouseMove->position.x;
            }
        }
    }

    void update() {

    }

    void render() {
        
        
        static std::array<sf::Vector2f, 6> pocketPositions {
            sf::Vector2f{-750.f, -375.f}, sf::Vector2f{0.f, -375.f}, sf::Vector2f{750.f, -375.f},
            sf::Vector2f{-750.f,  375.f}, sf::Vector2f{0.f,  375.f}, sf::Vector2f{750.f,  375.f},
        };

        window_.clear(sf::Color(50, 50, 50));

        window_.draw(table_);

        for (const auto& pos : pocketPositions) {
            pocket_.setPosition(pos);
            window_.draw(pocket_);
        }

        for (const auto& ball : balls_) {
            window_.draw(ball);
        }

        window_.draw(cue_);

        window_.display();
    }

    void setupTable() {
        table_.setFillColor(sf::Color(0, 100, 0));
        table_.setOutlineColor(sf::Color(139, 69, 19));
        table_.setOutlineThickness(Constants::tableOutlineThickness);
        table_.setOrigin(Constants::tableSize / 2.f);


        pocket_.setOrigin(sf::Vector2f{pocket_.getRadius(), pocket_.getRadius()});
        pocket_.setFillColor(sf::Color::Black);
    }

    void setupBalls() {
        float ballRadius = Constants::ballRadius;
        float diameter = ballRadius * 2.f;
        
        // The vertical distance between rows in a hex packing
        // Formula: Radius * sqrt(3)
        float rowOffset = ballRadius * std::sqrt(3.f);

        // std::vector<sf::CircleShape> balls;
        
        
        // Define the start position (The Apex of the pyramid)
        // We place it on the right side of the table
        sf::Vector2f apexPos = table_.getPosition();
        apexPos.x += Constants::tableSize.x / 4.f; // Shift to the "foot spot" (right side)
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
                    balls_.push_back(Ball{BallType::EightBall, 8, ballPosition, font_});
                } 
                else if (col == 4 && row == 0) {
                    //  ball.setFillColor(sf::Color::Yellow); // Corner 1 (Solid)
                    int ballNum = 0;
                    balls_.push_back(Ball{BallType::Solid, ballNum, ballPosition, font_});
                }
                else if (col == 4 && row == 4) {
                    // ball.setFillColor(sf::Color::Red);    // Corner 2 (Stripe)
                    int ballNum = 0;
                    balls_.push_back(Ball{BallType::Stripe, ballNum, ballPosition, font_});
                }
                else {
                    ball.setFillColor(sf::Color::Magenta);   // Rest are random
                    int ballNum = 0;
                    BallType type = BallType::Solid;
                    balls_.push_back(Ball{type, ballNum, ballPosition, font_});
                }

                ballCount++;
            }
        }
    }

    sf::RenderWindow window_;
    sf::View view_;

    // Rendering
    sf::RectangleShape table_;
    sf::CircleShape pocket_;

    sf::Font font_;


    Ball cue_;
    std::vector<Ball> balls_;

};