//
// Created by robin on 17.03.18.
//

#include "DataTable.h"

pmgui::DataTable::DataTable(const std::string &id, const std::string &label) : Control(id, label)
{
    m_type = ControlType::DataTable;
}

bool pmgui::DataTable::process()
{
    bool anyItemActivated = false;
    if(Control::process())
    {
        drawHeader();
        //for(auto & row : m_rows)
        for(int i = 0; i < m_rows.size(); ++i)
        {
            DataRow *row = m_rows.data()+i;

            //if(row->process())
            if(row->process())
            {
                m_selectedRow = row;
                anyItemActivated = true;
                if(row->isDoubleClicked())
                {
                    if(m_selectedRow.has_value())
                        onRowDoubleClicked(m_selectedRow.value());
                    row->resetIsDoubleClicked();
                }
            }

            if(m_selectedRow.has_value() && row != m_selectedRow.value())
                row->unmark();
        }

        ImGui::Separator();
        markSelectedRow();
    }

    return anyItemActivated;
}

pmgui::DataRow *pmgui::DataTable::newRow()
{

    //unique_ptr<DataRow> row = make_unique<DataRow>(fmt::format("row_{0}", m_rowIdCounter), &m_columnDefinition);

    //unique_ptr<DataRow> row = make_unique<DataRow>(m_id, &m_columnDefinition, m_rowIdCounter);
    //DataRow row(m_id, &m_columnDefinition, m_rowIdCounter);
    //m_rows.push_back(row);
    m_rows.emplace_back(m_id, &m_columnDefinition, m_rowIdCounter);
    //m_rows.push_back(move(row));
    m_rowIdCounter++;
    return &m_rows[m_rows.size()-1];
}

void pmgui::DataTable::addColumn(const std::string &columnName, const std::string &columnTitle, const DataColumnType &dataColumnType, size_t dataSize)
{
    m_columnDefinition.addColumn(columnName, columnTitle, dataSize, dataColumnType);
}

void pmgui::DataTable::drawHeader()
{

    ImGui::Columns(m_columnDefinition.getColumns().size(), m_id.c_str(), true);
    for(auto const &column : m_columnDefinition.getColumns())
    {

        if(column->hasWidth())
        {
            ImGui::SetColumnWidth(ImGui::GetColumnIndex(), column->getWidth());
        }

        if (ImGui::GetColumnIndex() == 0)
            ImGui::Separator();

        if(ImGui::Selectable(column->getTitle().c_str(), true))
        {
            onHeaderColumnClicked(column->getName());
        }

        ImGui::NextColumn();
    }

    //ImGui::PopStyleVar(m_columnDefinition.getColumns().size());
    ImGui::Columns(1); //Do this to prevent nesting, which makes things crash :(
}

void pmgui::DataTable::onHeaderColumnClicked(const std::string &id)
{

}

const std::vector<pmgui::DataColumn *> pmgui::DataTable::getColumns()
{
    std::vector<DataColumn *> columns;
    for(auto &item : m_columnDefinition.getColumns())
    {
        columns.push_back(item.get());
    }
    return columns;
}

const std::optional<pmgui::DataRow *> &pmgui::DataTable::getSelectedRow() const
{
    return m_selectedRow;
}

void pmgui::DataTable::onRowDoubleClicked(pmgui::DataRow *row)
{

}

void pmgui::DataTable::markSelectedRow()
{
    if(m_selectedRow.has_value())
    {
        m_selectedRow.value()->mark();
        if(m_lastMarkedRow == nullptr || (m_lastMarkedRow != m_selectedRow.value()))
        {
            m_lastMarkedRow = m_selectedRow.value();
            onRowMarked(m_selectedRow.value());
        }
    }
}

void pmgui::DataTable::onRowMarked(pmgui::DataRow *row)
{

}

