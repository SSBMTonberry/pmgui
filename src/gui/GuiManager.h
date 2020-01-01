//
// Created by robin on 18.12.17.
//

#ifndef PMGUI_GUIMANAGER_H
#define PMGUI_GUIMANAGER_H

#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#include "themes/Theme.h"
#include "../imgui/imgui-SFML.h"

#include "../fmt/format.h"

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Clock.hpp"
#include "SFML/Window/Event.hpp"
#include "../managers/EventManager.h"

#include "forms/Form.h"
#include "forms/TextEditorForm.h"


namespace pmgui
{
    class GuiManager
    {
            typedef std::function<void(void)> func_customdraw; //OnCancel(), for instance
        public:
            enum class GuiTheme : int
            {
                Classic = 0,
                Dark = 1,
                Light = 2,
                /*! Open the Style Editor of ImGui */
                Custom = 3
            };

            GuiManager();

            GuiManager(sf::RenderWindow *window, pmgui::EventManager *eventManager);


            void initialize(sf::RenderWindow *window, pmgui::EventManager *eventManager);
            void addForm(std::unique_ptr<pmgui::Form> form);
            void addFormReference(pmgui::Form * form);
            void addTestForm();
            bool isShowImguiDemoWindow() const;

            void setTheme(const GuiTheme &theme);
            void setTheme(Theme &theme);
            void setSetting(const GuiSetting &setting, const float &x, const float &y);
            void setShowImguiDemoWindow(bool showImguiDemoWindow);

            pmgui::Form *getForm(const std::string &id);

            template <typename T>
            T * getForm(const std::string &id);
            template <typename T>
            T * getFormReference(const std::string &id);

            void registerOnCustomDrawCallback(const func_customdraw &cb);

            void update();
            void draw(bool callWindowDisplay = false);
            void handleEvents();
            bool run(bool callWindowDisplay = false);

        protected:
            bool m_showImguiDemoWindow = false;
            ImGuiStyle *m_style = nullptr;
            ImGuiIO *m_io = nullptr;

            std::vector<std::unique_ptr<pmgui::Form>> m_forms;
            std::vector<pmgui::Form *> m_formRefs;
            std::vector<func_customdraw> m_callbackOnCustomDraw;
            //std::map<std::string, std::unique_ptr<pmgui::Form>> m_forms;
            sf::RenderWindow *m_window;
            pmgui::EventManager *m_eventManager;
            sf::Clock m_deltaClock;
    };
}

template<typename T>
T * pmgui::GuiManager::getForm(const std::string &id)
{
    pmgui::Form *form = nullptr;
    for(auto & item : m_forms)
    {
        if(item->getId().compare(id) == 0)
            form = item.get();
    }

    if(form == nullptr)
        return nullptr;

    T * reference = static_cast<T*>(form);
    return reference;
}

template<typename T>
T * pmgui::GuiManager::getFormReference(const std::string &id)
{
    pmgui::Form *form = nullptr;
    for(auto & item : m_formRefs)
    {
        if(item->getId().compare(id) == 0)
            form = item;
    }

    if(form == nullptr)
        return nullptr;

    T * reference = static_cast<T*>(form);
    return reference;
}

#endif //PMGUI_GUIMANAGER_H
