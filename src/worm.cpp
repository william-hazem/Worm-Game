#include "Worm/Worm.hpp"

/// \todo MOVE TO SERVER SIDE 
int RES = 32;
int GAMESIZE = RES*16;
// 8 pixel per second
float VFATOR = 8.f;

sf::Color headColor = sf::Color::Red;
sf::Color bodyColor = sf::Color::White;
sf::Color bodyColor2 = sf::Color::Cyan;

Worm::Worm(float x, float y, float size_length)
{
    size = size_length;
    sf::Vector2f headSize((float) size_length, (float) size_length);
    sf::RectangleShape newBody;

    newBody.setSize(headSize);
    newBody.setPosition( (float) x, (float) y);
    bodyPos.push_back({x, y});
    newBody.setFillColor(headColor);

    body.push_back(newBody);
}

Worm::~Worm() {
    body.clear();
    bodyPos.clear();
}

void Worm::grow() {
    int i = body.size();
    printf("%d\n", i);
    body.push_back(sf::RectangleShape());
    // body = new sf::RectangleShape[size];
    body[i].setSize({size, size});
    sf::Vector2f pos = body[size-1].getPosition();
    body[i].setPosition({pos.x - size*v[0], pos.y - size*v[1]});
    // body[size-1].setOrigin({size, 0});
    if((int) i % 2 == 0) body[i].setFillColor(bodyColor);
    else body[i].setFillColor(bodyColor2);
}

void Worm::update() {
    auto pos = body[0].getPosition();
    float dx = pos.x + v[0]*VFATOR;
    float dy = pos.y + v[1]*VFATOR;
    if (dx >= GAMESIZE) dx = 0;
    if (dx < 0) dx = GAMESIZE;
    if (dy >= GAMESIZE) dy = 0;
    if (dy < 0) dy = GAMESIZE;
    
    
    int i = body.size()-1;
    // Moving body
    while(i >= 0) {
        pos = body[i-1].getPosition();
        body[i].setPosition(pos);
        i--;
    
    }
    //Moving Head
    body[0].setPosition(dx, dy);
}

void Worm::draw(sf::RenderWindow &target){
    int i = 0, j = body.size();
    while(i < j) {
        target.draw(body[i]);
        i = i + 1;
    }
    
}

void Worm::up() {
    v[0] = 0;
    v[1] = -1;
}

void Worm::down() {
    v[0] = 0;
    v[1] = 1;
}

void Worm::left() {
    v[0] = -1;
    v[1] = 0;
}

void Worm::right() {
    v[0] = 1;
    v[1] = 0;
}