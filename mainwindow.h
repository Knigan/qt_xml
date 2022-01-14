#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QTreeView>
#include <QAction>

#include "parser.h"

class MainWindow : public QMainWindow
{

    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void OpenAction() noexcept;
    void CloseAllAction() noexcept;
    void ActiveAction(const QPoint&) noexcept;

private:
    Parser* XMLParser;
    QTreeView* Tree;
};

#endif // MAINWINDOW_H
