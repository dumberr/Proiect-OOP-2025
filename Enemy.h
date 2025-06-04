#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"
#include "Texture.h"
#include "Player.h"

#include <vector>
#include <iostream>
#include <stdexcept>
#include <memory>

// Claşa abstracta EnemyBase (Tema 2)
class EnemyBase : public Entity {
protected:
    // Folosite de operatorii << si >> (Tema 1)
    virtual void print(std::ostream& os) const override = 0;
    virtual void read(std::istream& is) override = 0;

    // Comparatii interne (folosite la operator==)
    virtual bool equals(const EnemyBase& other) const = 0;

public:
    virtual ~EnemyBase() = default;

    // Returneaza numarul de vieti (sau starea alive/dead)
    virtual bool isAlive() const = 0;
    virtual void takeDamage(int dmg) = 0;

    // Operator== pentru teste (Tema 1)
    friend bool operator==(const EnemyBase& a, const EnemyBase& b) {
        return a.equals(b);
    }
    friend bool operator!=(const EnemyBase& a, const EnemyBase& b) {
        return !(a == b);
    }

    virtual int getX() const = 0;
    virtual int getY() const = 0;


};

// Inamic simplu: asteroid (se misca drept, fara tragere)
class Asteroid : public EnemyBase {
protected:
    void print(std::ostream& os) const override;
    void read(std::istream& is) override;
    bool equals(const EnemyBase& other) const override;

private:
    int posX;
    int posY;
    int speed;
    int health;
    int boomTimer = 0;

public:
    Asteroid(int x, int y, int speed);
    Asteroid(const Asteroid& other);
    Asteroid& operator=(const Asteroid& other);
    ~Asteroid();

    bool isAlive() const override;
    void takeDamage(int dmg) override;
    void update() override;
    void render() const override;

    int getX() const override { return posX; }
    int getY() const override { return posY; }


};

// Inamic shooter (trage proiectile)
class Shooter : public EnemyBase {
protected:
    void print(std::ostream& os) const override;
    void read(std::istream& is) override;
    bool equals(const EnemyBase& other) const override;

private:
    int posX;
    int posY;
    int speed;
    int health;
    int shootInterval;
    int shootCounter;
    int boomTimer = 0;

    // Container de proiectile ale inamicului
    ProjectileContainer<std::pair<int,int>> enemyBullets;

public:
    Shooter(int x, int y, int speed, int shootInterval);
    Shooter(const Shooter& other);
    Shooter& operator=(const Shooter& other);
    ~Shooter();

    bool isAlive() const override;
    void takeDamage(int dmg) override;
    void update() override;
    void render() const override;

    ProjectileContainer<std::pair<int, int>>& getEnemyBullets() { return enemyBullets; }
    const ProjectileContainer<std::pair<int, int>>& getEnemyBullets() const { return enemyBullets; }
    int getX() const override { return posX; }
    int getY() const override { return posY; }


};

// Factory pentru inamici (Tema 3)
class EnemyFactory {
public:
    enum Type { ASTEROID, SHOOTER };

    static std::unique_ptr<EnemyBase> create(Type t, int x, int y);
};

#endif // ENEMY_H
