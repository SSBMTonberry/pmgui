//
// Created by robin on 07.03.18.
//

#include "Child.h"

/*!
 * A child form for creating layouts inside a form
 * ocornut: Note that you can pass a (0,0) for fit all or a negative value to align from the right or lower edge. Works on both axis independently.
 * @param id
 * @param label
 * @param size
 * @param hasBorder
 * @param flags
 */
pmgui::Child::Child(const std::string &id, const std::string &label, const sf::Vector2<int> &size, bool hasBorder,
                  pmgui::FormFlags flags) : Control(id, label), m_size {size}, m_hasBorder {hasBorder}, m_flags{flags}
{
    //m_size = size;
    //m_hasBorder = hasBorder;
    //m_flags = flags;
    m_type = ControlType::Child;
}

bool pmgui::Child::process()
{
    bool anyItemActivated = false;
    if(Control::process())
    {
        ImGui::BeginChild(m_imguiId.c_str(), m_size, m_hasBorder, getFlagsAsImGuiFlags());
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
        ImGui::EndChild();
    }
    return anyItemActivated;
}

void pmgui::Child::add(std::unique_ptr<pmgui::Control> control)
{
    m_items.push_back(std::move(control));
}

void pmgui::Child::addReference(pmgui::Control *ref)
{
    m_itemRefs.push_back(ref);
}

void pmgui::Child::setFlags(const pmgui::FormFlags &formFlags)
{
    m_flags = formFlags;
}

void pmgui::Child::addFlag(const pmgui::FormFlags &formFlag)
{
    m_flags |= formFlag;
}

void pmgui::Child::removeFlag(const pmgui::FormFlags &formFlag)
{
    m_flags &= ~formFlag;
}

bool pmgui::Child::hasFlag(const pmgui::FormFlags &formFlag)
{
    return ((m_flags & formFlag) == formFlag) ? true : false;
}

ImGuiWindowFlags_ pmgui::Child::getFlagsAsImGuiFlags()
{
    return (ImGuiWindowFlags_)m_flags;
}

pmgui::FormFlags pmgui::Child::getFlags() const
{
    return m_flags;
}

const sf::Vector2<int> &pmgui::Child::getSize() const
{
    return m_size;
}

void pmgui::Child::setSize(const sf::Vector2<int> &size)
{
    m_size = size;
}

bool pmgui::Child::hasBorder() const
{
    return m_hasBorder;
}

void pmgui::Child::setHasBorder(bool hasBorder)
{
    m_hasBorder = hasBorder;
}



