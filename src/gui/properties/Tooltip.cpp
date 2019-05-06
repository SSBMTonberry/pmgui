//
// Created by robin on 11.01.18.
//

#include "Tooltip.h"


pmgui::Tooltip::Tooltip()
{
    m_type = TooltipType::Basic;
}

pmgui::Tooltip::Tooltip(const std::string &text)
{
    m_text = text;
    m_type = TooltipType::Basic;
}

void pmgui::Tooltip::show()
{
    ImGui::SetTooltip(m_text.c_str());
}

const std::string &pmgui::Tooltip::getText() const
{
    return m_text;
}

void pmgui::Tooltip::setText(const std::string &text)
{
    m_text = text;
}


