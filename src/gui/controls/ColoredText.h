//
// Created by robin on 12.08.18.
//

#ifndef PMGUI_COLOREDTEXT_H
#define PMGUI_COLOREDTEXT_H

#include "Control.h"

namespace pmgui
{
    class ColoredText : public pmgui::Control
    {
        public:
            ColoredText();
            ColoredText(const std::string &text, const sf::Color &color = {255, 255, 255, 255});
            ColoredText(const std::string &id, const std::string &text, const sf::Color &color = {255, 255, 255, 255});

            bool process() override;

            void setText(const std::string &text);
            const std::string &getText() const;

        protected:
            sf::Color m_color = {255,255,255,255};
    };
}

#endif //PMGUI_COLOREDTEXT_H
