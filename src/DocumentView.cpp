#include "DocumentView.h"

#include <QFile>
#include <QFileInfo>
#include <QFontDatabase>
#include <QPlainTextEdit>
#include <QSaveFile>
#include <QScrollBar>
#include <QTextBrowser>
#include <QTextDocument>

#ifdef SCRAWL_HAS_SYNTAX
#include <KSyntaxHighlighting/Definition>
#include <KSyntaxHighlighting/Repository>
#include <KSyntaxHighlighting/SyntaxHighlighter>
#include <KSyntaxHighlighting/Theme>
#endif

DocumentView::DocumentView(const QString &path, QWidget *parent)
    : QSplitter(Qt::Horizontal, parent)
    , filePath(path)
    , markdown(false)
    , editor(new QPlainTextEdit(this))
    , preview(new QTextBrowser(this))
{
    const QString suffix = QFileInfo(path).suffix().toLower();
    markdown = suffix == "md" || suffix == "markdown";

    editor->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    editor->setTabStopDistance(editor->fontMetrics().horizontalAdvance(' ') * 4);
    editor->setLineWrapMode(QPlainTextEdit::WidgetWidth);

    QFile file(path);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        editor->setPlainText(QString::fromUtf8(file.readAll()));
    }
    editor->document()->setModified(false);

#ifdef SCRAWL_HAS_SYNTAX
    static KSyntaxHighlighting::Repository repository;
    auto *highlighter = new KSyntaxHighlighting::SyntaxHighlighter(editor->document());
    highlighter->setDefinition(repository.definitionForFileName(path));
    highlighter->setTheme(repository.defaultTheme(KSyntaxHighlighting::Repository::DarkTheme));
#endif

    preview->setOpenExternalLinks(true);
    preview->setVisible(markdown);

    addWidget(editor);
    addWidget(preview);
    setStretchFactor(0, 1);
    setStretchFactor(1, 1);

    connect(editor->document(), &QTextDocument::modificationChanged, this, &DocumentView::modificationChanged);
    if (markdown) {
        connect(editor, &QPlainTextEdit::textChanged, this, &DocumentView::updatePreview);
        updatePreview();
    }
}

QString DocumentView::path() const
{
    return filePath;
}

bool DocumentView::isModified() const
{
    return editor->document()->isModified();
}

bool DocumentView::save()
{
    QSaveFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }
    file.write(editor->toPlainText().toUtf8());
    if (!file.commit()) {
        return false;
    }
    editor->document()->setModified(false);
    return true;
}

void DocumentView::updatePreview()
{
    const int scroll = preview->verticalScrollBar()->value();
    preview->setMarkdown(editor->toPlainText());
    preview->verticalScrollBar()->setValue(scroll);
}
