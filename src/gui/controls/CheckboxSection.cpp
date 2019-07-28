//
// Created by robin on 14.01.18.
//

#include "CheckboxSection.h"

pmgui::CheckboxSection::CheckboxSection(const std::string &id, const std::string &label) : Control(id, label)
{
    m_type = ControlType::CheckboxSection;
}

/*!
 *
 * @return true if any checkbox is checked/unchecked. false otherwise.
 */
bool pmgui::CheckboxSection::process()
{

    if(Control::process())
    {
        int i = 0;
        bool hasAnythinngBeenChecked = false;
        for (auto & [id, item] : m_data)
        {
            bool sameLine = (!m_isHorizontal) ? false : isSameLine(i);

            bool action = ImGui::Checkbox(item->getImguiId().c_str(), item->getChecked());
            if(action)
                hasAnythinngBeenChecked = action;

            //Do not apply SameLine on the last element
            if (sameLine && i < m_data.size() - 1)
                ImGui::SameLine();

            ++i;
        }

        return hasAnythinngBeenChecked;
    }
    return false;
}

void pmgui::CheckboxSection::setHorizontal(size_t itemsPerRow)
{
    m_itemsPerRow = itemsPerRow;
    m_isHorizontal = true;
}

void pmgui::CheckboxSection::setVertical()
{
    m_isHorizontal = false;
}

void pmgui::CheckboxSection::addCheckbox(const size_t id, const std::string &label, const bool isChecked)
{
    m_data[id] = std::make_unique<pmgui::Checkbox>(fmt::format("{0}_{1}", m_id, id), label, isChecked);
}

bool pmgui::CheckboxSection::isSameLine(int i)
{
    if(m_itemsPerRow == 0)
        return true;

    int value = i + 1;
    bool sameLine = false;

    sameLine = ((value % m_itemsPerRow) != 0) ? true : false;

    return sameLine;
}
