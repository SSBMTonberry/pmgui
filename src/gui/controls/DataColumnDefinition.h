//
// Created by robin on 17.03.18.
//

#ifndef PMGUI_DATACOLUMNDEFINITION_H
#define PMGUI_DATACOLUMNDEFINITION_H

#include "DataColumn.h"

namespace pmgui
{
    class DataColumnDefinition
    {
        public:
            DataColumnDefinition() = default;

            void addColumn(const std::string &columnName, const std::string &columnTitle, size_t dataSize = 25, const DataColumnType &columnType = DataColumnType::Selectable);

            DataColumn * getColumn(const std::string &columnName);
            const std::vector<std::unique_ptr<DataColumn>> &getColumns() const;

        protected:
            std::vector<std::unique_ptr<pmgui::DataColumn>> m_columns;
    };
}

#endif //PMGUI_DATACOLUMNDEFINITION_H
