#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <map>
#include "SDL.h"
#include "SDL_image.h"
#include <stdexcept>

// Clasa pentru incarcarea si desenarea texturilor
class Texture {
private:
    Texture() = default;
    ~Texture();

    std::map<std::string, SDL_Texture*> TextureMap;  // privat (Tema 1)

public:
    static Texture* GetInstance() {
        static Texture instance;
        return &instance;
    }

    // Nu permitem copierea sau mutarea
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    bool Load(const std::string& id, const std::string& filename);
    void Draw(const std::string& id, int x, int y, int width, int height, SDL_RendererFlip flip = SDL_FLIP_NONE);

    void Drop(const std::string& id);
    void Clean();


};
#endif // TEXTURE_H
