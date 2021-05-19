#include <SFML/Graphics.hpp>
#include "Worm/Worm.hpp"
#include "Worm/Util.h"
#include <math.h>
#include <vector>
#include <string>


// Screen Constants
const int ZERO = 0;
int width = 640, height = 640;
int res = 32;
int tileNumber = 16;
int gameSize = res*16;

// Velocity
float vfator = 4.f;

sf::Vertex quad[4];
sf::RectangleShape border;
void draw_border(sf::RenderWindow& target);
void draw_border(sf::RenderWindow& target) 
{
    // int thickness = 5;
    // sf::Vector2f size = {width-thickness*2, height-thickness*2};
    // border.setSize(size);
    // // border.setOrigin(0,0);
    // border.setPosition(5,5);
    // border.setFillColor(sf::Color::Transparent);
    // border.setOutlineColor(sf::Color::Red);
    // border.setOutlineThickness(5.f);

    // target.draw(border);
    // // target.display();
    sf::RectangleShape tile({res,res});
    for(int i = 0; i < 16; i++) {
        for(int j = 0; j < 16; j++) {
            tile.setPosition({i*res, j*res});
            if((i+j) % 2 == 0) 
                tile.setFillColor(sf::Color::Black);
            else 
                tile.setFillColor(sf::Color({0xFF, 0xFF, 0xFF, 80}));
            target.draw(tile);
        }
    }
}


class Fruit {

    sf::Vector2f pos, size;

public:
    Fruit(sf::Vector2f pos) {
        size = {24.f, 24.f};
        this->pos = pos;
    }

    void Draw(sf::RenderWindow& window) {
        sf::RectangleShape fruit(size);
        fruit.setPosition(pos.x*res + 4, pos.y*res + 4);
        fruit.setFillColor(sf::Color::Green);
        window.draw(fruit);
    }
    
    const sf::Vector2f& getPos() const{
        return pos;
    }

};


int main()
{
    int potuation(0);
    Fruit *fruit;
    bool bLose = false;

    sf::RectangleShape gameArea(sf::Vector2f(res*16, res*16));
    gameArea.setFillColor(sf::Color::Transparent);
    gameArea.setOutlineThickness(1.f);



    sf::Text text;
    sf::Font font;
    sf::String str("Last key: "), strPontuation = "Pontuation: 0";
    // LOAD CUSTOM FONT FILE
    if (!font.loadFromFile("assets/texture/CaviarDreams.ttf")) 
    {
        printf("Erro!");
    }

    char key, okey = '-';
    int keycount = 0U;
    sf::Vector2f playerPos = {0, 0};
    sf::ContextSettings settings;
    settings.antialiasingLevel = 10;
    sf::RenderWindow window(sf::VideoMode(640, 640, 32U), "Worm", sf::Style::Default, settings);
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Red);
    shape.setPosition(sf::Vector2f(100, 100));
    
    Worm *worm;
    {
        int x, y;

        x = randNumber(tileNumber);
        y = randNumber(tileNumber);
        
        fruit = new Fruit({(float) x, (float) y});

        x = randNumber(tileNumber);
        y = randNumber(tileNumber);

        worm = new Worm(x, y, res);
    
    }
    worm->grow();
    
    worm->grow();
    worm->update();
    
    window.setFramerateLimit(30);
    
    

    while (window.isOpen() && !bLose)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                
                if(event.key.code == sf::Keyboard::D)
                {
                    playerPos.x = (playerPos.x + res) >= gameSize ? playerPos.x : playerPos.x + res;
                    worm->right();
                }
                else if(event.key.code == sf::Keyboard::A)
                {
                    playerPos.x = (playerPos.x - res) < 0 ? playerPos.x : playerPos.x - res;
                    worm->left(); 
                }
                else if(event.key.code == sf::Keyboard::W)
                {
                    playerPos.y = (playerPos.y - res) < 0 ? playerPos.y : playerPos.y - res;
                    worm->up();
                }
                else if(event.key.code == sf::Keyboard::S)
                {
                    playerPos.y = (playerPos.y + res) >= gameSize ? playerPos.y : playerPos.y + res;
                    worm->down();
                }
                
                key = event.key.code;
            }
            
                
        }

        // Detecting if the Worm eat the Fruit
        if(worm->isPos(fruit->getPos()))
        {
            worm->grow();
            potuation++;
            delete fruit;
            int x = randNumber(tileNumber),
                y = randNumber(tileNumber);
            fruit = new Fruit({x, y});
            strPontuation = "Pontuation: " + std::to_string(potuation);
        }
        if(worm->isCollided()) {
            printf("YOU LOSE!");
            bLose = true;
            str.clear();
            str = "YOU LOSE";
            text.setFillColor(sf::Color::Red);
            text.setPosition({gameSize/2, gameSize/2});
            text.setString(str);

            window.draw(text);
            window.display();
            sf::sleep(sf::Time(sf::seconds(3)));
            break;
        }


        str.clear();
        str.insert(str.getSize(), char('A' + (int)okey));
        if(okey == key) {
            str.insert(str.getSize(), "");
        }
        else keycount = 0;
        okey = key != ' ' ? key : okey;
        // printf("Key: %c\n", 'A' + key);


        text.setPosition(8, res*16);
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::White);
        text.setFont(font);

        sf::String str2 = str + "\n" + strPontuation;
        text.setString(str2);


        window.clear();
        // window.draw(border);
        draw_border(window);
        // Game area
        
        
        
        
        if(!bLose) {
            if(fruit != nullptr)
                fruit->Draw(window);
        
            window.draw(gameArea);
            window.draw(text);
        
            worm->update();
            worm->draw(window);
        }
        
       
        
        window.display();
        
        sf::sleep(sf::milliseconds(90));

    }


    delete worm;
    delete fruit;
    return 0;
}