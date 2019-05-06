//
// Created by robin on 15.03.18.
//

#ifndef PMGUI_DATACOLUMN_H
#define PMGUI_DATACOLUMN_H

#include <iostream>
#include "Control.h"
#include "Selectable.h"
#include <optional>

namespace pmgui
{
    enum class DataColumnType : unsigned
    {
        Selectable = 0,
        Textbox = 1,
        Image = 2
    };
    class DataColumn
    {
        public:
            DataColumn(const std::string &columnName, const std::string &columnTitle, size_t dataSize = 25, const DataColumnType &dataColumnType = DataColumnType::Selectable);

            void setWidth(const std::optional<float> &width);

            void setDataColumnType(DataColumnType dataColumnType);
            void setDataSize(size_t dataSize);

            size_t getDataSize() const;
            const std::string &getName() const;
            const std::string &getTitle() const;
            const float getWidth() const;
            DataColumnType getDataColumnType() const;
            const bool hasWidth();


        protected:
            std::string m_name;
            std::string m_title;
            size_t m_dataSize; //For controls that needs a datasie specified. Like textboxes
            DataColumnType m_dataColumnType = DataColumnType::Selectable;

            std::optional<float> m_width = std::nullopt;
    };
}
#endif //PMGUI_DATACOLUMN_H
