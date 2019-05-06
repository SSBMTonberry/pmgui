//
// Created by robin on 07.03.18.
//

#ifndef PMGUI_GROUP_H
#define PMGUI_GROUP_H

#include "Control.h"

namespace pmgui
{
    class Group : public Control
    {
        public:
            Group(const std::string &id, const std::string &label);

            bool process() override;

            void add(std::unique_ptr<pmgui::Control> control);
            void addReference(pmgui::Control *ref);

            template <typename T>
            T * getControl(const std::string &id);
            template <typename T>
            T * getControlReference(const std::string &id);

        protected:
            std::vector<std::unique_ptr<pmgui::Control>> m_items;
            std::vector<pmgui::Control *> m_itemRefs; //Pointers to items owned by something else
    };

    template<typename T>
    T * pmgui::Group::getControl(const std::string &id)
    {
        pmgui::Control *control = nullptr;
        for(auto & item : m_items)
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
    T * pmgui::Group::getControlReference(const std::string &id)
    {
        pmgui::Control *control = nullptr;
        for(auto & item : m_itemRefs)
        {
            if(item->getId().compare(id) == 0)
                control = item;
        }

        if(control == nullptr)
            return nullptr;

        T * reference = static_cast<T*>(control);
        return reference;
    }
}

#endif //PMGUI_GROUP_H
