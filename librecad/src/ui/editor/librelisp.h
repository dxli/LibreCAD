#ifndef LIBRELISP_H
#define LIBRELISP_H

#include "texteditor.h"
#include "librepad.h"
#include <QWidget>

#ifdef DEVELOPER

class QG_Lsp_CommandWidget;

class LibreLisp : public Librepad
{
    Q_OBJECT
public:
    LibreLisp(QWidget *parent = nullptr, const QString& fileName="");

    void run();
    void loadScript();

private:
    QG_Lsp_CommandWidget* commandWidget {nullptr};
};

#endif // DEVELOPER

#endif // LIBRELISP_H
