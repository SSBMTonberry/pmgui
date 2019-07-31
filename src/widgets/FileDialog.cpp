//
// Created by robin on 20.01.19.
//

#include "FileDialog.h"

pmgui::FileDialog::FileDialog(const DialogType &dialogType, std::string id, std::string title) : //, std::string imguiId) :
        Popup(std::move(id), std::move(title)) //, std::move(imguiId))
{
    m_dialogType = dialogType;
    initialize();
}

pmgui::FileDialog::FileDialog(const DialogType &dialogType, const sf::Vector2<int> &position, const sf::Vector2<int> &size,
                            std::string id, std::string title) //, std::string imguiId)
                            : Popup(position, size, std::move(id), std::move(title)) //, std::move(imguiId))
{
    m_dialogType = dialogType;
    initialize(m_position, m_size);
}

void pmgui::FileDialog::initialize(const std::optional<sf::Vector2<int>> &position, const std::optional<sf::Vector2<int>> &size)
{
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    if(!size.has_value()) setSize({(int)desktop.width / 3, (int)desktop.height / 3});
    if(!position.has_value()) setPosition({((int)desktop.width / 2) - (m_size.x / 2), ((int)desktop.height / 2) - (m_size.y / 2)});
    setFormFlags(pmgui::FormFlags::NoResize | pmgui::FormFlags::NoCollapse | pmgui::FormFlags::AlwaysAutoResize);// | pmgui::FormFlags::NoScrollbar);

    setPath(fs::current_path());
    m_fileTable.setFileTypes(&m_fileTypes);

    m_msgPopup.setParentId(m_id);
    m_msgPopupNoFileChosen.setParentId(m_id);
    m_msgPopupFileDoesNotExist.setParentId(m_id);

    //registerOnFileChosenCallback(std::bind(&ProgramManager::onChosenNewProject, this, std::placeholders::_1));
    initializePopups();
    createDefaultFileTypes();
    createDefaultFileTypesCollection();
    createLayout();
    update();
}

void pmgui::FileDialog::createLayout()
{
    m_upperLabel.setHasTitle(false);
    m_upperLeftChild.setSize({(int)(m_size.x * 0.05f), (int)(m_size.y * 0.07f)});
    m_upperLeftChild.addReference(&m_upperLabel);

    m_upperMiddleChild.setSize({(int)(m_size.x * 0.8f), (int)(m_size.y * 0.07f)});
    m_upperMiddleChild.setOnSameLine(true);
    m_filepathtext.setHasLabel(false);
    m_filepathtext.setValue(getPath().u8string());
    m_upperMiddleChild.addReference(&m_filepathtext);

    m_upperRightChild.setSize({(int)(m_size.x * 0.15f), (int)(m_size.y * 0.07f)});
    m_upperRightChild.setOnSameLine(true);
    m_upperRightChild.addReference(&m_refreshbtn);

    m_upperGroup.addReference(&m_upperLeftChild);
    m_upperGroup.addReference(&m_upperMiddleChild);
    m_upperGroup.addReference(&m_upperRightChild);

    m_midRightChild.setSize({-1, (int)(m_size.y * 0.7f)});
    m_midRightChild.setHasBorder(true);
    m_midRightChild.addReference(&m_fileTable);

    m_midGroup.addReference(&m_midRightChild);

    m_bottomchildleft.setSize({(int)(m_size.x * 0.1f), (int)(m_size.y * 0.18f)});
    m_filenameLabel.setHasTitle(false);
    m_filetypeLabel.setHasTitle(false);
    m_bottomchildleft.addReference(&m_filenameLabel);
    m_bottomchildleft.addReference(&m_filetypeLabel);

    m_bottomchildmid.setSize({(int)(m_size.x * 0.74f), (int)(m_size.y * 0.18f)});
    m_bottomchildmid.setOnSameLine(true);
    m_filenametext.setHasLabel(false);
    m_fileTypeCombo.setHasLabel(false);
    m_bottomchildmid.addReference(&m_filenametext);
    m_bottomchildmid.addReference(&m_fileTypeCombo);

    m_bottomchildright.setSize({(int)(m_size.x * 0.2f), (int)(m_size.y * 0.18f)});
    m_bottomchildright.setOnSameLine(true);
    m_okBtn.setLabel(getOkBtnText());
    m_okBtn.setSize({(int)(m_size.x * 0.1f), (int)(m_size.y * 0.045f)});
    m_cancelBtn.setSize({(int)(m_size.x * 0.1f), (int)(m_size.y * 0.045f)});
    m_bottomchildright.addReference(&m_okBtn);
    m_bottomchildright.addReference(&m_cancelBtn);

    m_bottomGroup.addReference(&m_bottomchildleft);
    m_bottomGroup.addReference(&m_bottomchildmid);
    m_bottomGroup.addReference(&m_bottomchildright);

    addReference(&m_upperGroup);
    addReference(&m_midGroup);
    addReference(&m_bottomGroup);

    setFileTypeCollection("default", true);
}

void pmgui::FileDialog::handleEvents()
{
    if(m_open)
    {
        if (m_refreshbtn.isPressed() || m_filepathtext.isChanged())
        {
            m_path = (m_filepathtext.isChanged()) ? fs::path(m_filepathtext.getValue())
                                                  : m_fileTable.getLastOpenedPath();
            if (m_refreshbtn.isPressed())
                m_filepathtext.setValue(m_path.u8string());

            if (fs::exists(m_path))
            {
                m_fileTable.listFilesByDirectory(m_path,
                                                 (m_path.has_parent_path()) ? m_path.parent_path() : fs::path(""));
            }
        }

        if (m_fileTable.canCallPathOpening())
        {
            const auto pathToOpen = m_fileTable.getPathToOpen();

            if (fs::exists(pathToOpen) && fs::is_directory(pathToOpen))
            {
                m_filepathtext.setValue(pathToOpen.u8string());
                m_path = pathToOpen;
                fs::path parentPath = (m_path.has_parent_path()) ? m_path.parent_path() : fs::path("");
                //std::string pathStr = m_path.u8string();
                //std::string parentPathStr = parentPath.u8string();
                m_fileTable.listFilesByDirectory(m_path, parentPath);
            }
            m_fileTable.resetPathOpeningCall();
        }

        if (m_fileTypeCombo.valueHasBeenChosen())
        {
            std::string filter = m_filetypeFilter[m_fileTypeCombo.getValue()];
            m_fileTable.setFileFilter(filter);
        }

        if (m_fileTable.canCallRowChangeEvent())
        {
            std::string selectedFile = m_fileTable.getSelectedFile();
            if (selectedFile.length() > 0)
            {
                m_filenametext.setValue(selectedFile);
                m_fileTable.resetRowChangeCall();
            }
        }

        if (m_cancelBtn.isPressed())
        {
            m_open = false;
            for (auto &callback : m_callbackOnClose)
                callback();
        }


        if (m_okBtn.isPressed())
        {
            handleFileChosen();
        }
    }
}

const fs::path &pmgui::FileDialog::getPath() const
{
    return m_path;
}

void pmgui::FileDialog::setPath(const fs::path &path)
{
    m_path = path;
}

void pmgui::FileDialog::onOpen()
{
    if(!fs::exists(m_path))
        m_path = m_filepathtext.getValue();
    else
        m_filepathtext.setValue(m_path.u8string());

    if(fs::exists(m_path))
    {
        m_fileTable.listFilesByDirectory(m_path, (m_path.has_parent_path()) ? m_path.parent_path() : fs::path(""));
    }
    refreshFiletype();
}

std::string pmgui::FileDialog::getOkBtnText() const
{
    switch(m_dialogType)
    {
        case DialogType::OpenDirectory:
        case DialogType::OpenFile:
            return "Open";

        case DialogType::SaveFile:
        case DialogType::SaveDirectory:
            return "Save";
    }

    return "<undefined>";
}

void pmgui::FileDialog::assignEnvironmentMap(unordered_map<string, string> *env)
{
    m_environmentMap = env;
}

std::string pmgui::FileDialog::getEnv(const std::string_view &id)
{
    if(m_environmentMap != nullptr && m_environmentMap->count(id.data()) > 0)
        return m_environmentMap->at(id.data());

    return "";
}

void pmgui::FileDialog::assignDefaults()
{
    std::string home = getEnv("HOME");
    fs::path homePath = fs::path(home);
    if (home.length() > 0 && fs::exists(homePath))
    {
        m_path = homePath;
        m_filepathtext.setValue(home);
    }
}

/*!
 * Register a callback:
 *
 *
 * @param cb Example of valid parameter: std::bind(&ProgramManager::onChosenProject, this, std::placeholders::_1)
 * Actual function - pmgui::ProgramManager::onChosenProject(const std::string &path)
 */
void pmgui::FileDialog::registerOnFileChosenCallback(const pmgui::func_path &cb)
{
    m_callbackOnFileChosen.emplace_back(cb);
}

void pmgui::FileDialog::registerOnCancelCallback(const pmgui::func_void &cb)
{
    m_callbackOnClose.emplace_back(cb);
}

void pmgui::FileDialog::onMessageResponse(const pmgui::Button *btn, const MessagePopupResponse &response)
{
    if(response == MessagePopupResponse::Yes)
    {
        m_open = false;
        for (auto &callback : m_callbackOnFileChosen)
        {
            callback(m_chosenFile.string());
        }
    }
}

const fs::path &pmgui::FileDialog::getChosenFile() const
{
    return m_chosenFile;
}

/*!
 * Sets properties for controls based on the dialog type
 */
void pmgui::FileDialog::update()
{
    switch(m_dialogType)
    {
        case DialogType::SaveFile:
            //
            break;

        case DialogType::OpenFile:
            m_filenametext.setTextboxFlags(TextboxFlags::ReadOnly);
            break;

        case DialogType::OpenDirectory:
            m_filenametext.setTextboxFlags(TextboxFlags::ReadOnly);
            setFileTypeCollection("directory", false);
            setFileType("directory");
            break;
    }
}

void pmgui::FileDialog::handleFileChosen()
{
    switch(m_dialogType)
    {
        case DialogType::SaveFile:
        {
            string extension = m_fileTable.getFileFilter();
            string filename = m_filenametext.getValue();

            if(filename.length() == 0)
            {
                m_msgPopupNoFileChosen.setOpen(true);
                return;
            }

            if (filename.length() > extension.length() && extension != "*.*")
            {
                std::string sub = filename.substr(filename.length() - extension.length(), filename.length());
                if (sub != extension)
                    filename = filename + extension;
            }
            else if (extension != "*.*")
            {
                filename = filename + extension;
            }
            m_path = m_fileTable.getLastOpenedPath();
            string path = fmt::format("{0}/{1}", m_path.string(), filename);

            if (fs::exists(m_path))
            {
                m_chosenFile = path;
                if (!fs::exists(path))
                {
                    m_open = false;
                    for (auto &callback : m_callbackOnFileChosen)
                        callback(m_chosenFile.string());
                }
                else
                    m_msgPopup.setOpen(true);
            }
        }
            break;

        case DialogType::OpenFile:
        {
            string extension = m_fileTable.getFileFilter();
            string filename = m_filenametext.getValue();

            if(filename.length() == 0)
            {
                m_msgPopupNoFileChosen.setOpen(true);
                return;
            }

            m_path = m_fileTable.getLastOpenedPath();
            string path = fmt::format("{0}/{1}", m_path.string(), filename);

            if (fs::exists(m_path))
            {
                m_chosenFile = path;
                if (fs::exists(path))
                {
                    m_open = false;
                    for (auto &callback : m_callbackOnFileChosen)
                        callback(m_chosenFile.string());
                }
                else
                    m_msgPopupFileDoesNotExist.setOpen(true);
            }
        }
        break;

        case DialogType::OpenDirectory:
        {
            m_path = m_fileTable.getLastOpenedPath();

            if (fs::exists(m_path))
            {
                m_chosenFile = m_path;
                if (fs::exists(m_path) && fs::is_directory(m_path))
                {
                    m_open = false;
                    for (auto &callback : m_callbackOnFileChosen)
                        callback(m_chosenFile.string());
                }
                else
                    m_msgPopupFileDoesNotExist.setOpen(true);
            }
        }
        break;
    }
}

void pmgui::FileDialog::initializePopups()
{
    m_msgPopup.setMessage("File already exists!\nAre you sure you want to overwrite \nthe existing file?");
    m_msgPopup.setSize({300, 150});
    m_msgPopup.refresh();
    m_msgPopup.registerOnMsgResponseCallback(std::bind(&FileDialog::onMessageResponse, this, std::placeholders::_1, std::placeholders::_2));

    m_msgPopupNoFileChosen.setMessage("No file chosen!");
    m_msgPopupNoFileChosen.setSize({300, 150});
    m_msgPopupNoFileChosen.refresh();

    m_msgPopupFileDoesNotExist.setMessage("File does not exist!");
    m_msgPopupFileDoesNotExist.setSize({300, 150});
    m_msgPopupFileDoesNotExist.refresh();
}

void pmgui::FileDialog::setFileTypeCollection(const std::string &name, bool includeAllFilesOption)//(const pmgui::FileTypeMode &mode)
{

    m_fileTypeCombo.clear();

    //#error Find the collection

    auto collection = std::find_if(m_fileCollections.begin(), m_fileCollections.end(), [&](const FileTypeCollection &collection) { return collection.getName() == name; });
    bool valueIsSet = false;
    if(collection != m_fileCollections.end())
    {
        for(auto const &filetype : collection->getFileTypes())
        {
            m_fileTypeCombo.addValue(filetype->getName());
            m_filetypeFilter[filetype->getName()] = filetype->getExtension();
            if(!valueIsSet)
            {
                m_fileTypeCombo.setValue(filetype->getName());
                valueIsSet = true;
            }
        }
    }

    if (includeAllFilesOption)
    {
        auto all = std::find_if(m_fileTypes.begin(), m_fileTypes.end(), [](const auto &fileType) { return fileType.second.getExtension() == "*.*"; });
        if (all != m_fileTypes.end())
        {
            m_fileTypeCombo.addValue(all->second.getName());
            m_filetypeFilter[all->second.getName()] = all->second.getExtension();
            if(!valueIsSet)
            {
                m_fileTypeCombo.setValue(all->second.getName());
                valueIsSet = true;
            }
        }
    }
}

std::vector<pmgui::FileType*> pmgui::FileDialog::getFileTypesByExtensions(const std::initializer_list<std::string> &fileExtensions)
{
    std::vector<pmgui::FileType*> fileTypes;
    for(const auto &fileext : fileExtensions)
    {
        for(auto &[extension, fileType] : m_fileTypes)
        {
            if(fileType.getExtension() == fileext)
                fileTypes.push_back(&fileType);
        }
    }
    return fileTypes;
}

void pmgui::FileDialog::createDefaultFileTypes()
{
    addFileType({"*.*", "All (*.*)"});
    addFileType({".as", ".as", pmgui::Image("img_as",pmgui_fm::gui::filetypes::_ACTIONSCRIPT_PNG, pmgui_fm::gui::filetypes::_ACTIONSCRIPT_PNG_SIZE)});
    addFileType({".avi", ".avi", pmgui::Image("img_avi",pmgui_fm::gui::filetypes::_VIDEO_PNG, pmgui_fm::gui::filetypes::_VIDEO_PNG_SIZE)});
    addFileType({".c", ".c", pmgui::Image("img_c",pmgui_fm::gui::filetypes::_C_PNG, pmgui_fm::gui::filetypes::_C_PNG_SIZE)});
    addFileType({".cpp", ".cpp", pmgui::Image("img_cpp",pmgui_fm::gui::filetypes::_CPP_PNG, pmgui_fm::gui::filetypes::_CPP_PNG_SIZE)});
    addFileType({".csv", ".csv", pmgui::Image("img_csv",pmgui_fm::gui::filetypes::_CSV_PNG, pmgui_fm::gui::filetypes::_CSV_PNG_SIZE)});
    addFileType({".flv", ".flv", pmgui::Image("img_flv",pmgui_fm::gui::filetypes::_VIDEO_PNG, pmgui_fm::gui::filetypes::_VIDEO_PNG_SIZE)});
    addFileType({".gif", ".gif", pmgui::Image("img_gif",pmgui_fm::gui::filetypes::_IMAGE_PNG, pmgui_fm::gui::filetypes::_IMAGE_PNG_SIZE)});
    addFileType({".h", ".h", pmgui::Image("img_h",pmgui_fm::gui::filetypes::_HH_PNG, pmgui_fm::gui::filetypes::_HH_PNG_SIZE)});
    addFileType({".hpp", ".hpp", pmgui::Image("img_hpp",pmgui_fm::gui::filetypes::_HH_PNG, pmgui_fm::gui::filetypes::_HH_PNG_SIZE)});
    addFileType({".html", ".html", pmgui::Image("img_html",pmgui_fm::gui::filetypes::_HTML_PNG, pmgui_fm::gui::filetypes::_HTML_PNG_SIZE)});
    addFileType({".js", ".js", pmgui::Image("img_js",pmgui_fm::gui::filetypes::_JS_PNG, pmgui_fm::gui::filetypes::_JS_PNG_SIZE)});
    addFileType({".json", ".json", pmgui::Image("img_json",pmgui_fm::gui::filetypes::_JSON_PNG, pmgui_fm::gui::filetypes::_JSON_PNG_SIZE)});
    addFileType({".jpg", "Joint Photographic Experts Group (*.jpg)", pmgui::Image("img_jpg",pmgui_fm::gui::filetypes::_IMAGE_PNG, pmgui_fm::gui::filetypes::_IMAGE_PNG_SIZE)});
    addFileType({".lua", ".lua", pmgui::Image("img_lua",pmgui_fm::gui::filetypes::_LUA_PNG, pmgui_fm::gui::filetypes::_LUA_PNG_SIZE)});
    addFileType({".log", ".log", pmgui::Image("img_log",pmgui_fm::gui::filetypes::_LOG_PNG, pmgui_fm::gui::filetypes::_LOG_PNG_SIZE)});
    addFileType({".mov", ".mov", pmgui::Image("img_mov",pmgui_fm::gui::filetypes::_VIDEO_PNG, pmgui_fm::gui::filetypes::_VIDEO_PNG_SIZE)});
    addFileType({".mp3", ".mp3", pmgui::Image("img_mp3",pmgui_fm::gui::filetypes::_AUDIO_PNG, pmgui_fm::gui::filetypes::_AUDIO_PNG_SIZE)});
    addFileType({".mp4", ".mp4", pmgui::Image("img_mp4",pmgui_fm::gui::filetypes::_VIDEO_PNG, pmgui_fm::gui::filetypes::_VIDEO_PNG_SIZE)});
    addFileType({".mpeg", ".mpeg", pmgui::Image("img_mpeg",pmgui_fm::gui::filetypes::_VIDEO_PNG, pmgui_fm::gui::filetypes::_VIDEO_PNG_SIZE)});
    addFileType({".ogg", "OGG/Vorbis (*.ogg)", pmgui::Image("img_ogg",pmgui_fm::gui::filetypes::_AUDIO_PNG, pmgui_fm::gui::filetypes::_AUDIO_PNG_SIZE)});
    addFileType({".FLAC", "FLAC (Free Lossless Audio Codec) (*.FLAC)", pmgui::Image("img_flac",pmgui_fm::gui::filetypes::_AUDIO_PNG, pmgui_fm::gui::filetypes::_AUDIO_PNG_SIZE)});
    addFileType({".pdf", ".pdf", pmgui::Image("img_pdf",pmgui_fm::gui::filetypes::_PDF_PNG, pmgui_fm::gui::filetypes::_PDF_PNG_SIZE)});
    addFileType({".pmgui", ".pmgui", pmgui::Image("img_pmgui",pmgui_fm::gui::misc::_APPLICATION_PNG, pmgui_fm::gui::misc::_APPLICATION_PNG_SIZE)});
    addFileType({".png", "Portable Network Graphics (*.png)", pmgui::Image("img_png",pmgui_fm::gui::filetypes::_IMAGE_PNG, pmgui_fm::gui::filetypes::_IMAGE_PNG_SIZE)});
    addFileType({".sh", ".sh", pmgui::Image("img_sh",pmgui_fm::gui::filetypes::_SHELL_PNG, pmgui_fm::gui::filetypes::_SHELL_PNG_SIZE)});
    addFileType({".txt", ".txt", pmgui::Image("img_txt",pmgui_fm::gui::filetypes::_TEXT_PNG, pmgui_fm::gui::filetypes::_TEXT_PNG_SIZE)});
    addFileType({".wav", "Wave (*.wav)", pmgui::Image("img_wav",pmgui_fm::gui::filetypes::_AUDIO_PNG, pmgui_fm::gui::filetypes::_AUDIO_PNG_SIZE)});
    addFileType({".wmv", ".wmv", pmgui::Image("img_wmv",pmgui_fm::gui::filetypes::_VIDEO_PNG, pmgui_fm::gui::filetypes::_VIDEO_PNG_SIZE)});
    addFileType({".xml", ".xml", pmgui::Image("img_xml",pmgui_fm::gui::filetypes::_XML_PNG, pmgui_fm::gui::filetypes::_XML_PNG_SIZE)});
    addFileType({".ay", "ZX Spectrum/Amstrad CPC (*.ay)", pmgui::Image("img_ay",pmgui_fm::gui::filetypes::_AUDIO_PNG, pmgui_fm::gui::filetypes::_AUDIO_PNG_SIZE)});
    addFileType({".gbs", "Nintendo Game Boy (*.gbs)", pmgui::Image("img_gbs",pmgui_fm::gui::filetypes::_AUDIO_PNG, pmgui_fm::gui::filetypes::_AUDIO_PNG_SIZE)});
    addFileType({".gym", "Sega Genesis/Mega Drive (*.gym)", pmgui::Image("img_gym",pmgui_fm::gui::filetypes::_AUDIO_PNG, pmgui_fm::gui::filetypes::_AUDIO_PNG_SIZE)});
    addFileType({".hes", "NEC TurboGrafx-16/PC Engine (*.hes)", pmgui::Image("img_hes",pmgui_fm::gui::filetypes::_AUDIO_PNG, pmgui_fm::gui::filetypes::_AUDIO_PNG_SIZE)});
    addFileType({".kss", "MSX Home Computer/other Z80 systems (*.kss)", pmgui::Image("img_kss",pmgui_fm::gui::filetypes::_AUDIO_PNG, pmgui_fm::gui::filetypes::_AUDIO_PNG_SIZE)});
    addFileType({".nsf", "Nintendo NES/Famicom (*.nsf)", pmgui::Image("img_nsf",pmgui_fm::gui::filetypes::_AUDIO_PNG, pmgui_fm::gui::filetypes::_AUDIO_PNG_SIZE)});
    addFileType({".nsfe", "Nintendo NES/Famicom Extended (*.nsfe)", pmgui::Image("img_nsfe",pmgui_fm::gui::filetypes::_AUDIO_PNG, pmgui_fm::gui::filetypes::_AUDIO_PNG_SIZE)});
    addFileType({".sap", "Atari systems (*.sap)", pmgui::Image("img_sap",pmgui_fm::gui::filetypes::_AUDIO_PNG, pmgui_fm::gui::filetypes::_AUDIO_PNG_SIZE)});
    addFileType({".spc", "Super Nintendo/Super Famicom (*.spc)", pmgui::Image("img_spc",pmgui_fm::gui::filetypes::_AUDIO_PNG, pmgui_fm::gui::filetypes::_AUDIO_PNG_SIZE)});
    addFileType({".vgm", "Videogame Music (*.vgm)", pmgui::Image("img_vgm",pmgui_fm::gui::filetypes::_AUDIO_PNG, pmgui_fm::gui::filetypes::_AUDIO_PNG_SIZE)});
    //Non
    addFileType({"default", "", pmgui::Image("img_default", pmgui_fm::gui::filetypes::_BLANK_PNG, pmgui_fm::gui::filetypes::_BLANK_PNG_SIZE)});
    addFileType({"directory", "Folder", pmgui::Image("img_directory", pmgui_fm::gui::folders::blue::_FOLDER_CLOSED_PNG, pmgui_fm::gui::folders::blue::_FOLDER_CLOSED_PNG_SIZE)});
}

void pmgui::FileDialog::createDefaultFileTypesCollection()
{
    createFileTypeCollection("images", {".png", ".jpg", ".jpeg", ".bmp", ".gif" });
    createFileTypeCollection("music", {".mp3", ".ogg", ".wav", ".FLAC"});
    createFileTypeCollection("directory", {"directory"});
    createFileTypeCollection("folder", {"directory"});
}

pmgui::Image *pmgui::FileDialog::getImgFileIcon(const std::string &key)
{
    if(m_fileTypes.count(key) > 0)
        return m_fileTypes[key].getImage();
    else
        return nullptr;
}

void pmgui::FileDialog::addFileType(const pmgui::FileType &fileType)
{
    m_fileTypes[fileType.getExtension()] = fileType;
}

void pmgui::FileDialog::createFileTypeCollection(const std::string &name, const std::initializer_list<std::string> &fileExtensions)
{
    std::vector<pmgui::FileType*> fileTypes = getFileTypesByExtensions(fileExtensions);
    m_fileCollections.emplace_back(name, fileTypes);
}

void pmgui::FileDialog::setFilename(const std::string &filename)
{
    m_filenametext.setValue(filename);
}

bool pmgui::FileDialog::onDraw()
{
    m_msgPopup.draw();
    m_msgPopupNoFileChosen.draw();
    m_msgPopupFileDoesNotExist.draw();
    return true;
}

void FileDialog::setUseFileIcons(bool useFileIcons)
{
    m_fileTable.setUseFileIcons(useFileIcons);
}

bool FileDialog::useFileIcons()
{
    return m_fileTable.useFileIcons();
}

void FileDialog::setScaleFactor(float scaleFactor)
{
    Form::setScaleFactor(scaleFactor);
    scaleFactor *= 0.75f; //Make it a bit smaller
    if(scaleFactor < 1.f)
        scaleFactor = 1.f;

    m_upperLeftChild.setSize({(int)(m_scaledSize.x * 0.05f * scaleFactor), (int)(m_scaledSize.y * 0.07f * scaleFactor)});
    m_upperMiddleChild.setSize({(int)(m_scaledSize.x * 0.8f * scaleFactor), (int)(m_scaledSize.y * 0.07f * scaleFactor)});
    m_upperRightChild.setSize({(int)(m_scaledSize.x * 0.15f * scaleFactor), (int)(m_scaledSize.y * 0.07f * scaleFactor)});
    m_midRightChild.setSize({-1, (int)(m_scaledSize.y * 0.7f * scaleFactor)});
    m_bottomchildleft.setSize({(int)(m_scaledSize.x * 0.1f * scaleFactor), (int)(m_scaledSize.y * 0.18f * scaleFactor)});
    m_bottomchildmid.setSize({(int)(m_scaledSize.x * 0.74f * scaleFactor), (int)(m_scaledSize.y * 0.18f * scaleFactor)});
    m_bottomchildright.setSize({(int)(m_scaledSize.x * 0.2f * scaleFactor), (int)(m_scaledSize.y * 0.18f * scaleFactor)});
    m_okBtn.setSize({(int)(m_scaledSize.x * 0.1f * scaleFactor), (int)(m_scaledSize.y * 0.045f * scaleFactor)});
    m_cancelBtn.setSize({(int)(m_scaledSize.x * 0.1f * scaleFactor), (int)(m_scaledSize.y * 0.045f * scaleFactor)});
}

/*!
 * Set filetype by extension. Example ".png", ".jpg"
 * @param filetype
 */
void FileDialog::setFileType(const std::string &filetype)
{
    std::string name = m_fileTypes[filetype].getName();
    m_fileTypeCombo.setValue(name);
    m_fileTable.setFileFilter(m_filetypeFilter[name]);
}

void FileDialog::refreshFiletype()
{
    m_fileTable.setFileFilter(m_filetypeFilter[m_fileTypeCombo.getValue()]);
}
