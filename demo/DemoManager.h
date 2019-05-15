//
// Created by robin on 06.05.19.
//

#ifndef PMGUI_DEMO_DEMOMANAGER_H
#define PMGUI_DEMO_DEMOMANAGER_H

#include "../src/misc/Tools.hpp"
#include <unordered_map>
#include "SFML/Graphics.hpp"
#include "SFML/Main.hpp"
#include "../src/gui/forms/Form.h"
#include "../src/gui/controls/Textbox.h"
#include "../src/pmgui.h"

namespace pmgui
{
    class DemoManager
    {
        public:
            DemoManager() = default;

            void initialize(const std::string &title, const sf::Vector2<uint32_t> &resolution, sf::Uint32 style = sf::Style::Default,
                            const sf::ContextSettings &settings = sf::ContextSettings());

            void run();

        protected:
            void createDemo();
            void update();

            void handleEvents();

            void draw();

            sf::RenderWindow m_window;
            sf::Clock m_deltaClock;
            sf::Color m_backgroundColor = {35, 65, 90, 255};

            Form m_demoForm {"demo_form", "PmGui Demo"};
            Textbox *m_demoText1 = nullptr;

            FileDialog m_fileDialogFile { DialogType::OpenFile, "main_file_dialog_file", "Choose a file", "main_file_dialog_file"};

    };
}

#endif //PMGUI_DEMO_DEMOMANAGER_H
