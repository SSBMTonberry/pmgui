//
// Created by robin on 14.01.18.
//

#ifndef PMGUI_CHECKBOX_H
#define PMGUI_CHECKBOX_H

#include "Control.h"

namespace pmgui
{
    class Checkbox : public Control
    {
        public:
            Checkbox();
            Checkbox(const std::string &id, const std::string &label, bool isChecked = false);

            void initialize(const std::string &id, const std::string &label, bool isChecked = false);

            bool process() override;
            bool toggle();

            bool * getChecked();
            bool isChecked() const;
            void setChecked(bool isChecked);

        protected:
            bool m_isChecked = false;
    };
}

#endif //PMGUI_CHECKBOX_H
