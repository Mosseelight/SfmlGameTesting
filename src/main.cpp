#include <SFML/Graphics.hpp>
#include <Headers/VectorField.hpp>
#include <iostream>

int main(int argc, char *argv[])
{
    sf::Texture texture;
    texture.loadFromFile("src/Content/arrow.png");
    sf::Sprite arrow(texture);
    VectorField vectorField;
    if(argc != 2)
        vectorField.CreateVectorField(50);
    if(argc == 2)
        vectorField.CreateVectorField(atoi(argv[1]));
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "GameTesting");
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        vectorField.ApplyFieldDirection();
        vectorField.DrawVectorSim(window, arrow);
    }

    return 0;
}
