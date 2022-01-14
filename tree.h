#ifndef TREE_H
#define TREE_H

#include <QVariant>
#include <QVector>

class Tree {

public:
    explicit Tree(const QVector<QVariant>& data, Tree* parentItem = nullptr);
    ~Tree();

    Tree* get_parent() const noexcept;
    QVector<Tree*> get_child() const noexcept;
    void set_child(Tree*) noexcept;
    QVector<QVariant> get_data() const noexcept;

    void delete_child() noexcept;
    int parent_of_child() const noexcept;
    void delete_rows(int, int) noexcept;

private:
    Tree* parent;
    QVector<Tree*> child;
    QVector<QVariant> data;
};

#endif // TREE_H
