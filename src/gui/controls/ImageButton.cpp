//
// Created by robin on 03.01.18.
//

#include "ImageButton.h"

pmgui::ImageButton::ImageButton(const std::string &id, const std::optional<sf::Vector2<int>> &size)
        : Button(id, "", {16, 16}), m_customSize {size}
{
    m_type = ControlType::ImageButton;
}

pmgui::ImageButton::ImageButton(const std::string &id, const void *imageData, const size_t imageDataSize,
                              const std::optional<sf::Vector2<int>> &size) : Button(id, "", {16, 16}),
                                                                             m_customSize {size}
{
    m_type = ControlType::ImageButton;
    create(imageData, imageDataSize);
}

bool pmgui::ImageButton::process()
{
    if(Control::process())
    {
        m_isPressed = false;
        bool popColors = false;
        bool toReturn = false;
        if (m_colorIsChanged && !m_useDefaultColor)
        {
            assignColors();
            popColors = true;
        }

        if (m_spacing > -1.f)
            ImGui::SameLine(0, m_spacing);

        if (ImGui::ImageButton(m_sprite, -1, sf::Color::Transparent, m_sprite.getColor()))
        {
            m_isPressed = true;
            toReturn = true;
        }

        if(ImGui::IsItemHovered() && m_tooltip.has_value())
            m_tooltip->show();

        if (popColors && !m_useDefaultColor)
        {
            ImGui::PopStyleColor(3);
        }
        return toReturn;
    }

    return false;
}

void pmgui::ImageButton::create(const void *imageData, const size_t imageDataSize)
{
    if(m_customSize.has_value())
        m_texture.loadFromMemory(imageData, imageDataSize, {{0,0}, {m_customSize.value().x, m_customSize.value().y}});
    else
        m_texture.loadFromMemory(imageData, imageDataSize);

    m_sprite.setTexture(m_texture);//*m_texture
    m_sprite.setOrigin((float)m_texture.getSize().x / 2, (float)m_texture.getSize().y / 2);
    m_size = {(int)m_texture.getSize().x, (int)m_texture.getSize().y};
}

sf::Sprite *pmgui::ImageButton::getImage()
{
    return &m_sprite;
}

