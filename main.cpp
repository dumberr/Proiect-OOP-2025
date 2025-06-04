#include "Ecran.h"
#include "Enemy.h"
#include "Player.h"
#include "Texture.h"
#include "Entity.h"
#include <iostream>
#include <cassert>
#include <direct.h>

int main(int argc, char* argv[]) {
    char cwd[1024];
    if (_getcwd(cwd, sizeof(cwd)))
        std::cout << "Working directory: " << cwd << std::endl;
    else
        std::cout << "Nu pot determina working directory!\n";

    try {
        // Inițializare ecran
        if (!Ecran::GetInstance()->Init()) {
            throw std::runtime_error("Eroare la init Ecran");
        }

        // Test minimal Tema 1: creăm un jucător, clonăm și comparăm
        {
            Player p1{100, 400, 3};
            p1.move(10, -20);
            p1.shoot();
            Player p2 = p1; // copy ctor
            assert(p1 == p2); // operator== :contentReference[oaicite:45]{index=45}
            p2.takeDamage(1);
            assert(p1 != p2);
            std::cout << "Teste Player: OK\n";
        }

        // Încărcăm muzica (dacă există)
        Mix_Music* muzica = Mix_LoadMUS("song.wav");
        if (muzica) Mix_PlayMusic(muzica, -1);

        // Bucla principală
        while (Ecran::GetInstance()->Running()) {
            Ecran::GetInstance()->HandleEvents();
            Ecran::GetInstance()->UpdateAll();
            Ecran::GetInstance()->RenderAll();
            SDL_Delay(16); // ~60 FPS
        }

        if (muzica) Mix_FreeMusic(muzica);
    }
    catch (const std::exception& ex) {
        std::cerr << "Exceptie: " << ex.what() << "\n";
        return -1;
    }

    //addEntity(std::make_shared<Player>(200, 500, 3, 1));
    /*


std::cout << "Initial: " << p << std::endl;
std::cout << "Introdu x y lives id: ";
std::cin >> p;
std::cout << "Dupa citire: " << p << std::endl;


int n = Player.getBullets().countIf([](const std::pair<int, int>& p){ return p.first > 100; });
std::cout << "Proiectile la dreapta de 100: " << n << std::endl;



    void processEntity(Entity* e)   {
        // Vrei să verifici dacă e Player sau Enemy:
        if (auto* player = dynamic_cast<Player*>(e)) {
            std::cout << "Este Player cu " << player->getLives() << " vieti!\n";
            // Poți apela metode specifice Player
        } else if (auto* enemy = dynamic_cast<Enemy*>(e)) {
            std::cout << "Este Enemy!\n";
            // Poți apela metode specifice Enemy
        } else {
            std::cout << "Este alt tip de Entity!\n";
        }
    }

        Player p1(100, 200, 3, 1);
        Enemy  e1(150, 220, 2, 5, 1);

        // Pointeri la baza Entity
        Entity* entities[2] = {&p1, &e1};

        for (int i = 0; i < 2; ++i) {
            processEntity(entities[i]);
        }
    */
    return 0;
}
