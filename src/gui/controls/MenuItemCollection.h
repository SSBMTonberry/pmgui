//
// Created by robin on 24.01.18.
//

#ifndef PMGUI_MENUITEMCOLLECTION_H
#define PMGUI_MENUITEMCOLLECTION_H

#include "Control.h"
namespace pmgui
{
    class MenuItemCollection : public Control
    {
        public:
            MenuItemCollection(const std::string &id, const std::string &label);

            bool process() override;

            void add(std::unique_ptr<pmgui::Control> menuItem);
            void addRef(pmgui::Control * menuItem);
            template <typename T>
            T * getMenuItem(const std::string &id);
            template <typename T>
            T * getMenuRefItem(const std::string &id);
        public:
            bool processMenuItems();
            std::vector<std::unique_ptr<pmgui::Control>> m_menuItems;
            std::vector<pmgui::Control *> m_menuRefItems;

            bool m_isPressed = false;
            bool m_isEnabled = true;
    };
}

template<typename T>
T * pmgui::MenuItemCollection::getMenuItem(const std::string &id)
{
    pmgui::Control *control = nullptr;
    for(auto & item : m_menuItems)
    {
        if(item->getId().compare(id) == 0)
            control = item.get();
    }

    if(control == nullptr)
        return nullptr;

    T * reference = static_cast<T*>(control);
    return reference;
}

template<typename T>
T * pmgui::MenuItemCollection::getMenuRefItem(const std::string &id)
{
    pmgui::Control *control = nullptr;
    for(auto item : m_menuRefItems)
    {
        if(item->getId() == id)
            control = item;
    }

    if(control == nullptr)
        return nullptr;

    T * reference = static_cast<T*>(control);
    return reference;
}

#endif //PMGUI_MENUITEMCOLLECTION_H
