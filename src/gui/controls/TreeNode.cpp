//
// Created by robin on 19.01.18.
//

#include "TreeNode.h"

pmgui::TreeNode::TreeNode(const std::string &id, const std::string &label, NodeType nodeType)
        : Control(id, label), m_nodeType {nodeType}
{
    //m_nodeType = nodeType;
    m_type = ControlType::TreeNode;
}

bool pmgui::TreeNode::process()
{
    bool isAnyItemActivated = false;
    if(Control::process())
    {
        switch(m_nodeType)
        {
            case NodeType::CollapsingHeader:
                pushWidth();
                if(ImGui::CollapsingHeader(m_imguiId.c_str()))
                {
                    ImGui::PushID(m_imguiId.c_str());
                    for (const auto &item : m_controls)
                    {
                        if (item->process())
                        {
                            isAnyItemActivated = true;
                        }
                    }
                    for (const auto &item : m_controlRefs)
                    {
                        if (item->process())
                        {
                            isAnyItemActivated = true;
                        }
                    }
                    ImGui::PopID();
                }
                popWidth();
                break;

            case NodeType::TreeNode:
                pushWidth();
                if(ImGui::TreeNode(m_imguiId.c_str()))
                {
                    for (const auto &item : m_controls)
                    {
                        if (item->process())
                        {
                            isAnyItemActivated = true;
                        }
                    }
                    for (const auto &item : m_controlRefs)
                    {
                        if (item->process())
                        {
                            isAnyItemActivated = true;
                        }
                    }
                    ImGui::TreePop();
                }
                popWidth();
                break;
        }
    }

    return isAnyItemActivated;
}

void pmgui::TreeNode::add(std::unique_ptr<pmgui::Control> control)
{
    m_controls.push_back(std::move(control));
}


void pmgui::TreeNode::addReference(pmgui::Control *ref)
{
    m_controlRefs.push_back(ref);
}
