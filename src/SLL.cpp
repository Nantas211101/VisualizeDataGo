#include "SLL.hpp"
#include "FileReader.hpp"
#include "NodesSaverIdentifiers.hpp"
#include "StringConvert.hpp"

const std::string Name = "SLL";

SLL::SLL(StateStack& stack, Context context)
    : State(stack, context),
      mBackgroundSprite(),
      mGUIContainer(),
      mDisplayer(*context.window, 5, textSize, add_x * 2, add_y,
                 context.fonts->get(Fonts::Main)),
      mSceneGraph(),
      randomHolder(),
      isSearching(false),
      usingData1(""),
      usingData2(""),
      timeSinceLastHandleEvent(sf::Time::Zero) {
    sf::Texture& texture = context.textures->get(Textures::WhiteBackground);
    mBackgroundSprite.setTexture(texture);

    int cnty = -1;
    // set Init button
    ++cnty;
    auto initButton =
        std::make_shared<GUI::Button>(*context.fonts, *context.textures);
    initButton->setPosition(start_x, start_y + cnty * add_y);
    initButton->setText("Init");
    initButton->setToggle(true);
    initButton->setCallback([this, context, cnty]() {
        if (nodeSaver.takeCurrentState() != NodesState::nothing)
            return;

        auto action = ([this]() {
            // nothing here now
        });

        resetButton(NumInitButton);

        int cntx = 0;

        auto ManuallyAction = ([this, context, cnty]() {
            if (nodeSaver.takeCurrentState() != NodesState::nothing)
                return;
            resetButton(NumInitButton + 3);
            printedError(context, "This feature have not include, please wait for next update patch");
        });

        auto FileAction = ([this, context]() {
            if (nodeSaver.takeCurrentState() != NodesState::nothing)
                return;
            resetButton(NumInitButton + 3);
            auto file_name = FileHolder.select();
            if (file_name.has_value()) {
                try {
                    data.clear();
                    nodeSaver.reset(mSceneGraph);
                    this->data = readIntegerFile(file_name.value());
                    nodeSaver.init(mSceneGraph, data, context);
                } catch (std::exception& e) {
                    // in lo
                    printedError(context, e.what());
                    data.clear();
                }
            }
        });

        auto RandomAction = ([this, context]() {
            if (nodeSaver.takeCurrentState() != NodesState::nothing)
                return;
            resetButton(NumInitButton + 3);
            nodeSaver.reset(mSceneGraph);
            data = randomHolder.randListData();
            nodeSaver.init(mSceneGraph, data, context);
        });

        setStateButton(context, start_x + (++cntx) * add_x,
                       start_y + cnty * add_y, "Manually", ManuallyAction);
        setStateButton(context, start_x + (++cntx) * add_x,
                       start_y + cnty * add_y, "File", FileAction);
        setStateButton(context, start_x + (++cntx) * add_x,
                       start_y + cnty * add_y, "Random", RandomAction);
    });

    // set Insert button
    ++cnty;
    auto insertButton =
        std::make_shared<GUI::Button>(*context.fonts, *context.textures);
    insertButton->setPosition(start_x, start_y + cnty * add_y);
    insertButton->setText("Insert");
    insertButton->setToggle(true);
    insertButton->setCallback([this, context, cnty]() {
        if (nodeSaver.takeCurrentState() != NodesState::nothing)
            return;

        auto action = ([this]() {
            // nothing here now
        });

        resetButton(NumInitButton);

        auto pushBackAction = ([this, context, cnty]() {
            if (nodeSaver.takeCurrentState() != NodesState::nothing)
                return;
            resetButton(NumInitButton + 3);
            data.clear();

            auto tmp = ([]() {});
            setInputButton(context, start_x + 1 * add_x,
                           start_y + (cnty + 1) * add_y, "", tmp);
            InputPosition.push_back(mGUIContainer.takeSize() - 1);
            data.push_back("");
            setLabel(context, start_x + 1 * add_x + add_x / 2,
                     start_y + (cnty + 2) * add_y + add_y / 2, "Value",
                     textSize);

            auto activeAction = ([this, context] {
                resetButton(NumInitButton + 6, false);
                if (checkInputError(inputID::Val, 0))
                    pushBackNode(context);
            });

            setStateButton(context, start_x + 3 * add_x,
                           start_y + (cnty + 1) * add_y, "Go", activeAction);
        });

        auto pushMiddleAction = ([this, context, cnty]() {
            if (nodeSaver.takeCurrentState() != NodesState::nothing)
                return;
            resetButton(NumInitButton + 3);
            data.clear();

            auto tmp = ([]() {});

            setInputButton(context, start_x + 1 * add_x,
                           start_y + (cnty + 1) * add_y, "", tmp);
            InputPosition.push_back(mGUIContainer.takeSize() - 1);
            data.push_back("");
            setLabel(context, start_x + 1 * add_x + add_x / 2,
                     start_y + (cnty + 2) * add_y + add_y / 2, "Position",
                     textSize);

            setInputButton(context, start_x + 2 * add_x,
                           start_y + (cnty + 1) * add_y, "", tmp);
            InputPosition.push_back(mGUIContainer.takeSize() - 1);
            data.push_back("");
            setLabel(context, start_x + 2 * add_x + add_x / 2,
                     start_y + (cnty + 2) * add_y + add_y / 2, "Value",
                     textSize);

            auto activeAction = ([this, context] {
                resetButton(NumInitButton + 8, false);
                if (checkInputError(inputID::Pos, 0) &&
                    checkInputError(inputID::Val, 1))
                    pushMiddleNode(context, toNum(data[0]));
            });

            setStateButton(context, start_x + 3 * add_x,
                           start_y + (cnty + 1) * add_y, "Go", activeAction);
        });

        auto pushfrontAction = ([this, context, cnty]() {
            if (nodeSaver.takeCurrentState() != NodesState::nothing)
                return;
            resetButton(NumInitButton + 3);
            data.clear();

            auto tmp = ([]() {});
            setInputButton(context, start_x + 1 * add_x,
                           start_y + (cnty + 1) * add_y, "", tmp);
            InputPosition.push_back(mGUIContainer.takeSize() - 1);
            data.push_back("");
            setLabel(context, start_x + 1 * add_x + add_x / 2,
                     start_y + (cnty + 2) * add_y + add_y / 2, "Value",
                     textSize);

            auto activeAction = ([this, context] {
                resetButton(NumInitButton + 6, false);
                if (checkInputError(inputID::Val, 0))
                    pushFrontNode(context);
            });

            setStateButton(context, start_x + 3 * add_x,
                           start_y + (cnty + 1) * add_y, "Go", activeAction);
        });

        auto ErrorAction = ([this, context]() {
            if (nodeSaver.takeCurrentState() != NodesState::nothing)
                return;
            resetButton(NumInitButton + 3);

            printedError(context, errorMessage + Name);
        });

        int cntx = 0;
        setStateButton(context, start_x + (++cntx) * add_x,
                       start_y + cnty * add_y, "Push", pushBackAction);
        setStateButton(context, start_x + (++cntx) * add_x,
                       start_y + cnty * add_y, "to Middle", pushMiddleAction);
        setStateButton(context, start_x + (++cntx) * add_x,
                       start_y + cnty * add_y, "to First", pushfrontAction);
    });

    // set Delete button
    ++cnty;
    auto deleteButton =
        std::make_shared<GUI::Button>(*context.fonts, *context.textures);
    deleteButton->setPosition(start_x, start_y + cnty * add_y);
    deleteButton->setText("Delete");
    deleteButton->setToggle(true);
    deleteButton->setCallback([this, context, cnty]() {
        if (nodeSaver.takeCurrentState() != NodesState::nothing)
            return;

        resetButton(NumInitButton);

        auto ErrorAction = ([this, context]() {
            if (nodeSaver.takeCurrentState() != NodesState::nothing)
                return;
            resetButton(NumInitButton + 3);

            printedError(context, errorMessage + Name);
        });

        auto popBackAction = ([this, context]() {
            if (nodeSaver.takeCurrentState() != NodesState::nothing)
                return;
            resetButton(NumInitButton + 4);
            popBackNode(context);
        });

        auto popMiddleAction = ([this, context, cnty]() {
            if (nodeSaver.takeCurrentState() != NodesState::nothing)
                return;
            resetButton(NumInitButton + 4);
            data.clear();

            auto tmp = ([]() {});
            setInputButton(context, start_x + 2 * add_x,
                           start_y + (cnty + 1) * add_y, "", tmp);
            InputPosition.push_back(mGUIContainer.takeSize() - 1);
            data.push_back("");
            setLabel(context, start_x + 2 * add_x + add_x / 2,
                     start_y + (cnty + 2) * add_y + add_y / 2, "Value",
                     textSize);

            auto activeAction = ([this, context] {
                resetButton(NumInitButton + 7, false);
                if (checkInputError(inputID::Val, 0))
                    popMiddleNode(context);
            });

            setStateButton(context, start_x + 3 * add_x,
                           start_y + (cnty + 1) * add_y, "Go", activeAction);
        });

        auto popFrontAction = ([this, context]() {
            if (nodeSaver.takeCurrentState() != NodesState::nothing)
                return;
            resetButton(NumInitButton + 4);
            popFrontNode(context);
        });

        auto ClearAction = ([this]() {
            if (nodeSaver.takeCurrentState() != NodesState::nothing)
                return;
            nodeSaver.reset(mSceneGraph);
        });

        int cntx = 0;
        setStateButton(context, start_x + (++cntx) * add_x,
                       start_y + cnty * add_y, "Pop", popBackAction);
        setStateButton(context, start_x + (++cntx) * add_x,
                       start_y + cnty * add_y, "at Middle", popMiddleAction);
        setStateButton(context, start_x + (++cntx) * add_x,
                       start_y + cnty * add_y, "at First", popFrontAction);
        setStateButton(context, start_x + (++cntx) * add_x,
                       start_y + cnty * add_y, "Clear", ClearAction);
    });

    // set Update Button
    ++cnty;
    auto updateButton =
        std::make_shared<GUI::Button>(*context.fonts, *context.textures);
    updateButton->setPosition(start_x, start_y + cnty * add_y);
    updateButton->setText("Update");
    updateButton->setToggle(true);
    updateButton->setCallback([this, context, cnty]() {
        if (nodeSaver.takeCurrentState() != NodesState::nothing)
            return;
        resetButton(NumInitButton);
        data.clear();

        auto tmp = ([this]() {
            // nothing here now
        });

        int cntx = 0;

        setInputButton(context, start_x + (++cntx) * add_x,
                       start_y + cnty * add_y, "", tmp);
        InputPosition.push_back(mGUIContainer.takeSize() - 1);
        data.push_back("");
        setLabel(context, start_x + (cntx)*add_x + add_x / 2,
                 start_y + cnty * add_y - add_y / 2, "At position", textSize);

        setInputButton(context, start_x + (++cntx) * add_x,
                       start_y + cnty * add_y, "", tmp);
        InputPosition.push_back(mGUIContainer.takeSize() - 1);
        data.push_back("");
        setLabel(context, start_x + (cntx)*add_x + add_x / 2,
                 start_y + cnty * add_y - add_y / 2, "With value", textSize);

        // auto stepByStepActiveAction

        auto runAtOneActiveAction([this, context]() {
            resetButton(NumInitButton + 6, false);
            if (checkInputError(inputID::Pos, 0) &&
                checkInputError(inputID::Val, 1)) {
                nodeSaver.setIsUpdating();
                nodeSaver.setIsRunAtOnce();
                usingData1 = "";
                usingData2 = "";
            }
        });

        auto stepByStepActiveAction([this, context]() {
            resetButton(NumInitButton + 6, false);
            if (checkInputError(inputID::Pos, 0) &&
                checkInputError(inputID::Val, 1)) {
                nodeSaver.setIsUpdating();
                nodeSaver.setIsStepByStep();
                usingData1 = "";
                usingData2 = "";
            }
        });

        setStateButton(context, start_x + (++cntx) * add_x,
                       start_y + cnty * add_y, "Go (Step by Step)",
                       stepByStepActiveAction);
        setStateButton(context, start_x + (++cntx) * add_x,
                       start_y + cnty * add_y, "Go (Run at once)",
                       runAtOneActiveAction);
    });

    // Set Search Button
    ++cnty;
    auto searchButton =
        std::make_shared<GUI::Button>(*context.fonts, *context.textures);
    searchButton->setPosition(start_x, start_y + cnty * add_y);
    searchButton->setText("Search");
    searchButton->setToggle(true);
    searchButton->setCallback([this, context, cnty]() {
        if (nodeSaver.takeCurrentState() != NodesState::nothing)
            return;

        auto action = ([this]() {

        });

        resetButton(NumInitButton);

        int cntx = 0;

        // Accessing

        auto accessAction = ([this, context, cnty]() {
            if (nodeSaver.takeCurrentState() != NodesState::nothing)
                return;
            resetButton(NumInitButton + 2);
            data.clear();

            auto tmp = ([]() {});
            setInputButton(context, start_x + 1 * add_x,
                           start_y + (cnty + 1) * add_y, "", tmp);
            InputPosition.push_back(mGUIContainer.takeSize() - 1);
            data.push_back("");
            setLabel(context, start_x + 3 * add_x + add_x / 2,
                     start_y + (cnty + 1) * add_y - add_y / 2, "Position",
                     textSize);

            auto stepByStepActiveAction = ([this, context] {
                resetButton(NumInitButton + 6, false);
                if (checkInputError(inputID::Pos, 0)) {
                    nodeSaver.setIsAccessing();
                    nodeSaver.setIsStepByStep();
                    usingData1 = "";
                }
            });

            auto runAtOnceActiveAction = ([this, context] {
                resetButton(NumInitButton + 6, false);
                if (checkInputError(inputID::Pos, 0)) {
                    nodeSaver.setIsAccessing();
                    nodeSaver.setIsRunAtOnce();
                    usingData1 = "";
                }
            });

            setStateButton(context, start_x + 3 * add_x,
                           start_y + (cnty + 1) * add_y, "Go (Step by Step)", stepByStepActiveAction);
            setStateButton(context, start_x + 4 * add_x,
                           start_y + (cnty + 1) * add_y, "Go (Run at once)", runAtOnceActiveAction);
        });

        setStateButton(context, start_x + (++cntx) * add_x,
                       start_y + cnty * add_y, "Accessing", accessAction);

        // Searching

        auto searchAction([this, context, cnty]() {
            if (nodeSaver.takeCurrentState() != NodesState::nothing)
                return;
            resetButton(NumInitButton + 2);
            data.clear();

            auto tmp = ([]() {});
            setInputButton(context, start_x + 2 * add_x,
                           start_y + (cnty + 1) * add_y, "", tmp);
            InputPosition.push_back(mGUIContainer.takeSize() - 1);
            data.push_back("");
            setLabel(context, start_x + 3 * add_x + add_x / 2,
                     start_y + (cnty + 1) * add_y - add_y / 2, "Value",
                     textSize);

            auto stepByStepActiveAction = ([this, context] {
                resetButton(NumInitButton + 6, false);
                if (checkInputError(inputID::Val, 0)) {
                    nodeSaver.setIsSearching();
                    nodeSaver.setIsStepByStep();
                    usingData1 = "";
                }
            });

            auto runAtOnceActiveAction = ([this, context] {
                resetButton(NumInitButton + 6, false);
                if (checkInputError(inputID::Val, 0)) {
                    nodeSaver.setIsSearching();
                    nodeSaver.setIsRunAtOnce();
                    usingData1 = "";
                }
            });

            setStateButton(context, start_x + 3 * add_x,
                           start_y + (cnty + 1) * add_y, "Go (Step by Step)", stepByStepActiveAction);
            setStateButton(context, start_x + 4 * add_x,
                           start_y + (cnty + 1) * add_y, "Go (Run at once)", runAtOnceActiveAction);
        });

        setStateButton(context, start_x + (++cntx) * add_x,
                       start_y + cnty * add_y, "Searching", searchAction);
    });

    // Set Speed Button
    auto speedButton =
        std::make_shared<GUI::SpeedButton>(*context.fonts, *context.textures);
    speedButton->setPosition(11 * start_x, start_y);
    speedButton->setText("x1 speed");
    speedButton->setToggle(true);
    speedButton->setCallback([this]() {
        nodeSaver.ChangeSpeed();
    });

    // set Back Button
    ++cnty;
    auto backButton =
        std::make_shared<GUI::Button>(*context.fonts, *context.textures);
    backButton->setPosition(start_x, start_y + cnty * add_y);
    backButton->setText("Back");
    backButton->setToggle(true);
    backButton->setCallback([this]() {
        requestStackPop();
        requestStackPush(States::Menu);
    });

    nodeSaver.init(mSceneGraph, data, context);

    mGUIContainer.pack(initButton);
    mGUIContainer.pack(insertButton);
    mGUIContainer.pack(deleteButton);
    mGUIContainer.pack(updateButton);
    mGUIContainer.pack(searchButton);
    mGUIContainer.pack(backButton);
    mGUIContainer.pack(speedButton);
}

void SLL::draw() {
    sf::RenderWindow& window =
        *getContext().window; // return dereference of context.window
    window.setView(window.getDefaultView());

    window.draw(mBackgroundSprite);
    window.draw(mGUIContainer);
    window.draw(mSceneGraph);
    mDisplayer.draw(window);
}

bool SLL::update(sf::Time dt) {
    nodeSaver.updatePos(mSceneGraph, dt);
    NodesState::ID curState = nodeSaver.takeCurrentState();
    NodesState::ActionType curActionType = nodeSaver.takeCurrentActionType();

    if (curState != NodesState::nothing) {
        if (curActionType == NodesState::RunAtOnce) {
            if (curState == NodesState::isSearching) {
                searchingNode(dt);
            }

            if (curState == NodesState::isAccesing) {
                accessingNode(dt);
            }

            if (curState == NodesState::isUpdating) {
                updatingNode(dt);
            }
        } else
            timeSinceLastHandleEvent += dt;
    }
    // right now there is nothing in here
    return true;
}

bool SLL::handleEvent(const sf::Event& event) {
    mGUIContainer.handleEvent(event);

    // take the data from input button
    for (int i = 0; i < std::min(data.size(), InputPosition.size()); ++i)
        data[i] = mGUIContainer.takeOutString(InputPosition[i]);
    handleRealTimeInput();
    //

    NodesState::ID curState = nodeSaver.takeCurrentState();
    NodesState::ActionType curActionType = nodeSaver.takeCurrentActionType();
    if (curState != NodesState::nothing &&
        curActionType == NodesState::StepByStep) {
        sf::Time dt = timeSinceLastHandleEvent;

        if (curState == NodesState::isSearching) {
            searchingNode(dt, event);
        }

        if (curState == NodesState::isAccesing) {
            accessingNode(dt, event);
        }

        if (curState == NodesState::isUpdating) {
            updatingNode(dt, event);
        }
        timeSinceLastHandleEvent = sf::Time::Zero;
    }
    return false;
}

bool SLL::handleRealTimeInput() {
    sf::RenderWindow& window = *getContext().window;
    mGUIContainer.handleRealTimeInput(window);
    return false;
}

void SLL::setStateButton(Context context, int posx, int posy,
                           const std::string& text,
                           std::function<void()> action) {
    auto stateButton =
        std::make_shared<GUI::Button>(*context.fonts, *context.textures);
    stateButton->setPosition(posx, posy);
    stateButton->setText(text);
    stateButton->setToggle(true);
    auto tmp = action;
    stateButton->setCallback(tmp);
    mGUIContainer.pack(stateButton);
}

void SLL::setLabel(Context context, int posx, int posy,
                     const std::string& text, int sizeoftext) {
    auto Label = std::make_shared<GUI::Label>("", *context.fonts, sizeoftext);
    Label->setPosition(posx, posy);
    Label->setText(text);
    mGUIContainer.pack(Label);
}

void SLL::setInputButton(Context context, int posx, int posy,
                           const std::string& text,
                           std::function<void()> action) {
    auto stateButton =
        std::make_shared<GUI::InputButton>(*context.fonts, *context.textures);
    stateButton->setPosition(posx, posy);
    stateButton->setText(text);
    stateButton->setToggle(true);
    auto tmp = action;
    stateButton->setCallback(tmp);
    mGUIContainer.pack(stateButton);
}

void SLL::resetButton(int size, bool isResetInputButton) {
    while (mGUIContainer.isOutOfSize(size))
        mGUIContainer.depackend();
    nodeSaver.resetColor(mSceneGraph);
    if (isResetInputButton)
        InputPosition.clear();
}

void SLL::printedError(Context context, const std::string& text,
                         int padding) {
    sf::Vector2f pos = context.window->getView().getCenter();
    setLabel(context, pos.x, pos.y + (padding)*add_y, text, textSize * 2);
}

bool SLL::checkInputError(inputID::ID kind, int p) {
    if (kind == inputID::Pos) {

        if (data[p] == "") {
            printedError(getContext(), "You did not input the position");
            return false;
        }

        if (!isContain(data[p], Constants::MINI::numOfNode,
                       Constants::MAXI::numOfNode)) {
            printedError(getContext(),
                         "The position (number) should in [" +
                             std::to_string(Constants::MINI::numOfNode) + ", " +
                             std::to_string(Constants::MAXI::numOfNode) + "]");
            return false;
        }
    }

    if (kind == inputID::Val) {

        if (data[p] == "") {
            printedError(getContext(), "You did not input the value");
            return false;
        }

        if (!isContain(data[p], Constants::MINI::valueOfNode,
                       Constants::MAXI::valueOfNode)) {
            printedError(
                getContext(),
                "The value should in [" +
                    std::to_string(Constants::MINI::valueOfNode) + ", " +
                    std::to_string(Constants::MAXI::valueOfNode) + "]");
            return false;
        }
    }

    return true;
}

void SLL::pushFrontNode(Context context) {
    if (!nodeSaver.pushMiddleNode(mSceneGraph, data[0], 1, context))
        printedError(context, Constants::outOfSizeError);
}

void SLL::pushMiddleNode(Context context, int id) {
    if (!nodeSaver.pushMiddleNode(mSceneGraph, data[1], id, context))
        printedError(context, Constants::outOfSizeError);
}

void SLL::pushBackNode(Context context) {
    if (!nodeSaver.pushBackNode(mSceneGraph, data[0], context))
        printedError(context, Constants::outOfSizeError);
}

void SLL::popFrontNode(Context context) {
    if (!nodeSaver.popFrontNode(mSceneGraph))
        printedError(context, Constants::outOfSizeError);
}

void SLL::popMiddleNode(Context context) {
    if (!nodeSaver.popMiddleNode(mSceneGraph, toNum(data[0])))
        printedError(context, Constants::outOfSizeError);
}

void SLL::popBackNode(Context context) {
    if (!nodeSaver.popBackNode(mSceneGraph))
        printedError(context, Constants::outOfSizeError);
}

void SLL::searchingNode(sf::Time dt, const sf::Event& event) {
    if (usingData1 == "")
        usingData1 = data[0];
    std::string value = usingData1;

    ActionState::ID state =
        nodeSaver.searchingNode(mSceneGraph, dt, value, event);

    if (state == ActionState::DoneFalse) {
        printedError(getContext(), "Can not find");
    }

    if (state == ActionState::DoneTrue) {
        printedError(getContext(), "We find it");
    }
}

void SLL::accessingNode(sf::Time dt, const sf::Event& event) {
    if (usingData1 == "")
        usingData1 = data[0];
    int id = toNum(usingData1);

    ActionState::ID state = nodeSaver.accessingNode(mSceneGraph, dt, id, event);

    if (state == ActionState::DoneFalse) {
        printedError(getContext(), "Position out of data");
    }

    if (state == ActionState::DoneTrue) {
        std::string number = nodeSaver.takeValueOfNode(id);
        printedError(getContext(), "We find it with value is: " + number);
    }
}

void SLL::updatingNode(sf::Time dt, const sf::Event& event) {
    if (usingData1 == "") {
        usingData1 = data[0];
        usingData2 = data[1];
    }
    int id = toNum(usingData1);
    std::string value = usingData2;

    ActionState::ID state =
        nodeSaver.updatingNode(mSceneGraph, dt, id, value, event);

    if (state == ActionState::DoneFalse) {
        printedError(getContext(), "Position out of data");
    }

    if (state == ActionState::DoneTrue) {
        printedError(getContext(), "Update successfully");
    }
}