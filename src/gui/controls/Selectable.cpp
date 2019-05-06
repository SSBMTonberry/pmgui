//
// Created by robin on 17.01.18.
//

#include "Selectable.h"

pmgui::Selectable::Selectable(const std::string &id, const std::string &label, std::optional<pmgui::Image> image)
        : Control(id, label), m_hasSize {false}
{
    if(image.has_value())
        m_image = move(image);

    //m_hasSize = false;
    m_type = ControlType::Selectable;
}

pmgui::Selectable::Selectable(const std::string &id, const std::string &label, pmgui::Image *image)
        : Control(id, label), m_hasSize {false}
{
    m_imageRef = image;

    m_type = ControlType::Selectable;
}

pmgui::Selectable::Selectable(const std::string &id, const std::string &label, const sf::Vector2<int> &size,
                            std::optional<pmgui::Image> image) : Control(id, label), m_hasSize {true}, m_size {size}
{
    if(image.has_value())
        m_image = move(m_image);

    //m_hasSize = true;
    //m_size = size;
    m_type = ControlType::Selectable;
}

/*!
 * @param imageData
 * @param imageDataSize
 */
void pmgui::Selectable::setImage(const void *imageData, size_t imageDataSize, bool hasZoomTooltip)
{
    if(m_image.has_value())
        m_image->create(imageData, imageDataSize);
    else
        m_image = std::make_optional<pmgui::Image>("<empty>", imageData, imageDataSize, hasZoomTooltip);
    m_useImageRef = false;
}

void pmgui::Selectable::setImage(const std::string &id, const void *imageData, size_t imageDataSize, bool hasZoomTooltip)
{
    if(!m_image.has_value())
        m_image = std::make_optional<pmgui::Image>(id, imageData, imageDataSize, hasZoomTooltip);
    else
        m_image->create(imageData, imageDataSize);
    m_useImageRef = false;
}

void pmgui::Selectable::setImageRef(pmgui::Image * image)
{
    m_imageRef = image;
    m_useImageRef = true;
}

bool pmgui::Selectable::isSelected() const
{
    return m_isSelected;
}

void pmgui::Selectable::setSelected(bool isSelected)
{
    m_isSelected = isSelected;
}

void pmgui::Selectable::toggleSelected()
{
    m_isSelected = !m_isSelected;
}

//void pmgui::Selectable::setFocused()
//{
//    m_setFocused = true;
//}

/*!
 *
 * @return true if pressed, false otherwise
 */
bool pmgui::Selectable::process()
{
    if(Control::process())
    {

        bool isPressed = false;

        if(!m_useImageRef && m_image.has_value())
        {
            m_image.value().process();
            ImGui::SameLine();
        }
        else if(m_useImageRef && m_imageRef != nullptr)
        {
            m_imageRef->process();
            ImGui::SameLine();
        }

        if (m_hasSize)
        {
            if (ImGui::Selectable(m_label.c_str(), m_isSelected, 0, m_size)) {};
        }
        else
        {
            if (ImGui::Selectable(m_label.c_str(), m_isSelected)) {};
        }

        //if(m_setFocused)
        //{
        //    m_setFocused = false;
        //    ImGui::SetKeyboardFocusHere(-1);
        //}
        //performFocusCheck();

        isPressed = processMouseEvents();

        if(ImGui::IsMouseClicked(0))
        {
            m_rightClickContextActivated = false;
        }

        return isPressed;
    }
    return false;
}

//void pmgui::Selectable::performFocusCheck()
//{
//    bool previousFocusState = m_isFocused;
//    m_isFocused = ImGui::IsItemFocused();
//
//    if(m_isFocused && previousFocusState != m_isFocused)
//    {
//        for (auto &callback : m_callbackOnFocused)
//            callback(this);
//    }
//
//}

bool pmgui::Selectable::processMouseEvents()
{
    bool isPressed = false;
    ImGui::PushID(m_label.c_str());
    {

        if(ImGui::IsItemClicked(0))
        {
            if (m_isToggleable)
            {
                m_isSelected = !m_isSelected;
            }
            isPressed = true;
            for (auto &callback : m_callbackOnChosen)
                callback(this);
        }

        if(ImGui::IsItemClicked(1))
        {
            for (auto &callback : m_callbackOnRightClick)
            {
                if (callback(this))
                {
                    m_rightClickContextActivated = true;
                    m_isSelected = true;
                }
            }
        }

        if(m_isSelected && m_rightClickContextActivated)
        {
            if(ImGui::BeginPopupContextWindow())
            {
                for(auto &item : m_rightClickContextItems)
                {
                    if(item.process())
                    {
                        for (auto &callback : m_callbackOnChosenContextItem)
                        {
                            callback(this, &item);
                        }
                    }
                }
                ImGui::EndPopup();
            }
        }

        if(ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
        {
            for (auto &callback : m_callbackOnDoubleClick)
            {
                callback(this);
            }
        }
    }
    ImGui::PopID();

    return isPressed;
}

bool pmgui::Selectable::isToggleable() const
{
    return m_isToggleable;
}

void pmgui::Selectable::setIsToggleable(bool isToggleable)
{
    m_isToggleable = isToggleable;
}

const sf::Vector2<int> &pmgui::Selectable::getSize() const
{
    return m_size;
}

void pmgui::Selectable::setSize(const sf::Vector2<int> &size)
{
    m_size = size;
}

/*!
 * Creates a context of menuitems used for when the right click context has been activated.
 * This is a fresh context, and will overwrite any potential existing data.
 *
 * @param items Each item is defined as {"id", "label"}
 */
void pmgui::Selectable::createRightClickContextItemsAdvanced(
        const std::initializer_list<std::pair<std::string, std::string>> &items)
{
    m_rightClickContextItems.clear();
    for(auto &item : items)
    {
        m_rightClickContextItems.emplace_back(item.first, item.second);
    }
}

/*!
 * An easier way to create right click context, where ID's are automatically set to be the same as label, but in lower-case.
 * @param items
 */
void pmgui::Selectable::createRightClickContextItems(const std::initializer_list<std::string> &items)
{
    m_rightClickContextItems.clear();
    for(auto &item : items)
    {
        std::string id = item;
        std::transform(id.begin(), id.end(), id.begin(), ::tolower);
        m_rightClickContextItems.emplace_back(id, item);
    }
}

//void pmgui::Selectable::registerOnFocusedCallback(const func_selectable &cb)
//{
//    m_callbackOnFocused.emplace_back(cb);
//}

void pmgui::Selectable::registerOnChosenCallback(const func_selectable &cb)
{
    m_callbackOnChosen.emplace_back(cb);
}

void pmgui::Selectable::registerOnRightClickCallback(const func_onrightclick &cb)
{
    m_callbackOnRightClick.emplace_back(cb);
}

void pmgui::Selectable::registerOnDoubleClickCallback(const func_selectable &cb)
{
    m_callbackOnDoubleClick.emplace_back(cb);
}

void pmgui::Selectable::registerOnChosenContextItemCallback(const func_onchosencontextitem &cb)
{
    m_callbackOnChosenContextItem.emplace_back(cb);
}

pmgui::Image* pmgui::Selectable::getImage()
{
    return &m_image.value();
}

pmgui::Image *pmgui::Selectable::getImageRef() const
{
    return m_imageRef;
}

//bool pmgui::Selectable::isFocused() const
//{
//    return m_isFocused;
//}
