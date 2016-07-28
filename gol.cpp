#include<SFML/Graphics.hpp>
#include<SFML/System.hpp>
#include <SFML/Audio.hpp>
#include<SFML/Network.hpp>
#include<iostream>
#include<fstream>
#include<cctype>
#include<string>
#include<vector>
#include<sstream>

#include<cmath>
using namespace std;
sf::RenderWindow Window(sf::VideoMode(810, 480, 32), "Game Of Life");

class createRect {
public:
    sf::RectangleShape rectangle;
    int top,bottom,left,right;
    void construct (sf::Vector2i position, int size, sf::Color color, bool border = false) {
        rectangle.setPosition(sf::Vector2f(position.x,position.y));
        rectangle.setSize(sf::Vector2f(size,size));
        rectangle.setFillColor(color);
        if(border) {
            rectangle.setOutlineColor(sf::Color::White);
            rectangle.setOutlineThickness(1);
        }
    }
    void update (int x, int y) {
        bottom = x + rectangle.getSize().x;
        top = x;
        right = y + rectangle.getSize().y;
        left = y;
    }
    bool check (sf::Vector2i what) {
        if(what.y<left || what.x>bottom || what.x<top || what.y>right)
        {
            return false;
        }
            return true;
    }
    void setPosition (sf::Vector2i position) {
        rectangle.setPosition(sf::Vector2f(position.x,position.y));
    }
    void draw() {
        Window.draw(rectangle);
    }
};
int neighbors (vector<vector<bool> > &map, int x, int y) {
    int amount = 0;
    for (int i = x - 1; i <= x+ 1; i++) {
        for(int j = y-1; j <=y + 1;j++) {
            if(i==x && j==y) continue;
            if(i<0) continue;
            if(j<0) continue;
            if(i>=map.size()) continue;
            if(j>=map[i].size()) continue;
           // cout<<i<<" "<<j<<endl;
            if(map[i][j]) {
                amount ++;
            }
        }
    }
    return amount;
}
int main() {
    createRect cell; cell.construct(sf::Vector2i(0,0),15,sf::Color::White);
    createRect mapcell; mapcell.construct(sf::Vector2i(0,0),15,sf::Color::Black, true);
    int height = Window.getSize().x/15, width = Window.getSize().y/15;
    vector<vector<bool> > map(height);
    for(int i=0;i<map.size();i++) {
        map[i].resize(width);
    }
    vector<vector<bool> > prevmap(map);
    cout<<map.size()<<" "<<map[0].size()<<endl;
    sf::Vector2i mp;
    bool pause = true;
    sf::Clock speed;
    while(Window.isOpen())
    {
        sf::Event Event;
        while(Window.pollEvent(Event))
        {
            switch (Event.type)
            {
                case sf::Event::Closed:
                    Window.close();
                    break;
                case sf::Event::KeyPressed:
                    if (Event.key.code == sf::Keyboard::Escape)
                    {
                        return 0;
                    }
                    if (Event.key.code == sf::Keyboard::Return)
                    {
                        if(pause) 
                        pause = false;
                        else pause = true;
            
                    }
                    break;
            }
        }
        sf::Time timer = speed.getElapsedTime();
        mp = sf::Mouse::getPosition(Window);
        Window.clear();
        if(pause) {
        for(int i=0;i<map.size();i++) {
            for(int j=0;j<map[i].size();j++) {
                if(!map[i][j]) {
                    mapcell.setPosition(sf::Vector2i(i*15,j*15));
                    mapcell.update(i*15,j*15);
                    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    {
                        if(mapcell.check(mp)) {
                            map[i][j]=1;
                        }
                    }
                    mapcell.draw();
                }
                if(map[i][j]) {
                    cell.setPosition(sf::Vector2i(i*15,j*15));
                    cell.update(i*15,j*15);
                    cell.draw();
                }
            }
        }
            
        } //Finish drawing map during pause
         
        if(!pause) {
            
            prevmap = map;
            //starting of step
            for(int i=0;i<map.size();i++) {
                for(int j=0;j<map[i].size();j++) {
                    
                    int amount = neighbors (prevmap,i,j);
                    if(!map[i][j]) {
                        mapcell.setPosition(sf::Vector2i(i*15,j*15));
                        mapcell.update(i*15,j*15);
                        mapcell.draw();
                        if(timer.asSeconds()>=0.1) {
                        if(amount == 3) {
                            map[i][j]=1;
                        }
                            speed.restart();
                        }
                    }
                    if(map[i][j]) {
                        cell.setPosition(sf::Vector2i(i*15,j*15));
                        cell.update(i*15,j*15);
                        cell.draw();
                        if(timer.asSeconds()>=0.1) {
                        if(amount < 2) {
                            map[i][j]=0;
                        }
                        if(amount > 3) {
                            map[i][j]=0;
                        }
                            speed.restart();
                        }
                    }

                    }
                }
            //End of step

        }
        Window.display();
    }

    return 0;
}
