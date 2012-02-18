/********************************************************************************
** Form generated from reading UI file 'qg_cadtoolbar.ui'
**
** Created: Sat Feb 18 16:41:50 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QG_CADTOOLBAR_H
#define UI_QG_CADTOOLBAR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QG_CadToolBar
{
public:

    void setupUi(QWidget *QG_CadToolBar)
    {
        if (QG_CadToolBar->objectName().isEmpty())
            QG_CadToolBar->setObjectName(QString::fromUtf8("QG_CadToolBar"));
        QG_CadToolBar->resize(66, 400);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QG_CadToolBar->sizePolicy().hasHeightForWidth());
        QG_CadToolBar->setSizePolicy(sizePolicy);
        QG_CadToolBar->setMinimumSize(QSize(56, 448));

        retranslateUi(QG_CadToolBar);

        QMetaObject::connectSlotsByName(QG_CadToolBar);
    } // setupUi

    void retranslateUi(QWidget *QG_CadToolBar)
    {
        QG_CadToolBar->setWindowTitle(QApplication::translate("QG_CadToolBar", "CAD Tools", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QG_CadToolBar: public Ui_QG_CadToolBar {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QG_CADTOOLBAR_H
