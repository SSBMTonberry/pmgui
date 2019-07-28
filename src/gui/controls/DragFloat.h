//
// Created by robin on 28.01.18.
//

#ifndef PMGUI_DRAGFLOAT_H
#define PMGUI_DRAGFLOAT_H

#include "Control.h"

namespace pmgui
{
    class DragFloat : public Control
    {
        public:
            enum class DragFloatSize : unsigned
            {
                    One = 0,
                    Two = 1,
                    Three = 2,
                    Four = 3
            };

            DragFloat(const std::string &id, const std::string &label, float minimum, float maximum,
                      int decimals = 3, float speed = 1.f, DragFloatSize dragSize = DragFloatSize::One);

            bool process() override;

            void setValues(const std::initializer_list<float> &values);
            void setHideNumber(bool hideNumber);
            void setSpeed(float speed);
            void setWidth(float width);

            const float * getValues() const;
            bool getHideNumber() const;
            float getSpeed() const;
            float getWidth() const;

        protected:
            bool processDrag();
            bool performValueValidation();
            bool m_hideNumber = false;

            DragFloatSize m_dragSize = DragFloatSize::One;
            size_t m_arraySize = 1;
            std::unique_ptr<float[]> m_values;
            float m_minimum;
            float m_maximum;
            float m_speed;

            int m_decimals = 2;
            std::string m_format;
            float m_width = 100.f;

        private:
            void initialize();
    };
}

#endif //PMGUI_DRAGFLOAT_H
