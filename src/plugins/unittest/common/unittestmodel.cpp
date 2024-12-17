// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "unittestmodel.h"

class UnitTestModelPrivate
{
public:
    QStandardItem *findChildItem(QStandardItem *item, const Node *node);

public:
    WrapperItem *rootItem { nullptr };
};

QStandardItem *UnitTestModelPrivate::findChildItem(QStandardItem *item, const Node *node)
{
    auto find = [node](QStandardItem *item) {
        WrapperItem *witem = static_cast<WrapperItem *>(item);
        return witem ? witem->itemNode == node : false;
    };

    if (item) {
        if (find(item))
            return item;

        for (int i = 0; i < item->rowCount(); ++i) {
            if (auto found = findChildItem(item->child(i), node))
                return found;
        }
    }

    return nullptr;
}

UnitTestModel::UnitTestModel(QObject *parent)
    : QStandardItemModel(parent),
      d(new UnitTestModelPrivate())
{
}

UnitTestModel::~UnitTestModel()
{
    delete d;
}

void UnitTestModel::clear()
{
    while (d->rootItem->hasChildren()) {
        d->rootItem->removeRow(0);
    }
}

QVariant UnitTestModel::data(const QModelIndex &index, int role) const
{
    const Node *const node = nodeForIndex(index);
    if (!node)
        return {};

    switch (role) {
    case Qt::DisplayRole:
        return node->displayName();
    case Qt::ToolTipRole:
        return node->tooltip();
    case Qt::DecorationRole:
        return node->icon();
    case FilePathRole:
        return node->filePath();
    case StateRole: {
        const FileNode *fileNode = node->asFileNode();
        if (fileNode)
            return fileNode->state();
    } break;
    default:
        break;
    }

    return {};
}

int UnitTestModel::rowCount(const QModelIndex &index) const
{
    if (!index.isValid())
        return d->rootItem->rowCount();
    const auto *item = itemFromIndex(index);
    return item ? item->rowCount() : 0;
}

int UnitTestModel::columnCount(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return 1;
}

Node *UnitTestModel::nodeForIndex(const QModelIndex &index) const
{
    WrapperItem *item = static_cast<WrapperItem *>(itemFromIndex(index));
    return item ? item->itemNode : nullptr;
}

WrapperItem *UnitTestModel::wrapperForNode(const Node *node) const
{
    auto item = d->findChildItem(d->rootItem, node);
    return static_cast<WrapperItem *>(item);
}

QModelIndex UnitTestModel::indexForNode(const Node *node) const
{
    WrapperItem *item = wrapperForNode(node);
    return item ? indexFromItem(item) : QModelIndex();
}
