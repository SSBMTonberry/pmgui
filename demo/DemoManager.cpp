//
// Created by robin on 06.05.19.
//

#include "DemoManager.h"

void pmgui::DemoManager::initialize(const std::string &title, const sf::Vector2<uint32_t> &resolution, sf::Uint32 style, const sf::ContextSettings &settings)
{
    m_window.create(sf::VideoMode(resolution.x, resolution.y), title, style, settings);
    m_window.setView(sf::View(sf::FloatRect(0.f, 0.f, resolution.x, resolution.y)));
    m_window.setFramerateLimit(60);
    m_window.setVerticalSyncEnabled(true);


    ImGui::CreateContext(); //IMGUI 1.60
    ImGui::SFML::Init(m_window);
    //Enable Ctrl+TAB (New in ImGui 1.63)
    //m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; //Removed - 18.03.2019 - Disabled due to interrupting hotkeys...
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports;
    //Enable docking (New in ImGui 1.66 WIP) - Downloaded 14.10.2018
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    m_window.resetGLStates(); // call it if you only process ImGui. Otherwise not needed.

    createDemo();
}

void pmgui::DemoManager::run()
{
    while (m_window.isOpen())
    {
        m_window.clear(m_backgroundColor);
        update();
        handleEvents();
        draw();
        
        m_window.display();
    }
}

void pmgui::DemoManager::update()
{
    sf::Event event;
    while(m_window.pollEvent(event))
    {
        ImGui::SFML::ProcessEvent(event);
        switch(event.type)
        {
            case sf::Event::EventType::Closed:
                m_window.close();
                break;
        }
    }
    ImGui::SFML::Update(m_window, m_deltaClock.restart());
}

void pmgui::DemoManager::handleEvents()
{
    m_demoForm.handleEvents();
    m_fileDialogFile.handleEvents();
}

void pmgui::DemoManager::draw()
{
    ImGui::ShowDemoWindow();

    m_demoForm.draw();
    m_fileDialogFile.draw();
    ImGui::SFML::Render(m_window);
}

void pmgui::DemoManager::createDemo()
{
    m_demoText1 = m_demoForm.create<pmgui::Textbox>("demo_text_1", "Demo text 1");
    m_demoText1->setValue("Hello there!");

    m_fileDialogFile.setFileTypeCollection("images", true);
    m_fileDialogFile.setOpen(true);
}
