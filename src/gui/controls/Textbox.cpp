//
// Created by robin on 27.12.17.
//


#include "Textbox.h"

/*!
 * More stack memory friendly constructor.
 * Remember to call create() before running process()
 *
 * @param id
 */
pmgui::Textbox::Textbox(std::string id) : Control(std::move(id))
{

}

/*!
 *
 * @param id
 * @param label
 * @param size if 0: there will be no limit. Otherwise: limited to whatever size of characters presented
 * @param imguiId When in the same form, this ID MUST be unique, or the Textboxes will have the same state, and will be
 * recognized by ImGui as the physically same textbox.
 */
pmgui::Textbox::Textbox(const std::string &id, const std::string &label, size_t size, int imguiId) :
        Control(id, label), m_size {size}, m_imguiId {imguiId}
{
    m_type = ControlType::Textbox;

    //m_size = size;
}

/*!
 * Only needs to be called when calling the most basic constructor
 * @param label
 * @param size 0 = unlimited. Otherwise the size of the characters.
 * @param imguiId
 */
void pmgui::Textbox::create(const std::string &label, size_t size, int imguiId)
{
    m_label = label;
    m_size = size;
    m_imguiId = imguiId;
}

/*!
 *
 * @return true if anything has been typed into the textbox, false otherwise
 */
bool pmgui::Textbox::process()
{
    if(Control::process())
    {

        //char buf[m_size];
        //char* buf = new char[m_size];
        auto buf = std::make_unique<char[]>(m_size);

        if(m_size > 0)
            strncpy(buf.get(), m_text.c_str(), sizeof(buf));

        m_isChanged = false;

        bool popColors = false;
        if (m_colorIsChanged && !m_useDefaultColor)
        {
            assignColors();
            popColors = true;
        }

        //if(ImGui::InputText(getImguiId().c_str(), buf, m_arraySize))

        if(!m_hasLabel && !m_disablePushItemWidth)
            ImGui::PushItemWidth(-1);
        else
            pushWidth();

        std::string id;
        if(m_imguiId > -1)
            id = (!m_hasLabel) ? fmt::format("###{0}", getImguiId()) : getImguiId();
        else
            id = m_label;

        if(m_size > 0)
        {
            if (m_isMultiline)
            {
                if (ImGui::InputTextMultiline(id.c_str(), buf.get(), m_size, m_multilineTextboxSize, getFlagsAsImGuiFlags()))
                    m_isChanged = true;
            }
            else
            {
                if (ImGui::InputText(id.c_str(), buf.get(), m_size, getFlagsAsImGuiFlags()))
                    m_isChanged = true;
            }
        }
        else
        {
            if (m_isMultiline)
            {
                if (ImGui::InputTextMultiline(id.c_str(), &m_text, m_multilineTextboxSize, getFlagsAsImGuiFlags()))
                    m_isChanged = true;
            }
            else
            {
                if (ImGui::InputText(id.c_str(), &m_text, getFlagsAsImGuiFlags()))
                    m_isChanged = true;
            }
        }

        m_controlSize = ImGui::GetItemRectSize();
        
        if(!m_hasLabel && !m_disablePushItemWidth)
            ImGui::PopItemWidth();
        else
            popWidth();

        if(m_size > 0)
            m_text = buf.get();

        if (popColors && !m_useDefaultColor)
        {
            ImGui::PopStyleColor(4);
        }

        if (ImGui::IsItemHovered() && m_tooltip.has_value())
            m_tooltip->show();

        return m_isChanged;
    }
    return false;
}

void pmgui::Textbox::setValue(const std::string &text)
{
    m_text = text;
}

const std::string & pmgui::Textbox::getValue() const
{
    return m_text;
}

/*!
 *
 * @return true if something has been typed into or been removed from the Textbox during process()
 */
bool pmgui::Textbox::isChanged() const
{
    return m_isChanged;
}

void pmgui::Textbox::setColor(const sf::Color &text, const sf::Color &selection, const sf::Color &background, const sf::Color &readOnlyBackground)
{
    m_textColor = text;
    m_backgroundColor = background;
    m_selectionColor = selection;
    m_readOnlyBgColor = readOnlyBackground;
    m_colorIsChanged = true;
}

void pmgui::Textbox::assignColors()
{
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(m_textColor));
    ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, ImVec4(m_selectionColor));
    ImGui::PushStyleColor(ImGuiCol_TextDisabled, ImVec4(m_readOnlyBgColor));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(m_backgroundColor));
}

pmgui::TextboxFlags pmgui::Textbox::getTextboxFlags() const
{
    return m_flags;
}

void pmgui::Textbox::setTextboxFlags(const pmgui::TextboxFlags &textboxFlags)
{
    m_flags = textboxFlags;
}

void pmgui::Textbox::addTextboxFlag(const pmgui::TextboxFlags &textboxFlag)
{
    m_flags |= textboxFlag;
}

void pmgui::Textbox::removeTextboxFlag(const pmgui::TextboxFlags &textboxFlag)
{
    m_flags &= ~textboxFlag;
}

bool pmgui::Textbox::hasTextboxFlag(const pmgui::TextboxFlags &textboxFlag)
{
    return ((m_flags & textboxFlag) == textboxFlag) ? true : false;
}

ImGuiInputTextFlags_ pmgui::Textbox::getFlagsAsImGuiFlags()
{
    return (ImGuiInputTextFlags_)m_flags;
}

bool pmgui::Textbox::hasLabel() const
{
    return m_hasLabel;
}

/*!
 *
 * @param hasLabel
 * @param disablePushItemWidth Only set to true if you know ImGui::PushItemWidth(-1) will do weird things.
 * Example: Found it made things very long in a Popup where it has flag for auto-resize.
 */
void pmgui::Textbox::setHasLabel(bool hasLabel, bool disablePushItemWidth)
{
    m_hasLabel = hasLabel;
    m_disablePushItemWidth = disablePushItemWidth;
}

std::string pmgui::Textbox::getImguiId()
{
    return fmt::format("{0}###{1}", m_label, m_imguiId);
}

void pmgui::Textbox::setChanged()
{
    m_isChanged = true;
}

void pmgui::Textbox::setIsMultiline(bool isMultiline)
{
    m_isMultiline = isMultiline;
}

bool pmgui::Textbox::isMultiline() const
{
    return m_isMultiline;
}

const sf::Vector2i &pmgui::Textbox::getMultilineTextboxSize() const
{
    return m_multilineTextboxSize;
}

void pmgui::Textbox::setMultilineTextboxSize(const sf::Vector2i &multilineTextboxSize)
{
    m_multilineTextboxSize = multilineTextboxSize;
}

const sf::Vector2i &pmgui::Textbox::getControlSize() const
{
    return m_controlSize;
}



