#pragma once

#include <QMainWindow>

class QFileSystemModel;
class QTabWidget;
class QTreeView;
class DocumentView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    void openFolder();
    void openFile(const QString &path);
    void saveCurrent();
    void closeTab(int index);
    void setRoot(const QString &path);
    void refreshTabTitle(DocumentView *view);

    QFileSystemModel *model;
    QTreeView *tree;
    QTabWidget *tabs;
};
