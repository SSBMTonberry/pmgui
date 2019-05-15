//
// Created by robin on 14.05.19.
//

#include "FileType.h"

pmgui::FileType::FileType(std::string extension, std::string name, const Image &img)
                : m_extension {std::move(extension)}, m_name {std::move(name)}, m_image {img}
{

}

const std::string &pmgui::FileType::getExtension() const
{
    return m_extension;
}

void pmgui::FileType::setExtension(const std::string &extension)
{
    m_extension = extension;
}

const std::string &pmgui::FileType::getName() const
{
    return m_name;
}

void pmgui::FileType::setName(const std::string &name)
{
    m_name = name;
}

pmgui::Image *pmgui::FileType::getImage()
{
    return &m_image;
}

void pmgui::FileType::setImage(const pmgui::Image &image)
{
    m_image = image;
}
