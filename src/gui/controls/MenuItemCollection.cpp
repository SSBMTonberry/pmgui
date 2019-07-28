//
// Created by robin on 24.01.18.
//

#include "MenuItemCollection.h"

pmgui::MenuItemCollection::MenuItemCollection(const std::string &id, const std::string &label) : Control(id, label)
{
    m_type = ControlType::MenuItemCollection;
}

bool pmgui::MenuItemCollection::process()
{
    m_isPressed = false;
    if(Control::process())
    {
        if(ImGui::BeginMenu(m_imguiId.c_str(), m_isEnabled))
        {
            m_isPressed = true;
            processMenuItems();
            ImGui::EndMenu();
        }
    }

    return m_isPressed;
}

bool pmgui::MenuItemCollection::processMenuItems()
{
    bool anyItemActive = false;
    for(const auto &item : m_menuItems)
    {
        if(item->process())
        {
            anyItemActive = true;
        }
    }
    for(const auto &item : m_menuRefItems)
    {
        if(item->process())
        {
            anyItemActive = true;
        }
    }
    /*for(auto const & [pos, obj] : m_menuItems)
    {
        if(obj->process())
        {
            anyItemActive = true;
        }
    }*/

    return anyItemActive;
}

void pmgui::MenuItemCollection::add(std::unique_ptr<pmgui::Control> menuItem)
{
    m_menuItems.push_back(std::move(menuItem));
}

void pmgui::MenuItemCollection::addRef(pmgui::Control *menuItem)
{
    m_menuRefItems.push_back(menuItem);
}
