//
// Created by robin on 15.03.18.
//

#include "DataColumn.h"


pmgui::DataColumn::DataColumn(const std::string &columnName, const std::string &columnTitle, size_t dataSize, const DataColumnType &dataColumnType) :
                            m_name {columnName}, m_title {columnTitle}, m_dataSize {dataSize}, m_dataColumnType {dataColumnType}
{

}

void pmgui::DataColumn::setWidth(const std::optional<float> &width)
{
    m_width = width;
}

void pmgui::DataColumn::setDataColumnType(pmgui::DataColumnType dataColumnType)
{
    m_dataColumnType = dataColumnType;
}

const float pmgui::DataColumn::getWidth() const
{
    return m_width.value();
}

const bool pmgui::DataColumn::hasWidth()
{
    return (m_width.has_value());
}


const std::string &pmgui::DataColumn::getName() const
{
    return m_name;
}

const std::string &pmgui::DataColumn::getTitle() const
{
    return m_title;
}

pmgui::DataColumnType pmgui::DataColumn::getDataColumnType() const
{
    return m_dataColumnType;
}

size_t pmgui::DataColumn::getDataSize() const
{
    return m_dataSize;
}

void pmgui::DataColumn::setDataSize(size_t dataSize)
{
    m_dataSize = dataSize;
}


