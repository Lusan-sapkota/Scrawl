#include "MainWindow.h"

#include <QAction>
#include <QFileDialog>
#include <QFileInfo>
#include <QFileSystemModel>
#include <QKeySequence>
#include <QMenuBar>
#include <QMessageBox>
#include <QSplitter>
#include <QTabWidget>
#include <QTreeView>

#include "DocumentView.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , model(new QFileSystemModel(this))
    , tree(new QTreeView(this))
    , tabs(new QTabWidget(this))
{
    setWindowTitle("Scrawl");

    model->setReadOnly(true);
    tree->setModel(model);
    tree->setHeaderHidden(true);
    for (int column = 1; column < model->columnCount(); ++column) {
        tree->hideColumn(column);
    }

    tabs->setTabsClosable(true);
    tabs->setDocumentMode(true);

    auto *splitter = new QSplitter(Qt::Horizontal, this);
    splitter->addWidget(tree);
    splitter->addWidget(tabs);
    splitter->setStretchFactor(0, 0);
    splitter->setStretchFactor(1, 1);
    splitter->setSizes({260, 1020});
    setCentralWidget(splitter);

    auto *fileMenu = menuBar()->addMenu("&File");

    auto *openFolderAction = fileMenu->addAction("Open &Folder...");
    openFolderAction->setShortcut(QKeySequence("Ctrl+Shift+O"));
    connect(openFolderAction, &QAction::triggered, this, &MainWindow::openFolder);

    auto *saveAction = fileMenu->addAction("&Save");
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveCurrent);

    auto *closeAction = fileMenu->addAction("&Close Tab");
    closeAction->setShortcut(QKeySequence::Close);
    connect(closeAction, &QAction::triggered, this, [this] { closeTab(tabs->currentIndex()); });

    fileMenu->addSeparator();
    auto *quitAction = fileMenu->addAction("&Quit");
    quitAction->setShortcut(QKeySequence::Quit);
    connect(quitAction, &QAction::triggered, this, &QWidget::close);

    connect(tree, &QTreeView::doubleClicked, this, [this](const QModelIndex &index) {
        if (!model->isDir(index)) {
            openFile(model->filePath(index));
        }
    });
    connect(tabs, &QTabWidget::tabCloseRequested, this, &MainWindow::closeTab);
}

void MainWindow::openFolder()
{
    const QString dir = QFileDialog::getExistingDirectory(this, "Open Folder");
    if (!dir.isEmpty()) {
        setRoot(dir);
    }
}

void MainWindow::setRoot(const QString &path)
{
    tree->setRootIndex(model->setRootPath(path));
    setWindowTitle(QFileInfo(path).fileName() + " - Scrawl");
}

void MainWindow::openFile(const QString &path)
{
    for (int i = 0; i < tabs->count(); ++i) {
        auto *view = static_cast<DocumentView *>(tabs->widget(i));
        if (view->path() == path) {
            tabs->setCurrentIndex(i);
            return;
        }
    }

    auto *view = new DocumentView(path, this);
    const int index = tabs->addTab(view, QFileInfo(path).fileName());
    tabs->setTabToolTip(index, path);
    tabs->setCurrentIndex(index);
    connect(view, &DocumentView::modificationChanged, this, [this, view] { refreshTabTitle(view); });
}

void MainWindow::saveCurrent()
{
    auto *view = static_cast<DocumentView *>(tabs->currentWidget());
    if (view && !view->save()) {
        QMessageBox::warning(this, "Save failed", "Could not write " + view->path());
    }
}

void MainWindow::closeTab(int index)
{
    if (index < 0) {
        return;
    }
    auto *view = static_cast<DocumentView *>(tabs->widget(index));
    if (view->isModified()) {
        const auto answer = QMessageBox::question(
            this, "Unsaved changes", "Discard changes to " + QFileInfo(view->path()).fileName() + "?");
        if (answer != QMessageBox::Yes) {
            return;
        }
    }
    tabs->removeTab(index);
    view->deleteLater();
}

void MainWindow::refreshTabTitle(DocumentView *view)
{
    const int index = tabs->indexOf(view);
    if (index < 0) {
        return;
    }
    const QString name = QFileInfo(view->path()).fileName();
    tabs->setTabText(index, view->isModified() ? name + " *" : name);
}
