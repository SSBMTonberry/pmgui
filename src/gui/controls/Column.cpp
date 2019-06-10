//
// Created by robin on 30.01.18.
//

#include "Column.h"

pmgui::Column::Column(const std::string &id, const std::string &label, const std::optional<float> &width) : Control(id, label),
                                                                                                          m_width {width}
{
    //m_width = width;
}

bool pmgui::Column::process()
{
    bool anyItemActivated = false;
    if(Control::process())
    {
        for(const auto &item : m_items)
        {
            pushWidth();
            if(item->process())
                anyItemActivated = true;
            popWidth();
        }
    }
    return anyItemActivated;
}


void pmgui::Column::add(std::unique_ptr<pmgui::Control> control)
{
    m_items.push_back(std::move(control));
}

const float pmgui::Column::getWidth() const
{
    return m_width.value();
}

void pmgui::Column::setWidth(const std::optional<float> &width)
{
    m_width = width;
}

const bool pmgui::Column::hasWidth()
{
    return (m_width.has_value());
}

/*!
 * Sets the width of column based on the available space
 * Value range is from 0 to 100.
 * @param widthByPercentOfAvailableSpace
 */
void pmgui::Column::setWidthByPercent(size_t widthByPercentOfAvailableSpace)
{
    if(widthByPercentOfAvailableSpace > 100)
        widthByPercentOfAvailableSpace = 100;

    float width = ImGui::GetContentRegionAvailWidth() * ((float)widthByPercentOfAvailableSpace / 100);
    m_width = width;
}
