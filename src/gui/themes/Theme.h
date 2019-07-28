//
// Created by robin on 06.01.18.
//

#ifndef PMGUI_THEME_H
#define PMGUI_THEME_H

#include "../../imgui/imgui.h"
#include <map>
#include "SFML/Main.hpp"
namespace pmgui
{

    class Theme
    {

        public:
            enum class Control
            {
                Text = ImGuiCol_Text,
                TextDisabled = ImGuiCol_TextDisabled,
                WindowBg = ImGuiCol_WindowBg,                               // Background of normal windows
                ChildBg = ImGuiCol_ChildBg,                                 // Background of child windows
                PopupBg = ImGuiCol_PopupBg,                                 // Background of popups, menus, tooltips windows
                Border = ImGuiCol_Border,
                BorderShadow = ImGuiCol_BorderShadow,
                FrameBg = ImGuiCol_FrameBg,                                 // Background of checkbox, radio button, plot, slider, text input
                FrameBgHovered = ImGuiCol_FrameBgHovered,
                FrameBgActive = ImGuiCol_FrameBgActive,
                TitleBg = ImGuiCol_TitleBg,
                TitleBgActive = ImGuiCol_TitleBgActive,
                TitleBgCollapsed = ImGuiCol_TitleBgCollapsed,
                MenuBarBg = ImGuiCol_MenuBarBg,
                ScrollbarBg = ImGuiCol_ScrollbarBg,
                ScrollbarGrab = ImGuiCol_ScrollbarGrab,
                ScrollbarGrabHovered = ImGuiCol_ScrollbarGrabHovered,
                ScrollbarGrabActive = ImGuiCol_ScrollbarGrabActive,
                CheckMark = ImGuiCol_CheckMark,
                SliderGrab = ImGuiCol_SliderGrab,
                SliderGrabActive = ImGuiCol_SliderGrabActive,
                Button = ImGuiCol_Button,
                ButtonHovered = ImGuiCol_ButtonHovered,
                ButtonActive = ImGuiCol_ButtonActive,
                Header = ImGuiCol_Header,
                HeaderHovered = ImGuiCol_HeaderHovered,
                HeaderActive = ImGuiCol_HeaderActive,
                Separator = ImGuiCol_Separator,
                SeparatorHovered = ImGuiCol_SeparatorHovered,
                SeparatorActive = ImGuiCol_SeparatorActive,
                ResizeGrip = ImGuiCol_ResizeGrip,
                ResizeGripHovered = ImGuiCol_ResizeGripHovered,
                ResizeGripActive = ImGuiCol_ResizeGripActive,
                Tab = ImGuiCol_Tab,
                TabHovered = ImGuiCol_TabHovered,
                TabActive = ImGuiCol_TabActive,
                TabUnfocused = ImGuiCol_TabUnfocused,
                TabUnfocusedActive = ImGuiCol_TabUnfocusedActive,
                DockingPreview = ImGuiCol_DockingPreview,
                DockingEmptyBg = ImGuiCol_DockingEmptyBg,                   // Background color for empty node (e.g. CentralNode with no window docked into it)
                PlotLines = ImGuiCol_PlotLines,
                PlotLinesHovered = ImGuiCol_PlotLinesHovered,
                PlotHistogram = ImGuiCol_PlotHistogram,
                PlotHistogramHovered = ImGuiCol_PlotHistogramHovered,
                TextSelectedBg = ImGuiCol_TextSelectedBg,
                DragDropTarget = ImGuiCol_DragDropTarget,
                NavHighlight = ImGuiCol_NavHighlight,                       // Gamepad/keyboard: current highlighted item
                NavWindowingHighlight = ImGuiCol_NavWindowingHighlight,     // Highlight window when using CTRL+TAB
                NavWindowingDimBg = ImGuiCol_NavWindowingDimBg,             // Darken/colorize entire screen behind the CTRL+TAB window list, when active
                ModalWindowDimBg = ImGuiCol_ModalWindowDimBg                // Darken/colorize entire screen behind a modal window, when one is active
            };

            Theme();
            void setColor(const Control &control, const sf::Color &color);
            void assign();

        protected:
            virtual void initialize();

            std::map<pmgui::Theme::Control, sf::Color> m_colorMap;
    };
}

#endif //PMGUI_THEME_H
