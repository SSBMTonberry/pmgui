//
// Created by robin on 07.03.18.
//

#ifndef PMGUI_CHILD_H
#define PMGUI_CHILD_H

#include "Control.h"
namespace pmgui
{
    class Child : public Control
    {
        public:
            Child(const std::string &id, const std::string &label, const sf::Vector2<int> &size = {0,0}, bool hasBorder = false, pmgui::FormFlags flags = pmgui::FormFlags::None);

            bool process() override;

            void add(std::unique_ptr<pmgui::Control> control);
            void addReference(pmgui::Control *ref);

            void setSize(const sf::Vector2<int> &size);
            void setHasBorder(bool hasBorder);

            //Flags
            void setFlags(const FormFlags &formFlags);
            void addFlag(const FormFlags &formFlag);
            void removeFlag(const FormFlags &formFlag);
            bool hasFlag(const FormFlags &formFlag);

            template <typename T>
            T * getControl(const std::string &id);
            template <typename T>
            T * getControlReference(const std::string &id);

            ImGuiWindowFlags_ getFlagsAsImGuiFlags();
            FormFlags getFlags() const;
            const sf::Vector2<int> &getSize() const;
            bool hasBorder() const;

        protected:
            std::vector<std::unique_ptr<pmgui::Control>> m_items;
            std::vector<pmgui::Control *> m_itemRefs; //Pointers to items owned by something else
            sf::Vector2<int> m_size;
            bool m_hasBorder;
            pmgui::FormFlags m_flags;

    };

    template<typename T>
    T * pmgui::Child::getControl(const std::string &id)
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
    T * pmgui::Child::getControlReference(const std::string &id)
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

#endif //PMGUI_CHILD_H
