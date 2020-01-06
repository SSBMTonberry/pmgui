//

#include "Control.h"

//
// Created by robin on 27.12.17.

//pmgui::Control::Control() //: m_id {"unnamed"}, m_label {""}
//{
//
//}

/*!
 * A bare minimum implementation of Control.
 * Perfect for simple objects when allocating on stack memory.
 * @param id
 */
pmgui::Control::Control(std::string id) : m_id {std::move(id)}
{
    m_label = "";
    updateImguiId();
}

pmgui::Control::Control(const std::string &id, const std::string &label) : m_id {id}, m_label {label}
{
    //m_id = id;
    //m_propertyTitle = label;
    updateImguiId();
}

const std::string & pmgui::Control::getLabel() const
{
    return m_label;
}

void pmgui::Control::setLabel(const std::string &label)
{
    m_label = label;
    updateImguiId();
}

pmgui::ControlType pmgui::Control::getType() const
{
    return m_type;
}

const std::string &pmgui::Control::getId() const
{
    return m_id;
}

void pmgui::Control::setUseDefaultColor(bool useDefaultColor)
{
    m_useDefaultColor = useDefaultColor;
}

bool pmgui::Control::isUsingDefaultColor() const
{
    return m_useDefaultColor;
}

void pmgui::Control::assignColors()
{

}

bool pmgui::Control::isOnSameLine() const
{
    return m_putOnSameLine;
}

void pmgui::Control::setOnSameLine(bool putOnSameLine)
{
    m_putOnSameLine = putOnSameLine;
}

bool pmgui::Control::process()
{
    if(m_putOnSameLine)
        ImGui::SameLine();

    return m_isVisible;
}

const std::optional<pmgui::Tooltip> &pmgui::Control::getTooltip() const
{
    return m_tooltip;
}

void pmgui::Control::setTooltip(const std::optional<pmgui::Tooltip> &tooltip)
{
    m_tooltip = std::move(tooltip);
}

void pmgui::Control::createBasicTooltip(const std::string &text)
{
    m_tooltip = std::make_optional<Tooltip>(text);
}

bool pmgui::Control::isVisible() const
{
    return m_isVisible;
}

void pmgui::Control::setIsVisible(bool isVisible)
{
    m_isVisible = isVisible;
}

void pmgui::Control::setId(const std::string &id)
{
    m_id = id;
    updateImguiId();
}

float pmgui::Control::getSpacing() const
{
    return m_spacing;
}

void pmgui::Control::setSpacing(float spacing)
{
    m_spacing = spacing;
}

float pmgui::Control::getWidth() const
{
    return m_width;
}

void pmgui::Control::setWidth(float width)
{
    m_width = width;
}

/*!
 * Pushes width to item if width is defined as 1 or greater (Must remember to use pop width after control to activate)
 */
void pmgui::Control::pushWidth()
{
    if(m_width >= 1.f)
        ImGui::PushItemWidth(m_width);
}

/*!
 * Pops width to item if width is defined as 1 or greater (must be called after a pushWidth)
 */
void pmgui::Control::popWidth()
{
    if(m_width >= 1.f)
        ImGui::PopItemWidth();
}

/*!
 * Updates the ImGuiId based on existing variables.
 * The possible IDs generated:
 * 1. <label>###<id>_<parent_id> if they both have a value.
 * 2. <id>###<id>_<parent_id> if label is not set
 * 3. <label>###<label>_<parent_id> if no ID exists
 * 4. Left blank...
 *
 * Adds index number on the end if it is larger than -1.
 */
void pmgui::Control::updateImguiId(int index)
{
    if(!m_id.empty() && !m_label.empty())
        m_imguiId = fmt::format("{0}###{1}_{2}", m_label, m_id, m_parentId);
    else if(!m_id.empty() && m_label.empty())
        m_imguiId = fmt::format("{0}###{1}_{2}", m_id, m_id, m_parentId);
    else if(m_id.empty() && !m_label.empty())
        m_imguiId = fmt::format("{0}###{1}_{2}", m_label, m_label, m_parentId);

    if(!m_imguiId.empty() && index > -1)
        m_imguiId = fmt::format("{0}_{1}", m_imguiId, index);
}

const std::string &pmgui::Control::getParentId() const
{
    return m_parentId;
}

void pmgui::Control::setParentId(const std::string &parentId)
{
    m_parentId = parentId;
    updateImguiId();
}

const std::string &pmgui::Control::getImguiId() const
{
    return m_imguiId;
}
