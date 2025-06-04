#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "Texture.h"

#include <vector>
#include <utility>
#include <iostream>


constexpr int PLAYER_WIDTH = 58;
constexpr int PLAYER_HEIGHT = 64;
constexpr int DELAY_SHOOT = 25;

// Tema 3: Template pentru un container simplu de proiectile
template<typename T>
class ProjectileContainer {
private:
    std::vector<T> projectiles;

public:
    ProjectileContainer() = default;
    // constructor de copiere si operator= vor fi compiler-generated

    void add(const T& t) {
        projectiles.push_back(t);
    }
    void clear() {
        projectiles.clear();
    }
    size_t size() const {
        return projectiles.size();
    }
    // Tema 3: metodă dependentă de T
    template<typename U>
    void logAll(const U& logger) const {
        for (const auto& p : projectiles) {
            logger(std::cout, p);
        }
    }

    // Iteratori simpli
    typename std::vector<T>::const_iterator begin() const { return projectiles.begin(); }
    typename std::vector<T>::const_iterator end() const   { return projectiles.end();   }

    std::vector<T>& getVector() { return projectiles; }
    const std::vector<T>& getVector() const { return projectiles; }

    //fucntie template care numara bullets
    template<typename Predicate>
    int countIf(Predicate pred) const {
        int cnt = 0;
        for (const auto& elem : projectiles) {
            if (pred(elem)) ++cnt;
        }
        return cnt;
    }

};

// Clasa Player derivată din Entity (Tema 2)
class Player : public Entity {
private:
    int posX;
    int posY;
    int lives;

    int shootDelayCounter;  // contor pentru limitarea tirului

    // Container generic de proiectile (Tema 3)
    ProjectileContainer<std::pair<int,int>> bullets;

    // Helper privat
    void clampPosition();

    // Pentru testare (Tema 1), vom suprascrie operator==
    friend bool operator==(const Player& a, const Player& b);
    friend bool operator!=(const Player& a, const Player& b);

    int playerID;
    
public:
    // Tema 1: constructor explicit de initializare
    explicit Player(int x = 200, int y = 500, int lives = 3, int playerID = 1);

    // Regula celor trei (Tema 1)
    Player(const Player& other);
    Player& operator=(const Player& other);
    ~Player();

    // Accesori publici (minim 3 functii netriviale)
    void move(int dx, int dy);
    void shoot();
    void takeDamage(int amount);
    bool isDead() const;

    // Suprascriem Entity
    void update() override;
    void render() const override;


    // Alte metode utile
    int getX() const;
    int getY() const;
    int getLives() const;
    ProjectileContainer<std::pair<int, int>>& getBullets() { return bullets; }
    const ProjectileContainer<std::pair<int, int>>& getBullets() const { return bullets; }

    // Tema 1: operator<< si operator>> (delegat la print/read)
    void print(std::ostream& os) const override;
    void read(std::istream& is) override;

    void setX(int x_) { posX = x_; }
    void setY(int y_) { posY = y_; }
    void setHealth(int l_) { lives = l_; }

    friend std::ostream& operator<<(std::ostream& out, const Player& p);
    friend std::istream& operator>>(std::istream& in, Player& p);


};

#endif // PLAYER_H
