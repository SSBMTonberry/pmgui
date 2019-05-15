//
// Created by robin on 14.05.19.
//

#include "FileTypeCollection.h"

pmgui::FileTypeCollection::FileTypeCollection(std::string name, const std::initializer_list<FileType*> &filetypes) : m_name {name}, m_fileTypes {filetypes}
{

}

pmgui::FileTypeCollection::FileTypeCollection(std::string name, const std::vector<FileType*> &filetypes) : m_name {name}, m_fileTypes {filetypes}
{

}

const std::string &pmgui::FileTypeCollection::getName() const
{
    return m_name;
}

const std::vector<pmgui::FileType *> &pmgui::FileTypeCollection::getFileTypes() const
{
    return m_fileTypes;
}

void pmgui::FileTypeCollection::setName(const std::string &name)
{
    m_name = name;
}
