//
// Created by robin on 31.12.17.
//

#ifndef PMGUI_TEXTEDITORFORM_H
#define PMGUI_TEXTEDITORFORM_H


#include "Form.h"
#include "../TextEditor.h"
namespace pmgui
{
    class TextEditorForm : public Form
    {
        public:
            TextEditorForm(std::string id, std::string title); //, std::string imguiId = "");
            TextEditorForm(const sf::Vector2<int> &position, const sf::Vector2<int> &size, std::string id, std::string title); //, std::string imguiId = "");
            bool draw() override;

            TextEditor *getEditor();

        private:
            void initialize();

            TextEditor m_textEditor;
    };
}

#endif //PMGUI_TEXTEDITORFORM_H
