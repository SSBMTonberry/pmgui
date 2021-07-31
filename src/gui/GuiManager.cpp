//
// Created by robin on 18.12.17.
//

#include "GuiManager.h"
#include "themes/Theme.h"
//#include "../../PixelmoundStudio/files/files.h"


pmgui::GuiManager::GuiManager()
{

}

pmgui::GuiManager::GuiManager(sf::RenderWindow *window)//, pmgui::EventManager *eventManager)
{
    initialize(window);//, eventManager);
}

void pmgui::GuiManager::initialize(sf::RenderWindow *window)//, pmgui::EventManager *eventManager)
{
    ImGui::CreateContext(); //IMGUI 1.60
    m_window = window;
    m_style = &ImGui::GetStyle();
    m_io = &ImGui::GetIO();
    //m_eventManager = eventManager;

    ImGui::SFML::Init(*m_window);

    //m_io->ConfigResizeWindowsFromEdges = true;

    //Enable Ctrl+TAB (New in ImGui 1.63)
    m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    //Enable docking (New in ImGui 1.66 WIP) - Downloaded 14.10.2018
    m_io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

bool pmgui::GuiManager::run(bool callWindowDisplay)
{
    update();
    draw(callWindowDisplay);
    return true;
}

void pmgui::GuiManager::update()
{
    sf::Event event;

    while(m_window->pollEvent(event))
    {
        ImGui::SFML::ProcessEvent(event);
        if (event.type == sf::Event::Closed)
        {
            m_window->close();
        }
    }
    ImGui::SFML::Update(*m_window, m_deltaClock.restart());
}

void pmgui::GuiManager::draw(bool callWindowDisplay)
{
    for(auto const & form: m_forms)
        form->draw();
    for(auto const & form: m_formRefs)
        form->draw();
    for(auto const &callback : m_callbackOnCustomDraw)
        callback();

    if(m_showImguiDemoWindow)
        ImGui::ShowDemoWindow();

    ImGui::SFML::Render(*m_window);

    if(callWindowDisplay)
        m_window->display();

}

void pmgui::GuiManager::addTestForm()
{
    m_showImguiDemoWindow = true;
}

void pmgui::GuiManager::addForm(std::unique_ptr<pmgui::Form> form)
{
    m_forms.push_back(std::move(form));
}

void pmgui::GuiManager::addFormReference(pmgui::Form *form)
{
    m_formRefs.push_back(form);
}

/*pmgui::Form *pmgui::GuiManager::getForm(const string &id)
{
    for(const auto & item : m_forms)
    {
        if(item->getId().compare(id) == 0)
        {
            return item.get();
        }
    }

    return nullptr;
}*/

void pmgui::GuiManager::setTheme(const pmgui::GuiManager::GuiTheme &theme)
{
    switch (theme)
    {
        case GuiTheme::Classic:
            ImGui::StyleColorsClassic();
            break;

        case GuiTheme::Dark:
            ImGui::StyleColorsDark();
            break;

        case GuiTheme::Light:
            ImGui::StyleColorsLight();
            break;

        case GuiTheme::Custom:
            ImGui::ShowStyleEditor();
    }
}

void pmgui::GuiManager::setTheme(Theme &theme)
{
    theme.assign();
}

/*!
 * Settings for the gui style.
 *
 * @param setting
 * @param x First parameter. In cases where the setting just needs one value, this is the one that is used.
 * @param y Second parameter, only used for settings that uses two values
 */
void pmgui::GuiManager::setSetting(const pmgui::GuiSetting &setting, const float &x, const float &y)
{

    switch (setting)
    {
        case GuiSetting::Alpha:
            m_style->Alpha = x;

        case GuiSetting::WindowPadding:
            m_style->WindowPadding = ImVec2(x, y);
            break;

        case GuiSetting::PopupRounding:
            m_style->PopupRounding = x;
            break;

        case GuiSetting::FramePadding:
            m_style->FramePadding = ImVec2(x, y);
            break;

        case GuiSetting::ItemSpacing:
            m_style->ItemSpacing = ImVec2(x, y);
            break;

        case GuiSetting::ItemInnerSpacing:
            m_style->ItemInnerSpacing = ImVec2(x, y);
            break;

        case GuiSetting::TouchExtraPadding:
            m_style->TouchExtraPadding = ImVec2(x, y);
            break;

        case GuiSetting::IndentSpacing:
            m_style->IndentSpacing = x;
            break;

        case GuiSetting::ScrollbarSize:
            m_style->ScrollbarSize = x;
            break;

        case GuiSetting::GrabMinSize:
            m_style->GrabMinSize = x;
            break;

        case GuiSetting::WindowBorderSize:
            m_style->WindowBorderSize = x;
            break;

        case GuiSetting::ChildBorderSize:
            m_style->ChildBorderSize = x;
            break;

        case GuiSetting::PopupBorderSize:
            m_style->PopupBorderSize = x;
            break;

        case GuiSetting::FrameBorderSize:
            m_style->FrameBorderSize = x;
            break;

        case GuiSetting::WindowRounding:
            m_style->WindowRounding = x;
            break;

        case GuiSetting::ChildRounding:
            m_style->ChildRounding = x;
            break;

        case GuiSetting::FrameRounding:
            m_style->FrameRounding = x;
            break;

        case GuiSetting::ScrollbarRounding:
            m_style->ScrollbarRounding = x;
            break;

        case GuiSetting::GrabRounding:
            m_style->GrabRounding = x;
            break;

        case GuiSetting::WindowTitleAlign:
            m_style->WindowTitleAlign = ImVec2(x, y);
            break;

        case GuiSetting::ButtonTextAlign:
            m_style->ButtonTextAlign = ImVec2(x, y);
            break;

        case GuiSetting::TabRounding:
            m_style->TabRounding = x;
    }
}

void pmgui::GuiManager::handleEvents()
{
    for(auto const & form: m_forms)
        form->handleEvents();
    for(auto const & form: m_formRefs)
        form->handleEvents();
}

bool pmgui::GuiManager::isShowImguiDemoWindow() const
{
    return m_showImguiDemoWindow;
}

void pmgui::GuiManager::setShowImguiDemoWindow(bool showImguiDemoWindow)
{
    m_showImguiDemoWindow = showImguiDemoWindow;
}

void pmgui::GuiManager::registerOnCustomDrawCallback(const pmgui::GuiManager::func_customdraw &cb)
{
    m_callbackOnCustomDraw.emplace_back(cb);
}




