#include "../include/State.hpp"
#include "../include/StateStack.hpp"

State::Context::Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, Player& player):
    window(&window), // using & because the variable is a pointer (pointer save address)
    textures(&textures),
    fonts(&fonts),
    player(&player){
}

State::State(StateStack &stack, Context context):
    mStack(&stack),
    mContext(context){
}

State::~State(){
}

void State::requestStackPush(States::ID stateID){
    mStack->pushState(stateID);
}

void State::requestStackPop(){
    mStack->popState();
}

void State::requestStateClear(){
    mStack->clearStates();
}

auto State::getContext() -> Context const{
    return mContext;
}
