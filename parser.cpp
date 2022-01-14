#include "parser.h"

Parser::Parser(QObject* parent) : QAbstractItemModel(parent), Root(new Tree({tr("")})), reader(new QXmlStreamReader()), PMI(), font() {}

Parser::~Parser() {
    delete Root;
    delete reader;
}

QVariant Parser::data(const QModelIndex& index, int role) const {
    if (index.isValid()) {
        if (role == Qt::FontRole && index == PMI) {
            return font;
        }

        if (role != Qt::DisplayRole) {
            return QVariant();
        }

        Tree *item = static_cast<Tree*>(index.internalPointer());

        return item->get_data().at(index.column());
    } else
        return QVariant();
}

bool Parser::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (index.isValid()) {
        Tree *item = static_cast<Tree*>(index.internalPointer());
        if (role == Qt::EditRole) {
            item->get_data()[index.column()] = value;
            return true;
        }

        if (role == Qt::FontRole) {
            PMI = index;
            font = value.value<QFont>();
            return true;
        }
    }

    return false;
}

Qt::ItemFlags Parser::flags(const QModelIndex& index) const {
    return (index.isValid()) ? QAbstractItemModel::flags(index) : Qt::NoItemFlags;
}

QVariant Parser::headerData(int section, Qt::Orientation orientation, int role) const {
    return (orientation == Qt::Horizontal && role == Qt::DisplayRole) ? Root->get_data().at(section) : QVariant();
}

QModelIndex Parser::index(int row, int column, const QModelIndex& parent) const {
    Tree* parentItem;
    if (!parent.isValid()) {
        parentItem = Root;
    } else {
        parentItem = static_cast<Tree*>(parent.internalPointer());
    }

    Tree *childItem;

    if (row < 0 || row >= parentItem->get_child().size()) {
        childItem = nullptr;
    } else {
        childItem = parentItem->get_child().at(row);
    }

    if (childItem) {
        return createIndex(row, column, childItem);
    }

    return QModelIndex();
}

QModelIndex Parser::parent(const QModelIndex& child_index) const {
    Tree* child = static_cast<Tree*>(child_index.internalPointer());
    Tree* parent = child->get_parent();
    return (parent != Root && child != Root) ? createIndex(parent->parent_of_child(), 0, parent) : QModelIndex();
}


int Parser::rowCount(const QModelIndex& parent) const {
    Tree* Parent;
    if (parent.column() <= 0) {
        if (!parent.isValid()) {
            Parent = Root;
        } else {
            Parent = static_cast<Tree*>(parent.internalPointer());
        }

        return Parent->get_child().size();
    } else
        return 0;
}

int Parser::columnCount(const QModelIndex& parent) const {
    return Root->get_data().size();
}

bool Parser::removeRows(int row, int count, const QModelIndex &parent) {
    Tree* node;
    QModelIndex Parent;
    if (parent.isValid()) {
        node = static_cast<Tree*>(parent.internalPointer());
        Parent = parent;
    } else {
        node = Root;
        Parent = QModelIndex();
    }
    beginRemoveRows(Parent, row, row + count - 1);
    node->delete_rows(row, row + count);
    endRemoveRows();

    return true;
}

void Parser::load_file(const QString& filepath) const noexcept {
    QFile file(filepath);
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QStack<int> Crab;
    QStack<int> Lime;

    reader->setDevice(&file);
    QString filename = filepath.section("/", -1, -1);
    Tree* node = new Tree{{filename}, Root};
    Root->set_child(node);
    Tree* cur = node;

    int curLime = 0;
    while (!reader->atEnd() && !reader->hasError()) {
        QXmlStreamReader::TokenType token = reader->readNext();

        if (token == QXmlStreamReader::StartElement) {
            ++curLime;

            QString text = reader->name().toString();

                if (Lime.isEmpty() || curLime != Lime.top()) {
                    Crab.append(0);
                    Lime.append(curLime);
                }

            Tree* node = new Tree({text}, cur);
            cur->set_child(node);
            cur = node;
        }

        if (token == QXmlStreamReader::Characters) {
            Tree* node = new Tree({reader->text().toString()}, cur);
            cur->set_child(node);
        }

        if (token == QXmlStreamReader::EndElement) {
            --curLime;
            cur = cur->get_parent();
            QString text = reader->name().toString();
            if (text == "array" && curLime + 1 != Lime.top()) {
                Crab.pop();
                Lime.pop();
            }
        }
    }

    if (reader->hasError()) {
        QMessageBox message(QMessageBox::Critical,
                            tr("Error Message"),
                            tr("Parsing has failed!"),
                            QMessageBox::Ok);
        message.exec();
        Root->delete_child();
    }

    file.close();
}

void Parser::delete_rows() noexcept {
    removeRows(0, Root->get_child().size(), createIndex(-1, -1, nullptr));
}
