#include <SFML/Graphics.hpp>
#include <math.h>
#include <vector>
int width = 640, height = 640;
int res = 32;
int gameSize = res*16;

float vfator = 0.2;

sf::Color headColor = sf::Color::Red;
sf::Color bodyColor = sf::Color::White;
sf::Color bodyColor2 = sf::Color::Cyan;

sf::Vertex quad[4];
sf::RectangleShape border;
void draw_border(sf::RenderWindow& target);
void draw_border(sf::RenderWindow& target) 
{
    int thickness = 5;
    sf::Vector2f size = {width-thickness*2, height-thickness*2};
    border.setSize(size);
    // border.setOrigin(0,0);
    border.setPosition(5,5);
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineColor(sf::Color::Red);
    border.setOutlineThickness(5.f);

    target.draw(border);
    // target.display();
}

class Worm{
    float v[2] = {0, 0};
    int size;
    // sf::RectangleShape* body;
    // sf::RectangleShape body[10];
    std::vector<sf::RectangleShape> body;
public:
    Worm(int x, int y) : size(1) {
        sf::Vector2f resolution((float) res, (float) res);
        // body = new sf::RectangleShape();
        sf::RectangleShape newBody;
        newBody.setSize(resolution);
        newBody.setPosition( (float) x, (float) y);
        newBody.setFillColor(headColor);
        newBody.setOrigin({(float) res/2, (float) res/2});
        body.push_back(newBody);
        
    }

    ~Worm() {
        body.clear();
    }

    void grow() {
        size++;
        body.push_back(sf::RectangleShape());
        // body = new sf::RectangleShape[size];
        body[size-1].setSize({res, res});
        sf::Vector2f pos = body[size-1].getPosition();
        body[size-1].setPosition({pos.x - res*v[0], pos.y - res*v[1]});
        if(size % 2 == 0) body[size-1].setFillColor(bodyColor);
        else body[size-1].setFillColor(bodyColor2);
        printf("LOG: Growed\n");
        
    }

    void update() {
        auto pos = body[0].getPosition();
        float dx = pos.x + v[0]*res;
        float dy = pos.y + v[1]*res;
        if (dx >= gameSize) dx = 0;
        if (dx < 0) dx = gameSize;
        if (dy >= gameSize) dy = 0;
        if (dy < 0) dy = gameSize;
        
        
        int i = size-1;
        // Moving body
        while(i >= 0) {
            pos = body[i-1].getPosition();
            body[i].setPosition(pos);
            i--;
        
        }
        //Moving Head
        body[0].setPosition(dx, dy);
            
        
        
    }

    void draw(sf::RenderWindow &target){
        int i = 0;
        while(i < size) {
            target.draw(body[i]);
            i = i + 1;
        }
        
    }

    void up() {
        v[0] = 0;
        v[1] = -1 * vfator;
    }

    void down() {
        v[0] = 0;
        v[1] = 1 * vfator;
    }

    void left() {
        v[0] = -1 * vfator;
        v[1] = 0;
    }

    void right() {
        v[0] = 1 * vfator;
        v[1] = 0;
    }
};

int main()
{
    sf::Font font;
    if (!font.loadFromFile("CaviarDreams.ttf")) 
    {
        printf("Erro!");
    }
    sf::Text text;
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
        
        srand(time(NULL));
        // x = rand() % gameSize;
        // y = rand() % gameSize;
        x = 0, y = 0;
        worm = new Worm(x*res, y*res);
    }

    
    window.setFramerateLimit(30);
    

    while (window.isOpen())
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
                else if(event.key.code == sf::Keyboard::Space)
                {
                    worm->grow();
                }
                key = event.key.code;
            }
            
                
        }
        sf::String str("Last key: ");
        str.insert(str.getSize(), char('A' + (int)okey));
        if(okey == key) {
            str.insert(str.getSize(), "");
        }
        else keycount = 0;
        okey = key != ' ' ? key : okey;
        // printf("Key: %c\n", 'A' + key);
        text.setString(str);
        text.setPosition(0*16, res*16);
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::White);
        text.setFont(font);
        window.clear();
        // window.draw(border);
        draw_border(window);
        // Game area
        sf::RectangleShape gameArea(sf::Vector2f(res*16, res*16));
        gameArea.setFillColor(sf::Color::Transparent);
        gameArea.setOutlineThickness(1.f);
        window.draw(gameArea);

        // Player
        sf::RectangleShape player(sf::Vector2f(res, res));
        player.setPosition(playerPos);
        player.setFillColor(sf::Color::Green);
        
        worm->update();
        worm->draw(window);
        // window.draw(player);
       
        window.draw(text);
        window.display();

    }

    return 0;
}