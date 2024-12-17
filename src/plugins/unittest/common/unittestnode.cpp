// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "unittestnode.h"

#include "common/util/qtcassert.h"
#include "services/project/projectservice.h"

#include <DFileIconProvider>

#include <QFileInfo>

DWIDGET_USE_NAMESPACE
using namespace dpfservice;

ProjectNode *Node::parentProjectNode() const
{
    if (!nodeParentFolderNode)
        return nullptr;
    auto pn = nodeParentFolderNode->asProjectNode();
    if (pn)
        return pn;
    return nodeParentFolderNode->parentProjectNode();
}

FolderNode *Node::parentFolderNode() const
{
    return nodeParentFolderNode;
}

dpfservice::ProjectInfo Node::projectInfo() const
{
    auto prjSrv = dpfGetService(ProjectService);
    Q_ASSERT(prjSrv);

    const auto &allInfos = prjSrv->getAllProjectInfo();
    auto iter = std::find_if(allInfos.cbegin(), allInfos.cend(),
                             [this](const ProjectInfo &info) {
                                 return nodeFilePath.startsWith(info.workspaceFolder());
                             });

    return iter == allInfos.cend() ? ProjectInfo() : *iter;
}

QString Node::filePath() const
{
    return nodeFilePath;
}

QString Node::displayName() const
{
    return QFileInfo(nodeFilePath).fileName();
}

QString Node::tooltip() const
{
    return nodeFilePath;
}

QIcon Node::icon() const
{
    return DFileIconProvider::globalProvider()->icon(QFileInfo(nodeFilePath));
}

bool Node::sortByPath(const Node *a, const Node *b)
{
    return a->filePath() < b->filePath();
}

void Node::setParentFolderNode(FolderNode *parentFolder)
{
    nodeParentFolderNode = parentFolder;
}

void Node::setFilePath(const QString &filePath)
{
    nodeFilePath = filePath;
}

FileNode::FileNode(const QString &filePath)
{
    setFilePath(filePath);
}

void FileNode::setState(State state)
{
    nodeState = state;
}

FileNode::State FileNode::state() const
{
    return nodeState;
}

FolderNode::FolderNode(const QString &folderPath)
{
    setFilePath(folderPath);
    nodeDisplayName = Node::displayName();
}

void FolderNode::setDisplayName(const QString &name)
{
    nodeDisplayName = name;
}

QString FolderNode::displayName() const
{
    return nodeDisplayName;
}

void FolderNode::addNode(std::unique_ptr<Node> &&node)
{
    QTC_ASSERT(node, return );
    QTC_ASSERT(!node->parentFolderNode(), qDebug("Node has already a parent folder"));
    node->setParentFolderNode(this);
    children.emplace_back(std::move(node));
}

const QList<Node *> FolderNode::nodes() const
{
    QList<Node *> nodeList;
    std::transform(children.begin(), children.end(), std::back_inserter(nodeList),
                   [](const auto &pointer) {
                       return pointer.get();
                   });
    return nodeList;
}

VirtualFolderNode::VirtualFolderNode(const QString &folderPath)
    : FolderNode(folderPath)
{
}

ProjectNode::ProjectNode(const QString &projectFilePath)
    : FolderNode(projectFilePath)
{
}
