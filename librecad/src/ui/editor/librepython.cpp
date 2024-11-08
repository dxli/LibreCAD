#include "rs_python.h"
#include "librepython.h"

#include <QtWidgets>
#include <QFileDialog>
#include <QApplication>
#include <QMessageBox>
#include "qg_py_commandwidget.h"

#ifdef DEVELOPER

LibrePython::LibrePython(QWidget *parent, const QString& fileName)
    : Librepad(parent, fileName)
{
    showScriptToolBar();

    QDockWidget *dock = new QDockWidget(tr("LibrePython"), this);
    dock->setAllowedAreas(Qt::BottomDockWidgetArea|Qt::RightDockWidgetArea);
    commandWidget = new QG_Py_CommandWidget(this);
    dock->setWidget(commandWidget);
    addDockWidget(Qt::BottomDockWidgetArea, dock);
}

void LibrePython::run()
{
    save();
    RS_PYTHON->runFile(path());
}

void LibrePython::loadScript()
{
    QString selfilter = tr("Python Script (*.py)");
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Run file"),
        QApplication::applicationDirPath(),
        tr("Python files (*.py *.pyc);;Python Script (*.py);;Python compiled Script (*.pyc)" ),
        &selfilter
        );

    if (fileName.isEmpty())
        return;

    RS_PYTHON->runFile(fileName);
}

#endif // DEVELOPER
