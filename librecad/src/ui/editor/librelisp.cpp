#include "rs_lisp.h"
#include "librelisp.h"

#include <QtWidgets>
#include <QFileDialog>
#include <QApplication>
#include <QMessageBox>
#include "qg_lsp_commandwidget.h"

#ifdef DEVELOPER

LibreLisp::LibreLisp(QWidget *parent, const QString& fileName)
    : Librepad(parent, fileName)
{
    showScriptToolBar();

    QDockWidget *dock = new QDockWidget(tr("LibreLisp"), this);
    dock->setAllowedAreas(Qt::BottomDockWidgetArea|Qt::RightDockWidgetArea);
    commandWidget = new QG_Lsp_CommandWidget(this);
    dock->setWidget(commandWidget);
    addDockWidget(Qt::BottomDockWidgetArea, dock);
}

void LibreLisp::run()
{
    save();
    RS_LISP->runFile(path());
}

void LibreLisp::loadScript()
{
    QString selfilter = tr("AutoLisp (*.lsp)");
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Run file"),
        QApplication::applicationDirPath(),
        tr("Lisp files (*.lsp *.lisp *.mal);;AutoLisp (*.lsp);;Mal (*.mal)" ),
        &selfilter
        );

    if (fileName.isEmpty())
        return;

    RS_LISP->runFile(fileName);
}

#endif // DEVELOPER
