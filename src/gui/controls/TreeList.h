//
// Created by robin on 14.07.18.
//

#ifndef PMGUI_TREELIST_H
#define PMGUI_TREELIST_H

#include <map>
#include <vector>
#include <algorithm>
#include "Selectable.h"

namespace pmgui
{

    class TreeList : public Control
    {
            typedef std::function<void(TreeList* )> func_treelist_parent;
            typedef std::function<bool(TreeList* )> func_treelist_parent_rightclick;
            typedef std::function<void(TreeList*, MenuItem*)> func_treelist_parent_chosencontextitem;

        public:
            TreeList();
            TreeList(const std::string &id, const std::string &label);

            bool process() override;
            pmgui::Selectable* add(const std::string &label);
            pmgui::Selectable* add(const std::string &id, const std::string &label);
            pmgui::Selectable* add(const std::string &id, const std::string &label, const pmgui::Image &image);
            pmgui::Selectable* add(const std::string &label, const void *imageData, size_t imageDataSize);
            pmgui::Selectable* add(const std::string &label, pmgui::Image *imageRef);
            bool remove(const std::string &id);
            void clear();

            void createRightClickContextItemsAdvanced(const std::initializer_list<std::pair<std::string, std::string>> &items);
            void createRightClickContextItems(const std::initializer_list<std::string> &items);

            void registerOnChosenCallback(const func_treelist_parent &cb);
            void registerOnRightClickCallback(const func_treelist_parent_rightclick &cb);
            void registerOnDoubleClickCallback(const func_treelist_parent &cb);
            void registerOnChosenContextItemCallback(const func_treelist_parent_chosencontextitem &cb);
            void setHasParentNode(bool hasParentNode);
            void setPagingSize(int pagingSize);
            void setCurrentPage(int currentPage);

            void swap(const std::string &id1, const std::string &id2);

            pmgui::Selectable * getItem(const std::string &id);
            pmgui::Selectable * getItem(int index);
            std::string_view getItemId(int index);
            std::vector<pmgui::Selectable *> getItems();
            bool isOpen() const;
            bool hasParentNode() const;
            int getPagingSize() const;
            int getCurrentPage() const;

        protected:
            void processMouseEvents();

            std::vector<func_treelist_parent> m_callbackOnParentClicked; //When you click the TreeList.
            std::vector<func_treelist_parent> m_callbackOnParentDoubleClicked; //When you double-click the TreeList.
            std::vector<func_treelist_parent_rightclick> m_callbackOnParentRightClicked; //When you right click the TreeList
            std::vector<func_treelist_parent_chosencontextitem> m_callbackOnChosenParentContextItem; //When you have chosen a context item of the TreeList (after a right click)

            std::map<std::string, pmgui::Selectable> m_items;
            std::vector<std::string> m_eraseItems;
            std::vector<pmgui::MenuItem> m_rightClickContextItems;
            bool m_rightClickContextActivated = false;
            bool m_isOpen = false;
            bool m_hasParentNode = true; //true = Has a tree-node. false = Makes it pretty much work like a list

            int m_pagingSize = 0;
            int m_currentPage = 0;

        private:
            void processPaging();
    };
}

#endif //PMGUI_TREELIST_H
