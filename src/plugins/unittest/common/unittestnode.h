// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef UNITTESTNODE_H
#define UNITTESTNODE_H

#include "common/project/projectinfo.h"

#include <memory>

class FileNode;
class FolderNode;
class ProjectNode;

class Node
{
public:
    virtual ~Node() = default;

    virtual bool isFolderNodeType() const { return false; }
    virtual bool isProjectNodeType() const { return false; }
    virtual bool isVirtualFolderType() const { return false; }

    ProjectNode *parentProjectNode() const;
    FolderNode *parentFolderNode() const;

    dpfservice::ProjectInfo projectInfo() const;
    QString filePath() const;
    virtual QString displayName() const;
    virtual QString tooltip() const;
    virtual QIcon icon() const;

    virtual FileNode *asFileNode() { return nullptr; }
    virtual const FileNode *asFileNode() const { return nullptr; }
    virtual FolderNode *asFolderNode() { return nullptr; }
    virtual const FolderNode *asFolderNode() const { return nullptr; }
    virtual ProjectNode *asProjectNode() { return nullptr; }
    virtual const ProjectNode *asProjectNode() const { return nullptr; }

    static bool sortByPath(const Node *a, const Node *b);
    void setParentFolderNode(FolderNode *parentFolder);

protected:
    Node();
    Node(const Node &other) = delete;
    bool operator=(const Node &other) = delete;

    void setFilePath(const QString &filePath);

private:
    QString nodeFilePath;
    FolderNode *nodeParentFolderNode { nullptr };
};

class FileNode : public Node
{
public:
    enum State {
        None,
        Generating,
        Completed,
        Failed,
        Ignored
    };

    explicit FileNode(const QString &filePath);

    void setState(State state);
    State state() const;

    FileNode *asFileNode() final { return this; }
    const FileNode *asFileNode() const final { return this; }

private:
    State nodeState { None };
};

class FolderNode : public Node
{
public:
    explicit FolderNode(const QString &folderPath);

    void setDisplayName(const QString &name);
    QString displayName() const override;
    void addNode(std::unique_ptr<Node> &&node);
    const QList<Node *> nodes() const;

    bool isFolderNodeType() const override { return true; }

    FolderNode *asFolderNode() override { return this; }
    const FolderNode *asFolderNode() const override { return this; }

protected:
    std::vector<std::unique_ptr<Node>> children;

private:
    QString nodeDisplayName;
};

class VirtualFolderNode : public FolderNode
{
public:
    explicit VirtualFolderNode(const QString &folderPath);

    bool isFolderNodeType() const override { return false; }
    bool isVirtualFolderType() const override { return true; }
};

class ProjectNode : public FolderNode
{
public:
    explicit ProjectNode(const QString &projectFilePath);

    bool isFolderNodeType() const override { return false; }
    bool isProjectNodeType() const override { return true; }

    ProjectNode *asProjectNode() final { return this; }
    const ProjectNode *asProjectNode() const final { return this; }
};

#endif   // UNITTESTNODE_H
