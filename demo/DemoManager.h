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

            void onButtonPressed(const pmgui::Button *btn);

            sf::RenderWindow m_window;
            sf::Clock m_deltaClock;
            sf::Color m_backgroundColor = {35, 65, 90, 255};

            TextEditorForm m_codeEditor {"code_editor", "Code Editor"};
            Form m_demoForm {"demo_form", "PmGui Demo"};
            Textbox *m_demoText1 = nullptr;
            Button *m_openFileOpenDialogBtn = nullptr;
            Button *m_openDirectoryOpenDialogBtn = nullptr;
            Button *m_openFileSaveDialogBtn = nullptr;
            Button *m_openCodeEditorBtn = nullptr;

            FileDialog m_fileDialogFile { DialogType::OpenFile, "main_file_dialog_file", "Choose a file", false}; //, "main_file_dialog_file"};
            FileDialog m_fileDialogFolder { DialogType::OpenDirectory, "main_file_dialog_folder", "Choose a folder", false}; //, "main_file_dialog_file"};
            FileDialog m_fileDialogSave { DialogType::SaveFile, "main_file_dialog_save_file", "Save a file", false}; //, "main_file_dialog_file"};

    };
}

#endif //PMGUI_DEMO_DEMOMANAGER_H
