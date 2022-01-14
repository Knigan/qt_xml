#include "tree.h"

Tree::Tree(const QVector<QVariant> &data, Tree *parent) : parent(parent), data(data) {}

Tree::~Tree() {
    for (Tree* node : child) {
        parent = nullptr;
        delete node;
    }
}

Tree *Tree::get_parent() const noexcept {
    return parent;
}

QVector<Tree*> Tree::get_child() const noexcept {
    return child;
}

void Tree::set_child(Tree* node) noexcept {
    child.append(node);
}

QVector<QVariant> Tree::get_data() const noexcept {
    return data;
}

void Tree::delete_child() noexcept {
    child.pop_back();
}

int Tree::parent_of_child() const noexcept {
    return (parent) ? parent->child.indexOf(this) : 0;
}

void Tree::delete_rows(int begin, int end) noexcept {
    for (int i = begin; i < end; ++i) {
        delete child[begin];
        child.remove(begin);
    }
}
