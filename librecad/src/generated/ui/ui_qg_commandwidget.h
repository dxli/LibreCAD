/********************************************************************************
** Form generated from reading UI file 'qg_commandwidget.ui'
**
** Created: Sat Feb 18 16:41:50 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QG_COMMANDWIDGET_H
#define UI_QG_COMMANDWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "qg_commandedit.h"

QT_BEGIN_NAMESPACE

class Ui_QG_CommandWidget
{
public:
    QVBoxLayout *vboxLayout;
    QTextEdit *teHistory;
    QFrame *line1;
    QHBoxLayout *hboxLayout;
    QLabel *lCommand;
    QG_CommandEdit *leCommand;

    void setupUi(QWidget *QG_CommandWidget)
    {
        if (QG_CommandWidget->objectName().isEmpty())
            QG_CommandWidget->setObjectName(QString::fromUtf8("QG_CommandWidget"));
        QG_CommandWidget->resize(639, 102);
        vboxLayout = new QVBoxLayout(QG_CommandWidget);
        vboxLayout->setSpacing(0);
        vboxLayout->setContentsMargins(0, 0, 0, 0);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        teHistory = new QTextEdit(QG_CommandWidget);
        teHistory->setObjectName(QString::fromUtf8("teHistory"));
        teHistory->setMinimumSize(QSize(0, 23));
        teHistory->setFocusPolicy(Qt::NoFocus);
        teHistory->setLineWidth(0);
        teHistory->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        teHistory->setDocumentTitle(QString::fromUtf8(""));
        teHistory->setUndoRedoEnabled(false);
        teHistory->setReadOnly(true);
        teHistory->setHtml(QString::fromUtf8("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans'; font-size:10pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p></body></html>"));
        teHistory->setAcceptRichText(false);
        teHistory->setProperty("linkUnderline", QVariant(false));

        vboxLayout->addWidget(teHistory);

        line1 = new QFrame(QG_CommandWidget);
        line1->setObjectName(QString::fromUtf8("line1"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(line1->sizePolicy().hasHeightForWidth());
        line1->setSizePolicy(sizePolicy);
        line1->setFrameShape(QFrame::HLine);
        line1->setFrameShadow(QFrame::Sunken);

        vboxLayout->addWidget(line1);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        lCommand = new QLabel(QG_CommandWidget);
        lCommand->setObjectName(QString::fromUtf8("lCommand"));
        lCommand->setWordWrap(false);

        hboxLayout->addWidget(lCommand);

        leCommand = new QG_CommandEdit(QG_CommandWidget);
        leCommand->setObjectName(QString::fromUtf8("leCommand"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(leCommand->sizePolicy().hasHeightForWidth());
        leCommand->setSizePolicy(sizePolicy1);

        hboxLayout->addWidget(leCommand);


        vboxLayout->addLayout(hboxLayout);


        retranslateUi(QG_CommandWidget);
        QObject::connect(leCommand, SIGNAL(returnPressed()), QG_CommandWidget, SLOT(trigger()));
        QObject::connect(leCommand, SIGNAL(tabPressed()), QG_CommandWidget, SLOT(tabPressed()));
        QObject::connect(leCommand, SIGNAL(escape()), QG_CommandWidget, SLOT(escape()));
        QObject::connect(leCommand, SIGNAL(focusIn()), QG_CommandWidget, SLOT(setCommandMode()));
        QObject::connect(leCommand, SIGNAL(focusOut()), QG_CommandWidget, SLOT(setNormalMode()));

        QMetaObject::connectSlotsByName(QG_CommandWidget);
    } // setupUi

    void retranslateUi(QWidget *QG_CommandWidget)
    {
        QG_CommandWidget->setWindowTitle(QApplication::translate("QG_CommandWidget", "Command Line", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        teHistory->setToolTip(QApplication::translate("QG_CommandWidget", "Command history and output", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        lCommand->setText(QApplication::translate("QG_CommandWidget", "Command:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QG_CommandWidget: public Ui_QG_CommandWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QG_COMMANDWIDGET_H
