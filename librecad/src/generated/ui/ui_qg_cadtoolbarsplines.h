/********************************************************************************
** Form generated from reading UI file 'qg_cadtoolbarsplines.ui'
**
** Created: Sat Feb 18 16:41:50 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QG_CADTOOLBARSPLINES_H
#define UI_QG_CADTOOLBARSPLINES_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QG_CadToolBarSplines
{
public:
    QToolButton *bBack;
    QToolButton *bSpline;

    void setupUi(QWidget *QG_CadToolBarSplines)
    {
        if (QG_CadToolBarSplines->objectName().isEmpty())
            QG_CadToolBarSplines->setObjectName(QString::fromUtf8("QG_CadToolBarSplines"));
        QG_CadToolBarSplines->resize(65, 448);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QG_CadToolBarSplines->sizePolicy().hasHeightForWidth());
        QG_CadToolBarSplines->setSizePolicy(sizePolicy);
        QG_CadToolBarSplines->setMinimumSize(QSize(65, 448));
        bBack = new QToolButton(QG_CadToolBarSplines);
        bBack->setObjectName(QString::fromUtf8("bBack"));
        bBack->setGeometry(QRect(0, 0, 65, 20));
        bBack->setMinimumSize(QSize(65, 20));
        bBack->setMaximumSize(QSize(65, 20));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/extui/back.png"), QSize(), QIcon::Normal, QIcon::Off);
        bBack->setIcon(icon);
        bBack->setIconSize(QSize(65, 20));
        bSpline = new QToolButton(QG_CadToolBarSplines);
        bSpline->setObjectName(QString::fromUtf8("bSpline"));
        bSpline->setGeometry(QRect(0, 20, 32, 32));
        bSpline->setMinimumSize(QSize(32, 32));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/extui/splinesnurbs.png"), QSize(), QIcon::Normal, QIcon::Off);
        bSpline->setIcon(icon1);
        bSpline->setIconSize(QSize(32, 32));

        retranslateUi(QG_CadToolBarSplines);
        QObject::connect(bSpline, SIGNAL(clicked()), QG_CadToolBarSplines, SLOT(drawSpline()));
        QObject::connect(bBack, SIGNAL(clicked()), QG_CadToolBarSplines, SLOT(back()));

        QMetaObject::connectSlotsByName(QG_CadToolBarSplines);
    } // setupUi

    void retranslateUi(QWidget *QG_CadToolBarSplines)
    {
        QG_CadToolBarSplines->setWindowTitle(QApplication::translate("QG_CadToolBarSplines", "Splines", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        bBack->setToolTip(QApplication::translate("QG_CadToolBarSplines", "Back to main menu", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bBack->setText(QString());
#ifndef QT_NO_TOOLTIP
        bSpline->setToolTip(QApplication::translate("QG_CadToolBarSplines", "Spline", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bSpline->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class QG_CadToolBarSplines: public Ui_QG_CadToolBarSplines {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QG_CADTOOLBARSPLINES_H
