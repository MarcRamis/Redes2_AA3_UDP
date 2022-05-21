#include "SFML_Draw.h"

SFML_Draw::SFML_Draw()
{
	window = new sf::RenderWindow(sf::VideoMode(800, 600), "My window");

    //UpdateWindow();

    window->setActive(false);
    std::thread tDraw(&SFML_Draw::UpdateWindow, this);
    tDraw.detach();

}

SFML_Draw::~SFML_Draw()
{
    delete window;
}

void SFML_Draw::UpdateWindow()
{
    window->setActive(true);
    while (window->isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window->pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window->close();
        }

        // clear the window with black color
        window->clear(sf::Color::Black);

        // draw everything here...
        // window.draw(...);
        DrawSquares();

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

void SFML_Draw::AddSquare(float _sizeX, float _sizeY)
{
    sf::RectangleShape rectangle(sf::Vector2f(_sizeX, _sizeY));
    squares.push_back(rectangle);
}

void SFML_Draw::DeleteSquare(int _squareID)
{
    squares.erase(squares.begin() + _squareID);
}
