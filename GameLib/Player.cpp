#include "Player.h"
#include <iostream>

Player::Player()
{
	draw = new SFML_Draw();
	draw->AddSquare(150, 150);

	std::thread tInputs(&Player::GetInputs, this);
	tInputs.detach();

}

Player::Player(float _posX, float _posY)
{
    velocity = 100.f;
	draw = new SFML_Draw(sf::Vector2f(_posX, _posY), velocity);
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
            // Move keys
            if (event.key.code == sf::Keyboard::W)
            {
                direction.y = -velocity;
                tmp_Commands.push(CommandList::EType::MOVE_UP);
            }
            else if (event.key.code == sf::Keyboard::S)
            {
                direction.y = velocity;
                tmp_Commands.push(CommandList::EType::MOVE_DOWN);
            }

            else if (event.key.code == sf::Keyboard::A)
            {
                direction.x = -velocity;
                tmp_Commands.push(CommandList::EType::MOVE_LEFT);
            }
            else if (event.key.code == sf::Keyboard::D)
            {
                direction.x = velocity;
                tmp_Commands.push(CommandList::EType::MOVE_RIGHT);
            }
            
            // Shoot keys
            if (event.key.code == sf::Keyboard::Left && !shootKeyPressed)
            {
                Shoot(-1.f, 0.f);
                tmp_Commands.push(CommandList::EType::SHOOT_LEFT);
            }
            else if (event.key.code == sf::Keyboard::Right && !shootKeyPressed)
            {
                Shoot(1.f, 0.f);
                tmp_Commands.push(CommandList::EType::SHOOT_RIGHT);
            }
            else if (event.key.code == sf::Keyboard::Up && !shootKeyPressed)
            {
                Shoot(0.f, -1.f);
                tmp_Commands.push(CommandList::EType::SHOOT_UP);
            }
            else if (event.key.code == sf::Keyboard::Down && !shootKeyPressed)
            {   
                Shoot(0.f, 1.f);
                tmp_Commands.push(CommandList::EType::SHOOT_DOWN);
            }

        }

        // Stop movement
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

void Player::Shoot(float dirX, float dirY)
{
    draw->AddProjectile(20.f, 20.f,
        draw->GetPlayerTex().getPosition().x, 
        draw->GetPlayerTex().getPosition().y, dirX, dirY, draw->GetDelta());
    shootKeyPressed = true;
}

void Player::ClearCommands()
{
    std::queue<CommandList::EType> empty;
    std::swap(tmp_Commands, empty);
    std::cout << "Clear size: " << tmp_Commands.size() << std::endl;
}

sf::Vector2f Player::GetPlayerPos()
{
    return draw->GetPosition();
}
void Player::SetPlayerPos(sf::Vector2f newPos)
{
    draw->GetPlayerTex().setPosition(newPos);
}

void Player::NewWindow()
{
    draw->NewWindow();
}

sf::RenderWindow* Player::GetWindow()
{
    return draw->GetWindow();
}

void Player::SetWindow(sf::RenderWindow* _window)
{
    draw->SetWindow(_window);
}
