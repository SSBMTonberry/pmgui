//
// Created by robin on 01.01.18.
//

#include "Label.h"

/*!
 * More stack memory friendly constructor.
 * Remember to call create() before running process()
 *
 * @param id
 */
pmgui::Label::Label(std::string id) : Control(std::move(id))
{

}

pmgui::Label::Label(const std::string &id, const std::string &label, std::string text, const LabelType &labelType)
                  : Control(id, label),m_text {std::move(text)}, m_hasTitle {false}, m_labelType {labelType}
{
    m_type = ControlType::Label;
    //m_text = text;
    //m_hasTitle = false;
}

/*!
 * Only needs to be called when calling the most basic constructor
 * @param label MANDATORY Always used to display a value
 * @param text NOT mandatory. Can be empty
 * @param labelType LabelWithTitle = uses both label and text, OnlyText = Only uses label and displays it as text
 */
void pmgui::Label::create(const std::string &label, const std::string &text, const LabelType &labelType)
{
    m_label = label;
    m_text = text;
    m_labelType = labelType;
}

/*!
 *
 * @return returns true if visible, false otherwise
 */
bool pmgui::Label::process()
{
    if(Control::process())
    {
        bool popColors = false;
        if (m_colorIsChanged && !m_useDefaultColor)
        {
            assignColors();
            popColors = true;
        }

        if(m_labelType == LabelType::OnlyLabel)
        {
            ImGui::Text(m_label.c_str());
        }
        else
        {
            if (!m_hasTitle)
                ImGui::PushItemWidth(-1);

            ImGui::LabelText(m_label.c_str(), m_text.c_str());

            if (!m_hasTitle)
                ImGui::PopItemWidth();
        }

        if (popColors && !m_useDefaultColor)
        {
            ImGui::PopStyleColor(1);
        }

        if (ImGui::IsItemHovered() && m_tooltip.has_value())
            m_tooltip->show();

        return true;
    }

    return false;
}

std::string pmgui::Label::getValue() const
{
    return m_text;
}

void pmgui::Label::setValue(const std::string &text)
{
    m_text = text;
}

void pmgui::Label::setColor(const sf::Color &text)
{
    m_textColor = text;
    m_colorIsChanged = true;
}

void pmgui::Label::assignColors()
{
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(m_textColor));
}

bool pmgui::Label::hasTitle() const
{
    return m_hasTitle;
}

void pmgui::Label::setHasTitle(bool hasTitle)
{
    m_hasTitle = hasTitle;
}



