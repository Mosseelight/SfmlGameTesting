#ifndef VECTORFIELD_H
#define VECTORFIELD_H

#include <SFML/Graphics.hpp>

class VectorField
{
public:
    void CreateVectorField();
    void ApplyFieldDirection();
    void DrawVectorSim(sf::RenderWindow& window, sf::Sprite& arrow);
};


#endif 