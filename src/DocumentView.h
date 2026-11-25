#pragma once

#include <QSplitter>
#include <QString>

class QPlainTextEdit;
class QTextBrowser;

class DocumentView : public QSplitter
{
    Q_OBJECT

public:
    explicit DocumentView(const QString &path, QWidget *parent = nullptr);

    QString path() const;
    bool isModified() const;
    bool save();

signals:
    void modificationChanged(bool modified);

private:
    void updatePreview();

    QString filePath;
    bool markdown;
    QPlainTextEdit *editor;
    QTextBrowser *preview;
};
