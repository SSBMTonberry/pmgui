//
// Created by robin on 13.01.18.
//

#include "RadioSection.h"

pmgui::RadioSection::RadioSection(const std::string &id, const std::string &label) : Control(id, label)
{
    m_type = ControlType::RadioSection;
}

void pmgui::RadioSection::addRadioData(int id, const std::string &label)
{
    m_data.emplace_back(id, label);
}

/*!
 *
 * @return true if any radio-button is pressed. false otherwise
 */
bool pmgui::RadioSection::process()
{
    if(Control::process())
    {
        int i = 0;
        bool anythingPressed = false;
        for (const auto &item : m_data)
        {
            bool sameLine = (!m_isHorizontal) ? false : isSameLine(i);

            pushWidth();
            bool action = ImGui::RadioButton(item.getLabel().c_str(), &m_chosenItem, item.getId());
            popWidth();

            if(action)
                anythingPressed = true;

            //Do not apply SameLine on the last element
            if (sameLine && i < m_data.size() - 1)
                ImGui::SameLine();

            ++i;
        }

        return anythingPressed;
    }
    return false;
}

std::string pmgui::RadioSection::getValue() const
{
    std::string value = "";
    for(const auto & item : m_data)
    {
        if(item.getId() == m_chosenItem)
            value = item.getLabel();
    }
    return value;
}

pmgui::RadioData * pmgui::RadioSection::getValue()
{
    for(auto & item : m_data)
    {
        if(item.getId() == m_chosenItem)
            return &item;
    }

    return nullptr;
}

void pmgui::RadioSection::setHorizontal(size_t itemsPerRow)
{
    m_itemsPerRow = itemsPerRow;
    m_isHorizontal = true;
}

void pmgui::RadioSection::setVertical()
{
    m_isHorizontal = false;
}

void pmgui::RadioSection::setValue(int index)
{
    for (auto &i : m_data)
    {
        if(index == i.getId())
            m_chosenItem = index;
    }
}

bool pmgui::RadioSection::isSameLine(int i)
{
    if(m_itemsPerRow == 0)
        return true;

    int value = i + 1;
    bool sameLine = false;

    sameLine = ((value % m_itemsPerRow) != 0) ? true : false;

    return sameLine;
}