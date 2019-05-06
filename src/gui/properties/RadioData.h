//
// Created by robin on 13.01.18.
//

#ifndef PMGUI_RADIODATA_H
#define PMGUI_RADIODATA_H

#include <iostream>

namespace pmgui
{
    class RadioData
    {
        public:
            RadioData(const int &id, const std::string &label);

            int getId() const;
            const std::string &getLabel() const;

            void setId(int id);
            void setLabel(const std::string &label);

        protected:
            int m_id;
            std::string m_label;

    };
}

#endif //PMGUI_RADIODATA_H
