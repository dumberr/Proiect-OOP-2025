#include "Ecran.h"
#include "Texture.h"
#include <iostream>
#include <algorithm>
#include <memory>

bool meniu = true;
bool gameSolo = false;
bool gameCoop = false;
bool nivelMenuSolo = false;
bool nivelMenuCoop = false;
bool infoScreen = false;

int Level = 0;

Ecran::Ecran() : nivel_deblocat(NR_NIVELE, false), levelComplete(false), gameOver(false), win_timer(0) {
    nivel_deblocat[0] = true; // Doar nivelul 1 e deblocat la inceput
}

Ecran::~Ecran() {
    Texture::GetInstance()->Clean();
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();
}

void Ecran::Quit() { ok = false; }

void Ecran::addEntity(std::shared_ptr<Entity> obj) { objects.push_back(std::move(obj)); }
void Ecran::clearEntitys() { objects.clear(); }

bool Ecran::Init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        std::cerr << "Eroare la initializare SDL: " << SDL_GetError() << "\n";
        return ok = false;
    }
    if (!(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) & (IMG_INIT_PNG | IMG_INIT_JPG))) {
        std::cerr << "Eroare la initializare SDL_image: " << IMG_GetError() << "\n";
        return ok = false;
    }
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
        std::cerr << "Eroare la initializare SDL_mixer: " << Mix_GetError() << "\n";
        muted = true;
    }

    window = SDL_CreateWindow("Star Reacher", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    if (!window) {
        std::cerr << "Eroare la crearea ferestrei: " << SDL_GetError() << "\n";
        return ok = false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "Eroare la crearea renderer-ului: " << SDL_GetError() << "\n";
        return ok = false;
    }

    auto loadOrThrow = [&](const std::string& id, const std::string& path) {
        if (!Texture::GetInstance()->Load(id, path)) {
            std::cerr << "Nu s-a putut incarca textura `" << path << "`" << std::endl;
        }
    };
    try {
        loadOrThrow("player",           "Images/Racheta1.png");
        loadOrThrow("player2",          "Images/Racheta2.png");
        loadOrThrow("bullet",           "Images/bullet.png");
        loadOrThrow("background_level", "Images/Background_1.png");
        loadOrThrow("meniu",            "Images/Menu.png");
        loadOrThrow("asteroid",         "Images/steroid.png");
        loadOrThrow("boom",             "Images/boom.png");
        loadOrThrow("nivele",           "Images/Level_Menu.png");
        loadOrThrow("healthbar3",       "Images/full.png");
        loadOrThrow("healthbar2",       "Images/2.png");
        loadOrThrow("healthbar1",       "Images/1.png");
        loadOrThrow("healthbar0",       "Images/0.png");
        loadOrThrow("dead",             "Images/YOU_DIED.png");
        loadOrThrow("won",              "Images/LEVEL_COMPLETE.png");
        loadOrThrow("laser",            "Images/lazer.png");
        loadOrThrow("enemyBullet",      "Images/enemy_bullet.png");
        loadOrThrow("shooter",          "Images/shooter.png");
        loadOrThrow("controls",         "Images/CONTROLS.png");
        loadOrThrow("lock",             "Images/lock.png");
        loadOrThrow("paused",           "Images/Pause.png");
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << "\n";
        return ok = false;
    }

    ok = true;
    return ok;
}

// WIN/LOSS HANDLING
void Ecran::setLevelComplete() {
    levelComplete = true;
    win_timer = 80;
}

void Ecran::setGameOver() {
    gameOver = true;
    win_timer = 80;
}

void Ecran::resetWinLoss() {
    levelComplete = false;
    gameOver = false;
    win_timer = 0;
}

// -----------------------------
// UpdateAll: trateaza coliziuni + win/loss
void Ecran::UpdateAll() {
    if (paused || levelComplete || gameOver) {
        if (win_timer > 0) win_timer--;
        return;
    }

    // Update la toate entitatile
    for (auto& obj : objects) obj->update();

    // COLIZIUNI Player bullet vs Enemy/Asteroid
    for (auto& obj : objects) {
        if (auto player = std::dynamic_pointer_cast<Player>(obj)) {
            auto& bullets = player->getBullets().getVector();
            for (auto it = bullets.begin(); it != bullets.end(); ) {
                bool bulletUsed = false;
                for (auto& target : objects) {
                    if (auto enemy = std::dynamic_pointer_cast<EnemyBase>(target)) {
                        int ex = enemy->getX(), ey = enemy->getY();
                        int bx = it->first, by = it->second;
                        if (bx >= ex && bx <= ex+58 && by >= ey && by <= ey+54 && enemy->isAlive()) {
                            enemy->takeDamage(10);
                            it = bullets.erase(it);
                            bulletUsed = true;
                            break;
                        }
                    }
                }
                if (!bulletUsed) ++it;
            }
        }
    }

    // COLIZIUNI Enemy bullet vs Player
    for (auto& obj : objects) {
        if (auto enemy = std::dynamic_pointer_cast<Shooter>(obj)) {
            auto& bullets = enemy->getEnemyBullets().getVector();
            for (auto it = bullets.begin(); it != bullets.end(); ) {
                bool bulletUsed = false;
                for (auto& target : objects) {
                    if (auto player = std::dynamic_pointer_cast<Player>(target)) {
                        int px = player->getX(), py = player->getY();
                        int bx = it->first, by = it->second;
                        if (bx >= px && bx <= px+58 && by >= py && by <= py+64 && !player->isDead()) {
                            player->takeDamage(1);
                            it = bullets.erase(it);
                            bulletUsed = true;
                            break;
                        }
                    }
                }
                if (!bulletUsed) ++it;
            }
        }
    }

    // COLIZIUNI Enemy/Asteroid vs Player (contact direct)
    for (auto& obj : objects) {
        if (auto enemy = std::dynamic_pointer_cast<EnemyBase>(obj)) {
            if (!enemy->isAlive()) continue;
            for (auto& target : objects) {
                if (auto player = std::dynamic_pointer_cast<Player>(target)) {
                    int ex = enemy->getX(), ey = enemy->getY();
                    int px = player->getX(), py = player->getY();
                    if (ex < px+58 && ex+58 > px && ey < py+64 && ey+54 > py && !player->isDead()) {
                        player->takeDamage(1);
                        enemy->takeDamage(10); // distruge inamicul la contact
                    }
                }
            }
        }
    }

    // Elimina enemies/asteroizi morti
    objects.erase(
        std::remove_if(objects.begin(), objects.end(),
            [](const std::shared_ptr<Entity>& obj) {
                if (auto enemy = std::dynamic_pointer_cast<EnemyBase>(obj)) return !enemy->isAlive();
                return false;
            }),
        objects.end()
    );

    // CHECK WIN/LOSS
    bool anyPlayerAlive = false, anyEnemyAlive = false;
    for (auto& obj : objects) {
        if (auto p = std::dynamic_pointer_cast<Player>(obj)) if (!p->isDead()) anyPlayerAlive = true;
        if (auto e = std::dynamic_pointer_cast<EnemyBase>(obj)) if (e->isAlive()) anyEnemyAlive = true;
    }
    if (!anyPlayerAlive && !gameOver) setGameOver();
    if (!anyEnemyAlive && !levelComplete) {
        setLevelComplete();
        if (Level < NR_NIVELE-1) deblocheazaNivel(Level+1);
    }
}

// -----------------------------
void Ecran::RenderAll() const {
    SDL_SetRenderDrawColor(renderer, 94, 3, 252, 155);
    SDL_RenderClear(renderer);

    if (meniu) {
        Texture::GetInstance()->Draw("meniu", 0, 0, WIDTH, HEIGHT);
    } else if (nivelMenuSolo || nivelMenuCoop) {
        Texture::GetInstance()->Draw("nivele", 0, 0, WIDTH, HEIGHT);
        for (int i = 0; i < NR_NIVELE; ++i) {
            int y = 270 + (i)*50;
            if (!isNivelDeblocat(i))
                Texture::GetInstance()->Draw("lock", 170, y, 27, 30);
        }
    } else if (infoScreen) {
        Texture::GetInstance()->Draw("controls", 0, 0, WIDTH, HEIGHT);
    } else {
        Texture::GetInstance()->Draw("background_level", 0, 0, WIDTH, HEIGHT);
        for (const auto& obj : objects) obj->render();

        // WIN/LOSS overlays
        if (levelComplete)
            Texture::GetInstance()->Draw("won", 100, 300, 300, 50);
        if (gameOver)
            Texture::GetInstance()->Draw("dead", 100, 300, 300, 50);

        if (paused) {
            Texture::GetInstance()->Draw("paused", 0, 0, 170, 35);
        }
    }
    SDL_RenderPresent(renderer);
}

// -----------------------------
void Ecran::HandleEvents() {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT:
            Quit();
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (delay_click == 0) {
                SDL_GetMouseState(&mouseX, &mouseY);
                // logica butoanelor din meniu
                if (meniu) {
                    if (mouseX >= 200 && mouseX <= 295 && mouseY >= 265 && mouseY <= 310) {
                        meniu = false;
                        nivelMenuSolo = true;
                        delay_click = DELAY_TIME;
                    } else if (mouseX >= 200 && mouseX <= 295 && mouseY >= 325 && mouseY <= 370) {
                        meniu = false;
                        nivelMenuCoop = true;
                        delay_click = DELAY_TIME;
                    } else if (mouseX >= 28 && mouseX <= 70 && mouseY >= 628 && mouseY <= 670) {
                        meniu = false;
                        infoScreen = true;
                        delay_click = DELAY_TIME;
                    } else if (mouseX >= 200 && mouseX <= 295 && mouseY >= 385 && mouseY <= 430) {
                        Quit();
                    }
                }
                else if (nivelMenuSolo) {
                    for (int i = 0; i < NR_NIVELE; ++i) {
                        int buton_x = 200, buton_y = 270 + i * 50;
                        if (mouseX >= buton_x && mouseX <= buton_x + 90 &&
                            mouseY >= buton_y && mouseY <= buton_y + 35) {
                            if (isNivelDeblocat(i)) {
                                Level = i;
                                nivelMenuSolo = false;
                                gameSolo = true;
                                clearEntitys();
                                addEntity(std::make_shared<Player>(200, 500, 3, 1));
                                //
                                genereazaInamiciLevelsolo(Level);


                                resetWinLoss();
                            }
                        }
                    }
                }
                else if (nivelMenuCoop) {
                    for (int i = 0; i < NR_NIVELE; ++i) {
                        int buton_x = 200, buton_y = 270 + i * 50;
                        if (mouseX >= buton_x && mouseX <= buton_x + 90 &&
                            mouseY >= buton_y && mouseY <= buton_y + 35) {
                            if (isNivelDeblocat(i)) {
                                Level = i;
                                nivelMenuCoop = false;
                                gameCoop = true;
                                clearEntitys();
                                genereazaInamiciLevelcoop(Level);


                                resetWinLoss();
                            }
                        }
                    }
                }
                else if (infoScreen) {
                    infoScreen = false;
                    meniu = true;
                    delay_click = DELAY_TIME;
                }
            }
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE && delay_esc == 0) {
                delay_esc = DELAY_TIME;
                // ESC universal:
                if (gameSolo || gameCoop || levelComplete || gameOver) {
                    gameSolo = false;
                    gameCoop = false;
                    nivelMenuSolo = false;
                    nivelMenuCoop = false;
                    resetWinLoss();
                    meniu = true;
                    clearEntitys();
                } else if (nivelMenuSolo) {
                    nivelMenuSolo = false;
                    meniu = true;
                } else if (nivelMenuCoop) {
                    nivelMenuCoop = false;
                    meniu = true;
                } else if (infoScreen) {
                    infoScreen = false;
                    meniu = true;
                } else if (meniu) {
                    Quit();
                }
            }
            if (event.key.keysym.sym == SDLK_p && delay_paused == 0) {
                paused = !paused;
                delay_paused = DELAY_TIME;
            }
            if (event.key.keysym.sym == SDLK_m && delay_mute == 0) {
                muted = !muted;
                if (muted) Mix_PauseMusic();
                else Mix_ResumeMusic();
                delay_mute = DELAY_TIME;
            }
            break;
        default:
            break;
    }



    if (delay_click > 0) --delay_click;
    if (delay_esc > 0) --delay_esc;
    if (delay_paused > 0) --delay_paused;
    if (delay_mute > 0) --delay_mute;
}
