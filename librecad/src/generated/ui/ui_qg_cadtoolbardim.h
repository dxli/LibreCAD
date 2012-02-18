/********************************************************************************
** Form generated from reading UI file 'qg_cadtoolbardim.ui'
**
** Created: Sat Feb 18 16:41:50 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QG_CADTOOLBARDIM_H
#define UI_QG_CADTOOLBARDIM_H

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

class Ui_QG_CadToolBarDim
{
public:
    QGridLayout *gridLayout;
    QToolButton *bBack;
    QToolButton *bAligned;
    QToolButton *bLinear;
    QToolButton *bLinearHor;
    QToolButton *bLinearVer;
    QToolButton *bRadial;
    QToolButton *bDiametric;
    QToolButton *bAngular;
    QToolButton *bLeader;
    QSpacerItem *verticalSpacer;
    QToolButton *bHidden;

    void setupUi(QWidget *QG_CadToolBarDim)
    {
        if (QG_CadToolBarDim->objectName().isEmpty())
            QG_CadToolBarDim->setObjectName(QString::fromUtf8("QG_CadToolBarDim"));
        QG_CadToolBarDim->resize(65, 336);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QG_CadToolBarDim->sizePolicy().hasHeightForWidth());
        QG_CadToolBarDim->setSizePolicy(sizePolicy);
        QG_CadToolBarDim->setMinimumSize(QSize(65, 336));
        gridLayout = new QGridLayout(QG_CadToolBarDim);
        gridLayout->setSpacing(1);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        bBack = new QToolButton(QG_CadToolBarDim);
        bBack->setObjectName(QString::fromUtf8("bBack"));
        bBack->setMaximumSize(QSize(16777215, 20));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/extui/back.png"), QSize(), QIcon::Normal, QIcon::Off);
        bBack->setIcon(icon);
        bBack->setIconSize(QSize(65, 20));

        gridLayout->addWidget(bBack, 0, 0, 1, 2);

        bAligned = new QToolButton(QG_CadToolBarDim);
        bAligned->setObjectName(QString::fromUtf8("bAligned"));
        bAligned->setMaximumSize(QSize(32, 32));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/extui/dimaligned.png"), QSize(), QIcon::Normal, QIcon::Off);
        bAligned->setIcon(icon1);
        bAligned->setIconSize(QSize(32, 32));
        bAligned->setCheckable(true);
        bAligned->setAutoExclusive(true);

        gridLayout->addWidget(bAligned, 1, 0, 1, 1);

        bLinear = new QToolButton(QG_CadToolBarDim);
        bLinear->setObjectName(QString::fromUtf8("bLinear"));
        bLinear->setMaximumSize(QSize(32, 32));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/extui/dimlinear.png"), QSize(), QIcon::Normal, QIcon::Off);
        bLinear->setIcon(icon2);
        bLinear->setIconSize(QSize(32, 32));
        bLinear->setCheckable(true);
        bLinear->setAutoExclusive(true);

        gridLayout->addWidget(bLinear, 1, 1, 1, 1);

        bLinearHor = new QToolButton(QG_CadToolBarDim);
        bLinearHor->setObjectName(QString::fromUtf8("bLinearHor"));
        bLinearHor->setMaximumSize(QSize(32, 32));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/extui/dimhor.png"), QSize(), QIcon::Normal, QIcon::Off);
        bLinearHor->setIcon(icon3);
        bLinearHor->setIconSize(QSize(32, 32));
        bLinearHor->setCheckable(true);
        bLinearHor->setAutoExclusive(true);

        gridLayout->addWidget(bLinearHor, 2, 0, 1, 1);

        bLinearVer = new QToolButton(QG_CadToolBarDim);
        bLinearVer->setObjectName(QString::fromUtf8("bLinearVer"));
        bLinearVer->setMaximumSize(QSize(32, 32));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/extui/dimver.png"), QSize(), QIcon::Normal, QIcon::Off);
        bLinearVer->setIcon(icon4);
        bLinearVer->setIconSize(QSize(32, 32));
        bLinearVer->setCheckable(true);
        bLinearVer->setAutoExclusive(true);

        gridLayout->addWidget(bLinearVer, 2, 1, 1, 1);

        bRadial = new QToolButton(QG_CadToolBarDim);
        bRadial->setObjectName(QString::fromUtf8("bRadial"));
        bRadial->setMaximumSize(QSize(32, 32));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/extui/dimradial.png"), QSize(), QIcon::Normal, QIcon::Off);
        bRadial->setIcon(icon5);
        bRadial->setIconSize(QSize(32, 32));
        bRadial->setCheckable(true);
        bRadial->setAutoExclusive(true);

        gridLayout->addWidget(bRadial, 3, 0, 1, 1);

        bDiametric = new QToolButton(QG_CadToolBarDim);
        bDiametric->setObjectName(QString::fromUtf8("bDiametric"));
        bDiametric->setMaximumSize(QSize(32, 32));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/extui/dimdiametric.png"), QSize(), QIcon::Normal, QIcon::Off);
        bDiametric->setIcon(icon6);
        bDiametric->setIconSize(QSize(32, 32));
        bDiametric->setCheckable(true);
        bDiametric->setAutoExclusive(true);

        gridLayout->addWidget(bDiametric, 3, 1, 1, 1);

        bAngular = new QToolButton(QG_CadToolBarDim);
        bAngular->setObjectName(QString::fromUtf8("bAngular"));
        bAngular->setMaximumSize(QSize(32, 32));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/extui/dimangular.png"), QSize(), QIcon::Normal, QIcon::Off);
        bAngular->setIcon(icon7);
        bAngular->setIconSize(QSize(32, 32));
        bAngular->setCheckable(true);
        bAngular->setAutoExclusive(true);

        gridLayout->addWidget(bAngular, 4, 0, 1, 1);

        bLeader = new QToolButton(QG_CadToolBarDim);
        bLeader->setObjectName(QString::fromUtf8("bLeader"));
        bLeader->setMaximumSize(QSize(32, 32));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/extui/dimleader.png"), QSize(), QIcon::Normal, QIcon::Off);
        bLeader->setIcon(icon8);
        bLeader->setIconSize(QSize(32, 32));
        bLeader->setCheckable(true);
        bLeader->setAutoExclusive(true);

        gridLayout->addWidget(bLeader, 4, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 5, 0, 1, 1);

        bHidden = new QToolButton(QG_CadToolBarDim);
        bHidden->setObjectName(QString::fromUtf8("bHidden"));
        bHidden->setMaximumSize(QSize(0, 0));
        bHidden->setText(QString::fromUtf8(""));
        bHidden->setIconSize(QSize(0, 0));
        bHidden->setCheckable(true);
        bHidden->setAutoExclusive(true);

        gridLayout->addWidget(bHidden, 6, 0, 1, 1);


        retranslateUi(QG_CadToolBarDim);
        QObject::connect(bAligned, SIGNAL(clicked()), QG_CadToolBarDim, SLOT(drawDimAligned()));
        QObject::connect(bLinearHor, SIGNAL(clicked()), QG_CadToolBarDim, SLOT(drawDimLinearHor()));
        QObject::connect(bLinearVer, SIGNAL(clicked()), QG_CadToolBarDim, SLOT(drawDimLinearVer()));
        QObject::connect(bLinear, SIGNAL(clicked()), QG_CadToolBarDim, SLOT(drawDimLinear()));
        QObject::connect(bRadial, SIGNAL(clicked()), QG_CadToolBarDim, SLOT(drawDimRadial()));
        QObject::connect(bDiametric, SIGNAL(clicked()), QG_CadToolBarDim, SLOT(drawDimDiametric()));
        QObject::connect(bAngular, SIGNAL(clicked()), QG_CadToolBarDim, SLOT(drawDimAngular()));
        QObject::connect(bLeader, SIGNAL(clicked()), QG_CadToolBarDim, SLOT(drawDimLeader()));

        QMetaObject::connectSlotsByName(QG_CadToolBarDim);
    } // setupUi

    void retranslateUi(QWidget *QG_CadToolBarDim)
    {
        QG_CadToolBarDim->setWindowTitle(QApplication::translate("QG_CadToolBarDim", "Dimensions", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        bBack->setToolTip(QApplication::translate("QG_CadToolBarDim", "Back to main menu", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bBack->setText(QString());
#ifndef QT_NO_TOOLTIP
        bAligned->setToolTip(QApplication::translate("QG_CadToolBarDim", "Aligned Dimension", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bAligned->setText(QString());
#ifndef QT_NO_TOOLTIP
        bLinear->setToolTip(QApplication::translate("QG_CadToolBarDim", "Linear Dimension", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bLinear->setText(QString());
#ifndef QT_NO_TOOLTIP
        bLinearHor->setToolTip(QApplication::translate("QG_CadToolBarDim", "Horizontal Dimension", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bLinearHor->setText(QString());
#ifndef QT_NO_TOOLTIP
        bLinearVer->setToolTip(QApplication::translate("QG_CadToolBarDim", "Vertical Dimension", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bLinearVer->setText(QString());
#ifndef QT_NO_TOOLTIP
        bRadial->setToolTip(QApplication::translate("QG_CadToolBarDim", "Radial Dimension", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bRadial->setText(QString());
#ifndef QT_NO_TOOLTIP
        bDiametric->setToolTip(QApplication::translate("QG_CadToolBarDim", "Diametric Dimension", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bDiametric->setText(QString());
#ifndef QT_NO_TOOLTIP
        bAngular->setToolTip(QApplication::translate("QG_CadToolBarDim", "Angular Dimension", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bAngular->setText(QString());
#ifndef QT_NO_TOOLTIP
        bLeader->setToolTip(QApplication::translate("QG_CadToolBarDim", "Leader", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bLeader->setText(QString());
#ifndef QT_NO_TOOLTIP
        bHidden->setToolTip(QApplication::translate("QG_CadToolBarDim", "Concentric", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
    } // retranslateUi

};

namespace Ui {
    class QG_CadToolBarDim: public Ui_QG_CadToolBarDim {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QG_CADTOOLBARDIM_H
