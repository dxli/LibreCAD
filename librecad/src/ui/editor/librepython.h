#ifndef LIBREPYTHON_H
#define LIBREPYTHON_H

#include "texteditor.h"
#include "librepad.h"
#include <QWidget>

#ifdef DEVELOPER

class QG_Py_CommandWidget;

class LibrePython : public Librepad
{
    Q_OBJECT
public:
    LibrePython(QWidget *parent = nullptr, const QString& fileName="");

    void run();
    void loadScript();

private:
    QG_Py_CommandWidget* commandWidget {nullptr};
};

#endif // DEVELOPER

#endif // LIBREPYTHON_H
