//
// Created by robin on 14.05.19.
//

#ifndef PMGUI_PROJECT_FILETYPE_H
#define PMGUI_PROJECT_FILETYPE_H

#include <string>
#include "../gui/controls/Image.h"
#include "../files_mapper.h"
namespace pmgui
{
    class FileType
    {
        public:
            FileType() = default;
            FileType(std::string extension, std::string name, const Image &img = Image("filetype_img", files_mapper::gui::filetypes::_BLANK_PNG,
                                                                                                       files_mapper::gui::filetypes::_BLANK_PNG_SIZE));

            void setExtension(const std::string &extension);
            void setName(const std::string &name);
            void setImage(const Image &image);

            const std::string &getExtension() const;
            const std::string &getName() const;
            Image *getImage();

        private:
            std::string m_extension;
            std::string m_name;
            Image m_image; //{"filetype_img", files_mapper::gui::filetypes::_BLANK_PNG, files_mapper::gui::filetypes::_BLANK_PNG_SIZE};
    };
}

#endif //PMGUI_PROJECT_FILETYPE_H
