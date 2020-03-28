cmake_minimum_required(VERSION 3.11)
project(pmgui)

configure_file (
        "${PROJECT_SOURCE_DIR}/../PmguiConfig.h.in"
        "${PROJECT_SOURCE_DIR}/PmguiConfig.h")

include_directories(../external_libs/include)

set(FMT_SOURCE_FILES fmt/format.cc fmt/posix.cc)

set(IMGUI_SOURCE_FILES imgui/imgui.cpp imgui/imconfig.h
        imgui/imgui_draw.cpp imgui/imgui_widgets.cpp
        imgui/imgui_demo.cpp imgui/misc/cpp/imgui_stdlib.cpp
        imgui/imgui-SFML.cpp)

set(SOURCE_FILES gui/controls/Button.cpp gui/controls/Button.h gui/controls/Checkbox.cpp gui/controls/Checkbox.h
        gui/controls/CheckboxSection.cpp gui/controls/CheckboxSection.h gui/controls/Child.cpp gui/controls/Child.h
        gui/controls/ColorButton.cpp gui/controls/ColorButton.h gui/controls/ColoredText.cpp gui/controls/ColoredText.h
        gui/controls/ColorPicker.cpp gui/controls/ColorPicker.h gui/controls/Column.cpp gui/controls/Column.h
        gui/controls/Combobox.cpp gui/controls/Combobox.h gui/controls/Control.cpp gui/controls/Control.h
        gui/controls/DataColumn.cpp gui/controls/DataColumn.h gui/controls/DataColumnDefinition.cpp
        gui/controls/DataColumnDefinition.h gui/controls/DataRow.cpp gui/controls/DataRow.h gui/controls/DataTable.cpp
        gui/controls/DataTable.h gui/controls/DragFloat.cpp gui/controls/DragFloat.h gui/controls/DragInt.cpp
        gui/controls/DragInt.h gui/controls/Group.cpp gui/controls/Group.h gui/controls/Image.cpp gui/controls/Image.h
        gui/controls/ImageButton.cpp gui/controls/ImageButton.h gui/controls/InputFloat.cpp gui/controls/InputFloat.h
        gui/controls/InputInt.cpp gui/controls/InputInt.h gui/controls/Label.cpp gui/controls/Label.h
        gui/controls/Listbox.cpp gui/controls/Listbox.h gui/controls/MenuBar.cpp gui/controls/MenuBar.h
        gui/controls/MenuItem.cpp gui/controls/MenuItem.h gui/controls/MenuItemCollection.cpp
        gui/controls/MenuItemCollection.h gui/controls/Progressbar.cpp gui/controls/Progressbar.h
        gui/controls/RadioSection.cpp gui/controls/RadioSection.h gui/controls/Row.cpp gui/controls/Row.h
        gui/controls/Selectable.cpp gui/controls/Selectable.h gui/controls/SliderFloat.cpp gui/controls/SliderFloat.h
        gui/controls/SliderInt.cpp gui/controls/SliderInt.h gui/controls/Table.cpp gui/controls/Table.h
        gui/controls/Textbox.cpp gui/controls/Textbox.h gui/controls/TreeList.cpp gui/controls/TreeList.h
        gui/controls/TreeNode.cpp gui/controls/TreeNode.h gui/forms/Form.cpp gui/forms/Form.h
        gui/forms/MessagePopup.cpp gui/forms/MessagePopup.h gui/forms/Popup.cpp gui/forms/Popup.h
        gui/forms/TextEditorForm.cpp gui/forms/TextEditorForm.h gui/properties/RadioData.cpp gui/properties/RadioData.h
        gui/properties/Tooltip.cpp gui/properties/Tooltip.h gui/themes/Theme.cpp gui/themes/Theme.h
        gui/TextEditor.cpp gui/TextEditor.h pmgui.h managers/cpplinq.hpp managers/EventManager.h managers/EventManager.cpp gui/GuiManager.h gui/GuiManager.cpp
        widgets/FileDialog.h widgets/FileDialog.cpp widgets/FileTable.h widgets/FileTable.cpp
        widgets/FileType.cpp widgets/FileType.h widgets/FileTypeCollection.cpp widgets/FileTypeCollection.h)

add_library(pmgui ${SOURCE_FILES} ${IMGUI_SOURCE_FILES} ${FMT_SOURCE_FILES})