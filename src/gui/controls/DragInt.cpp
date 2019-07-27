//
// Created by robin on 28.01.18.
//

#include "DragInt.h"

pmgui::DragInt::DragInt(const std::string &id, const std::string &label, int minimum, int maximum,
                      float speed, pmgui::DragInt::DragIntSize dragSize)
        : Control(id, label), m_minimum {minimum}, m_maximum {maximum}, m_speed {speed}, m_dragSize {dragSize}
{
    //m_minimum = minimum;
    //m_maximum = maximum;
    //m_speed = speed;
    m_type = ControlType::DragInt;
    //m_dragSize = dragSize;
    initialize();
}

bool pmgui::DragInt::process()
{
    bool isChanged = false;
    if(Control::process())
    {
        pushWidth();
        if(processDrag())
        {
            performValueValidation();
            isChanged = true;
        }
        popWidth();
    }

    return isChanged;
}

void pmgui::DragInt::initialize()
{
    switch (m_dragSize)
    {
        case DragIntSize::One:
            m_values = std::make_unique<int[]>(1);
            m_values[0] = m_minimum;
            m_arraySize = 1;
            break;

        case DragIntSize::Two:
            m_values = std::make_unique<int[]>(2);
            m_values[0] = m_minimum;
            m_values[1] = m_minimum;
            m_arraySize = 2;
            break;

        case DragIntSize::Three:
            m_values = std::make_unique<int[]>(3);
            m_values[0] = m_minimum;
            m_values[1] = m_minimum;
            m_values[2] = m_minimum;
            m_arraySize = 3;
            break;

        case DragIntSize::Four:
            m_values = std::make_unique<int[]>(4);
            m_values[0] = m_minimum;
            m_values[1] = m_minimum;
            m_values[2] = m_minimum;
            m_values[3] = m_minimum;
            m_arraySize = 4;
            break;
    }
}

bool pmgui::DragInt::processDrag()
{
    std::string format = (m_hideNumber) ? "" : "%.0f";
    ImGui::PushItemWidth(m_width);
    bool result = false;
    switch (m_dragSize)
    {
        case DragIntSize::One:
            result = ImGui::DragInt(m_label.c_str(), m_values.get(), m_speed, m_minimum, m_maximum, format.c_str());
        case DragIntSize::Two:
            result = ImGui::DragInt2(m_label.c_str(), m_values.get(), m_speed, m_minimum, m_maximum, format.c_str());
        case DragIntSize::Three:
            result = ImGui::DragInt3(m_label.c_str(), m_values.get(), m_speed, m_minimum, m_maximum, format.c_str());
        case DragIntSize::Four:
            result = ImGui::DragInt4(m_label.c_str(), m_values.get(), m_speed, m_minimum, m_maximum, format.c_str());
    }
    ImGui::PopItemWidth();

    return result;
}

void pmgui::DragInt::setValues(const std::initializer_list<int> &values)
{
    size_t size = (m_arraySize > values.size()) ? values.size() : m_arraySize;

    size_t i = 0;
    for(const auto & v : values)
    {
        if(i < size)
            m_values[i] = v;

        i++;
    }
}

const int *pmgui::DragInt::getValues() const
{
    return m_values.get();
}

float pmgui::DragInt::getSpeed() const
{
    return m_speed;
}

void pmgui::DragInt::setSpeed(float speed)
{
    m_speed = speed;
}

bool pmgui::DragInt::getHideNumber() const
{
    return m_hideNumber;
}

void pmgui::DragInt::setHideNumber(bool hideNumber)
{
    m_hideNumber = hideNumber;
}

/*!
 * Simply makes sure none of the values are below minimum or above maximum.
 * @return if a value had to be corrected
 */
bool pmgui::DragInt::performValueValidation()
{
    bool valuesHasBeenCorrected = false;

    for(int i = 0; i < m_arraySize; i++)
    {
        if(m_values[i] < m_minimum)
        {
            m_values[i] = m_minimum;
            valuesHasBeenCorrected = true;
        }
        else if(m_values[i] > m_maximum)
        {
            m_values[i] = m_maximum;
            valuesHasBeenCorrected = true;
        }
    }

    return valuesHasBeenCorrected;
}

float pmgui::DragInt::getWidth() const
{
    return m_width;
}

void pmgui::DragInt::setWidth(float width)
{
    m_width = width;
}
