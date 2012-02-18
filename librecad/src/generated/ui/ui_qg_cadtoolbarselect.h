/********************************************************************************
** Form generated from reading UI file 'qg_cadtoolbarselect.ui'
**
** Created: Sat Feb 18 16:41:50 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QG_CADTOOLBARSELECT_H
#define UI_QG_CADTOOLBARSELECT_H

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

class Ui_QG_CadToolBarSelect
{
public:
    QGridLayout *gridLayout;
    QToolButton *bBack;
    QToolButton *bUnAll;
    QToolButton *bAll;
    QToolButton *bSingle;
    QToolButton *bContour;
    QToolButton *bUnWindow;
    QToolButton *bWindow;
    QToolButton *bUnInters;
    QToolButton *bInters;
    QToolButton *bInvert;
    QToolButton *bLayer;
    QToolButton *bDoit;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *QG_CadToolBarSelect)
    {
        if (QG_CadToolBarSelect->objectName().isEmpty())
            QG_CadToolBarSelect->setObjectName(QString::fromUtf8("QG_CadToolBarSelect"));
        QG_CadToolBarSelect->resize(65, 336);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QG_CadToolBarSelect->sizePolicy().hasHeightForWidth());
        QG_CadToolBarSelect->setSizePolicy(sizePolicy);
        QG_CadToolBarSelect->setMinimumSize(QSize(65, 336));
        gridLayout = new QGridLayout(QG_CadToolBarSelect);
        gridLayout->setSpacing(1);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        bBack = new QToolButton(QG_CadToolBarSelect);
        bBack->setObjectName(QString::fromUtf8("bBack"));
        bBack->setMaximumSize(QSize(16777215, 20));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/extui/back.png"), QSize(), QIcon::Normal, QIcon::Off);
        bBack->setIcon(icon);
        bBack->setIconSize(QSize(65, 20));

        gridLayout->addWidget(bBack, 0, 0, 1, 2);

        bUnAll = new QToolButton(QG_CadToolBarSelect);
        bUnAll->setObjectName(QString::fromUtf8("bUnAll"));
        bUnAll->setMaximumSize(QSize(32, 32));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/extui/selectnothing.png"), QSize(), QIcon::Normal, QIcon::Off);
        bUnAll->setIcon(icon1);
        bUnAll->setIconSize(QSize(32, 32));

        gridLayout->addWidget(bUnAll, 1, 0, 1, 1);

        bAll = new QToolButton(QG_CadToolBarSelect);
        bAll->setObjectName(QString::fromUtf8("bAll"));
        bAll->setMaximumSize(QSize(32, 32));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/extui/selectall.png"), QSize(), QIcon::Normal, QIcon::Off);
        bAll->setIcon(icon2);
        bAll->setIconSize(QSize(32, 32));

        gridLayout->addWidget(bAll, 1, 1, 1, 1);

        bSingle = new QToolButton(QG_CadToolBarSelect);
        bSingle->setObjectName(QString::fromUtf8("bSingle"));
        bSingle->setMaximumSize(QSize(32, 32));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/extui/selectsingle.png"), QSize(), QIcon::Normal, QIcon::Off);
        bSingle->setIcon(icon3);
        bSingle->setIconSize(QSize(32, 32));

        gridLayout->addWidget(bSingle, 2, 0, 1, 1);

        bContour = new QToolButton(QG_CadToolBarSelect);
        bContour->setObjectName(QString::fromUtf8("bContour"));
        bContour->setMaximumSize(QSize(32, 32));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/extui/selectcontour.png"), QSize(), QIcon::Normal, QIcon::Off);
        bContour->setIcon(icon4);
        bContour->setIconSize(QSize(32, 32));

        gridLayout->addWidget(bContour, 2, 1, 1, 1);

        bUnWindow = new QToolButton(QG_CadToolBarSelect);
        bUnWindow->setObjectName(QString::fromUtf8("bUnWindow"));
        bUnWindow->setMaximumSize(QSize(32, 32));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/extui/deselectwindow.png"), QSize(), QIcon::Normal, QIcon::Off);
        bUnWindow->setIcon(icon5);
        bUnWindow->setIconSize(QSize(32, 32));

        gridLayout->addWidget(bUnWindow, 3, 0, 1, 1);

        bWindow = new QToolButton(QG_CadToolBarSelect);
        bWindow->setObjectName(QString::fromUtf8("bWindow"));
        bWindow->setMaximumSize(QSize(32, 32));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/extui/selectwindow.png"), QSize(), QIcon::Normal, QIcon::Off);
        bWindow->setIcon(icon6);
        bWindow->setIconSize(QSize(32, 32));

        gridLayout->addWidget(bWindow, 3, 1, 1, 1);

        bUnInters = new QToolButton(QG_CadToolBarSelect);
        bUnInters->setObjectName(QString::fromUtf8("bUnInters"));
        bUnInters->setMaximumSize(QSize(32, 32));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/extui/deselectinters.png"), QSize(), QIcon::Normal, QIcon::Off);
        bUnInters->setIcon(icon7);
        bUnInters->setIconSize(QSize(32, 32));

        gridLayout->addWidget(bUnInters, 4, 0, 1, 1);

        bInters = new QToolButton(QG_CadToolBarSelect);
        bInters->setObjectName(QString::fromUtf8("bInters"));
        bInters->setMaximumSize(QSize(32, 32));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/extui/selectinters.png"), QSize(), QIcon::Normal, QIcon::Off);
        bInters->setIcon(icon8);
        bInters->setIconSize(QSize(32, 32));

        gridLayout->addWidget(bInters, 4, 1, 1, 1);

        bInvert = new QToolButton(QG_CadToolBarSelect);
        bInvert->setObjectName(QString::fromUtf8("bInvert"));
        bInvert->setMaximumSize(QSize(32, 32));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/extui/selectinvert.png"), QSize(), QIcon::Normal, QIcon::Off);
        bInvert->setIcon(icon9);
        bInvert->setIconSize(QSize(32, 32));

        gridLayout->addWidget(bInvert, 5, 0, 1, 1);

        bLayer = new QToolButton(QG_CadToolBarSelect);
        bLayer->setObjectName(QString::fromUtf8("bLayer"));
        bLayer->setMaximumSize(QSize(32, 32));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/extui/selectlayer.png"), QSize(), QIcon::Normal, QIcon::Off);
        bLayer->setIcon(icon10);
        bLayer->setIconSize(QSize(32, 32));

        gridLayout->addWidget(bLayer, 5, 1, 1, 1);

        bDoit = new QToolButton(QG_CadToolBarSelect);
        bDoit->setObjectName(QString::fromUtf8("bDoit"));
        bDoit->setMaximumSize(QSize(16777215, 20));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/extui/forward.png"), QSize(), QIcon::Normal, QIcon::Off);
        bDoit->setIcon(icon11);
        bDoit->setIconSize(QSize(65, 20));

        gridLayout->addWidget(bDoit, 6, 0, 1, 2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 7, 0, 1, 1);


        retranslateUi(QG_CadToolBarSelect);
        QObject::connect(bSingle, SIGNAL(clicked()), QG_CadToolBarSelect, SLOT(selectSingle()));
        QObject::connect(bDoit, SIGNAL(clicked()), QG_CadToolBarSelect, SLOT(runNextAction()));
        QObject::connect(bAll, SIGNAL(clicked()), QG_CadToolBarSelect, SLOT(selectAll()));
        QObject::connect(bWindow, SIGNAL(clicked()), QG_CadToolBarSelect, SLOT(selectWindow()));
        QObject::connect(bUnAll, SIGNAL(clicked()), QG_CadToolBarSelect, SLOT(deselectAll()));
        QObject::connect(bUnWindow, SIGNAL(clicked()), QG_CadToolBarSelect, SLOT(deselectWindow()));
        QObject::connect(bContour, SIGNAL(clicked()), QG_CadToolBarSelect, SLOT(selectContour()));
        QObject::connect(bUnInters, SIGNAL(clicked()), QG_CadToolBarSelect, SLOT(deselectIntersected()));
        QObject::connect(bInters, SIGNAL(clicked()), QG_CadToolBarSelect, SLOT(selectIntersected()));
        QObject::connect(bInvert, SIGNAL(clicked()), QG_CadToolBarSelect, SLOT(selectInvert()));
        QObject::connect(bLayer, SIGNAL(clicked()), QG_CadToolBarSelect, SLOT(selectLayer()));

        QMetaObject::connectSlotsByName(QG_CadToolBarSelect);
    } // setupUi

    void retranslateUi(QWidget *QG_CadToolBarSelect)
    {
        QG_CadToolBarSelect->setWindowTitle(QApplication::translate("QG_CadToolBarSelect", "Select", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        bBack->setToolTip(QApplication::translate("QG_CadToolBarSelect", "Back to main menu", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bBack->setText(QString());
#ifndef QT_NO_TOOLTIP
        bUnAll->setToolTip(QApplication::translate("QG_CadToolBarSelect", "Deselect all", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bUnAll->setText(QString());
#ifndef QT_NO_TOOLTIP
        bAll->setToolTip(QApplication::translate("QG_CadToolBarSelect", "Select all", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bAll->setText(QString());
#ifndef QT_NO_TOOLTIP
        bSingle->setToolTip(QApplication::translate("QG_CadToolBarSelect", "(De-)Select entity", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bSingle->setText(QString());
#ifndef QT_NO_TOOLTIP
        bContour->setToolTip(QApplication::translate("QG_CadToolBarSelect", "(De-)Select contour", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bContour->setText(QString());
#ifndef QT_NO_TOOLTIP
        bUnWindow->setToolTip(QApplication::translate("QG_CadToolBarSelect", "Deselect Window", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bUnWindow->setText(QString());
#ifndef QT_NO_TOOLTIP
        bWindow->setToolTip(QApplication::translate("QG_CadToolBarSelect", "Select Window", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bWindow->setText(QString());
#ifndef QT_NO_TOOLTIP
        bUnInters->setToolTip(QApplication::translate("QG_CadToolBarSelect", "Deselect intersected entities", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bUnInters->setText(QString());
#ifndef QT_NO_TOOLTIP
        bInters->setToolTip(QApplication::translate("QG_CadToolBarSelect", "Select intersected entities", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bInters->setText(QString());
#ifndef QT_NO_TOOLTIP
        bInvert->setToolTip(QApplication::translate("QG_CadToolBarSelect", "Invert Selection", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bInvert->setText(QString());
#ifndef QT_NO_TOOLTIP
        bLayer->setToolTip(QApplication::translate("QG_CadToolBarSelect", "Select layer", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bLayer->setText(QString());
#ifndef QT_NO_TOOLTIP
        bDoit->setToolTip(QApplication::translate("QG_CadToolBarSelect", "Continue action", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bDoit->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class QG_CadToolBarSelect: public Ui_QG_CadToolBarSelect {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QG_CADTOOLBARSELECT_H
