//
// Created by robin on 22.02.18.
//

#ifndef PMGUI_POPUP_H
#define PMGUI_POPUP_H

#include "Form.h"

namespace pmgui
{
    /*!
     * A popup works pretty much like a Form, but puts the popup in focus, so you cannot click outside of it.
     */
    class Popup : public Form
    {
        public:
            Popup(std::string id, std::string title); //, std::string imguiId = "");

            Popup(const sf::Vector2<int> &position, const sf::Vector2<int> &size, std::string id,
                  std::string title); //, std::string imguiId = "");

            bool draw() override;
            void handleEvents() override;
    };
}

#endif //PMGUI_POPUP_H
