#include "../PmguiConfig.h"
#include <iostream>
#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"
#include "fmt/format.h"
#include "DemoManager.h"


int main()
{
    pmgui::DemoManager demo;
    sf::VideoMode mode = sf::VideoMode::getDesktopMode();
    demo.initialize("Pmgui Demo", {mode.width, mode.height});

    demo.run();
    return 0;
}