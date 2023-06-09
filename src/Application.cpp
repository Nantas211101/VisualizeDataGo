#include "Application.hpp"


// Application quite similar to the Game.cpp
const sf::Time Application::TimePerFrame = sf::seconds(1.f / 60.f);

Application::Application():
    mWindow(sf::VideoMode(2560, 1440), "Game States (Beta Version) World", sf::Style::Fullscreen),
    mTextures(),
    mFonts(),
    mStateStack(State::Context(mWindow, mTextures, mFonts)),
    mStayText(),
    mStayUpdateTime(),
    mStayNumFrames(0){
        // Not let the key repeat
        mWindow.setKeyRepeatEnabled(false);
        // mWindow.create()

        // Loading resource
        mFonts.load(Fonts::Main, Path_Font);
        mTextures.load(Textures::TitleScreen, Path_TitleScreen);
        mTextures.load(Textures::Menu, Path_MenuScreen);
        mTextures.load(Textures::WhiteBackground, Path_WhiteBackground);
        mTextures.load(Textures::ButtonNormal, Path_ButtonNormal);
        mTextures.load(Textures::ButtonSelected, Path_ButtonSelected);
        mTextures.load(Textures::ButtonPressed, Path_ButtonPressed);

        // Set Font, position, and size for the text fps (this quite similar with the Game.hpp)
        mStayText.setFont(mFonts.get(Fonts::Main));
        mStayText.setPosition(5.f, 5.f);
        mStayText.setCharacterSize(10u);

        registerStates();
        // start with the title state
        mStateStack.pushState(States::Title);
}

void Application::registerStates(){
    mStateStack.registerState<TitleState>(States::Title);
    mStateStack.registerState<MenuState>(States::Menu);
    mStateStack.registerState<PauseState>(States::Pause);   
    mStateStack.registerState<Stack>(States::Stack);
    mStateStack.registerState<Queue>(States::Queue);
    mStateStack.registerState<StaticArray>(States::StaticArray);
    mStateStack.registerState<DynamicArray>(States::DynamicArray);
    mStateStack.registerState<SLL>(States::SLL);
    mStateStack.registerState<DLL>(States::DLL);
    mStateStack.registerState<CLL>(States::CLL);
}

void Application::processInput(){
    // Feeding events
    sf::Event event;
    while(mWindow.pollEvent(event)){
        mStateStack.handleEvent(event);

        if(mStateStack.isEmpty() || event.type == sf::Event::Closed)
            mWindow.close();
    }
}

void Application::update(sf::Time dt){
    mStateStack.update(dt);
}

void Application::render(){
    mWindow.clear();
    mStateStack.draw();
    mWindow.setView(mWindow.getDefaultView());
    mWindow.draw(mStayText);
    mWindow.display();
}

void Application::run(){
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while(mWindow.isOpen()){
        sf::Time elapsedTime = clock.restart();
        timeSinceLastUpdate += elapsedTime;
        while(timeSinceLastUpdate > TimePerFrame){
            timeSinceLastUpdate -= TimePerFrame;
            processInput();
            update(TimePerFrame);

            // Maybe after update the stack will be empty
            if(mStateStack.isEmpty())
                mWindow.close();
        }

        updateStatic(elapsedTime);
        render();
    }
}

void Application::updateStatic(sf::Time elapsedTime){
    mStayUpdateTime += elapsedTime;
    mStayNumFrames += 1;

    if(mStayUpdateTime >= sf::seconds(1.0f)){
        mStayText.setString(
            "Frames / Second = " + toString(mStayNumFrames) + "\n" +
            // mStayNumFrames can be consider at Numofupdate
            "Time / Update = " + toString(mStayUpdateTime.asSeconds() / mStayNumFrames) + "us");
        // reset the mStay
        mStayUpdateTime -= sf::seconds(1.0f);
        mStayNumFrames = 0;
    }
}
