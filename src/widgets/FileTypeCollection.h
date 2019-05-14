//
// Created by robin on 14.05.19.
//

#ifndef PMGUI_PROJECT_FILETYPECOLLECTION_H
#define PMGUI_PROJECT_FILETYPECOLLECTION_H

#include "FileType.h"
#include <initializer_list>
#include <vector>
namespace pmgui
{
    class FileTypeCollection
    {
        public:
            FileTypeCollection() = default;
            FileTypeCollection(std::string name, std::initializer_list<FileType*> filetypes);

            void setName(const std::string &name);

            const std::string &getName() const;
            const std::vector<FileType *> &getFileTypes() const;

        private:
            std::string m_name;
            std::vector<FileType*> m_fileTypes;
    };
}

#endif //PMGUI_PROJECT_FILETYPECOLLECTION_H
