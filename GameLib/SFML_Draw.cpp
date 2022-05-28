#include "SFML_Draw.h"

SFML_Draw::SFML_Draw()
{
}

SFML_Draw::SFML_Draw(sf::Vector2f _playerPosition, float _velocity) : velocity(_velocity)
{
    myPlayerTex = sf::RectangleShape(sf::Vector2f(50.f, 50.f));
    myPlayerTex.setPosition(_playerPosition);

    window = new sf::RenderWindow(sf::VideoMode(WIN_SIZE_X, WIN_SIZE_Y), "My window");
}

SFML_Draw::~SFML_Draw()
{
    delete window;
}

void SFML_Draw::UpdateWindow()
{
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

void SFML_Draw::MovePlayer(sf::Vector2f newPos)
{
    myPlayerTex.move(newPos.x * delta, newPos.y * delta);
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

float SFML_Draw::GetDelta()
{
    return delta;
}

void SFML_Draw::SetDelta(float _delta)
{
    delta = _delta;
}

void SFML_Draw::AddProjectile(float _sizeX, float _sizeY, float _positionX, float _positionY, float _directionX, float _directionY, float _delta)
{
    sf::RectangleShape projectileTex(sf::Vector2f(_sizeX, _sizeY));
    projectileTex.setPosition(_positionX, _positionY);
    Projectile newPorjectile(sf::Vector2f(_positionX, _positionY), sf::Vector2f(_directionX, _directionY), 0.2f, projectileTex);
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

sf::RectangleShape SFML_Draw::GetPlayerTex()
{
    return myPlayerTex;
}

sf::Vector2f SFML_Draw::GetPosition()
{
    return myPlayerTex.getPosition();
}