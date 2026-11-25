#include <QApplication>
#include <QIcon>
#include <QPalette>
#include <QStyleFactory>

#include "MainWindow.h"

static void applyDarkTheme(QApplication &app)
{
    app.setStyle(QStyleFactory::create("Fusion"));
    QPalette p;
    p.setColor(QPalette::Window, QColor(24, 24, 27));
    p.setColor(QPalette::WindowText, QColor(228, 228, 231));
    p.setColor(QPalette::Base, QColor(18, 18, 20));
    p.setColor(QPalette::AlternateBase, QColor(30, 30, 34));
    p.setColor(QPalette::Text, QColor(228, 228, 231));
    p.setColor(QPalette::Button, QColor(39, 39, 42));
    p.setColor(QPalette::ButtonText, QColor(228, 228, 231));
    p.setColor(QPalette::Highlight, QColor(59, 130, 246));
    p.setColor(QPalette::HighlightedText, Qt::white);
    p.setColor(QPalette::ToolTipBase, QColor(39, 39, 42));
    p.setColor(QPalette::ToolTipText, QColor(228, 228, 231));
    app.setPalette(p);
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("Scrawl");

    QIcon icon;
    for (const int size : {16, 24, 32, 48, 64, 128, 256, 512}) {
        icon.addFile(QString(":/icons/scrawl-%1.png").arg(size), QSize(size, size));
    }
    QApplication::setWindowIcon(icon);

    applyDarkTheme(app);

    MainWindow window;
    window.resize(1280, 800);
    window.show();

    return app.exec();
}
