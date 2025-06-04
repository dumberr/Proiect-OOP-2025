#include "Player.h"
#include "Texture.h"
#include <cassert>
#include <SDL.h> // pentru input tastatura

// Constructor explicit (Tema 1)
Player::Player(int x, int y, int lives_, int playerID)
    : posX(x), posY(y), lives(lives_), shootDelayCounter(0), playerID(playerID)
{
}

// Copy constructor (Tema 1)
Player::Player(const Player& other)
    : posX(other.posX),
      posY(other.posY),
      lives(other.lives),
      shootDelayCounter(other.shootDelayCounter),
      bullets(other.bullets) // vectorul este copiat pe deep copy implicit
{
}

// operator= (Tema 1), implementat prin copy-and-swap (rule of three)
Player& Player::operator=(const Player& other) {
    if (this == &other) return *this;
    Player temp(other);
    std::swap(posX, temp.posX);
    std::swap(posY, temp.posY);
    std::swap(lives, temp.lives);
    std::swap(shootDelayCounter, temp.shootDelayCounter);
    std::swap(bullets, temp.bullets);
    return *this;
}

// Destructor (Tema 1)
Player::~Player() {
    // Nu avem delete, deci nu trebuie sa eliberam nimic aici
}

// Mutare pe ecran (clamp pe interior)
void Player::move(int dx, int dy) {
    posX += dx;
    posY += dy;
    clampPosition();
}

// Trage (adauga un proiectil in container daca a trecut delay-ul)
void Player::shoot() {
    if (shootDelayCounter == 0 && lives > 0) {
        bullets.add({posX, posY - 1});
        shootDelayCounter = DELAY_SHOOT;
    }
}

// Primeste daune, daca ajunge la 0, e considerat mort
void Player::takeDamage(int amount) {
    lives -= amount;
    if (lives < 0) lives = 0;
}

// Verifica daca e mort
bool Player::isDead() const {
    return lives <= 0;
}

// update() este apelat de „Ecran” si trateaza input-ul de la tastatura
void Player::update() {
    // *** ADAUGAT: tratare tastatura pentru miscare si tragere ***
    const Uint8* keystates = SDL_GetKeyboardState(NULL);

    if (playerID == 1) {
        if (keystates[SDL_SCANCODE_A])  move(-5, 0);
        if (keystates[SDL_SCANCODE_D]) move(5, 0);
        if (keystates[SDL_SCANCODE_SPACE]) shoot();
    } else if (playerID == 2) {
        if (keystates[SDL_SCANCODE_LEFT]) move(-5, 0);
        if (keystates[SDL_SCANCODE_RIGHT]) move(5, 0);
        if (keystates[SDL_SCANCODE_KP_ENTER]) shoot();
    }


    if (shootDelayCounter > 0) {
        --shootDelayCounter;
    }
    // Actualizam pozitia proiectilelor
    ProjectileContainer<std::pair<int,int>> newBullets;
    for (auto& p : bullets) {
        int newX = p.first;
        int newY = p.second - 2;
        if (newY >= 0) {
            newBullets.add({newX, newY});
        }
    }
    bullets = std::move(newBullets);
}

// render() deseneaza jucatorul si proiectilele
void Player::render() const {
    if (lives > 0) {
        Texture::GetInstance()->Draw("player", posX, posY, PLAYER_WIDTH, PLAYER_HEIGHT);
        // Desenam proiectilele
        for (auto& p : bullets) {
            Texture::GetInstance()->Draw("bullet", p.first, p.second, 58, 18);
        }
    }
}

// Operator<< (Tema 1)
void Player::print(std::ostream& os) const {
    os << "Player { x=" << posX
       << ", y=" << posY
       << ", lives=" << lives
       << ", bullets=" << bullets.size()
       << " }";
}

// Operator>> (Tema 1)
void Player::read(std::istream& is) {
    is >> posX >> posY >> lives;
    bullets.clear();
    // Citire simpla: numarul de proiectile si pozitiile lor
    size_t count;
    is >> count;
    for (size_t i = 0; i < count; ++i) {
        int bx, by;
        is >> bx >> by;
        bullets.add({bx, by});
    }
}

// Accesorii - Tema 1
int Player::getX() const { return posX; }
int Player::getY() const { return posY; }
int Player::getLives() const { return lives; }

// Helper privat: nu iese din ecran (500×700)
void Player::clampPosition() {
    if (posX < 0) posX = 0;
    if (posX > 500 - PLAYER_WIDTH)  posX = 500 - PLAYER_WIDTH;
    if (posY < 0) posY = 0;
    if (posY > 700 - PLAYER_HEIGHT) posY = 700 - PLAYER_HEIGHT;
}

// Suprascriere operator== si != pentru teste (Tema 1)
bool operator==(const Player& a, const Player& b) {
    if (a.posX != b.posX || a.posY != b.posY || a.lives != b.lives) return false;
    if (a.bullets.size() != b.bullets.size()) return false;
    auto it1 = a.bullets.begin();
    auto it2 = b.bullets.begin();
    for (; it1 != a.bullets.end() && it2 != b.bullets.end(); ++it1, ++it2) {
        if (*it1 != *it2) return false;
    }
    return true;
}

bool operator!=(const Player& a, const Player& b) {
    return !(a == b);
}



std::ostream& operator<<(std::ostream& out, const Player& p) {
    p.print(out);
    return out;
}

std::istream& operator>>(std::istream& in, Player& p) {
    p.read(in);
    return in;
}


