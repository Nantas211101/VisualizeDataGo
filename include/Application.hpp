#pragma once
#include <SFML/Graphics.hpp>
#include "StateStack.hpp"
#include "ResourceIdentifiers.hpp"
#include "ResourceHolder.hpp"
#include "Player.hpp"
#include "State.hpp"
#include "StringConvert.hpp"

const std::string Path_TitleScreen = "resource/Textures/TitleScreen";
const std::string Path_Font = "resource/Sansation.ttf";

class Application{

    public:
        Application();
        void run();

    public:
        void processInput();
        void update(sf::Time dt);
        void render();
    
    public:
        void updateStatic(sf::Time dt);
        void registerStates();


    private:
        static const sf::Time	TimePerFrame;

		sf::RenderWindow		mWindow;
		TextureHolder			mTextures;
	  	FontHolder				mFonts;
		Player					mPlayer;

		StateStack				mStateStack;

		sf::Text				mStayText;
		sf::Time				mStayUpdateTime;
		std::size_t				mStayNumFrames;
};