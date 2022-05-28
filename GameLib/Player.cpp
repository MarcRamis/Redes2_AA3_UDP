#include "Player.h"
#include <iostream>

Player::Player()
{
	draw = new SFML_Draw();
	draw->AddSquare(150, 150);

	std::thread tInputs(&Player::GetInputs, this);
	tInputs.detach();

}

Player::Player(float _posX, float _posY, float _velocity) : velocity(_velocity)
{
	draw = new SFML_Draw(sf::Vector2f(250.0f, 200.0f), velocity);
}

void Player::Update()
{
    sf::Clock clock;

    while (draw->GetWindow()->isOpen())
    {
        draw->SetDelta(clock.restart().asSeconds());

        // Inputs
        GetInputs();

        // clear the window with black color
        draw->GetWindow()->clear(sf::Color::Black);

        // draw everything here...
        // window.draw(...);
        
        draw->MovePlayer(direction);
        draw->MoveProjectiles();

        draw->DrawSquares();
        draw->DrawPlayer();
        draw->DrawProjectiles();
        draw->DrawProjectiles();

        // end the current frame
        draw->GetWindow()->display();
    }
}

void Player::GetInputs()
{
    // check all the window's events that were triggered since the last iteration of the loop
    sf::Event event;
    while (draw->GetWindow()->pollEvent(event))
    {
        // "close requested" event: we close the window
        if (event.type == sf::Event::Closed)
            draw->GetWindow()->close();

        if (event.type == sf::Event::KeyPressed)
        {

            if (event.key.code == sf::Keyboard::W)
            {
                //_playerDirection->y = -_velocity * delta;
                direction.y = -velocity;
            }
            else if (event.key.code == sf::Keyboard::S)
            {
                //_playerDirection->y = _velocity * delta;
                direction.y = velocity;
            }


            if (event.key.code == sf::Keyboard::A)
            {
                //_playerDirection->x = -_velocity * delta;
                direction.x = -velocity;
            }
            else if (event.key.code == sf::Keyboard::D)
            {
                //_playerDirection->x = _velocity * delta;
                direction.x = velocity;
            }

            if (event.key.code == sf::Keyboard::Left && !shootKeyPressed)
            {
                draw->AddProjectile(20.f, 20.f, draw->GetPlayerTex().getPosition().x, draw->GetPlayerTex().getPosition().y, -1.0f, 0.0f, draw->GetDelta());
                shootKeyPressed = true;
            }
            else if (event.key.code == sf::Keyboard::Right && !shootKeyPressed)
            {
                draw->AddProjectile(20.f, 20.f, draw->GetPlayerTex().getPosition().x, draw->GetPlayerTex().getPosition().y, 1.0f, 0.0f, draw->GetDelta());
                shootKeyPressed = true;
            }
            else if (event.key.code == sf::Keyboard::Up && !shootKeyPressed)
            {
                draw->AddProjectile(20.f, 20.f, draw->GetPlayerTex().getPosition().x, draw->GetPlayerTex().getPosition().y, 0.0f, -1.0f, draw->GetDelta());
                shootKeyPressed = true;
            }
            else if (event.key.code == sf::Keyboard::Down && !shootKeyPressed)
            {
                draw->AddProjectile(20.f, 20.f, draw->GetPlayerTex().getPosition().x, draw->GetPlayerTex().getPosition().y, 0.0f, 1.0f, draw->GetDelta());
                shootKeyPressed = true;
            }

        }

        if (event.type == sf::Event::KeyReleased)
        {
            if (event.key.code == sf::Keyboard::W)
            {
                direction.y = 0.0f;
            }
            else if (event.key.code == sf::Keyboard::S)
            {
                direction.y = 0.0f;
            }

            if (event.key.code == sf::Keyboard::A)
            {
                direction.x = 0.0f;
            }
            else if (event.key.code == sf::Keyboard::D)
            {
                direction.x = 0.0f;
            }

            if (event.key.code == sf::Keyboard::Left)
            {

                shootKeyPressed = false;
            }
            else if (event.key.code == sf::Keyboard::Right)
            {

                shootKeyPressed = false;
            }
            else if (event.key.code == sf::Keyboard::Up)
            {

                shootKeyPressed = false;
            }
            else if (event.key.code == sf::Keyboard::Down)
            {

                shootKeyPressed = false;
            }
        }

    }
}

sf::Vector2f Player::GetPlayerPos()
{
	return draw->GetSquarepos(0);
}
