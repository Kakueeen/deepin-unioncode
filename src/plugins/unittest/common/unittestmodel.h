// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef UNITTESTMODEL_H
#define UNITTESTMODEL_H

#include "unittestnode.h"

#include <QAbstractItemModel>

class WrapperItem : public QStandardItem
{
public:
    explicit WrapperItem(Node *node)
        : itemNode(node) {}
    Node *itemNode { nullptr };
};

class UnitTestModelPrivate;
class UnitTestModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    enum ItemRole {
        FilePathRole = Qt::UserRole + 1,
        StateRole
    };

    explicit UnitTestModel(QObject *parent = nullptr);
    ~UnitTestModel();

    void setRootItem(WrapperItem *root);
    WrapperItem *rootItem() const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &index = QModelIndex()) const override;
    int columnCount(const QModelIndex &index) const override;
    Node *nodeForIndex(const QModelIndex &index) const;
    QModelIndex indexForNode(const Node *node) const;

private:
    UnitTestModelPrivate *const d;
};

#endif   // UNITTESTMODEL_H
