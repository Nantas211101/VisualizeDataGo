#pragma once
#include <SFML/Graphics.hpp>
#include "GUI_Component.hpp"
#include "GUI_Button.hpp"
#include "GUI_InputButton.hpp"

#include <iostream>
#include <assert.h>
#include <memory>
#include <vector>
#include <string>

namespace GUI{

class Container : public Component{
    public:
        typedef std::shared_ptr<Container> Ptr;

    public:
        Container();
        void pack(Component::Ptr component);
        void depackend();
        void depackend(int n);
        auto takeOutString(int pos) -> std::string;
        virtual bool isSelectable() const;
        virtual void handleEvent(const sf::Event &event);
        virtual bool handleRealTimeInput(const sf::RenderWindow &window);
        virtual std::string takeSaveText();
        bool isOutOfSize(int size);
        int takeSize();


    private:
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

        auto hasSelection() -> bool const;
        auto hasActivate() -> bool const;
        void select(std::size_t index);
        void selectNext();
        void selectPrev();

    private:
        std::vector<Component::Ptr> mChildren;
        bool isFocus;
        int mSeletedChild;
        int mActiveChild;
};

}