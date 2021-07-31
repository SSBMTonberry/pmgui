//
// Created by robin on 20.01.19.
//

#ifndef PMGUI_FILETABLE_H
#define PMGUI_FILETABLE_H

//#include "../PmguiConfig.h"
#include "../gui/controls/DataTable.h"
#include "../pmgui_files/files_mapper.h"
#include "FileTypeCollection.h"

#include <map>
//#include "../managers/cpplinq.hpp"
#include "../fmt/time.h"
#include <ctime>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <sstream>

//namespace fs = std::experimental::filesystem;


namespace pmgui_fm = pmgui_files_mapper;
//using namespace cpplinq;

#if _MSC_VER && !__INTEL_COMPILER
#include <Windows.h>
#endif


namespace pmgui
{

    class FileTable : public DataTable
    {
        public:
            FileTable(const std::string &id, const std::string &label, bool useFileIcons = true);

            void listFilesByDirectory(const fs::path &path, const fs::path &parentDirectory);
            //std::pair<const unsigned char *, size_t> getFileIcon(const std::string &key);
            //pmgui::Image *getImgFileIcon(const std::string &key);

            void sort();
            void sort(const std::string &columnName, bool sortDesc = false);
            void sortAsInt(const std::string &columnName, bool sortDesc = false);

            void resetPathOpeningCall();
            void resetRowChangeCall();

            void setFileTypes(std::map<std::string, pmgui::FileType> *fileTypes);

            void setFileFilter(const std::string &filter);
            void setUseFileIcons(bool useFileIcons);
            void setScaleFactor(float scaleFactor);

            bool canCallPathOpening() const;
            const fs::path &getPathToOpen() const;
            const fs::path &getLastOpenedPath() const;
            const std::string &getFileFilter() const;
            const std::string &getSelectedFile() const;
            bool useFileIcons() const;
            bool canCallRowChangeEvent() const;
            float getScaleFactor() const;
            pmgui::Image *getImgFileIcon(const std::string &key);

        protected:
            void create();
            void onHeaderColumnClicked(const std::string &id) override;
            void onRowDoubleClicked(DataRow *row) override;

            std::string getFileTimeString(const fs::directory_entry & entry);

#if _MSC_VER && !__INTEL_COMPILER
            std::string getWindowsTimeStampString(const fs::path &path);
#elif __clang__
            std::string getOsxTimeStampString(const fs::path &path);
#endif
            std::string getTimeStampString(const fs::path &path); //Should work for ALL OSes (Windows, Linux, Mac)
            void onRowMarked(DataRow *row) override;

            const std::string generatePathId(const DataRow &row) const;


            template <typename TP>
            std::time_t to_time_t(TP tp);

            //void initializeImgFilemap();
            //std::map<std::string, std::pair<const unsigned char *, size_t>> m_fileMap;
            //std::map<std::string, pmgui::Image> m_imgFileMap;
            std::map<std::string, pmgui::FileType> *m_fileTypes;
            std::map<std::string, fs::path> m_pathMap;
            std::pair<std::string, bool> m_previousSortAction = {"filename", false}; //first: columnName, second: orderDesc
            FileType m_defaultFiletype = {"", "default"};

            std::string m_selectedFile = "";

            bool m_sortDesc = false;
            bool m_callPathOpening = false;
            bool m_callRowChangeEvent = false;

            bool m_useFileIcons = true;
            std::string m_fileFilter = "*.*"; //Shows all as standard
            float m_scaleFactor = 1.f;

            fs::path m_lastOpenedPath;
            fs::path m_pathToOpen;
            fs::path m_parentDirectory;
    };

    template <typename TP>
    std::time_t pmgui::FileTable::to_time_t(TP tp)
    {
        using namespace std::chrono;
        auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now()
                                                            + system_clock::now());
        return system_clock::to_time_t(sctp);
    }
}

#endif //PMGUI_FILETABLE_H
