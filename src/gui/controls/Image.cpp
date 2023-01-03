//
// Created by robin on 21.01.18.
//

#include <SFML/Graphics/RectangleShape.hpp>
#include "Image.h"

pmgui::Image::Image() : Control("unnamed", "")
{
    m_type = ControlType::Image;
}

//pmgui::Image::Image(const pmgui::Image &img) : Control(img)
//{
//    copy(img);
//    m_type = ControlType::Image;
//}

/*!
 *
 * @param texture
 * @param size Used if you want onther size than the original texture holds.
 */
pmgui::Image::Image(const sf::Texture &texture, const sf::Vector2<int> &size) : Control("unnamed", ""), m_texture {texture}
{
    create(m_texture, size);
    m_type = ControlType::Image;
}

pmgui::Image::Image(const std::string &id, bool hasZoomTooltip) : Control(id, ""), m_hasZoomTooltip {hasZoomTooltip}
{
    //m_size = size;
    m_type = ControlType::Image;
}

pmgui::Image::Image(const std::string &id, const void *imageData, size_t imageDataSize, bool hasZoomTooltip) :
            Control(id, ""), m_hasZoomTooltip {hasZoomTooltip}
{
    //m_size = size;
    m_type = ControlType::Image;
    create(imageData, imageDataSize);
}

pmgui::Image &pmgui::Image::operator=(const pmgui::Image &img)
{
    copy(img);
    return *this;
}


pmgui::Image& pmgui::Image::copy(const pmgui::Image &img)
{
    create(img.getTexture(), img.getSize());
    setZoomTooltipScaleRatio(img.getZoomTooltipScaleRatio());
    setScale(img.getScale());
    setSpriteScale(img.getSpriteScale());
    setRotation(img.getSprite().getRotation());
    setHasZoomTooltip(img.hasZoomTooltip());
    return *this;
}


void pmgui::Image::create(const void *imageData, size_t imageDataSize)
{
    m_texture.loadFromMemory(imageData, imageDataSize);
    m_sprite.setTexture(m_texture);//*m_texture
    m_size = {(int)m_texture.getSize().x, (int)m_texture.getSize().y};
    m_sprite.setTextureRect({{0,0}, {m_size.x, m_size.y}});
    m_sprite.setOrigin((float)m_texture.getSize().x / 2, (float)m_texture.getSize().y / 2);
    m_sprite.setColor(sf::Color::White);
}

void pmgui::Image::create(const sf::Image &img, const sf::IntRect &rectangle)
{
    m_texture.loadFromImage(img, rectangle);
    m_sprite.setTexture(m_texture);
    m_size = {(int)m_texture.getSize().x, (int)m_texture.getSize().y};
    m_sprite.setTextureRect(rectangle);
    m_sprite.setOrigin((float)m_texture.getSize().x / 2, (float)m_texture.getSize().y / 2);
    m_sprite.setColor(sf::Color::White);
}

/*!
 *
 * @param texture
 * @param size Used if you want another size than the original texture holds.
 */
void pmgui::Image::create(const sf::Texture &texture, const sf::Vector2<int> &size)
{
    m_texture = texture;
    m_sprite.setTexture(m_texture);//*m_texture
    m_size = (size.x == -1 && size.y == -1) ? sf::Vector2<int>((int)m_texture.getSize().x, (int)m_texture.getSize().y) : size;
    m_sprite.setTextureRect({{0,0}, {m_size.x, m_size.y}});
    m_sprite.setOrigin((float)m_texture.getSize().x / 2, (float)m_texture.getSize().y / 2);
    m_sprite.setColor(sf::Color::White);
}

void pmgui::Image::create(const sf::Sprite &sprite, const sf::IntRect& rectangle)
{
    m_sprite = sprite;
    m_texture = *m_sprite.getTexture();
    m_size = {(int)m_texture.getSize().x, (int)m_texture.getSize().y};
    m_sprite.setTextureRect(rectangle);
    m_sprite.setOrigin((float)m_texture.getSize().x / 2, (float)m_texture.getSize().y / 2);
    m_sprite.setColor(sf::Color::White);
}

/*!
 *
 * @return true if hovered, false otherwise
 */
bool pmgui::Image::process()
{
    bool isHovered = false;
    if(Control::process())
    {
        sf::Vector2f size = {(float)m_sprite.getTextureRect().width * m_scale.x, (float)m_sprite.getTextureRect().height * m_scale.y};
        sf::FloatRect rect = static_cast<sf::FloatRect>(m_sprite.getTextureRect());
        if(size.x > 0 && size.y > 0)
        {
            ImGui::Image(m_texture, size, m_sprite.getColor());
            if (ImGui::IsItemHovered())
            {
                isHovered = true;
                if (m_hasZoomTooltip)
                    showZoomTooltip();
            }
        }
    }
    return isHovered;
}

const sf::Vector2<int> &pmgui::Image::getSize() const
{
    return m_size;
}

/*!
 * Set scale on the sf::Sprite object
 * @param scale
 */
void pmgui::Image::setSpriteScale(const sf::Vector2f &scale)
{
    m_sprite.setScale(scale);
}

void pmgui::Image::setColor(const sf::Color &color)
{
    m_sprite.setColor(color);
}

void pmgui::Image::setRotation(float angle)
{
    m_sprite.setRotation(angle);
}

void pmgui::Image::setZoomTooltipScaleRatio(float zoomTooltipScaleRatio)
{
    m_zoomTooltipScaleRatio = zoomTooltipScaleRatio;
}

void pmgui::Image::showZoomTooltip()
{
    ImGui::BeginTooltip();
    sf::Vector2f size = {m_sprite.getTextureRect().width * m_zoomTooltipScaleRatio * m_scale.x, m_sprite.getTextureRect().height * m_zoomTooltipScaleRatio * m_scale.y};
    sf::FloatRect rect = static_cast<sf::FloatRect>(m_sprite.getTextureRect());

    ImGui::Image(m_texture, size, m_sprite.getColor());
    ImGui::EndTooltip();
}


float pmgui::Image::getZoomTooltipScaleRatio() const
{
    return m_zoomTooltipScaleRatio;
}

bool pmgui::Image::hasZoomTooltip() const
{
    return m_hasZoomTooltip;
}

void pmgui::Image::setHasZoomTooltip(bool hasZoomTooltip)
{
    m_hasZoomTooltip = hasZoomTooltip;
}

/*!
 * Creates a simple colored rect. Typically used when image is not presented, or otherwise is invalid
 * @param width
 * @param height
 * @param color
 */
void pmgui::Image::create(unsigned int width, unsigned int height, const sf::Color &color)
{
    sf::Image img;
    img.create(width, height, color);
    m_texture.loadFromImage(img);
    m_sprite.setTexture(m_texture);
    m_size = {(int)m_texture.getSize().x, (int)m_texture.getSize().y};
    m_sprite.setTextureRect({{0,0}, {m_size.x, m_size.y}});
    m_sprite.setOrigin((float)m_texture.getSize().x / 2, (float)m_texture.getSize().y / 2);

    m_sprite.setColor(color);
}

const sf::Texture &pmgui::Image::getTexture() const
{
    return m_texture;
}

const sf::Sprite &pmgui::Image::getSprite() const
{
    return m_sprite;
}

const sf::Vector2f &pmgui::Image::getScale() const
{
    return m_scale;
}

/*!
 * Set scale on internal object.
 * @param scale
 */
void pmgui::Image::setScale(const sf::Vector2f &scale)
{
    m_scale = scale;
}

const sf::Vector2f &pmgui::Image::getSpriteScale() const
{
    return m_sprite.getScale();
}

sf::Sprite *pmgui::Image::getSpritePtr()
{
    return &m_sprite;
}