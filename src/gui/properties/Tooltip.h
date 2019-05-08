//
// Created by robin on 11.01.18.
//

#ifndef PMGUI_TOOLTIP_H
#define PMGUI_TOOLTIP_H

#include <iostream>
#include "../../fmt/format.h"
#include "../../imgui/imgui.h"
#include "../../imgui/imgui-SFML.h"

namespace pmgui
{
    enum class TooltipType : uint32_t
    {
        None = 0,
        Basic = 1
    };

    class Tooltip
    {
        public:
            Tooltip();
            Tooltip(const std::string &text);

            virtual void show();

            void setText(const std::string &text);

            const std::string &getText() const;

        protected:
            std::string m_text = "";
            TooltipType m_type;
    };
}

#endif //PMGUI_TOOLTIP_H
