//
// Created by robin on 20.01.19.
//

#include "FileTable.h"

pmgui::FileTable::FileTable(const std::string &id, const std::string &label, bool useFileIcons) : DataTable(id, label), m_useFileIcons {useFileIcons}
{
    create();
}

void pmgui::FileTable::create()
{
    //initializeFilemap();
    //initializeImgFilemap();
    addColumn("filename", "Filename");
    addColumn("size", "Size");
    addColumn("type", "Type");
    addColumn("modified", "Modified");
}

void pmgui::FileTable::listFilesByDirectory(const fs::path &path,const fs::path &parentDirectory)
{
    if(fs::exists(path) && fs::is_directory(path))
    {
        m_rows.clear();
        m_lastOpenedPath = path;
        m_pathMap.clear();
        m_pathToOpen = "";
        m_parentDirectory = parentDirectory;
        m_selectedRow = std::nullopt;
        m_callPathOpening = false;

        //Add parent directory
        if(fs::exists(parentDirectory) && fs::is_directory(parentDirectory))
        {
            DataRow *row = newRow();

            row->setValue("filename", "..");
            auto icon = getImgFileIcon("directory");
            row->setImageRef("filename", icon);

            row->setValue("type", "parent directory");
            row->setValue("modified", ""); //timefmt); //std::asctime(std::localtime(&cftime)));
            m_pathMap[generatePathId(*row)] = parentDirectory;
        }

        for (const auto &entry : fs::directory_iterator(path))
        {
            auto filename = entry.path().filename();
            auto extension = entry.path().extension();

            if (fs::is_directory(entry.status()))
            {
                DataRow *row = newRow();

                std::string timefmt = getFileTimeString(entry);
                row->setValue("filename", filename.u8string());
                auto icon = getImgFileIcon("directory");
                row->setImageRef("filename", icon);
                row->setValue("type", "directory");
                row->setValue("modified", timefmt); //std::asctime(std::localtime(&cftime)));
                m_pathMap[generatePathId(*row)] = entry.path();
            }
            else if (fs::is_regular_file(entry.status()))
            {
                if(m_fileFilter == "*.*" || m_fileFilter == extension)
                {
                    auto err = std::error_code{};
                    auto filesize = fs::file_size(entry, err);

                    std::string timefmt = getFileTimeString(entry);

                    DataRow *row = newRow();
                    row->setValue("filename", filename.string());

                    if(m_useFileIcons)
                    {
                        auto icon = getImgFileIcon(extension.string());
                        row->setImageRef("filename", icon);
                    }

                    row->setValue("type", extension.string());
                    row->setValue("size", fmt::format("{0}", filesize));
                    row->setValue("modified", timefmt); //std::asctime(std::localtime(&cftime)));

                    m_pathMap[generatePathId(*row)] = entry.path();
                }
            }
            else
            {
                if(m_fileFilter == "*.*" || m_fileFilter == extension)
                {

                    DataRow *row = newRow();
                    row->setValue("filename", filename.string());

                    if(m_useFileIcons)
                    {
                        auto icon = getImgFileIcon(extension.string());
                        row->setImageRef("filename", icon);
                    }
                    row->setValue("type", extension.string());
                    row->setValue("size", "0");
                    row->setValue("modified", "???");

                    m_pathMap[generatePathId(*row)] = entry.path().string();
                }
            }
        }
    }
    sort();
}

std::string pmgui::FileTable::getFileTimeString(const fs::directory_entry & entry)
{
    //#if MSVC
    //    //fs::file_time_type timeEntry = fs::last_write_time(entry);
    //    //std::string timefmt = getWindowsTimeStampString(entry);
    //    std::filesystem::file_time_type timeEntry = fs::last_write_time(entry);
    //    time_t cftime = to_time_t(timeEntry);
    //    std::string timefmt = fmt::format("{0:%Y.%m.%d %H:%M:%S}", *std::localtime(&cftime));
    //#elif APPLE
    //    //auto timeEntry = fs::last_write_time(entry);
    //                    //std::time_t cftime = decltype(timeEntry)::clock::to_time_t(timeEntry);
    //                    //time_t cftime = chrono::system_clock::to_time_t(timeEntry);
    //                    std::string timefmt = getOsxTimeStampString(entry);//"<not supported by Clang!>";//fmt::format("{0:%Y.%m.%d %H:%M:%S}", *std::localtime(&cftime));
    //#elif __GNUC__ > 8
    //    std::filesystem::file_time_type timeEntry = fs::last_write_time(entry);
    //    time_t cftime = to_time_t(timeEntry);
    //    std::string timefmt = fmt::format("{0:%Y.%m.%d %H:%M:%S}", *std::localtime(&cftime));
//
    //#else
    //    auto timeEntry = fs::last_write_time(entry);
    //                    time_t cftime = std::chrono::system_clock::to_time_t(timeEntry);
    //                    std::string timefmt = fmt::format("{0:%Y.%m.%d %H:%M:%S}", *std::localtime(&cftime));
    //#endif

    #if !defined (__clang__) && defined(__GNUC__) && __GNUC__ < 9
        auto timeEntry = fs::last_write_time(entry);
        time_t cftime = std::chrono::system_clock::to_time_t(timeEntry);
        std::string timefmt = fmt::format("{0:%Y.%m.%d %H:%M:%S}", *std::localtime(&cftime));
    #else
        std::string timefmt = getTimeStampString(entry);
    #endif

    return timefmt;
}



void pmgui::FileTable::onHeaderColumnClicked(const std::string &id)
{
    if(id == "filename" || id == "type")
    {
        sort(id, m_sortDesc);
        m_sortDesc = !m_sortDesc;
    }
    else if(id == "size")
    {
        sortAsInt(id, m_sortDesc);
        m_sortDesc = !m_sortDesc;
    }
    else if(id == "modified")
    {
        sort(id, m_sortDesc);
        m_sortDesc = !m_sortDesc;
    }

    m_previousSortAction = {id, !m_sortDesc};
}

void pmgui::FileTable::sort(const std::string &columnName, bool sortDesc)
{
    std::sort(m_rows.begin(), m_rows.end(), [&columnName, &sortDesc](DataRow const &a, DataRow const &b) -> bool
    {
        //Sort directories for themselves. Always appears on top
        if(a["type"] == "parent directory")
            return true;
        else if(b["type"] == "parent directory")
            return false;
        else if(a["type"] == "directory" && b["type"] != "directory")
            return true; //a < b
        else if(b["type"] == "directory" && a["type"] != "directory")
            return false; // a > b

        return lexicographical_compare(
                begin(a[columnName]), end(a[columnName]),
                begin(b[columnName]), end(b[columnName]),
                [&sortDesc](const char& char1, const char& char2) {
                    return (sortDesc) ? (tolower(char1) > tolower(char2)) : (tolower(char1) < tolower(char2));
                }
        );
    });
}

void pmgui::FileTable::sortAsInt(const std::string &columnName, bool sortDesc)
{
    std::sort(m_rows.begin(), m_rows.end(), [&columnName, &sortDesc](DataRow const &a, DataRow const &b) -> bool
    {
        //Sort directories for themselves. Always appears on top
        if(a["type"] == "parent directory")
            return true;
        else if(b["type"] == "parent directory")
            return false;
        else if(a["type"] == "directory" && b["type"] != "directory")
            return true; //a < b
        else if(b["type"] == "directory" && a["type"] != "directory")
            return false; // a > b

        int num_a = (a[columnName].size() == 0) ? 0 : stoi(a[columnName]);
        int num_b = (b[columnName].size() == 0) ? 0 : stoi(b[columnName]);

        return (sortDesc) ? (num_a > num_b) : (num_a < num_b);
    });
}

/*!
 * Will perform default or previous sort action
 */
void pmgui::FileTable::sort()
{
    sort(m_previousSortAction.first, m_previousSortAction.second);
}

void pmgui::FileTable::onRowDoubleClicked(DataRow *row)
{
    size_t size = m_pathMap.size();
    std::string path = m_pathMap[generatePathId(*row)].string();
    if(m_pathMap.count(generatePathId(*row)) > 0)
    {
        std::string pathId = generatePathId(*row);
        m_pathToOpen = m_pathMap[pathId];
        m_callPathOpening = true;
    }
    //Be careful to edit values of selectables on the way. It somewhat makes them unclickable after a while.
    //if(row->isDoubleClicked())
    //    row->setValue("type", "Double boy");
}

const fs::path &pmgui::FileTable::getPathToOpen() const
{
    std::string str = m_pathToOpen.string();
    return m_pathToOpen;
}

const std::string pmgui::FileTable::generatePathId(const DataRow &row) const
{
    return fmt::format("{0}_{1}", row["filename"], row["type"]);
}

bool pmgui::FileTable::canCallPathOpening() const
{
    return m_callPathOpening;
}

void pmgui::FileTable::resetPathOpeningCall()
{
    m_callPathOpening = false;
}

const fs::path &pmgui::FileTable::getLastOpenedPath() const
{
    return m_lastOpenedPath;
}

void pmgui::FileTable::onRowMarked(DataRow *row)
{
    fs::path path = m_pathMap[generatePathId(*row)];
    m_selectedFile = (fs::is_regular_file(path)) ? path.filename().string() : "";
    m_callRowChangeEvent = true;
}

bool pmgui::FileTable::canCallRowChangeEvent() const
{
    return m_callRowChangeEvent;
}

void pmgui::FileTable::resetRowChangeCall()
{
    m_callRowChangeEvent = false;
}

const std::string &pmgui::FileTable::getSelectedFile() const
{
    return m_selectedFile;
}

void pmgui::FileTable::setFileFilter(const std::string &filter)
{
    m_fileFilter = filter;
    listFilesByDirectory(m_lastOpenedPath, m_parentDirectory);
}

const std::string &pmgui::FileTable::getFileFilter() const
{
    return m_fileFilter;
}

bool pmgui::FileTable::useFileIcons() const
{
    return m_useFileIcons;
}

void pmgui::FileTable::setUseFileIcons(bool useFileIcons)
{
    m_useFileIcons = useFileIcons;
}

float pmgui::FileTable::getScaleFactor() const
{
    return m_scaleFactor;
}

void pmgui::FileTable::setScaleFactor(float scaleFactor)
{
    m_scaleFactor = scaleFactor;
}

void pmgui::FileTable::setFileTypes(std::map<std::string, pmgui::FileType> *fileTypes)
{
    m_fileTypes = fileTypes;
}

pmgui::Image *pmgui::FileTable::getImgFileIcon(const std::string &key)
{
    if(m_fileTypes != nullptr && m_fileTypes->count(key) > 0)
        return m_fileTypes->at(key).getImage();

    return m_defaultFiletype.getImage();
}


#if __clang__
std::string pmgui::FileTable::getOsxTimeStampString(const fs::path &path)
{
    auto timeEntry = fs::last_write_time(path);
    std::time_t cftime = decltype(timeEntry)::clock::to_time_t(timeEntry);
    std::string timefmt = fmt::format("{0:%Y.%m.%d %H:%M:%S}", *std::localtime(&cftime));

    return timefmt;
}
#endif

#if _MSC_VER && !__INTEL_COMPILER
std::string to_string( FILETIME ftime ) // ISO format, time zone designator Z == zero (UTC)
{
    SYSTEMTIME utc ;
    ::FileTimeToSystemTime( std::addressof(ftime), std::addressof(utc) );

    std::ostringstream stm;
    const auto w2 = std::setw(2) ;
    stm << std::setfill('0') << std::setw(4) << utc.wYear << '.' << w2 << utc.wMonth
        << '.' << w2 << utc.wDay << ' ' << w2 << utc.wHour
        << ':' << w2 << utc.wMinute << ':' << w2 << utc.wSecond;

    return stm.str() ;
}

std::string pmgui::FileTable::getWindowsTimeStampString(const fs::path &path)
{
    FILETIME filetime = { 0 };
    WIN32_FILE_ATTRIBUTE_DATA data;
    if (GetFileAttributesExW(path.wstring().c_str(), GetFileExInfoStandard, &data))
    {
        filetime = data.ftLastWriteTime;
        return to_string(filetime);
    }
    return "<date not found>";
}
#endif

/*!
 * Get file timestamp (works for all OSes)
 * @param path
 * @return
 */
std::string pmgui::FileTable::getTimeStampString(const fs::path &path)
{
    std::filesystem::file_time_type timeEntry = fs::last_write_time(path);
    time_t cftime = to_time_t(timeEntry);
    std::string timefmt = fmt::format("{0:%Y.%m.%d %H:%M:%S}", *std::localtime(&cftime));

    return timefmt;
}