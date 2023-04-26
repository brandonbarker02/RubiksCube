#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <sstream>
#include <iostream>
#include "Cube.h"
#include <iomanip>
#include <functional>

using namespace std;

//allows user to click button
struct Button {
    sf::RectangleShape shape;
    sf::Text text;
    std::function<void()> onClick;
};

//draws the cube given a vector of all rows
void drawColorImages(std::vector<std::string> layout, sf::RenderWindow &window,
                     const std::map<std::string, sf::Texture> &colorTextures);

//figures out the correct vector to send to drawColorImages
void displayCube(Cube &cube, sf::RenderWindow &window, const std::map<std::string, sf::Texture> &colorTextures);

//handles if a button is clicked
void handleButtonClick(const sf::Vector2f &mouseClickPos, std::vector<Button> &buttons);

//incrementally performs all moves in a given movesest
int doMoveSet(Cube &uiCube, std::vector<std::string> &moveset, int previous, sf::RenderWindow &window,
              const std::map<std::string, sf::Texture> &colorTextures);


int main() {
    float speed = 1.0;
    int moving = 0;
    bool freeControl = false;
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Window");
    window.setFramerateLimit(60);
    std::vector<Button> buttons;
    sf::Vector2f clickPos;
    // Set font and text for moves taken and time taken
    sf::Font font;
    if (!font.loadFromFile("Font/times.ttf")) // Load Times New Roman font
    {
        return EXIT_FAILURE;
    }
    sf::Text movesText("Number of Moves Taken to Solve: 0", font, 18);
    movesText.setPosition(10, 10);
    sf::Color whiteColor(255, 255, 255);
    movesText.setFillColor(whiteColor); // Set color of number of moves taken text to white

    // Set slider and text for slider values
    float leftOffset = 60.0f;
    sf::RectangleShape slider(sf::Vector2f(300, 5));
    slider.setPosition(10 + leftOffset, window.getSize().y - slider.getSize().y - 50);
    slider.setFillColor(sf::Color::White); // Set slider color to white
    sf::Text minVal("1", font, 18); // Initialize text for minimum slider value
    minVal.setPosition(slider.getPosition().x + 5, slider.getPosition().y - minVal.getLocalBounds().height - 10);
    minVal.setFillColor(whiteColor);
    sf::Text maxValue("100,000", font, 18);
    maxValue.setPosition(slider.getPosition().x + slider.getSize().x - maxValue.getLocalBounds().width - 5, slider.getPosition().y - maxValue.getLocalBounds().height - 10);
    maxValue.setFillColor(whiteColor);

    // Set font and text for algorithm turn speed
    sf::Text speedText("Algorithm Turn Speed", font, 18);
    float textX = window.getSize().x - speedText.getLocalBounds().width - 10;
    speedText.setPosition(sf::Vector2f(590, 05));
    speedText.setFillColor(whiteColor); // Set color of algorithm turn speed text to gray

    movesText.setStyle(sf::Text::Bold);
    minVal.setStyle(sf::Text::Bold);
    maxValue.setStyle(sf::Text::Bold);
    speedText.setStyle(sf::Text::Bold);

    float buttonGap = 10;

    // Create the button text
    sf::Text maxRotate;
    maxRotate.setFont(font);
    maxRotate.setString("100,000");
    maxRotate.setCharacterSize(18);
    maxRotate.setFillColor(sf::Color::White);

    // Create the button rectangle
    sf::RectangleShape maxRotateText(sf::Vector2f(maxRotate.getGlobalBounds().width + 25, maxRotate.getGlobalBounds().height + 30));
    maxRotateText.setFillColor(sf::Color::Magenta);

// Create the "Scramble" button
    sf::RectangleShape buttonScramble(sf::Vector2f(100, 50));
    buttonScramble.setPosition(50, window.getSize().y - 75);
    buttonScramble.setFillColor(sf::Color::Blue);
    sf::Text buttonScrambleText("Scramble", font, 18);
    buttonScrambleText.setPosition(60, window.getSize().y - 60);
    buttonScrambleText.setFillColor(sf::Color::White);

// Position the "100,000" button above the "Scramble" button
    float verticalOffset = 10.0f;
    sf::Vector2f buttonPosition(buttonScramble.getPosition().x,
                                buttonScramble.getPosition().y - maxRotateText.getSize().y - verticalOffset);
    maxRotateText.setPosition(buttonPosition);
    maxRotate.setPosition(buttonPosition.x + 10, buttonPosition.y + 10);


// Set buttons for algorithm turn speed
    float width = 25.f;
    float buttonHeight = 16.5f;
    sf::CircleShape button025(buttonHeight);
    button025.setScale(width / buttonHeight, 1);
    button025.setPosition(textX, speedText.getPosition().y + speedText.getLocalBounds().height + buttonGap);
    button025.setFillColor(sf::Color::Magenta);
    sf::Text button025Text(".25x", font, 18);
    button025Text.setPosition(
            button025.getPosition().x + width / 2 - button025Text.getLocalBounds().width / 2,
            button025.getPosition().y + buttonHeight - button025Text.getLocalBounds().height / 1.5);

    sf::CircleShape button050(buttonHeight);
    button050.setScale(width / buttonHeight, 1);
    button050.setPosition(button025.getPosition().x + button025.getGlobalBounds().width + buttonGap,
                          button025.getPosition().y);
    button050.setFillColor(sf::Color::Magenta);
    sf::Text button050Text(".50x", font, 18);
    button050Text.setPosition(
            button050.getPosition().x + width / 2 - button050Text.getLocalBounds().width / 2,
            button050.getPosition().y + buttonHeight - button050Text.getLocalBounds().height / 1.5);

    sf::CircleShape button075(buttonHeight);
    button075.setScale(width / buttonHeight, 1);
    button075.setPosition(button050.getPosition().x + button050.getGlobalBounds().width + buttonGap,
                          button025.getPosition().y);
    button075.setFillColor(sf::Color::Magenta);
    sf::Text button075Text(".75x", font, 18);
    button075Text.setPosition(
            button075.getPosition().x + width / 2 - button075Text.getLocalBounds().width / 2,
            button075.getPosition().y + buttonHeight - button075Text.getLocalBounds().height / 1.5);

    sf::CircleShape button100(buttonHeight);
    button100.setScale(width / buttonHeight, 1);
    button100.setPosition(textX, button025.getPosition().y + button025.getGlobalBounds().height + buttonGap);
    button100.setFillColor(sf::Color::Magenta);
    sf::Text button100Text("1x", font, 18);
    button100Text.setPosition(
            button100.getPosition().x + width / 2 - button100Text.getLocalBounds().width / 2,
            button100.getPosition().y + buttonHeight - button100Text.getLocalBounds().height / 1.5);

    sf::CircleShape buttonInstant(buttonHeight);
    buttonInstant.setScale(width / buttonHeight, 1);
    buttonInstant.setPosition(button100.getPosition().x + button100.getGlobalBounds().width + buttonGap,
                              button100.getPosition().y);
    buttonInstant.setFillColor(sf::Color::Magenta);
    sf::Text buttonInstantText("Instant", font, 18);
    buttonInstantText.setPosition(
            buttonInstant.getPosition().x + width / 2 - buttonInstantText.getLocalBounds().width / 2,
            buttonInstant.getPosition().y + buttonHeight - buttonInstantText.getLocalBounds().height / 1.5);

    //Position and size the text to fit inside the ovals
    float scale = 0.8f;
    sf::Vector2f offset(width * 0.8f, buttonHeight * 0.8f);

    button025Text.setScale(scale, scale);
    button025Text.setOrigin(button025Text.getLocalBounds().width * 0.4f, button025Text.getLocalBounds().height * 0.5f);
    button025Text.setPosition(button025.getPosition() + offset);

    button050Text.setScale(scale, scale);
    button050Text.setOrigin(button050Text.getLocalBounds().width * 0.4f, button050Text.getLocalBounds().height * 0.5f);
    button050Text.setPosition(button050.getPosition() + offset);

    button075Text.setScale(scale, scale);
    button075Text.setOrigin(button075Text.getLocalBounds().width * 0.4f, button075Text.getLocalBounds().height * 0.5f);
    button075Text.setPosition(button075.getPosition() + offset);

    button100Text.setScale(scale, scale);
    button100Text.setOrigin(button100Text.getLocalBounds().width * 0.4f, button100Text.getLocalBounds().height * 0.5f);
    button100Text.setPosition(button100.getPosition() + offset);

    buttonInstantText.setScale(scale, scale);
    buttonInstantText.setOrigin(buttonInstantText.getLocalBounds().width * 0.4f,
                                buttonInstantText.getLocalBounds().height * 0.5f);
    buttonInstantText.setPosition(buttonInstant.getPosition() + offset);

    float buttonVerticalOffset = .00010f;
    // Add three buttons: "Free Control", "Algorithm 1", and "Algorithm 2"
    sf::RectangleShape buttonFreeControl(sf::Vector2f(100, 25));
    buttonFreeControl.setPosition(window.getSize().x - 350,slider.getPosition().y - buttonFreeControl.getSize().y - buttonVerticalOffset);
    buttonFreeControl.setFillColor(sf::Color::Red);
    sf::Text buttonFreeControlText("Free Control", font, 15);
    buttonFreeControlText.setPosition(buttonFreeControl.getPosition().x + 10, buttonFreeControl.getPosition().y + 3);
    buttonFreeControlText.setFillColor(sf::Color::White);

    sf::RectangleShape buttonAlgorithm1(sf::Vector2f(100, 25));
    buttonAlgorithm1.setPosition(window.getSize().x - 220,slider.getPosition().y - buttonAlgorithm1.getSize().y - buttonVerticalOffset);
    buttonAlgorithm1.setFillColor(sf::Color::Red);
    sf::Text buttonAlgorithm1Text("Method 1", font, 15);
    buttonAlgorithm1Text.setPosition(buttonFreeControl.getPosition().x + 143, buttonFreeControl.getPosition().y + 3);
    buttonAlgorithm1Text.setFillColor(sf::Color::White);
    buttonAlgorithm1Text.setFillColor(sf::Color::White);

    sf::RectangleShape buttonAlgorithm2(sf::Vector2f(100, 25));
    buttonAlgorithm2.setPosition(window.getSize().x - 100,slider.getPosition().y - buttonAlgorithm2.getSize().y - buttonVerticalOffset);
    buttonAlgorithm2.setFillColor(sf::Color::Red);
    sf::Text buttonAlgorithm2Text("Method 2", font, 15);
    buttonAlgorithm2Text.setPosition(buttonFreeControl.getPosition().x + 263, buttonFreeControl.getPosition().y + 3);
    buttonAlgorithm2Text.setFillColor(sf::Color::White);
    buttonAlgorithm2Text.setFillColor(sf::Color::White);

    //creates two cube objects and loads the six tile images
    Cube uiCube = Cube();
    Cube backCube = Cube();
    std::vector<std::string> moveset;
    std::map<std::string, sf::Texture> colorTextures;
    std::vector<std::string> colors = {"W", "R", "B", "O", "G", "Y", "WS", "RS", "BS", "OS", "GS", "YS", "WT", "RT", "BT", "OT", "GT", "YT", "border"};
    for (const auto &color: colors) {
        sf::Texture texture;
        if (!texture.loadFromFile(color + ".png")) {
            return 1;
        }
        colorTextures[color] = texture;
    }
    //Free Control button clicked
    Button freeControlButton;
    freeControlButton.shape = buttonFreeControl;
    freeControlButton.text = buttonFreeControlText;
    freeControlButton.onClick = [&]() {
        if (moving == 0) {
            std::cout << "Free Control button clicked!" << std::endl;
            freeControl = true;
        }
    };
    buttons.push_back(freeControlButton);
    //Algorithm 1 button clicked
    Button algorithm1Button;
    algorithm1Button.shape = buttonAlgorithm1;
    algorithm1Button.text = buttonAlgorithm1Text;
    algorithm1Button.onClick = [&]() {
        if (moving == 0) {
            std::cout << "Algorithm 1 button clicked!" << std::endl;
            backCube.beginnersMethodSolve();
            moveset = backCube.getMovesDone();
            moving = 1;
            freeControl = false;
        }
    };
    buttons.push_back(algorithm1Button);
    //Algorithm 2 button clicked
    Button algorithm2Button;
    algorithm2Button.shape = buttonAlgorithm2;
    algorithm2Button.text = buttonAlgorithm2Text;
    algorithm2Button.onClick = [&]() {
        if (moving == 0) {
            std::cout << "Algorithm 2 button clicked!" << std::endl;
            backCube.oldPochmannSolve();
            moveset = backCube.getMovesDone();
            moving = 1;
            freeControl = false;
        }
    };
    buttons.push_back(algorithm2Button);

    //scramble button clicked
    Button scrambleButton;
    scrambleButton.shape = buttonScramble;
    scrambleButton.text = buttonScrambleText;
    scrambleButton.onClick = [&]() {
        if (moving == 0) {
            std::cout << "Scramble button clicked!" << std::endl;
            backCube.scramble(30);
            moveset = backCube.getMovesDone();
            moving = 1;
            freeControl = false;
        }
    };
    buttons.push_back(scrambleButton);

    // Start the game loop
    while (window.isOpen()) {
        // Process events
        sf::Event event;
        if (moving > 0) {
            moving = doMoveSet(uiCube, moveset, moving, window, colorTextures);
        }
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseMoved) {
                int x = event.mouseMove.x;
                int y = event.mouseMove.y;
                if (x >= slider.getPosition().x + minVal.getLocalBounds().width + 10 &&
                    x <= slider.getPosition().x + slider.getSize().x - maxValue.getLocalBounds().width - 10 &&
                    y >= slider.getPosition().y && y <= slider.getPosition().y + slider.getSize().y) {
                    float value = (float) (x - slider.getPosition().x - minVal.getLocalBounds().width - 10) /
                                  (float) (slider.getSize().x - minVal.getLocalBounds().width - 20) * 100000.0f;
                    int intValue = (int) value;
                    std::ostringstream ss;
                    ss << intValue;
                    minVal.setString(ss.str());
                }
            }
            //free control, controls
            if (event.type == sf::Event::KeyPressed && freeControl) { // R-right L-left U-up F-front B-back D-down
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) {
                        backCube.counterClockwiseRotation('R');
                        uiCube.counterClockwiseRotation('R');
                    } else {
                        backCube.clockwiseRotation('R');
                        uiCube.clockwiseRotation('R');
                    }
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::L)) {
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) {
                        backCube.counterClockwiseRotation('L');
                        uiCube.counterClockwiseRotation('L');
                    } else {
                        backCube.clockwiseRotation('L');
                        uiCube.clockwiseRotation('L');
                    }
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::U)) {
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) {
                        backCube.counterClockwiseRotation('U');
                        uiCube.counterClockwiseRotation('U');
                    } else {
                        backCube.clockwiseRotation('U');
                        uiCube.clockwiseRotation('U');
                    }
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F)) {
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) {
                        backCube.counterClockwiseRotation('F');
                        uiCube.counterClockwiseRotation('F');
                    } else {
                        backCube.clockwiseRotation('F');
                        uiCube.clockwiseRotation('F');
                    }
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::B)) {
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) {
                        backCube.counterClockwiseRotation('B');
                        uiCube.counterClockwiseRotation('B');
                    } else {
                        backCube.clockwiseRotation('B');
                        uiCube.clockwiseRotation('B');
                    }
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) {
                        backCube.counterClockwiseRotation('D');
                        uiCube.counterClockwiseRotation('D');
                    } else {
                        backCube.clockwiseRotation('D');
                        uiCube.clockwiseRotation('D');
                    }
                }
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                int mouseX = event.mouseButton.x;
                int mouseY = event.mouseButton.y;
                if (event.mouseButton.button == sf::Mouse::Left) {
                    clickPos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                    if (button025.getGlobalBounds().contains(clickPos)) {
                        // Button 0.25x clicked
                        std::cout << "0.25x button clicked!" << std::endl;
                        speed = 0.25;
                    }
                    if (button050.getGlobalBounds().contains(clickPos)) {
                        // Button 0.50x clicked
                        std::cout << "0.50x button clicked!" << std::endl;
                        speed = 0.5;
                    }
                    if (button075.getGlobalBounds().contains(clickPos)) {
                        // Button 0.75x clicked
                        std::cout << "0.75x button clicked!" << std::endl;
                        speed = 0.75;
                    }
                    if (button100.getGlobalBounds().contains(clickPos)) {
                        // Button 1.0x clicked
                        std::cout << "1.0x button clicked!" << std::endl;
                        speed = 1.0;
                    }
                    if (buttonInstant.getGlobalBounds().contains(clickPos)) {
                        // Button Instant clicked
                        std::cout << "Instant button clicked!" << std::endl;
                        speed = 0.0;
                    }
                    //100,000 clicked, displays on output
                    if (maxRotateText.getGlobalBounds().contains(clickPos)) {
                        std::cout << "100,000 button clicked" << std::endl;
                        Cube cube = Cube();
                        int bestSolve = INT_MAX;
                        int worstSolve = 0;
                        int successes = 0;
                        int fails = 0;
                        long averageRotations = 0;
                        int numSolves = 100000;
                        for (int i = 0; i < numSolves; i++) {
                            std::string newScramble = cube.scramble(50);
                            cube.setNumRotations(0);
                            cube.beginnersMethodSolve();
                            if (cube.isSolved()) {
                                if (cube.getNumRotations() > worstSolve) {
                                    worstSolve = cube.getNumRotations();
                                }
                                if (cube.getNumRotations() < bestSolve) {
                                    bestSolve = cube.getNumRotations();
                                }
                                successes++;
                                averageRotations += (long) cube.getNumRotations();
                            } else {
                                std::cout << " Failure! Scramble was: " << newScramble << std::endl;
                                fails++;
                            }
                        }
                        averageRotations /= successes;
                        float successRate = (float) successes / (successes + fails) * 100;
                        std::cout << numSolves << " solves with beginners method completed. Success rate = "
                                  << std::fixed << std::setprecision(2)
                                  << successRate << "% Average Number of Moves = " << averageRotations
                                  << " Best solve = " << bestSolve << " moves Worst solve = " << worstSolve << " moves" << std::endl;
                        bestSolve = INT_MAX;
                        worstSolve = 0;
                        successes = 0;
                        averageRotations = 0;
                        fails = 0;
                        numSolves = 100000;
                        for (int i = 0; i < numSolves; i++) {
                            std::string newScramble = cube.scramble(50);
                            cube.setNumRotations(0);
                            cube.oldPochmannSolve();
                            if (cube.isSolved()) {
                                if (cube.getNumRotations() > worstSolve) {
                                    worstSolve = cube.getNumRotations();
                                }
                                if (cube.getNumRotations() < bestSolve) {
                                    bestSolve = cube.getNumRotations();
                                }
                                successes++;
                                averageRotations += (long) cube.getNumRotations();
                            } else {
                                std::cout << " Failure! Scramble was: " << newScramble << std::endl;
                                fails++;
                            }
                        }
                        averageRotations /= successes;
                        successRate = (float) successes / (successes + fails) * 100;
                        std::cout << numSolves << " solves with Old Pochmann method completed. Success rate = "
                                  << std::fixed << std::setprecision(2)
                                  << successRate << "% Average Number of Moves = " << averageRotations
                                  << " Best solve = " << bestSolve << " moves Worst solve = " << worstSolve << " moves" << std::endl;
                    }
                    handleButtonClick(clickPos, buttons);
                }
                if (mouseX >= slider.getPosition().x +
                              ((float) atoi(minVal.getString().toAnsiString().c_str()) / 100000.0f) *
                              slider.getSize().x - 10 && mouseX <= slider.getPosition().x + ((float) atoi(
                        minVal.getString().toAnsiString().c_str()) / 100000.0f) * slider.getSize().x + 10 &&
                    mouseY >= slider.getPosition().y - 10 &&
                    mouseY <= slider.getPosition().y + slider.getSize().y + 10) {
                    while (window.pollEvent(event)) {
                        if (event.type == sf::Event::MouseButtonReleased) {
                            break;
                        }
                    }
                }
            }
        }
        window.clear();
        displayCube(uiCube, window, colorTextures);
        std::ostringstream ss2;
        ss2 << "Number of Rotations: " << backCube.getNumRotations();
        movesText.setString(ss2.str());

        // Draw text for number of moves taken and time taken
        window.draw(movesText);

        // Draw buttons for algorithm turn speed
        window.draw(speedText);
        window.draw(button025);
        window.draw(button025Text);
        window.draw(button050);
        window.draw(button050Text);
        window.draw(button075);
        window.draw(button075Text);
        window.draw(button100);
        window.draw(button100Text);
        window.draw(buttonInstant);
        window.draw(buttonInstantText);

        //draw the new buttons and text
        window.draw(buttonFreeControl);
        window.draw(buttonFreeControlText);
        window.draw(buttonAlgorithm1);
        window.draw(buttonAlgorithm1Text);
        window.draw(buttonAlgorithm2);
        window.draw(buttonAlgorithm2Text);

        //draw the new button and text
        window.draw(buttonScramble);
        window.draw(buttonScrambleText);

        window.draw(maxRotateText);
        window.draw(maxRotate);

        for (const Button &button: buttons) {
            window.draw(button.shape);
            window.draw(button.text);
        }
        // Update the window
        window.display();
        if (speed != 0) {
            usleep((int) (100000 * (1 / speed)));
        }
    }
    return 0;
}

//draws colored tiles
void drawColorImages(std::vector<std::string> layout, sf::RenderWindow &window,
                     const std::map<std::string, sf::Texture> &colorTextures) {
    int tileSize = 20;
    int offsetX = 10;
    int offsetY = 20;
    std::vector<std::pair<int, int>> offsets = {{3, 1},
                                                {0, 3},
                                                {3, 3},
                                                {6, 3},
                                                {9, 3},
                                                {3, 6}};
    int rowNum = 0;
    int x = 0;
    int y = 0;
    int numIterations = 0;
    for (const auto &row: layout) {
        for (const auto &color: row) {
            sf::Sprite sprite(colorTextures.at(std::string(1, color)));
            sprite.setPosition(offsetX + (x + offsets[rowNum].first) * tileSize,
                               offsetY + (y + offsets[rowNum].second) * tileSize);
            sprite.setScale(sf::Vector2f(.05f, .05f));
            window.draw(sprite);
            x++;
        }
        if (numIterations % 3 == 2) {
            rowNum++;
            y = 0;
        }
        numIterations++;
        x = 0;
        y++;
    }
    int xPos = 250;
    int yPos = 275;
    tileSize = 72;
    offsetX = xPos;
    offsetY = yPos;
    //print big front
    for (int i = 6; i < 9; i++) {//iterate through each row
        for (int j = 0; j < layout[i].size(); j++) { //iterate through each column
            sf::Sprite sprite(colorTextures.at(std::string(1, layout[i][j])));
            sprite.setPosition(offsetX + (j) * tileSize,
                               offsetY + (i-6) * tileSize);
            sprite.setScale(sf::Vector2f(.2f, .2f));
            window.draw(sprite);
        }
    }
    tileSize = 73;
    offsetX = xPos+105;
    offsetY = yPos-135;
    //print big top
    for (int i = 0; i < 3; i++) {//iterate through each row
        for (int j = 0; j < layout[i].size(); j++) { //iterate through each column
            std::string print = "";
            print.push_back(layout[i][j]);
            print.push_back('T');
            sf::Sprite sprite(colorTextures.at(print));
            sprite.setPosition(((0-i) * 54) +offsetX + (j) * tileSize,
                               offsetY + (i) * (tileSize - 30));
            sprite.setScale(sf::Vector2f(.53f, .4f));
            window.draw(sprite);
        }
    }
    tileSize = 73;
    offsetX = xPos+205;
    offsetY = yPos-45;
    //print big side
    for (int i = 9; i < 12; i++) {//iterate through each row
        for (int j = 0; j < layout[i].size(); j++) { //iterate through each column
            std::string print = "";
            print.push_back(layout[i][j]);
            print.push_back('S');
            sf::Sprite sprite(colorTextures.at(print));
            sprite.setPosition(offsetX + (j) * (tileSize - 20),offsetY + (i-9) * (tileSize) - ((j) * 45));
            sprite.setScale(sf::Vector2f(.52f, .52f));
            window.draw(sprite);
        }
    }
    //print border
    sf::Sprite border(colorTextures.at("border"));
    border.setPosition(xPos-27, yPos-138);
    border.setScale(sf::Vector2f(.8315f, .8315f));
    window.draw(border);
}

//extracting current color of a Rubiks Cube and then drawing it
void displayCube(Cube &cube, sf::RenderWindow &window, const std::map<std::string, sf::Texture> &colorTextures) {
    std::vector<std::string> layout;
    std::vector<std::string> layout0 = cube.findCommon({{'U', 'B', 'L', 'R'}}, 0);
    for (int i = 0; i < layout0.size(); i++)
        layout.push_back(layout0[i]);
    layout0.clear();
    layout0 = cube.findCommon({{'L', 'U', 'B', 'F'}}, 0);
    for (int i = 0; i < layout0.size(); i++)
        layout.push_back(layout0[i]);
    layout0.clear();
    layout0 = cube.findCommon({{'F', 'U', 'L', 'R'}}, 0);
    for (int i = 0; i < layout0.size(); i++)
        layout.push_back(layout0[i]);
    layout0.clear();
    layout0 = cube.findCommon({{'R', 'U', 'F', 'B'}}, 0);
    for (int i = 0; i < layout0.size(); i++)
        layout.push_back(layout0[i]);
    layout0.clear();
    layout0 = cube.findCommon({{'B', 'U', 'L', 'R'}}, 0);
    for (int i = 0; i < layout0.size(); i++)
        layout.push_back(layout0[i]);

    layout0 = cube.findCommon({{'D', 'F', 'L', 'R'}}, 0);
    for (int i = 0; i < layout0.size(); i++)
        layout.push_back(layout0[i]);
    drawColorImages(layout, window, colorTextures);
}

//shows button clicks to ensure click is working
void handleButtonClick(const sf::Vector2f &mouseClickPos,
                       sf::RectangleShape &freeControlButton,
                       sf::RectangleShape &algorithm1Button,
                       sf::RectangleShape &algorithm2Button,
                       sf::RectangleShape &scrambleButton,
                       sf::CircleShape &button025, sf::Text &button025Text,
                       sf::CircleShape &button050, sf::Text &button050Text,
                       sf::CircleShape &button075, sf::Text &button075Text,
                       sf::CircleShape &button100, sf::Text &button100Text,
                       sf::CircleShape &buttonInstant, sf::Text &buttonInstantText) {

    if (button025.getGlobalBounds().contains(mouseClickPos)) {
        // Button 0.25x clicked
        std::cout << "0.25x button clicked!" << std::endl;
    }
    if (button050.getGlobalBounds().contains(mouseClickPos)) {
        // Button 0.50x clicked
        std::cout << "0.50x button clicked!" << std::endl;
    }
    if (button075.getGlobalBounds().contains(mouseClickPos)) {
        // Button 0.70x clicked
        std::cout << "0.70x button clicked!" << std::endl;
    }
    if (button100.getGlobalBounds().contains(mouseClickPos)) {
        // Button 0.50x clicked
        std::cout << "0.100x button clicked!" << std::endl;
    }
    if (buttonInstant.getGlobalBounds().contains(mouseClickPos)) {
        // Button 0.50x clicked
        std::cout << "Instant button clicked!" << std::endl;
    }
}

//executing moves on rubiks cibe
int doMoveSet(Cube &uiCube, std::vector<std::string> &moveset, int previous, sf::RenderWindow &window,
              const std::map<std::string, sf::Texture> &colorTextures) {
    std::vector<std::string> currentMove;
    if (previous > moveset.size()) {
        return 0;
    }
    currentMove.push_back(moveset[previous - 1]);
    uiCube.algorithm(currentMove);
    return previous + 1;
}

void handleButtonClick(const sf::Vector2f &mouseClickPos, std::vector<Button> &buttons) {
    for (Button &button: buttons) {
        if (button.shape.getGlobalBounds().contains(mouseClickPos)) {
            // Button clicked - run the associated onClick function
            button.onClick();
        }
    }
}