/********************************************************************************
** Form generated from reading UI file 'qg_cadtoolbarpoints.ui'
**
** Created: Sat Feb 18 16:41:50 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QG_CADTOOLBARPOINTS_H
#define UI_QG_CADTOOLBARPOINTS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QG_CadToolBarPoints
{
public:
    QToolButton *bBack;
    QToolButton *bPoint;

    void setupUi(QWidget *QG_CadToolBarPoints)
    {
        if (QG_CadToolBarPoints->objectName().isEmpty())
            QG_CadToolBarPoints->setObjectName(QString::fromUtf8("QG_CadToolBarPoints"));
        QG_CadToolBarPoints->resize(56, 448);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QG_CadToolBarPoints->sizePolicy().hasHeightForWidth());
        QG_CadToolBarPoints->setSizePolicy(sizePolicy);
        QG_CadToolBarPoints->setMinimumSize(QSize(56, 448));
        bBack = new QToolButton(QG_CadToolBarPoints);
        bBack->setObjectName(QString::fromUtf8("bBack"));
        bBack->setGeometry(QRect(0, 0, 56, 20));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/extui/back.png"), QSize(), QIcon::Normal, QIcon::Off);
        bBack->setIcon(icon);
        bPoint = new QToolButton(QG_CadToolBarPoints);
        bPoint->setObjectName(QString::fromUtf8("bPoint"));
        bPoint->setGeometry(QRect(0, 20, 28, 28));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/extui/points.png"), QSize(), QIcon::Normal, QIcon::Off);
        bPoint->setIcon(icon1);

        retranslateUi(QG_CadToolBarPoints);
        QObject::connect(bPoint, SIGNAL(clicked()), QG_CadToolBarPoints, SLOT(drawPoint()));
        QObject::connect(bBack, SIGNAL(clicked()), QG_CadToolBarPoints, SLOT(back()));

        QMetaObject::connectSlotsByName(QG_CadToolBarPoints);
    } // setupUi

    void retranslateUi(QWidget *QG_CadToolBarPoints)
    {
        QG_CadToolBarPoints->setWindowTitle(QApplication::translate("QG_CadToolBarPoints", "Points", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        bBack->setToolTip(QApplication::translate("QG_CadToolBarPoints", "Back to main menu", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bBack->setText(QString());
#ifndef QT_NO_TOOLTIP
        bPoint->setToolTip(QApplication::translate("QG_CadToolBarPoints", "Single points", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bPoint->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class QG_CadToolBarPoints: public Ui_QG_CadToolBarPoints {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QG_CADTOOLBARPOINTS_H
