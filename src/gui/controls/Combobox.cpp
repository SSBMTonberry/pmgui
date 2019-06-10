//
// Created by robin on 12.01.18.
//

#include "Combobox.h"

pmgui::Combobox::Combobox(const std::string &id, const std::string &label) : Control(id, label)
{
    m_type = ControlType::Combobox;
}

/*!
 *
 * @return true if value is chosen. false otherwise
 */
bool pmgui::Combobox::process()
{
    if(Control::process())
    {
        bool popColors = false;
        if (m_colorIsChanged && !m_useDefaultColor)
        {
            assignColors();
            popColors = true;
        }

        //char* currentItem[255];
        m_valueHasBeenChosen = false;
        if(!m_hasLabel && !m_disablePushItemWidth)
            ImGui::PushItemWidth(-1);
        else
            pushWidth();

        std::string id = (!m_hasLabel && m_disablePushItemWidth) ? fmt::format("###{0}", m_id) : m_label;

        if (ImGui::BeginCombo(id.c_str(), m_currentItem))
        {
            for (int i = 0; i < m_list.size(); i++)
            {
                bool is_selected = (m_currentItem == nullptr) ? false : (m_list[i].compare(m_currentItem) == 0);
                if (ImGui::Selectable(m_list[i].c_str(), is_selected))
                {
                    m_currentItem = m_list[i].c_str();
                    m_valueHasBeenChosen = true;
                }

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        if(!m_hasLabel && !m_disablePushItemWidth)
            ImGui::PopItemWidth();
        else
            popWidth();

        if (popColors && !m_useDefaultColor)
        {
            ImGui::PopStyleColor(2);
        }

        if (m_currentItem != nullptr)
            m_value = m_currentItem;

        if (ImGui::IsItemHovered() && m_tooltip.has_value())
            m_tooltip->show();

        return m_valueHasBeenChosen;
    }

    return false;
}

void pmgui::Combobox::addValue(const std::string &value)
{
    m_list.push_back(value);
}

void pmgui::Combobox::removeValue(const std::string &value)
{
    for(int i = 0; i < m_list.size(); i++)
    {
        if(m_list[i].compare(value) == 0)
        {
            m_list.erase(m_list.begin() + i);
            i--;
        }
    }
}

void pmgui::Combobox::clear()
{
    m_list.clear();
}

void pmgui::Combobox::addValueRange(const std::initializer_list<std::string> &values)
{
    for(const auto & item : values)
    {
        addValue(item);
    }
}

void pmgui::Combobox::setColor(const sf::Color &text, const sf::Color &background)
{
    m_textColor = text;
    m_backgroundColor = background;
    m_colorIsChanged = true;
}

void pmgui::Combobox::assignColors()
{
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(m_textColor));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(m_backgroundColor));
}

const std::string &pmgui::Combobox::getValue() const
{
    return m_value;
}

bool pmgui::Combobox::hasLabel() const
{
    return m_hasLabel;
}

/*!
 *
 * @param hasLabel Has label?
 * @param disablePushItemWidth Use this in cases where you want to disable labels inside popups, or else they might expand throughout the screen.
 */
void pmgui::Combobox::setHasLabel(bool hasLabel, bool disablePushItemWidth)
{
    m_hasLabel = hasLabel;
    m_disablePushItemWidth = disablePushItemWidth;
}

void pmgui::Combobox::setValue(int index)
{
    if(index < m_list.size())
    {
        m_currentItem = m_list[index].c_str();
        m_value = m_currentItem;
        m_valueHasBeenChosen = true;
    }
}

/*!
 * Sets value by string, if it exists.
 * @param value
 * @return true if value was found, false otherwise. If value has not been found, first element is set.
 */
bool pmgui::Combobox::setValue(const std::string &value)
{
    for(int i = 0; i < m_list.size(); ++i)
    {
        if(m_list[i] == value)
        {
            setValue(i);
            return true;
        }
    }

    setValue(0);
    return false;
}

bool pmgui::Combobox::valueHasBeenChosen() const
{
    return m_valueHasBeenChosen;
}

/*!
 * Gets the index of the chosen value
 * @return -1 if list is empty or no value otherwise has been chosen. The index otherwise (0 = first element)
 */
int pmgui::Combobox::getIndex()
{
    for(int i = 0; i < m_list.size(); i++)
    {
        if(!m_value.empty() && m_value == m_list[i])
            return i;
    }

    return -1;
}

/*!
 * Gets the index of a specific ID
 * @param value
 * @return -1 if ID is invalid. The index of the ID otherwise
 */
int pmgui::Combobox::getIndex(const std::string &value)
{
    for(int i = 0; i < m_list.size(); i++)
    {
        if(!value.empty() && value == m_list[i])
            return i;
    }
    return -1;
}

/*!
 * Selects next item. if on the bottom of the list, it will go back to the top.
 */
void pmgui::Combobox::selectNext()
{
    int currentIndex = getIndex();
    if(currentIndex > -1 || m_list.size() > 0)
    {
        int maxIndex = m_list.size() - 1;
        currentIndex = (currentIndex == maxIndex) ? 0 : ++currentIndex;
        setValue(currentIndex);
    }
}

/*!
 * Selects the previous item. if on the top of the list, it will go to the bottom.
 */
void pmgui::Combobox::selectPrevious()
{
    int currentIndex = getIndex();
    if(currentIndex > -1 || m_list.size() > 0)
    {
        int maxIndex = m_list.size() - 1;
        currentIndex = (currentIndex <= 0) ? maxIndex : --currentIndex;
        setValue(currentIndex);
    }
}
