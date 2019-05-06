//
// Created by robin on 30.01.18.
//

#ifndef PMGUI_COLUMN_H
#define PMGUI_COLUMN_H

#include "Control.h"
#include <optional>

namespace pmgui
{
    class Column : public Control
    {
        public:
            Column(const std::string &id, const std::string &label, const std::optional<float> &width = std::nullopt);

            bool process() override;

            void add(std::unique_ptr<pmgui::Control> control);

            const float getWidth() const;
            const bool hasWidth();
            void setWidth(const std::optional<float> &width);
            void setWidthByPercent(size_t widthByPercentOfAvailableSpace);

        protected:
            std::vector<std::unique_ptr<pmgui::Control>> m_items;
            std::optional<float> m_width = std::nullopt;
    };
}

#endif //PMGUI_COLUMN_H
