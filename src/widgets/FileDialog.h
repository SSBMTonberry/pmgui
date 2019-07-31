//
// Created by robin on 20.01.19.
//

#ifndef PMGUI_FILEDIALOG_H
#define PMGUI_FILEDIALOG_H

#include "../../PmguiConfig.h"

#if MSVC
#include <filesystem>
    namespace fs = std::filesystem;
#elif MINGW
#if __MINGW64_VERSION_MAJOR > 6
#include <filesystem>
namespace fs = std::filesystem;
#else
#include <experimental/filesystem>
        namespace fs = std::experimental::filesystem;
#endif
#elif APPLE
#include <experimental/filesystem>
    namespace fs = std::experimental::filesystem;
#else
#if __GNUC__ < 8 //GCC major version less than 8
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
#include <filesystem>
        namespace fs = std::filesystem;
#endif
#endif

#include "../gui/forms/Popup.h"
#include "../gui/controls/ImageButton.h"
#include "FileTable.h"
#include "../pmgui_files/files_mapper.h"
#include <map>
#include "../gui/forms/MessagePopup.h"
#include "../gui/controls/Combobox.h"
#include <SFML/Window/VideoMode.hpp>

using namespace std;
using namespace pmgui;
namespace pmgui_fm = pmgui_files_mapper;

namespace pmgui
{
    typedef std::function<void(fs::path)> func_path; //OnFileChosen, for instance
    typedef std::function<void(void)> func_void; //OnCancel(), for instance

    enum class DialogType : unsigned
    {
        OpenDirectory = 0,
        SaveDirectory = 1,
        OpenFile = 2,
        SaveFile = 3
    };

    //enum class FileTypeMode : unsigned
    //{
    //    None = 0,
    //    EmuFiles = 1,
    //    Sprites = 2,
    //    Folder = 3,
    //    EmuPlaylists = 4,
    //    SoundFiles = 5
    //};

    class FileDialog : public pmgui::Popup
    {
        public:
            static const int MAX_FILE_PATH_LENGTH = 512;

            FileDialog(const DialogType &dialogType, std::string id, std::string title); //, std::string imguiId = "");

            FileDialog(const DialogType &dialogType, const sf::Vector2<int> &position, const sf::Vector2<int> &size,
                       std::string id, std::string title); //, std::string imguiId = "");

            void assignEnvironmentMap(unordered_map<string, string> *env);
            void assignDefaults();

            //bool draw() override;
            void handleEvents() override;

            void addFileType(const FileType &fileType);
            void createFileTypeCollection(const std::string &name, const std::initializer_list<std::string> &fileExtensions);

            void setFileTypeCollection(const std::string &name, bool includeAllFilesOption); //(const pmgui::FileTypeMode &mode);
            void setFileType(const std::string &filetype);
            void setFilename(const std::string &filename);
            void setUseFileIcons(bool useFileIcons);
            pmgui::Image *getImgFileIcon(const std::string &key);

            void registerOnFileChosenCallback(const func_path &cb);
            void registerOnCancelCallback(const func_void &cb);

            const fs::path &getPath() const;
            const fs::path &getChosenFile() const;
            bool useFileIcons();

            void setPath(const fs::path &path);

            void setScaleFactor(float scaleFactor) override;

            //Static helpers
            static fs::path getParentPath(const fs::path &path);

        protected:

            void update();
            void initializePopups();
            void handleFileChosen();
            void refreshFiletype();

            void createDefaultFileTypes();
            void createDefaultFileTypesCollection();
            std::vector<FileType*> getFileTypesByExtensions(const std::initializer_list<std::string> &fileExtensions);

            bool onDraw() override;

            //void getHomeEnvironmentVariable();
            std::string getOkBtnText() const;

            unordered_map<string, string> *m_environmentMap = nullptr;

            std::vector<FileTypeCollection> m_fileCollections;
            std::map<std::string, pmgui::FileType> m_fileTypes;

            FileTable m_fileTable {"filetable", "Filetable!"};

            pmgui::Group m_upperGroup {"upper_group", "upper_group"};
            pmgui::Group m_midGroup {"middle_group", "middle_group"};
            pmgui::Group m_bottomGroup {"bottom_group", "bottom_group"};

            pmgui::Child m_upperLeftChild {"upper_left_child", "upper_left_child"}; //Must be reinitialized
            pmgui::Child m_upperMiddleChild {"upper_middle_child", "upper_middle_child"};
            pmgui::Child m_upperRightChild {"upper_right_child", "upper_right_child"};
            pmgui::Child m_midRightChild {"mid_right_child", "mid_right_child"};
            pmgui::Child m_bottomchildleft {"bottom_child_left", "bottom_child_left"};
            pmgui::Child m_bottomchildmid {"bottom_child_mid", "bottom_child_mid"};
            pmgui::Child m_bottomchildright {"bottom_child_right", "bottom_child_right"};

            pmgui::Label m_upperLabel {"upper_label", "", "Path: "};
            pmgui::Textbox m_filepathtext {"filepathtext", "filepath", 0};//MAX_FILE_PATH_LENGTH};
            pmgui::ImageButton m_refreshbtn {"refresh_img", pmgui_files_mapper::gui::actions::_REFRESH_PNG, pmgui_files_mapper::gui::actions::_REFRESH_PNG_SIZE};
            pmgui::Label m_filenameLabel {"file_name_lbl", "", "File name: "};
            pmgui::Label m_filetypeLabel {"file_type_lbl", "", "File type: "};
            pmgui::Textbox m_filenametext {"file_name_text", "File name text", 0};//50};
            pmgui::Combobox m_fileTypeCombo {"file_type_combo", "File type combo"}; //dropdown
            pmgui::Button m_okBtn {"ok_btn", "Unset", {10, 10}};
            pmgui::Button m_cancelBtn {"cancel_btn", "Cancel", {10, 10}};

            void onMessageResponse(const Button *btn, const MessagePopupResponse &response);
            void onOpen() override;

            void initialize(const std::optional<sf::Vector2<int>> &position = std::nullopt, const std::optional<sf::Vector2<int>> &size = std::nullopt);
            void createLayout();
            std::string getEnv(const std::string_view &id);

            DialogType m_dialogType = DialogType::SaveFile;
            fs::path m_path;
            fs::path m_chosenFile;

            MessagePopup m_msgPopup {"msg_popup_file_exists", "File already exists!", MessagePopupType::YesNo}; //Since the file dialog has the ###0, this one must have another (using: ###1)
            MessagePopup m_msgPopupNoFileChosen {"msg_popup_no_file_chosen", "No file chosen!", MessagePopupType::Ok};
            MessagePopup m_msgPopupFileDoesNotExist {"msg_popup_file_does_not_exist", "File does not exist!", MessagePopupType::Ok};

            vector<func_path> m_callbackOnFileChosen;
            vector<func_void> m_callbackOnClose;

            unordered_map<std::string, std::string> m_filetypeFilter; //first: the text displayed in dropdown of file type, second: the file extension
    };
}


#endif //PMGUI_FILEDIALOG_H
