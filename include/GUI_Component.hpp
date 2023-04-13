#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

namespace GUI{ // put in the namespace to prevent the conflix of the component

class Component :   public sf::Drawable, 
                    public sf::Transformable, 
                    private sf::NonCopyable{
    public:
        typedef std::shared_ptr<Component> Ptr;

    public:
        Component();
        virtual ~Component();
        
        virtual bool isSelectable() const = 0;
        bool isSelected() const;
        virtual void select();
        virtual void deselect();
        virtual bool isActive() const;
        virtual void activate();
        virtual void deactivate();
        virtual void handleEvent(const sf::Event& event);

    private:
        bool mIsSelected;
        bool mIsActive;
};

}