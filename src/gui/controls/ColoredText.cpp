//
// Created by robin on 12.08.18.
//

#include "ColoredText.h"

pmgui::ColoredText::ColoredText() : Control("undef", "")
{
    m_type = ControlType::ColoredText;
}

pmgui::ColoredText::ColoredText(const std::string &text, const sf::Color &color) : Control("<undef>", text), m_color {color}
{
    m_type = ControlType::ColoredText;
}

pmgui::ColoredText::ColoredText(const std::string &id, const std::string &text, const sf::Color &color)
                              : Control(id, text), m_color {color}
{
    m_type = ControlType::ColoredText;
}


bool pmgui::ColoredText::process()
{
    if(Control::process())
    {
        ImGui::PushStyleColor(0, m_color);
        pushWidth();
        ImGui::Text(m_label.c_str());
        popWidth();
        ImGui::PopStyleColor(1);
        return true;
    }
    return false;
}

/*!
 * Set text.
 * Actually uses the label-string which is a part of the base-class, but made a setText-wrapper
 * to make more sense.
 * @param text
 */
void pmgui::ColoredText::setText(const std::string &text)
{
    m_label = text;
}

/*!
 *
 * @return The text, which has its value contained in the label-variable of the Control base-class.
 * getLabel() would give the same value, as getText is a wrapper to the label-variable.
 */
const std::string &pmgui::ColoredText::getText() const
{
    return m_label;
}
