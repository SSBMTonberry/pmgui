//
// Created by robin on 21.02.18.
//

#ifndef PMGUI_TABLE_H
#define PMGUI_TABLE_H

#include "Control.h"
#include "Row.h"

namespace pmgui
{
    class Table : public Control
    {
        public:
            Table(const std::string &id, const std::string &label);

            bool process() override;
            void add(std::unique_ptr<pmgui::Row> row);
            //void addRange(std::initializer_list<std::unique_ptr<pmgui::Column>> &columns);

        protected:
            std::vector<std::unique_ptr<pmgui::Row>> m_rows;

    };
}

#endif //PMGUI_TABLE_H
