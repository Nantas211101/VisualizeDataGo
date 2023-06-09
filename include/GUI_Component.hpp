#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <assert.h>
#include <iostream>

namespace GUI{ // put in the namespace to prevent the conflix of the component

class Component :   public sf::Drawable, 
                    public sf::Transformable, 
                    private sf::NonCopyable{
    public:
        typedef std::shared_ptr<Component> Ptr;

    public:
        Component();
        virtual ~Component();
        
        virtual std::string takeSaveText() = 0;
        virtual bool isSelectable() const = 0;
        bool isSelected() const;
        virtual void select();
        virtual void deselect();
        virtual bool isActive() const;
        virtual void activate();
        virtual void deactivate();
        virtual void handleEvent(const sf::Event& event) = 0;
        virtual bool handleRealTimeInput(const sf::RenderWindow &window) = 0;
        virtual bool isSpeedButton();
        virtual bool isSpeedUp();

    private:
        bool mIsSelected;
        bool mIsActive;
};

}