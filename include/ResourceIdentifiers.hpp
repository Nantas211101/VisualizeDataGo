#pragma once
#include <SFML/Graphics.hpp>

namespace Textures{
    enum ID{
        Eagle,
        Raptor,
        Desert,
        TitleScreen,
    };
}

namespace Fonts{
    enum ID{
        Main,
    };
}

template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID> FontHolder;