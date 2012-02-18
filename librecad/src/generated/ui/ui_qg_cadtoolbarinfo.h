/********************************************************************************
** Form generated from reading UI file 'qg_cadtoolbarinfo.ui'
**
** Created: Sat Feb 18 16:41:50 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QG_CADTOOLBARINFO_H
#define UI_QG_CADTOOLBARINFO_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QG_CadToolBarInfo
{
public:
    QGridLayout *gridLayout;
    QToolButton *bBack;
    QToolButton *bDist;
    QToolButton *bDist2;
    QToolButton *bAngle;
    QToolButton *bTotalLength;
    QToolButton *bArea;
    QSpacerItem *verticalSpacer;
    QToolButton *bHidden;

    void setupUi(QWidget *QG_CadToolBarInfo)
    {
        if (QG_CadToolBarInfo->objectName().isEmpty())
            QG_CadToolBarInfo->setObjectName(QString::fromUtf8("QG_CadToolBarInfo"));
        QG_CadToolBarInfo->resize(65, 336);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QG_CadToolBarInfo->sizePolicy().hasHeightForWidth());
        QG_CadToolBarInfo->setSizePolicy(sizePolicy);
        QG_CadToolBarInfo->setMinimumSize(QSize(65, 336));
        gridLayout = new QGridLayout(QG_CadToolBarInfo);
        gridLayout->setSpacing(1);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        bBack = new QToolButton(QG_CadToolBarInfo);
        bBack->setObjectName(QString::fromUtf8("bBack"));
        bBack->setMaximumSize(QSize(16777215, 20));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/extui/back.png"), QSize(), QIcon::Normal, QIcon::Off);
        bBack->setIcon(icon);
        bBack->setIconSize(QSize(65, 20));

        gridLayout->addWidget(bBack, 0, 0, 1, 2);

        bDist = new QToolButton(QG_CadToolBarInfo);
        bDist->setObjectName(QString::fromUtf8("bDist"));
        bDist->setMaximumSize(QSize(32, 32));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/extui/infodist.png"), QSize(), QIcon::Normal, QIcon::Off);
        bDist->setIcon(icon1);
        bDist->setIconSize(QSize(32, 32));
        bDist->setCheckable(true);
        bDist->setAutoExclusive(true);

        gridLayout->addWidget(bDist, 1, 0, 1, 1);

        bDist2 = new QToolButton(QG_CadToolBarInfo);
        bDist2->setObjectName(QString::fromUtf8("bDist2"));
        bDist2->setMaximumSize(QSize(32, 32));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/extui/infodist2.png"), QSize(), QIcon::Normal, QIcon::Off);
        bDist2->setIcon(icon2);
        bDist2->setIconSize(QSize(32, 32));
        bDist2->setCheckable(true);
        bDist2->setAutoExclusive(true);

        gridLayout->addWidget(bDist2, 1, 1, 1, 1);

        bAngle = new QToolButton(QG_CadToolBarInfo);
        bAngle->setObjectName(QString::fromUtf8("bAngle"));
        bAngle->setMaximumSize(QSize(32, 32));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/extui/infoangle.png"), QSize(), QIcon::Normal, QIcon::Off);
        bAngle->setIcon(icon3);
        bAngle->setIconSize(QSize(32, 32));
        bAngle->setCheckable(true);
        bAngle->setAutoExclusive(true);

        gridLayout->addWidget(bAngle, 2, 0, 1, 1);

        bTotalLength = new QToolButton(QG_CadToolBarInfo);
        bTotalLength->setObjectName(QString::fromUtf8("bTotalLength"));
        bTotalLength->setMaximumSize(QSize(32, 32));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/extui/infototallength.png"), QSize(), QIcon::Normal, QIcon::Off);
        bTotalLength->setIcon(icon4);
        bTotalLength->setIconSize(QSize(32, 32));
        bTotalLength->setCheckable(true);
        bTotalLength->setAutoExclusive(true);

        gridLayout->addWidget(bTotalLength, 2, 1, 1, 1);

        bArea = new QToolButton(QG_CadToolBarInfo);
        bArea->setObjectName(QString::fromUtf8("bArea"));
        bArea->setMaximumSize(QSize(32, 32));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/extui/infoarea.png"), QSize(), QIcon::Normal, QIcon::Off);
        bArea->setIcon(icon5);
        bArea->setIconSize(QSize(32, 32));
        bArea->setCheckable(true);
        bArea->setAutoExclusive(true);

        gridLayout->addWidget(bArea, 3, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 4, 0, 1, 1);

        bHidden = new QToolButton(QG_CadToolBarInfo);
        bHidden->setObjectName(QString::fromUtf8("bHidden"));
        bHidden->setMaximumSize(QSize(0, 0));
        bHidden->setText(QString::fromUtf8(""));
        bHidden->setIconSize(QSize(0, 0));
        bHidden->setCheckable(true);
        bHidden->setAutoExclusive(true);

        gridLayout->addWidget(bHidden, 5, 0, 1, 1);


        retranslateUi(QG_CadToolBarInfo);
        QObject::connect(bDist, SIGNAL(clicked()), QG_CadToolBarInfo, SLOT(infoDist()));
        QObject::connect(bDist2, SIGNAL(clicked()), QG_CadToolBarInfo, SLOT(infoDist2()));
        QObject::connect(bAngle, SIGNAL(clicked()), QG_CadToolBarInfo, SLOT(infoAngle()));
        QObject::connect(bTotalLength, SIGNAL(clicked()), QG_CadToolBarInfo, SLOT(infoTotalLength()));
        QObject::connect(bArea, SIGNAL(clicked()), QG_CadToolBarInfo, SLOT(infoArea()));

        QMetaObject::connectSlotsByName(QG_CadToolBarInfo);
    } // setupUi

    void retranslateUi(QWidget *QG_CadToolBarInfo)
    {
        QG_CadToolBarInfo->setWindowTitle(QApplication::translate("QG_CadToolBarInfo", "Info", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        bBack->setToolTip(QApplication::translate("QG_CadToolBarInfo", "Back to main menu", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bBack->setText(QString());
#ifndef QT_NO_TOOLTIP
        bDist->setToolTip(QApplication::translate("QG_CadToolBarInfo", "Distance (Point, Point)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bDist->setText(QString());
#ifndef QT_NO_TOOLTIP
        bDist2->setToolTip(QApplication::translate("QG_CadToolBarInfo", "Distance (Entity, Point)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bDist2->setText(QString());
#ifndef QT_NO_TOOLTIP
        bAngle->setToolTip(QApplication::translate("QG_CadToolBarInfo", "Angle", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bAngle->setText(QString());
#ifndef QT_NO_TOOLTIP
        bTotalLength->setToolTip(QApplication::translate("QG_CadToolBarInfo", "Total length of selected entities", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bTotalLength->setText(QString());
#ifndef QT_NO_TOOLTIP
        bArea->setToolTip(QApplication::translate("QG_CadToolBarInfo", "Area of polygon", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bArea->setText(QString());
#ifndef QT_NO_TOOLTIP
        bHidden->setToolTip(QApplication::translate("QG_CadToolBarInfo", "Concentric", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
    } // retranslateUi

};

namespace Ui {
    class QG_CadToolBarInfo: public Ui_QG_CadToolBarInfo {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QG_CADTOOLBARINFO_H
