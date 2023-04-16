#include "../include/GUI_Container.hpp"


namespace GUI{

Container::Container():
    mChildren(),
    mSeletedChild(-1){

}

void Container::pack(Component::Ptr component){
    mChildren.push_back(component);
    if(!hasSelection() && component->isSelectable())
        select(mChildren.size() - 1);
}

// Container is not a selectable object
bool Container::isSelectable() const{
    return false;
}

void Container::handleEvent(const sf::Event &event){
    if(hasSelection() && mChildren[mSeletedChild] -> isActive()){
        mChildren[mSeletedChild]->handleEvent(event);
    }
    else if(event.type == sf::Event::KeyReleased){
        if(event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up
        || event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left){
            selectPrev();
        }
        else if(event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down
            ||  event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right){
            selectNext();
        }
        else if(event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space){
            if(hasSelection())
                mChildren[mSeletedChild]->activate();
        }
    }
    else if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
        if(hasSelection())
                mChildren[mSeletedChild]->activate();
    }
}


void Container::draw(sf::RenderTarget &target, sf::RenderStates states) const{
    states.transform *= getTransform();
    for(const Component::Ptr &child : mChildren)
        target.draw(*child, states);
}

auto Container::hasSelection() -> bool const{
    return mSeletedChild >= 0;
}

void Container::select(std::size_t index){
    if(mChildren[index]->isSelectable()){
        if(hasSelection())
            mChildren[mSeletedChild]->deselect();
        mChildren[index]->select();
        mSeletedChild = index;
    }
}

void Container::selectNext(){
    if(!hasSelection())
        return;
    // searching for the next component that may be selectable
    int next = mSeletedChild;
    // a little slow but code will be ez to read
    do{
        next = (next + 1);
        next %= mChildren.size(); // circular index
    }while(!mChildren[next]->isSelectable());
    // And finally select it
    select(next);
}

// quite the same selectNext();
void Container::selectPrev(){
    if(!hasSelection())
        return;
    int prev = mSeletedChild;
    do{
        prev = (prev - 1);
        prev = (prev + mChildren.size()) % mChildren.size();
    }while(!mChildren[prev]->isSelectable());
    select(prev);
}

}