//
// Created by robin on 27.12.17.
//

#include "Form.h"

pmgui::Form::Form(std::string id, std::string title) ://, std::string imguiId) :
                m_id {std::move(id)}, m_title {std::move(title)}//, m_imguiId {std::move(imguiId)}
{

}

pmgui::Form::Form(const sf::Vector2<int> &position, const sf::Vector2<int> &size, std::string id,
                std::string title) ://, std::string imguiId) :
                m_position {position}, m_size {size}, m_scaledSize {size}, m_id {std::move(id)}, m_title {std::move(title)}, //m_imguiId {std::move(imguiId)},
                m_positionHasBeenChanged {true}
{

}

/*void pmgui::Form::update()
{
    for(auto const & item : m_controls)
        item->update();
}*/

/*!
 *
 * @return if any items are changed
 */
bool pmgui::Form::draw()
{
    m_formIsHovered = false;
    m_formIsActive = false;
    if(!m_isVisible || !m_open)
        return false;

    m_previousWindowPosition = m_currentWindowPosition;
    m_previousWindowSize = m_currentWindowSize;

    bool anyItemChanged = false;
    if(m_positionHasBeenChanged || m_useLockedPosition)
    {
        ImGui::SetNextWindowPos(ImVec2(m_position));
        ImGui::SetNextWindowSize(ImVec2(m_scaledSize));
        m_positionHasBeenChanged = false;
    }

    ImGui::Begin(getImguiId().c_str(), &m_open, getFlagsAsImGuiFlags());
    m_currentWindowSize = ImGui::GetWindowSize();
    m_currentWindowPosition = ImGui::GetWindowPos();
    m_position = {(int)m_currentWindowPosition.x, (int) m_currentWindowPosition.y};

    if (ImGui::IsWindowHovered())
    {
        m_formIsHovered = true;
    }
    if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows))
    {
        m_formIsActive = true;
    }

    if(m_currentWindowSize != m_previousWindowSize) onWindowResize();
    if(m_currentWindowPosition != m_previousWindowPosition) onMoved();

    for(auto const & item : m_controls)
    {
        if(item->process())
            anyItemChanged = true;
    }
    for(const auto &item : m_controlRefs)
    {
        if(item->process())
            anyItemChanged = true;

    }
    customDraw();
    ImGui::End();

    return anyItemChanged;
}

void pmgui::Form::handleEvents()
{

}

/*!
 *
 * @return m_title + ### + m_imguiId
 */
std::string pmgui::Form::getImguiId()
{
    return (!m_id.empty()) ? fmt::format("{0}###{1}_{2}", m_title, m_id, m_parentId) : m_title;
}

void pmgui::Form::add(std::unique_ptr<pmgui::Control> control)
{
    control->setParentId(m_id);
    m_controls.push_back(std::move(control));
}

void pmgui::Form::addReference(pmgui::Control *ref, bool setThisAsParent)
{
    if(setThisAsParent)
        ref->setParentId(m_id);

    m_controlRefs.push_back(ref);
}

const std::string &pmgui::Form::getId() const
{
    return m_id;
}

pmgui::FormFlags pmgui::Form::getFormFlags() const
{
    return m_flags;
}

void pmgui::Form::setFormFlags(const pmgui::FormFlags &formFlags)
{
    m_flags = formFlags;
}

void pmgui::Form::addFormFlag(const pmgui::FormFlags &formFlag)
{
    m_flags |= formFlag;
}

void pmgui::Form::removeFormFlag(const pmgui::FormFlags &formFlag)
{
    m_flags &= ~formFlag;
}

bool pmgui::Form::hasFormFlag(const pmgui::FormFlags &formFlag)
{
    return ((m_flags & formFlag) == formFlag) ? true : false;
}

ImGuiWindowFlags_ pmgui::Form::getFlagsAsImGuiFlags()
{
    return (ImGuiWindowFlags_)m_flags;
}

bool pmgui::Form::isOpen() const
{
    return m_open;
}

void pmgui::Form::setOpen(bool open)
{
    m_open = open;
    if(open)
        onOpen();
}

bool pmgui::Form::isVisible() const
{
    return m_isVisible;
}

void pmgui::Form::setIsVisible(bool isVisible)
{
    m_isVisible = isVisible;
}

const sf::Vector2<int> &pmgui::Form::getSize() const
{
    return m_size;
}

void pmgui::Form::setSize(const sf::Vector2<int> &size)
{
    m_positionHasBeenChanged = true;
    m_size = size;
    m_scaledSize = {(int)(size.x * m_scaleFactor), (int)(size.y * m_scaleFactor)};
}

const sf::Vector2<int> &pmgui::Form::getPosition() const
{
    return m_position;
}

void pmgui::Form::setPosition(const sf::Vector2<int> &position)
{
    m_position = position;
}

//bool pmgui::Form::hasImguiId() const
//{
//    return m_id.empty() ? false : true;
//}

bool pmgui::Form::customDraw()
{
    return false;
}

const sf::Vector2<size_t> &pmgui::Form::getCurrentWindowSize() const
{
    return m_currentWindowSize;
}

const sf::Vector2<size_t> &pmgui::Form::getCurrentWindowPosition() const
{
    return m_currentWindowPosition;
}

/*!
 * Used to scale the size of the form and/or its elements.
 * Default is set to 1.
 * @param scaleFactor
 */
float pmgui::Form::getScaleFactor() const
{
    return m_scaleFactor;
}

/*!
 * Used to scale the size of the form and/or its elements.
 * Default is set to 1.
 * @param scaleFactor
 */
void pmgui::Form::setScaleFactor(float scaleFactor)
{
    m_scaleFactor = scaleFactor;
    m_scaledSize = {(int)(m_size.x * m_scaleFactor), (int)(m_size.y * m_scaleFactor)};
    m_positionHasBeenChanged = true;
}

std::vector<std::unique_ptr<pmgui::Control>> *pmgui::Form::getControls()
{
    return &m_controls;
}

void pmgui::Form::clear()
{
    m_controls.clear();
    m_controlRefs.clear();
}

const std::string &pmgui::Form::getParentId() const
{
    return m_parentId;
}

void pmgui::Form::setParentId(const std::string &parentId)
{
    m_parentId = parentId;
}

const std::string &pmgui::Form::getTitle() const
{
    return m_title;
}

