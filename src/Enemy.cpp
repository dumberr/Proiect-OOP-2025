#include "Enemy.h"
#include <utility>
#include <cassert>

// ==================== Asteroid ====================

// Constructor explicit (Tema 1)
Asteroid::Asteroid(int x, int y, int speed_)
    : posX(x), posY(y), speed(speed_), health(1)   // un hit kill
{
}

// Copy constructor (Tema 1)
Asteroid::Asteroid(const Asteroid& other)
    : posX(other.posX), posY(other.posY), speed(other.speed), health(other.health)
{
}

// operator= (Tema 1)
Asteroid& Asteroid::operator=(const Asteroid& other) {
    if (this == &other) return *this;
    posX = other.posX;
    posY = other.posY;
    speed = other.speed;
    health = other.health;
    return *this;
}

// Destructor (Tema 1)
Asteroid::~Asteroid() {
    // nu eliberam nimic dinamic
}

bool Asteroid::isAlive() const {
    return boomTimer > 0 || health > 0;
}

void Asteroid::takeDamage(int dmg) {
    if (health > 0) {
        health -= dmg;
        if (health <= 0) {
            health = 0;
            boomTimer = 30;
        }
    }
}

void Asteroid::update() {
    if (posY < 700) {
        posY += speed;
    }
    if (boomTimer > 0) {
        boomTimer--;
    }
}

void Asteroid::render() const {
    if (health>0) {
        Texture::GetInstance()->Draw("asteroid", posX, posY, 52, 54);
    } else if (boomTimer > 0) {
        Texture::GetInstance()->Draw("boom", posX, posY, 52, 54);
    }
}

void Asteroid::print(std::ostream& os) const {
    os << "Asteroid { x=" << posX
       << ", y=" << posY
       << ", speed=" << speed
       << ", health=" << health
       << " }";
}

void Asteroid::read(std::istream& is) {
    is >> posX >> posY >> speed >> health;
}

bool Asteroid::equals(const EnemyBase& other) const {
    auto p = dynamic_cast<const Asteroid*>(&other);
    if (!p) return false;
    return posX == p->posX
        && posY == p->posY
        && speed == p->speed
        && health == p->health;
}

// ==================== Shooter ====================

// Constructor explicit (Tema 1)
Shooter::Shooter(int x, int y, int speed_, int shootInterval_)
    : posX(x), posY(y), speed(speed_), health(1), shootInterval(shootInterval_), shootCounter(0)
{
}

// Copy constructor (Tema 1)
Shooter::Shooter(const Shooter& other)
    : posX(other.posX),
      posY(other.posY),
      speed(other.speed),
      health(other.health),
      shootInterval(other.shootInterval),
      shootCounter(other.shootCounter),
      enemyBullets(other.enemyBullets)
{
}

// operator= (Tema 1)
Shooter& Shooter::operator=(const Shooter& other) {
    if (this == &other) return *this;
    posX = other.posX;
    posY = other.posY;
    speed = other.speed;
    health = other.health;
    shootInterval = other.shootInterval;
    shootCounter = other.shootCounter;
    enemyBullets = other.enemyBullets;
    return *this;
}

// Destructor (Tema 1)
Shooter::~Shooter() {
    // nu eliberam nimic dinamic
}

bool Shooter::isAlive() const {
    return boomTimer > 0 || health > 0;
}

void Shooter::takeDamage(int dmg) {
    if (health > 0) {
        health -= dmg;
        if (health <= 0) {
            health = 0;
            boomTimer = 30;
        }
    }
}

void Shooter::update() {
    if (posY < 700) {
        posY += speed;
    }
    if (boomTimer > 0) {
        boomTimer--;
    }
    // Trage la interval
    if (shootCounter == 0 && isAlive()) {
        enemyBullets.add({posX, posY + 64});
        shootCounter = shootInterval;
    }
    if (shootCounter > 0) --shootCounter;

    // Actualizam proiectilele inamicului
    ProjectileContainer<std::pair<int,int>> newEB;
    for (auto& p : enemyBullets) {
        int nx = p.first;
        int ny = p.second + 2;
        if (ny < 700) {
            newEB.add({nx, ny});
        }
    }
    enemyBullets = std::move(newEB);
}

void Shooter::render() const {
    if (health >0) {
        Texture::GetInstance()->Draw("shooter", posX, posY, 58, 64);

        }
    if (boomTimer > 0)
        {
            Texture::GetInstance()->Draw("boom", posX, posY, 58, 64);
        }
    for (auto& p : enemyBullets) {
        Texture::GetInstance()->Draw("enemyBullet", p.first, p.second, 58, 18);
    }
}

void Shooter::print(std::ostream& os) const {
    os << "Shooter { x=" << posX
       << ", y=" << posY
       << ", speed=" << speed
       << ", health=" << health
       << ", shootInterval=" << shootInterval
       << ", bullets=" << enemyBullets.size()
       << " }";
}

void Shooter::read(std::istream& is) {
    is >> posX >> posY >> speed >> health >> shootInterval;
    // nu citim proiectilele direct, ci resetam container-ul
    enemyBullets.clear();
}

bool Shooter::equals(const EnemyBase& other) const {
    auto p = dynamic_cast<const Shooter*>(&other);
    if (!p) return false;
    if (posX != p->posX || posY != p->posY || speed != p->speed || health != p->health || shootInterval != p->shootInterval) {
        return false;
    }
    if (enemyBullets.size() != p->enemyBullets.size()) return false;
    auto it1 = enemyBullets.begin();
    auto it2 = p->enemyBullets.begin();
    for (; it1 != enemyBullets.end() && it2 != p->enemyBullets.end(); ++it1, ++it2) {
        if (*it1 != *it2) return false;
    }
    return true;
}

// ==================== EnemyFactory ====================

// Factory (Tema 3)
std::unique_ptr<EnemyBase> EnemyFactory::create(Type t, int x, int y) {
    switch (t) {
        case ASTEROID:
            return std::make_unique<Asteroid>(x, y, 1);   // viteza 1 ca exemplu
        case SHOOTER:
            return std::make_unique<Shooter>(x, y, 1, 500); // speed 1, interval 100
        default:
            throw std::invalid_argument("EnemyFactory: tip necunoscut");
    }
}
