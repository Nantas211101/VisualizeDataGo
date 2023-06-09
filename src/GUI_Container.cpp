#include "GUI_Container.hpp"

namespace GUI {

Container::Container()
    : mChildren(), isFocus(true), mSeletedChild(-1), mActiveChild(-1) {}

void Container::pack(Component::Ptr component) {
    mChildren.push_back(component);
    // if(!hasSelection() && component->isSelectable())
    // select(mChildren.size() - 1);
}

void Container::depackend() { mChildren.erase(mChildren.end() - 1); }

void Container::depackend(int n) { mChildren.erase(mChildren.end() - n - 1); }

auto Container::takeOutString(int pos) -> std::string{
    return mChildren[pos]->takeSaveText();
}

// Container is not a selectable object
bool Container::isSelectable() const { return false; }

void Container::handleEvent(const sf::Event& event) {

    // if(event.type == sf::Event::GainedFocus){
    //     isFocus = true;
    // };

    // if(event.type == sf::Event::LostFocus){
    //     isFocus = false;
    // };

    // // stop if no focus
    // if(!isFocus)
    //     return;

    if (hasActivate()) {
        mChildren[mActiveChild]->handleEvent(event);
    };
    if (event.type == sf::Event::MouseButtonPressed)
        if (event.mouseButton.button == sf::Mouse::Left) {
            if (hasSelection() && !mChildren[mSeletedChild]->isSpeedButton()) {
                mChildren[mSeletedChild]->deselect();
                if (hasActivate())
                    mChildren[mActiveChild]->deactivate();
                mChildren[mSeletedChild]->activate();
                mActiveChild = mSeletedChild;
                mSeletedChild = -1;
            } else {
                if (hasActivate())
                    mChildren[mActiveChild]->deactivate();
                mActiveChild = -1;
            }
        }
    if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            if (hasSelection() && mChildren[mSeletedChild]->isSpeedButton()) {
                mChildren[mSeletedChild]->deselect();
                mChildren[mSeletedChild]->activate();
                mChildren[mSeletedChild]->deactivate();
                mChildren[mSeletedChild]->select();
            }
        }
    }
    // else if(event.type == sf::Event::KeyReleased){
    //     if(event.key.code == sf::Keyboard::W || event.key.code ==
    //     sf::Keyboard::Up
    //     || event.key.code == sf::Keyboard::A || event.key.code ==
    //     sf::Keyboard::Left){
    //         selectPrev();
    //     }
    //     else if(event.key.code == sf::Keyboard::S || event.key.code ==
    //     sf::Keyboard::Down
    //         ||  event.key.code == sf::Keyboard::D || event.key.code ==
    //         sf::Keyboard::Right){ selectNext();
    //     }
    //     else if(event.key.code == sf::Keyboard::Return || event.key.code ==
    //     sf::Keyboard::Space){
    //         if(hasSelection())
    //             mChildren[mSeletedChild]->activate();
    //     }
    // }
    // handleRealTimeInput();
}

bool Container::handleRealTimeInput(const sf::RenderWindow& window) {
    // Checking for selecting
    for (int index = 0; index < mChildren.size(); ++index)
        if (mActiveChild != index &&
            mChildren[index]->handleRealTimeInput(window)) {
            select(index);
        }

    // checking if the mouse is out of the bounds of button
    if (hasSelection() &&
        !mChildren[mSeletedChild]->handleRealTimeInput(window)) {
        mChildren[mSeletedChild]->deselect();
        mSeletedChild = -1;
    }
    // Checking for clicking
    // if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
    //     if(hasSelection()){
    //         mChildren[mSeletedChild]->deselect();
    //         if(hasActivate())
    //             mChildren[mActiveChild]->deactivate();
    //         mChildren[mSeletedChild]->activate();
    //         mActiveChild = mSeletedChild;
    //         mSeletedChild = -1;
    //     }
    //     else {
    //         if(hasActivate())
    //             mChildren[mActiveChild]->deactivate();
    //         mActiveChild = -1;
    //     }
    // }
    return false;
}

std::string Container::takeSaveText(){
    return "";
}

bool Container::isOutOfSize(int size) { return mChildren.size() > size; }

void Container::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    for (const Component::Ptr& child : mChildren)
        target.draw(*child, states);
}

int Container::takeSize() { return mChildren.size(); }


auto Container::hasSelection() -> bool const { return mSeletedChild >= 0; }

auto Container::hasActivate() -> bool const { return mActiveChild >= 0; }

void Container::select(std::size_t index) {
    if (mChildren[index]->isSelectable()) {
        if (hasSelection())
            mChildren[mSeletedChild]->deselect();
        mChildren[index]->select();
        mSeletedChild = index;
    }
}

void Container::selectNext() {
    if (!hasSelection())
        return;
    // searching for the next component that may be selectable
    int next = mSeletedChild;
    // a little slow but code will be ez to read
    do {
        next = (next + 1);
        next %= mChildren.size(); // circular index
    } while (!mChildren[next]->isSelectable());
    // And finally select it
    select(next);
}

// quite the same selectNext();
void Container::selectPrev() {
    if (!hasSelection())
        return;
    int prev = mSeletedChild;
    do {
        prev = (prev - 1);
        prev = (prev + mChildren.size()) % mChildren.size();
    } while (!mChildren[prev]->isSelectable());
    select(prev);
}

} // namespace GUI