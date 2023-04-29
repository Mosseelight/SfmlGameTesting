#include <../Headers/VectorField.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <iterator>
#include <iostream>
#include <list>
#include <cmath>
#include <stdlib.h>
#include <algorithm>

struct Object
{
public:
    sf::Vector2f pos;
    sf::Vector2f curDir;
};

sf::Vector2f* vectorsDirs;
sf::Vector2f* vectorsPos;
sf::Vector2f screenMiddle(960, 540);
int vectorFieldXLen = 36;
int vectorFieldXLenOffset = 400;
int vectorFieldYLen = 36;
int vectorFieldYLenOffset = 0;
float vectorFieldScale = 30.0f;
int vectorFieldXLenDir;
int vectorFieldYLenDir;
float speed = 0.5f;
int dirCount;
int index = 0;

//objects
std::vector<sf::Vector2f>* objPos;
std::vector<sf::Vector2f>* objCurDir;

sf::Vector2f CalculateVectorValue(float x, float y)
{
    return sf::Vector2f(-y,x);
}

sf::Vector2f NormalizeVector(sf::Vector2f& v)
{
    float length = sqrt((v.x * v.x) + (v.y * v.y));
    if (length != 0)
        return sf::Vector2f(v.x / length, v.y / length);
    else
        return v;
}

float getDistance(sf::Vector2f& v1, sf::Vector2f& v2) {
    return std::sqrt(((v2.x - v1.x) * (v2.x - v1.x)) + ((v2.y - v1.y) * (v2.y - v1.y)));
}

void VectorField::CreateVectorField()
{
    if(vectorFieldXLen % 2 == 1)
    {
        vectorFieldXLen++;
    }
    if(vectorFieldYLen % 2 == 1)
    {
        vectorFieldYLen++;
    }
    vectorFieldXLenDir = vectorFieldXLen / 2;
    vectorFieldYLenDir = vectorFieldYLen / 2;
    vectorsDirs = new sf::Vector2f[vectorFieldXLen * vectorFieldYLen];
    vectorsPos = new sf::Vector2f[vectorFieldXLen * vectorFieldYLen];
    objPos = new std::vector<sf::Vector2f>[2];
    objCurDir = new std::vector<sf::Vector2f>[2];
    for (int y = 0; y < vectorFieldYLen; y++)
    {
        for (int x = 0; x < vectorFieldXLen; x++)
        {
            vectorsPos[x + vectorFieldXLen * y].x = x * vectorFieldScale + vectorFieldXLenOffset;
            vectorsPos[x + vectorFieldXLen * y].y = y * vectorFieldScale + vectorFieldYLenOffset;
        }
    }

    for (int y = -vectorFieldYLenDir; y < vectorFieldYLenDir; y++)
    {
        for (int x = -vectorFieldXLenDir; x < vectorFieldXLenDir; x++)
        {
            vectorsDirs[dirCount] = CalculateVectorValue(x, y);
            dirCount++;
        }
    }
    Object obj;
    obj.pos = sf::Vector2f(1 + (rand() % 1400),1 + (rand() % 1080));
    obj.curDir = sf::Vector2f(0, 0);
    objPos->push_back(obj.pos);
    objCurDir->push_back(obj.curDir);
}

void VectorField::ApplyFieldDirection()
{
    for (int o = 0; o < objPos->size(); o++)
    {
        std::vector<float> distances;
        float minDist = 0;
        for (int i = 0; i < vectorFieldXLen * vectorFieldYLen; i++)
        {
            distances.push_back(getDistance(vectorsPos[i], objPos->at(o)));
            if(distances.size() == vectorFieldXLen * vectorFieldYLen)
            {
                auto result = std::min_element(std::begin(distances), std::end(distances));
                if (std::end(distances)!=result)
                    minDist = *result;
            }
        }
        for (int i = 0; i < distances.size(); i++)
        {
            if(minDist == distances[i])
            {
                index = i;
                break;
            }
        }
        objCurDir->at(o).x = vectorsDirs[index].x * 1;
        objCurDir->at(o).y = vectorsDirs[index].y * 1;
        objPos->at(o) += NormalizeVector(objCurDir->at(o)) * speed;
        distances.clear();
    }
}

void CheckBounds()
{
    if(objPos->size() < 1)
    {
        for (int i = 0; i < objPos->size(); i++)
        {
            if(objPos->at(i).x < 0)
            {
                objPos->at(i) = sf::Vector2f(vectorFieldXLenDir, vectorFieldYLenDir);
                objCurDir->at(i) = sf::Vector2f(0,0);
            }
            if(objPos->at(i).x > 1920)
            {
                objPos->at(i) = sf::Vector2f(vectorFieldXLenDir, vectorFieldYLenDir);
                objCurDir->at(i) = sf::Vector2f(0,0);
            }
            if(objPos->at(i).y < 0)
            {
                objPos->at(i) = sf::Vector2f(vectorFieldXLenDir, vectorFieldYLenDir);
                objCurDir->at(i) = sf::Vector2f(0,0);
            }
            if(objPos->at(i).y > 1080)
            {
                objPos->at(i) = sf::Vector2f(vectorFieldXLenDir, vectorFieldYLenDir);
                objCurDir->at(i) = sf::Vector2f(0,0);
            }
        }
        
    }
}

void VectorField::DrawVectorSim(sf::RenderWindow& window, sf::Sprite& arrow)
{
    CheckBounds();
    sf::CircleShape circle(16.0f);
    circle.setFillColor(sf::Color::Red);
    window.clear(sf::Color::White);
    for (int i = 0; i < vectorFieldXLen * vectorFieldYLen; i++)
    {
        float angle = std::atan2(vectorsDirs[i].y, vectorsDirs[i].x)*180/M_PI;
        arrow.setPosition(vectorsPos[i]);
        arrow.setRotation(angle);
        window.draw(arrow);
    }
    
    for (int o = 0; o < objPos->size(); o++)
    {
        circle.setPosition(objPos->at(o));
        window.draw(circle);
    }
    window.display();
}

