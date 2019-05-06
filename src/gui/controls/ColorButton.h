//
// Created by robin on 28.01.18.
//

#ifndef PMGUI_COLORBUTTON_H
#define PMGUI_COLORBUTTON_H

#include "Control.h"

namespace pmgui
{
    class ColorButton : public Control
    {
        public:
            ColorButton(const std::string &id, const std::string &label, const sf::Color &color, const sf::Vector2<int> &size = {0,0});

            bool process() override;

            const sf::Color &getColor() const;

            void setColor(const sf::Color &color);

        protected:
            sf::Color m_color;
            sf::Vector2<int> m_size;
            bool m_isClicked = false;
    };
}

#endif //PMGUI_COLORBUTTON_H
