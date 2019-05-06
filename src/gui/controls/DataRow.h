//
// Created by robin on 17.03.18.
//

#ifndef PMGUI_DATAROW_H
#define PMGUI_DATAROW_H

#include "DataColumnDefinition.h"
#include "Textbox.h"
#include <unordered_map>
#include <any>

namespace pmgui
{
    class DataRow
    {
        public:
            explicit DataRow(const std::string &id, DataColumnDefinition *columnDefinition, size_t imguiId);

            bool process();

            void mark();
            void unmark();

            void setValue(const std::string &key, const std::string &value);
            void setValues(const std::initializer_list<std::string> &values);
            void setValuesWithKey(const std::initializer_list<std::pair<std::string, std::string>> &values);
            void setImage(const std::string &key, const void *imageData, size_t imageDataSize);
            void setImageRef(const std::string &key, pmgui::Image *img);
            void setHasVerticalBorders(bool hasVerticalBorders);
            void setHasHorizontalBorders(bool hasHorizontalBorders);

            bool isSelected() const;

            bool isDoubleClicked() const;
            void resetIsDoubleClicked();

            const std::string & operator[](const std::string &item) const
            {
                return m_values.find(item)->second->getLabel();
            }

            const std::string &getValue(const std::string &item) const;
            bool hasVerticalBorders() const;
            bool hasHorizontalBorders() const;


        protected:
            template <typename T>
            T * getAs(const std::string &key);

            void initialize();
            void refreshColumns();
            //bool processColumn(const DataColumn *col);

            void create(const DataColumn *col);

            std::string m_id;
            size_t m_imguiId;
            bool m_hasVerticalBorders = true;
            bool m_hasHorizontalBorders = true;
            bool m_isSelected = false;
            bool m_isMarked = false;
            bool m_isDoubleClicked = false;


            DataColumnDefinition *m_columnDefinition;
            std::unordered_map<std::string, std::shared_ptr<pmgui::Control>> m_values;
    };

    template<typename T>
    T * pmgui::DataRow::getAs(const std::string &key)
    {
        T * reference = static_cast<T*>(m_values[key].get());
        return reference;
    }
}

#endif //PMGUI_DATAROW_H
