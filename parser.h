#ifndef XMLMODEL_H
#define XMLMODEL_H

#include <QAbstractItemModel>
#include <QFile>
#include <QXmlStreamReader>
#include <QMessageBox>
#include <QModelIndex>
#include <QStack>
#include <QPersistentModelIndex>
#include <QErrorMessage>
#include "tree.h"

class Parser : public QAbstractItemModel {

    Q_OBJECT

public:
    explicit Parser(QObject *parent = nullptr);
    ~Parser();

    QVariant data(const QModelIndex& index, int) const override;
    bool setData(const QModelIndex&, const QVariant&, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex&) const override;
    QVariant headerData(int, Qt::Orientation, int role = Qt::DisplayRole) const override;
    QModelIndex index(int, int, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex&) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    bool removeRows(int, int, const QModelIndex &parent = QModelIndex()) override;

    void load_file(const QString&) const noexcept;
    void delete_rows() noexcept;

private:
    Tree* Root;
    QXmlStreamReader* reader;
    QPersistentModelIndex PMI;
    QFont font;
};

#endif // XMLMODEL_H
