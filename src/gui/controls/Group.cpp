//
// Created by robin on 07.03.18.
//

#include "Group.h"

/*!
 * Useful for grouping controls together. For example, you can group something after a pmgui::Child
 * @param id
 * @param label
 */
pmgui::Group::Group(const std::string &id, const std::string &label) : Control(id, label)
{
    m_type = ControlType::Group;
}

bool pmgui::Group::process()
{
    bool anyItemActivated = false;
    if(Control::process())
    {
        ImGui::BeginGroup();
        for(const auto &item : m_items)
        {
            if(item->process())
                anyItemActivated = true;

        }
        for(const auto &item : m_itemRefs)
        {
            if(item->process())
                anyItemActivated = true;

        }
        ImGui::EndGroup();
    }
    return anyItemActivated;
}

void pmgui::Group::add(std::unique_ptr<pmgui::Control> control)
{
    m_items.push_back(std::move(control));
}

void pmgui::Group::addReference(pmgui::Control *ref)
{
    m_itemRefs.push_back(ref);
}
