#include "Texture.h"
#include "Ecran.h"
#include <SDL_image.h>
#include <iostream>

bool Texture::Load(const std::string& id, const std::string& filename) {
    SDL_Surface* surface = IMG_Load(filename.c_str());
    if (!surface) {
        std::cerr << "Eroare la incarcarea imagini: " << filename << ", " << IMG_GetError() << "\n";
        return false;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(Ecran::GetInstance()->GetRenderer(), surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        std::cerr << "Eroare la crearea texturii: " << SDL_GetError() << "\n";
        return false;
    }
    TextureMap[id] = texture;
    return true;
}

void Texture::Draw(const std::string& id, int x, int y, int width, int height, SDL_RendererFlip flip) {
    auto it = TextureMap.find(id);
    if (it == TextureMap.end()) {
        std::cerr << "Texture::Draw: texture `" << id << "` nu exista\n";
        return;
    }
    SDL_Rect srcRect = {0, 0, width, height};
    SDL_Rect dstRect = {x, y, width, height};
    SDL_RenderCopyEx(Ecran::GetInstance()->GetRenderer(), it->second, &srcRect, &dstRect, 0, nullptr, flip);
}

void Texture::Drop(const std::string& id) {
    auto it = TextureMap.find(id);
    if (it != TextureMap.end()) {
        SDL_DestroyTexture(it->second);
        TextureMap.erase(it);
    }
}

void Texture::Clean() {
    for (auto& kv : TextureMap) {
        if (kv.second) {
            std::cout << "Distrug textura: " << kv.first << std::endl;
            SDL_DestroyTexture(kv.second);
        }
    }
    TextureMap.clear();
}

Texture::~Texture() {
    Clean();
}
