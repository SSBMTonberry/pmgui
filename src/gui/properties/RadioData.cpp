//
// Created by robin on 13.01.18.
//

#include "RadioData.h"


pmgui::RadioData::RadioData(const int &id, const std::string &label)
{
    m_id = id;
    m_label = label;
}

int pmgui::RadioData::getId() const
{
    return m_id;
}

const std::string &pmgui::RadioData::getLabel() const
{
    return m_label;
}

void pmgui::RadioData::setId(int id)
{
    m_id = id;
}


void pmgui::RadioData::setLabel(const std::string &label)
{
    m_label = label;
}
