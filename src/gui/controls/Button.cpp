//
// Created by robin on 01.01.18.
//

#include "Button.h"

/*!
 * More stack memory friendly constructor.
 * Remember to call create() before running process()
 *
 * @param id
 */
pmgui::Button::Button(std::string id) : Control(std::move(id))
{

}


pmgui::Button::Button(const std::string &id, const std::string &label, const sf::Vector2<int> &size) : Control(id, label),
                                                                                                     m_size {size}
{
    //m_size = size;
    m_type = ControlType::Button;
}

/*!
 * Only needs to be called when calling the most basic constructor
 * @param label
 * @param size
 */
void pmgui::Button::create(const std::string &label, const sf::Vector2<int> &size)
{
    m_label = label;
    m_size = size;
    updateImguiId();
}

/*!
 *
 * @return true if button is pressed, false otherwise
 */
bool pmgui::Button::process()
{
    if(Control::process())
    {
        m_isPressed = false;
        bool popColors = false;
        bool toReturn = false;
        if (m_colorIsChanged && !m_useDefaultColor)
        {
            assignColors();
            popColors = true;
        }

        if (ImGui::Button(m_imguiId.c_str(), ImVec2(m_size)))
        {
            m_isPressed = true;
            toReturn = true;

            for(const auto &callback : m_callbackOnPressed)
                callback(this);
        }

        if (popColors && !m_useDefaultColor)
        {
            ImGui::PopStyleColor(3);
        }

        return toReturn;
    }
    return false;
}


bool pmgui::Button::isPressed() const
{
    return m_isPressed;
}

void pmgui::Button::assignColors()
{
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(m_color));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(m_colorHovered));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(m_colorPressed));
}

void pmgui::Button::setColor(const sf::Color &color, const sf::Color &colorHovered, const sf::Color &colorPressed)
{
    m_color = color;
    m_colorHovered = colorHovered;
    m_colorPressed = colorPressed;
    m_colorIsChanged = true;
}

const std::string &pmgui::Button::getCommand() const
{
    return m_command;
}

void pmgui::Button::setCommand(const std::string &command)
{
    m_command = command;
}

void pmgui::Button::registerOnPressedCallback(const std::function<void(pmgui::Button*)> &cb)
{
    m_callbackOnPressed.emplace_back(cb);
}

void pmgui::Button::clearCallbacks()
{
    m_callbackOnPressed.clear();
}

const sf::Vector2<int> &pmgui::Button::getSize() const
{
    return m_size;
}

void pmgui::Button::setSize(const sf::Vector2<int> &size)
{
    m_size = size;
}







