#pragma once
#include <SFML/Graphics.hpp>
#include "StateStack.hpp"
#include "State.hpp"
#include "ResourceIdentifiers.hpp"
#include "ResourceHolder.hpp"
#include "TextureManipulate.hpp"
#include "GUI_Container.hpp"
#include "GUI_Button.hpp"

#include <memory>
#include <string>

const int OFFSET_X = 100;
const int OFFSET_Y = 250;
const int y_add = 50;
const int x_add = 300;

class MenuState : public State{
    public:
        MenuState(StateStack &stack, Context context);

        virtual void draw();
        virtual bool update(sf::Time dt);
        virtual bool handleEvent(const sf::Event &event);
        virtual bool handleRealTimeInput();

    private:
        void setStateButton(Context context, int x_times, int y_times, const std::string &text, States::ID id);

    private:
        sf::Sprite mBackgroundSprite;
        GUI::Container mGUIContainer;
};