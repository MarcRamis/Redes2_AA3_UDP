#include "Player.h"

Player::Player()
{
	draw = new SFML_Draw();
	draw->AddSquare(150, 150);

	std::thread tInputs(&Player::GetInputs, this);
	tInputs.detach();

}

Player::Player(float _posX, float _posY)
{
    velocity = MAX_VEL_PLAYER;
	draw = new SFML_Draw(sf::Vector2f(_posX, _posY), velocity);
}

void Player::Update()
{
    while (draw->GetWindow()->isOpen())
    {
        // Inputs
        GetInputs();

        // clear the window with black color
        draw->GetWindow()->clear(sf::Color::Black);
        
        // draw everything here...
        draw->MoveProjectiles();
        
        draw->DrawSquares();
        draw->DrawPlayer();
        for (PlayerTex *p : other_players)
        {
            draw->GetWindow()->draw(*p->tex);
        }
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
        {
            draw->GetWindow()->close();
            closedGame = true;
        }

        if (event.type == sf::Event::KeyPressed)
        {
            // Move keys
            if (event.key.code == sf::Keyboard::W && !moveKeyPressed)
            {
                //direction.y = -velocity;
                draw->MovePlayer(sf::Vector2f(0.0f, -velocity));
                tmp_Commands.push(CommandList::EType::MOVE_UP);
                moveKeyPressed = true;
            }
            else if (event.key.code == sf::Keyboard::S && !moveKeyPressed)
            {
                //direction.y = velocity;
                draw->MovePlayer(sf::Vector2f(0.0f, velocity));
                tmp_Commands.push(CommandList::EType::MOVE_DOWN);
                moveKeyPressed = true;
            }

            else if (event.key.code == sf::Keyboard::A && !moveKeyPressed)
            {
                //direction.x = -velocity;
                draw->MovePlayer(sf::Vector2f(-velocity, 0.0f));
                tmp_Commands.push(CommandList::EType::MOVE_LEFT);
                moveKeyPressed = true;
            }
            else if (event.key.code == sf::Keyboard::D && !moveKeyPressed)
            {
                //direction.x = velocity;
                draw->MovePlayer(sf::Vector2f(velocity, 0.0f));
                tmp_Commands.push(CommandList::EType::MOVE_RIGHT);
                moveKeyPressed = true;
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
                //direction.y = 0.0f;
                moveKeyPressed = false;
            }
            else if (event.key.code == sf::Keyboard::S)
            {
                //direction.y = 0.0f;
                moveKeyPressed = false;
            }

            if (event.key.code == sf::Keyboard::A)
            {
                //direction.x = 0.0f;
                moveKeyPressed = false;
            }
            else if (event.key.code == sf::Keyboard::D)
            {
                //direction.x = 0.0f;
                moveKeyPressed = false;
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
        draw->GetPlayerTex().getPosition().x + 15, 
        draw->GetPlayerTex().getPosition().y + 15, dirX, dirY, draw->GetDelta());
    shootKeyPressed = true;
}

void Player::ClearCommands()
{
    std::queue<CommandList::EType> empty;
    std::swap(tmp_Commands, empty);
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
    if (draw->GetWindow() == nullptr) return nullptr;
    else return draw->GetWindow();
}

void Player::SetWindow(sf::RenderWindow* _window)
{
    draw->SetWindow(_window);
}

bool Player::IsWindowActive()
{
    if (draw == nullptr)
    {
        return false;
    }
    else
    {
        return draw->IsWindowActive();
    }
}

void Player::AddNewPlayer(int posX, int posY, int _port)
{
    PlayerTex* p = new PlayerTex(new sf::RectangleShape(sf::Vector2f(50, 50)), _port);
    p->tex->setPosition(posX, posY);
    other_players.push_back(p);
}


PlayerTex* Player::FindNewPlayer(int _port)
{
    for (PlayerTex *p : other_players)
    {
        if (p->port == _port) return p;
    }

    return nullptr;
}

void Player::MoveOtherPlayer(sf::Vector2f pos, int _port)
{
    for (PlayerTex *p : other_players)
    {
        if (p->port == _port)
        {
            p->tex->setPosition(pos);
        }
    }
}
