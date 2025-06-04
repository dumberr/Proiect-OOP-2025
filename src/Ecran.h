#ifndef ECRAN_H
#define ECRAN_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "Entity.h"
#include "Player.h"
#include "Enemy.h"
#include <vector>
#include <memory>
#include <stdexcept>

constexpr int WIDTH = 500;
constexpr int HEIGHT = 700;
constexpr int DELAY_TIME = 30;
constexpr int NR_NIVELE = 5;

class Ecran {
private:
    Ecran();
    ~Ecran();

    bool ok = false;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    std::vector<std::shared_ptr<Entity>> objects;

    // Stare joc
    bool paused = false;
    bool muted = false;

    // Timere
    int delay_click = 0;
    int delay_esc = 0;
    int delay_paused = 0;
    int delay_mute = 0;

    int mouseX = 0;
    int mouseY = 0;

    // Sistem de deblocare pe 5 niveluri
    std::vector<bool> nivel_deblocat;

    // Pentru win/loss
    bool levelComplete = false;
    bool gameOver = false;
    int win_timer = 0;

public:
    static Ecran* GetInstance() {
        static Ecran instance;
        return &instance;
    }
    Ecran(const Ecran&) = delete;
    Ecran& operator=(const Ecran&) = delete;

    bool Init();
    void Quit();

    void UpdateAll();
    void RenderAll() const;
    void HandleEvents();

    bool Running() const { return ok; }
    SDL_Renderer* GetRenderer() const { return renderer; }

    void addEntity(std::shared_ptr<Entity> obj);
    void clearEntitys();

    // logica lock
    bool isNivelDeblocat(int idx) const {
        if (idx < 0 || idx >= NR_NIVELE) return false;
        return nivel_deblocat[idx];
    }
    void deblocheazaNivel(int idx) {
        if (idx > 0 && idx < NR_NIVELE) nivel_deblocat[idx] = true;
    }

    void genereazaInamiciLevelsolo(int level) {
        addEntity(std::make_shared<Player>(200, 500, 3, 1));

        // Pentru fiecare nivel pui o structura fixa:
        if (level == 0) {
            addEntity(EnemyFactory::create(EnemyFactory::ASTEROID, 100, 60));
            addEntity(EnemyFactory::create(EnemyFactory::ASTEROID, 350, 100));
            addEntity(EnemyFactory::create(EnemyFactory::ASTEROID, 130, 80));
            addEntity(EnemyFactory::create(EnemyFactory::ASTEROID, 270, 60));
            addEntity(EnemyFactory::create(EnemyFactory::ASTEROID, 180, 60));
        } else if (level == 1) {
            addEntity(EnemyFactory::create(EnemyFactory::SHOOTER, 120, 80));
            addEntity(EnemyFactory::create(EnemyFactory::ASTEROID, 340, 90));
            addEntity(EnemyFactory::create(EnemyFactory::SHOOTER, 200, 60));
            addEntity(EnemyFactory::create(EnemyFactory::ASTEROID, 270, 60));
            addEntity(EnemyFactory::create(EnemyFactory::ASTEROID, 100, 60));
        } else if (level == 2) {
            addEntity(EnemyFactory::create(EnemyFactory::ASTEROID, 110, 80));
            addEntity(EnemyFactory::create(EnemyFactory::ASTEROID, 350, 70));
            addEntity(EnemyFactory::create(EnemyFactory::SHOOTER, 250, 90));
            addEntity(EnemyFactory::create(EnemyFactory::ASTEROID, 180, 60));
            addEntity(EnemyFactory::create(EnemyFactory::SHOOTER, 200, 60));
        } else if (level == 3) {
            addEntity(EnemyFactory::create(EnemyFactory::SHOOTER, 120, 90));
            addEntity(EnemyFactory::create(EnemyFactory::ASTEROID, 320, 70));
            addEntity(EnemyFactory::create(EnemyFactory::SHOOTER, 400, 90));
            addEntity(EnemyFactory::create(EnemyFactory::ASTEROID, 250, 60));
            addEntity(EnemyFactory::create(EnemyFactory::ASTEROID, 180, 120));
            addEntity(EnemyFactory::create(EnemyFactory::ASTEROID, 100, 60));
        } else if (level == 4) {
            addEntity(EnemyFactory::create(EnemyFactory::SHOOTER, 110, 60));
            addEntity(EnemyFactory::create(EnemyFactory::SHOOTER, 190, 60));
            addEntity(EnemyFactory::create(EnemyFactory::ASTEROID, 270, 60));
            addEntity(EnemyFactory::create(EnemyFactory::ASTEROID, 350, 60));
            addEntity(EnemyFactory::create(EnemyFactory::SHOOTER, 430, 60));
            addEntity(EnemyFactory::create(EnemyFactory::SHOOTER, 400, 90));
        }
    }

    void genereazaInamiciLevelcoop(int level) {
        addEntity(std::make_shared<Player>(200, 500, 3, 1));
        addEntity(std::make_shared<Player>(300, 500, 3, 2));

        // Pentru fiecare nivel pui o structura fixa:
        if (level == 0) {
            addEntity(EnemyFactory::create(EnemyFactory::ASTEROID, 100, 60));
            addEntity(EnemyFactory::create(EnemyFactory::ASTEROID, 350, 100));
            addEntity(EnemyFactory::create(EnemyFactory::ASTEROID, 130, 80));
            addEntity(EnemyFactory::create(EnemyFactory::ASTEROID, 270, 60));
            addEntity(EnemyFactory::create(EnemyFactory::ASTEROID, 180, 60));
        } else if (level == 1) {
            addEntity(EnemyFactory::create(EnemyFactory::SHOOTER, 120, 80));
            addEntity(EnemyFactory::create(EnemyFactory::ASTEROID, 340, 90));
            addEntity(EnemyFactory::create(EnemyFactory::SHOOTER, 200, 60));
            addEntity(EnemyFactory::create(EnemyFactory::ASTEROID, 270, 60));
            addEntity(EnemyFactory::create(EnemyFactory::ASTEROID, 100, 60));
        } else if (level == 2) {
            addEntity(EnemyFactory::create(EnemyFactory::ASTEROID, 110, 80));
            addEntity(EnemyFactory::create(EnemyFactory::ASTEROID, 350, 70));
            addEntity(EnemyFactory::create(EnemyFactory::SHOOTER, 250, 90));
            addEntity(EnemyFactory::create(EnemyFactory::ASTEROID, 180, 60));
            addEntity(EnemyFactory::create(EnemyFactory::SHOOTER, 200, 60));
        } else if (level == 3) {
            addEntity(EnemyFactory::create(EnemyFactory::SHOOTER, 120, 90));
            addEntity(EnemyFactory::create(EnemyFactory::ASTEROID, 320, 70));
            addEntity(EnemyFactory::create(EnemyFactory::SHOOTER, 400, 90));
            addEntity(EnemyFactory::create(EnemyFactory::ASTEROID, 250, 60));
            addEntity(EnemyFactory::create(EnemyFactory::ASTEROID, 180, 120));
            addEntity(EnemyFactory::create(EnemyFactory::ASTEROID, 100, 60));
        } else if (level == 4) {
            addEntity(EnemyFactory::create(EnemyFactory::SHOOTER, 110, 60));
            addEntity(EnemyFactory::create(EnemyFactory::SHOOTER, 190, 60));
            addEntity(EnemyFactory::create(EnemyFactory::ASTEROID, 270, 60));
            addEntity(EnemyFactory::create(EnemyFactory::ASTEROID, 350, 60));
            addEntity(EnemyFactory::create(EnemyFactory::SHOOTER, 430, 60));
            addEntity(EnemyFactory::create(EnemyFactory::SHOOTER, 400, 90));
        }
    }


    void setLevelComplete();
    void setGameOver();
    bool isLevelComplete() const { return levelComplete; }
    bool isGameOver() const { return gameOver; }
    void resetWinLoss();
};

#endif // ECRAN_H
