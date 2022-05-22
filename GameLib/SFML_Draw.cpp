#include "SFML_Draw.h"

SFML_Draw::SFML_Draw()
{
}

SFML_Draw::SFML_Draw(sf::Vector2f _playerPosition, float _velocity) : velocity(_velocity)
{
    myPlayerTex = sf::RectangleShape(sf::Vector2f(50.f, 50.f));
    myPlayerTex.setPosition(_playerPosition);

    std::thread tDraw(&SFML_Draw::UpdateWindow, this);
    tDraw.detach();
}

SFML_Draw::~SFML_Draw()
{
    delete window;
}

void SFML_Draw::UpdateWindow()
{
    window = new sf::RenderWindow(sf::VideoMode(WIN_SIZE_X, WIN_SIZE_Y), "My window");

    sf::Clock clock;

    while (window->isOpen())
    {

        float delta = clock.restart().asSeconds();

        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window->pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window->close();

            if (event.type == sf::Event::KeyPressed)
            {

                if (event.key.code == sf::Keyboard::W)
                {
                    //_playerDirection->y = -_velocity * delta;
                    direction.y = -velocity * delta;
                }
                else if (event.key.code == sf::Keyboard::S)
                {
                    //_playerDirection->y = _velocity * delta;
                    direction.y = velocity * delta;
                }


                if (event.key.code == sf::Keyboard::A)
                {
                    //_playerDirection->x = -_velocity * delta;
                    direction.x = -velocity * delta;
                }
                else if (event.key.code == sf::Keyboard::D)
                {
                    //_playerDirection->x = _velocity * delta;
                    direction.x = velocity * delta;
                }

                if (event.key.code == sf::Keyboard::Left && !shootKeyPreesed)
                {
                    AddProjectile(20.f, 20.f, myPlayerTex.getPosition().x, myPlayerTex.getPosition().y, -1.0f, 0.0f, delta);
                    shootKeyPreesed = true;
                }
                else if (event.key.code == sf::Keyboard::Right && !shootKeyPreesed)
                {
                    AddProjectile(20.f, 20.f, myPlayerTex.getPosition().x, myPlayerTex.getPosition().y, 1.0f, 0.0f, delta);
                    shootKeyPreesed = true;
                }
                else if (event.key.code == sf::Keyboard::Up && !shootKeyPreesed)
                {
                    AddProjectile(20.f, 20.f, myPlayerTex.getPosition().x, myPlayerTex.getPosition().y, 0.0f, -1.0f, delta);
                    shootKeyPreesed = true;
                }
                else if (event.key.code == sf::Keyboard::Down && !shootKeyPreesed)
                {
                    AddProjectile(20.f, 20.f, myPlayerTex.getPosition().x, myPlayerTex.getPosition().y, 0.0f, 1.0f, delta);
                    shootKeyPreesed = true;
                }

            }

            if (event.type == sf::Event::KeyReleased)
            {
                if (event.key.code == sf::Keyboard::W)
                {
                    direction.y = 0.0f;
                }
                else if(event.key.code == sf::Keyboard::S)
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

                    shootKeyPreesed = false;
                }
                else if (event.key.code == sf::Keyboard::Right)
                {

                    shootKeyPreesed = false;
                }
                else if (event.key.code == sf::Keyboard::Up)
                {

                    shootKeyPreesed = false;
                }
                else if (event.key.code == sf::Keyboard::Down)
                {

                    shootKeyPreesed = false;
                }
            }

        }
        // clear the window with black color
        window->clear(sf::Color::Black);

        // draw everything here...
        // window.draw(...);
        
        DrawSquares();
        DrawPlayer();
        DrawProjectiles();
       

        // end the current frame
        window->display();
    }
}

void SFML_Draw::DrawSquares()
{
    for (sf::RectangleShape square : squares)
    {
        window->draw(square);
    }
}

void SFML_Draw::DrawPlayer()
{
    window->draw(myPlayerTex);
}

void SFML_Draw::AddSquare(float _sizeX, float _sizeY)
{
    sf::RectangleShape rectangle(sf::Vector2f(_sizeX, _sizeY));
    squares.push_back(rectangle);
}

void SFML_Draw::MovePlayer()
{
    myPlayerTex.move(direction.x, direction.y);
}

sf::Vector2f SFML_Draw::GetSquarepos(int _squareID)
{
    return squares[0].getPosition();
}

void SFML_Draw::SetSquarePos(int _squareID, float _newPosX, float _newPosY)
{
    squares[_squareID].setPosition(_newPosX, _newPosY);
}

void SFML_Draw::DeleteSquare(int _squareID)
{
    squares.erase(squares.begin() + _squareID);
}

sf::RenderWindow* SFML_Draw::GetWindow()
{
    return window;
}

void SFML_Draw::AddProjectile(float _sizeX, float _sizeY, float _positionX, float _positionY, float _directionX, float _directionY, float _delta)
{
    sf::RectangleShape projectileTex(sf::Vector2f(_sizeX, _sizeY));
    projectileTex.setPosition(_positionX, _positionY);
    Projectile newPorjectile(sf::Vector2f(_positionX, _positionY), sf::Vector2f(_directionX, _directionY), 0.1f * _delta, projectileTex);
    projectiles.push_back(newPorjectile);
}

void SFML_Draw::MoveProjectiles()
{
    for (Projectile& projectile : projectiles)
    {
        if (projectile.DeleteItem())
        {
            for (int i = 0; i < projectiles.size(); ++i)
            {
                if (projectile.GetTexture().getPosition() == projectiles[i].GetTexture().getPosition())
                {
                    projectiles.erase(projectiles.begin() + i);
                }
            }
        }
        else
        {
            projectile.Update();
        }
        
    }
}

void SFML_Draw::DrawProjectiles()
{
    for (Projectile projectile : projectiles)
    {
        window->draw(projectile.GetTexture());
    }
}
