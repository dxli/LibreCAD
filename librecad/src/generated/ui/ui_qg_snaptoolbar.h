/********************************************************************************
** Form generated from reading UI file 'qg_snaptoolbar.ui'
**
** Created: Sat Feb 18 16:41:51 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QG_SNAPTOOLBAR_H
#define UI_QG_SNAPTOOLBAR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QG_SnapToolBar
{
public:
    QHBoxLayout *horizontalLayout;
    QToolButton *bEndpoint;
    QToolButton *bMiddle;
    QToolButton *bCenter;
    QToolButton *bIntersection;
    QToolButton *bOnEntity;
    QToolButton *bDist;
    QToolButton *bIntersectionManual;
    QToolButton *bGrid;
    QToolButton *bResHorizontal;
    QToolButton *bResVertical;
    QToolButton *bRelZero;
    QToolButton *bLockRelZero;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *QG_SnapToolBar)
    {
        if (QG_SnapToolBar->objectName().isEmpty())
            QG_SnapToolBar->setObjectName(QString::fromUtf8("QG_SnapToolBar"));
        QG_SnapToolBar->resize(375, 30);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QG_SnapToolBar->sizePolicy().hasHeightForWidth());
        QG_SnapToolBar->setSizePolicy(sizePolicy);
        QG_SnapToolBar->setMinimumSize(QSize(373, 24));
        horizontalLayout = new QHBoxLayout(QG_SnapToolBar);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(1, 0, 0, 0);
        bEndpoint = new QToolButton(QG_SnapToolBar);
        bEndpoint->setObjectName(QString::fromUtf8("bEndpoint"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/extui/snapendpoint.png"), QSize(), QIcon::Normal, QIcon::Off);
        bEndpoint->setIcon(icon);
        bEndpoint->setCheckable(true);

        horizontalLayout->addWidget(bEndpoint);

        bMiddle = new QToolButton(QG_SnapToolBar);
        bMiddle->setObjectName(QString::fromUtf8("bMiddle"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/extui/snapmiddle.png"), QSize(), QIcon::Normal, QIcon::Off);
        bMiddle->setIcon(icon1);
        bMiddle->setCheckable(true);

        horizontalLayout->addWidget(bMiddle);

        bCenter = new QToolButton(QG_SnapToolBar);
        bCenter->setObjectName(QString::fromUtf8("bCenter"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/extui/snapcenter.png"), QSize(), QIcon::Normal, QIcon::Off);
        bCenter->setIcon(icon2);
        bCenter->setCheckable(true);

        horizontalLayout->addWidget(bCenter);

        bIntersection = new QToolButton(QG_SnapToolBar);
        bIntersection->setObjectName(QString::fromUtf8("bIntersection"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/extui/snapintersection.png"), QSize(), QIcon::Normal, QIcon::Off);
        bIntersection->setIcon(icon3);
        bIntersection->setCheckable(true);

        horizontalLayout->addWidget(bIntersection);

        bOnEntity = new QToolButton(QG_SnapToolBar);
        bOnEntity->setObjectName(QString::fromUtf8("bOnEntity"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/extui/snaponentity.png"), QSize(), QIcon::Normal, QIcon::Off);
        bOnEntity->setIcon(icon4);
        bOnEntity->setCheckable(true);

        horizontalLayout->addWidget(bOnEntity);

        bDist = new QToolButton(QG_SnapToolBar);
        bDist->setObjectName(QString::fromUtf8("bDist"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/extui/snapdist.png"), QSize(), QIcon::Normal, QIcon::Off);
        bDist->setIcon(icon5);
        bDist->setCheckable(true);

        horizontalLayout->addWidget(bDist);

        bIntersectionManual = new QToolButton(QG_SnapToolBar);
        bIntersectionManual->setObjectName(QString::fromUtf8("bIntersectionManual"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/extui/snapintersectionm.png"), QSize(), QIcon::Normal, QIcon::Off);
        bIntersectionManual->setIcon(icon6);
        bIntersectionManual->setCheckable(true);

        horizontalLayout->addWidget(bIntersectionManual);

        bGrid = new QToolButton(QG_SnapToolBar);
        bGrid->setObjectName(QString::fromUtf8("bGrid"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/extui/snapgrid.png"), QSize(), QIcon::Normal, QIcon::Off);
        bGrid->setIcon(icon7);
        bGrid->setCheckable(true);

        horizontalLayout->addWidget(bGrid);

        bResHorizontal = new QToolButton(QG_SnapToolBar);
        bResHorizontal->setObjectName(QString::fromUtf8("bResHorizontal"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/extui/restricthorizontal.png"), QSize(), QIcon::Normal, QIcon::Off);
        bResHorizontal->setIcon(icon8);
        bResHorizontal->setCheckable(true);

        horizontalLayout->addWidget(bResHorizontal);

        bResVertical = new QToolButton(QG_SnapToolBar);
        bResVertical->setObjectName(QString::fromUtf8("bResVertical"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/extui/restrictvertical.png"), QSize(), QIcon::Normal, QIcon::Off);
        bResVertical->setIcon(icon9);
        bResVertical->setCheckable(true);

        horizontalLayout->addWidget(bResVertical);

        bRelZero = new QToolButton(QG_SnapToolBar);
        bRelZero->setObjectName(QString::fromUtf8("bRelZero"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/extui/relzeromove.png"), QSize(), QIcon::Normal, QIcon::Off);
        bRelZero->setIcon(icon10);
        bRelZero->setCheckable(false);

        horizontalLayout->addWidget(bRelZero);

        bLockRelZero = new QToolButton(QG_SnapToolBar);
        bLockRelZero->setObjectName(QString::fromUtf8("bLockRelZero"));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/extui/relzerolock.png"), QSize(), QIcon::Normal, QIcon::Off);
        bLockRelZero->setIcon(icon11);
        bLockRelZero->setCheckable(true);

        horizontalLayout->addWidget(bLockRelZero);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        retranslateUi(QG_SnapToolBar);
        QObject::connect(bGrid, SIGNAL(clicked()), QG_SnapToolBar, SLOT(snapGrid()));
        QObject::connect(bEndpoint, SIGNAL(clicked()), QG_SnapToolBar, SLOT(snapEndpoint()));
        QObject::connect(bOnEntity, SIGNAL(clicked()), QG_SnapToolBar, SLOT(snapOnEntity()));
        QObject::connect(bCenter, SIGNAL(clicked()), QG_SnapToolBar, SLOT(snapCenter()));
        QObject::connect(bMiddle, SIGNAL(clicked()), QG_SnapToolBar, SLOT(snapMiddle()));
        QObject::connect(bDist, SIGNAL(clicked()), QG_SnapToolBar, SLOT(snapDist()));
        QObject::connect(bIntersection, SIGNAL(clicked()), QG_SnapToolBar, SLOT(snapIntersection()));
        QObject::connect(bRelZero, SIGNAL(clicked()), QG_SnapToolBar, SLOT(setRelativeZero()));
        QObject::connect(bLockRelZero, SIGNAL(toggled(bool)), QG_SnapToolBar, SLOT(lockRelativeZero(bool)));
        QObject::connect(bResHorizontal, SIGNAL(clicked()), QG_SnapToolBar, SLOT(restrictHorizontal()));
        QObject::connect(bResVertical, SIGNAL(clicked()), QG_SnapToolBar, SLOT(restrictVertical()));
        QObject::connect(bIntersectionManual, SIGNAL(clicked()), QG_SnapToolBar, SLOT(snapIntersectionManual()));

        QMetaObject::connectSlotsByName(QG_SnapToolBar);
    } // setupUi

    void retranslateUi(QWidget *QG_SnapToolBar)
    {
        QG_SnapToolBar->setWindowTitle(QApplication::translate("QG_SnapToolBar", "Snap", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        bEndpoint->setToolTip(QApplication::translate("QG_SnapToolBar", "Snap to Endpoints", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bEndpoint->setText(QString());
#ifndef QT_NO_TOOLTIP
        bMiddle->setToolTip(QApplication::translate("QG_SnapToolBar", "Snap to equidistant points of entity", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bMiddle->setText(QString());
#ifndef QT_NO_TOOLTIP
        bCenter->setToolTip(QApplication::translate("QG_SnapToolBar", "Snap to center points", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bCenter->setText(QString());
#ifndef QT_NO_TOOLTIP
        bIntersection->setToolTip(QApplication::translate("QG_SnapToolBar", "Snap to intersections automatically", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bIntersection->setText(QString());
#ifndef QT_NO_TOOLTIP
        bOnEntity->setToolTip(QApplication::translate("QG_SnapToolBar", "Snap to closest point on entity", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bOnEntity->setText(QString());
#ifndef QT_NO_TOOLTIP
        bDist->setToolTip(QApplication::translate("QG_SnapToolBar", "Snap to point with given distance to endpoint", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bDist->setText(QString());
#ifndef QT_NO_TOOLTIP
        bIntersectionManual->setToolTip(QApplication::translate("QG_SnapToolBar", "Snap to intersections manually", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bIntersectionManual->setText(QString());
#ifndef QT_NO_TOOLTIP
        bGrid->setToolTip(QApplication::translate("QG_SnapToolBar", "Snap to grid", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bGrid->setText(QString());
#ifndef QT_NO_TOOLTIP
        bResHorizontal->setToolTip(QApplication::translate("QG_SnapToolBar", "Restrict Horizontally", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bResHorizontal->setText(QString());
#ifndef QT_NO_TOOLTIP
        bResVertical->setToolTip(QApplication::translate("QG_SnapToolBar", "Restrict Vertically", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bResVertical->setText(QString());
#ifndef QT_NO_TOOLTIP
        bRelZero->setToolTip(QApplication::translate("QG_SnapToolBar", "Move relative Zero", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bRelZero->setText(QString());
#ifndef QT_NO_TOOLTIP
        bLockRelZero->setToolTip(QApplication::translate("QG_SnapToolBar", "Lock relative Zero", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bLockRelZero->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class QG_SnapToolBar: public Ui_QG_SnapToolBar {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QG_SNAPTOOLBAR_H
