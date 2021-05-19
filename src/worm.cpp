#include "Worm/Worm.hpp"

/// \todo MOVE TO SERVER SIDE 
int RES = 32;
int GAMESIZE = RES*16;
// 8 pixel per second
float VFATOR = 1.f;

sf::Color headColor = sf::Color::Red;
sf::Color bodyColor = sf::Color::White;
sf::Color bodyColor2 = sf::Color::Cyan;

Worm::Worm(float x, float y, float size_length)
{
    size = size_length;
    sf::Vector2f headSize((float) size_length, (float) size_length);
    sf::RectangleShape newBody;

    newBody.setSize(headSize);

    bodyPos.push_back({x, y});
    newBody.setFillColor(headColor);

    body.push_back(newBody);
    v[0] = 1;

    
}

Worm::~Worm() {
    body.clear();
    bodyPos.clear();
}

void Worm::grow() {
    int i = body.size();
    body.push_back(sf::RectangleShape());
    bodyPos.push_back(sf::Vector2f());

    // body = new sf::RectangleShape[size];
    body[i].setSize({size, size});
    sf::Vector2f pos = body[i-1].getPosition();
    body[i].setPosition({pos.x - size*v[0], pos.y - size*v[1]});
    // body[size-1].setOrigin({size, 0});
    if((int) i % 2 == 0) body[i].setFillColor(bodyColor);
    else body[i].setFillColor(bodyColor2);

    
    
}

void Worm::update() {
    auto pos = bodyPos[0];
    float dx = pos.x + v[0]*VFATOR;
    float dy = pos.y + v[1]*VFATOR;
    if (dx > GAMESIZE/RES) dx = 0;
    if (dx < 0) dx = GAMESIZE/RES;
    if (dy > GAMESIZE/RES) dy = 0;
    if (dy < 0) dy = GAMESIZE/RES;
    
    
    int i = body.size()-1;
    // Moving body
    while(i > 0) {
        pos = bodyPos[i-1];
        bodyPos[i] = pos;
        i--;
    
    }
    //Moving Head
    bodyPos[0] = {dx, dy};
}

void Worm::draw(sf::RenderWindow &target){
    int i = 0, j = body.size();
    while(i < j) {
        body[i].setPosition(bodyPos[i] * 32.f);
        target.draw(body[i]);
        i = i + 1;
    }
    
}

void Worm::up() {
    if(v[1] == 0) {
        v[0] = 0;
        v[1] = -1;
    }
    
}

void Worm::down() {
    if (v[1] == 0) {
        v[0] = 0;
        v[1] = 1;
    }
    
}

void Worm::left() {
    if(v[0] == 0) {
        v[0] = -1;
        v[1] = 0;
    }
    
}

void Worm::right() {
    if(v[0] == 0) {
        v[0] = 1;
        v[1] = 0;
    }
}

bool Worm::isBorder(float &xi, float &yi, float &xf, float &yf) {
    // Worm head position
    float x = bodyPos[0].x, y = bodyPos[0].y;
    if(x < xi || x > xf || y < yi || y > yf)
        return true;
    return false;
}

bool Worm::isPos(const sf::Vector2f& pos) {
    return this->bodyPos[0] == pos;
}

bool Worm::isCollided() const {
    auto head = bodyPos[0];
    for(int i = 1; i < body.size()-1; i++)
    {
        if(head == bodyPos[i])
            return true;
    }
    return false;

}