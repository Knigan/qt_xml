#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {

    QMenu* FileMenu = new QMenu(tr("File"));
    QAction* OpenAction = new QAction(tr("Open"), this);
    QAction* CloseAllAction = new QAction(tr("Close all"), this);
    QAction* ExitAction = new QAction(tr("Exit"), this);

    menuBar()->addMenu(FileMenu);
    FileMenu->addAction(OpenAction);
    FileMenu->addAction(CloseAllAction);
    FileMenu->addAction(ExitAction);

    connect(OpenAction, &QAction::triggered, this, &MainWindow::OpenAction);
    connect(CloseAllAction, &QAction::triggered, this, &MainWindow::CloseAllAction);
    connect(ExitAction, &QAction::triggered, this, &QMainWindow::close);

    XMLParser = new Parser;
    Tree = new QTreeView;
    Tree->setModel(XMLParser);
    Tree->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(Tree, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(ActiveAction(QPoint)));
    setCentralWidget(Tree);
}

MainWindow::~MainWindow() {
    delete XMLParser;
    delete Tree;
}

void MainWindow::ActiveAction(const QPoint& pos) noexcept {
    if (Tree->currentIndex().parent() == QModelIndex()) {
        QMenu* active_menu = new QMenu(this);
        QAction* active = new QAction(tr("Active"), this);
        active_menu->addAction(active);
        connect(active, &QAction::triggered, this, [this]() {
            QModelIndex currentIndex = Tree->currentIndex();
            QFont font;
            font.setBold(true);
            XMLParser->setData(currentIndex, font, Qt::FontRole);
        });
        active_menu->popup(Tree->viewport()->mapToGlobal(pos));
    }
}

void MainWindow::OpenAction() noexcept {
    QString filename = QFileDialog::getOpenFileName(this, tr("Selection XML file"), QDir::homePath(), tr("*.xml"));
    XMLParser->load_file(filename);
        Tree->reset();
    QFont font;
    font.setBold(true);
    XMLParser->setData(XMLParser->index(0, 0), font, Qt::FontRole);
}

void MainWindow::CloseAllAction() noexcept {
    XMLParser->delete_rows();
    Tree->reset();
}
