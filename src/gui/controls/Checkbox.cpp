//
// Created by robin on 14.01.18.
//

#include "Checkbox.h"

pmgui::Checkbox::Checkbox() : Control("", "")
{
    m_type = ControlType::Checkbox;
}

pmgui::Checkbox::Checkbox(const std::string &id, const std::string &label, bool isChecked) : Control(id, label),
                                                                                           m_isChecked {isChecked}
{
    m_type = ControlType::Checkbox;
}

void pmgui::Checkbox::initialize(const std::string &id, const std::string &label, bool isChecked)
{
    m_id = id;
    m_label = label;
    m_isChecked = isChecked;
    updateImguiId();
}

/*!
 *
 * @return true if checkbox is check/unchecked, false otherwise
 */
bool pmgui::Checkbox::process()
{
    if(Control::process())
    {
        pushWidth();
        bool action = ImGui::Checkbox(m_imguiId.c_str(), &m_isChecked);
        popWidth();
        if(ImGui::IsItemHovered() && m_tooltip.has_value())
            m_tooltip->show();

        return action;
    }

    return false;
}

bool pmgui::Checkbox::isChecked() const
{
    return m_isChecked;
}

void pmgui::Checkbox::setChecked(bool isChecked)
{
    m_isChecked = isChecked;
}

/*!
 * Returns a pointer to the actual bool.
 * If you don't need the pointer, use isChecked() instead.
 * @return
 */
bool * pmgui::Checkbox::getChecked()
{
    return &m_isChecked;
}

bool pmgui::Checkbox::toggle()
{
    m_isChecked = !m_isChecked;
    return m_isChecked;
}
