/********************************************************************************
** Form generated from reading UI file 'qg_cadtoolbarmodify.ui'
**
** Created: Sat Feb 18 16:41:50 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QG_CADTOOLBARMODIFY_H
#define UI_QG_CADTOOLBARMODIFY_H

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

class Ui_QG_CadToolBarModify
{
public:
    QGridLayout *gridLayout;
    QToolButton *bBack;
    QToolButton *bMove;
    QToolButton *bRotate;
    QToolButton *bScale;
    QToolButton *bMirror;
    QToolButton *bMoveRotate;
    QToolButton *bRotate2;
    QToolButton *bTrim;
    QToolButton *bTrim2;
    QToolButton *bTrimAmount;
    QToolButton *bOffset;
    QToolButton *bBevel;
    QToolButton *bRound;
    QToolButton *bCut;
    QToolButton *bStretch;
    QToolButton *bEntity;
    QToolButton *bAttributes;
    QToolButton *bDelete;
    QToolButton *bExplode;
    QToolButton *bExplodeText;
    QToolButton *bEntityText;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *QG_CadToolBarModify)
    {
        if (QG_CadToolBarModify->objectName().isEmpty())
            QG_CadToolBarModify->setObjectName(QString::fromUtf8("QG_CadToolBarModify"));
        QG_CadToolBarModify->resize(65, 448);
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QG_CadToolBarModify->sizePolicy().hasHeightForWidth());
        QG_CadToolBarModify->setSizePolicy(sizePolicy);
        QG_CadToolBarModify->setMinimumSize(QSize(65, 336));
        gridLayout = new QGridLayout(QG_CadToolBarModify);
        gridLayout->setSpacing(1);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        bBack = new QToolButton(QG_CadToolBarModify);
        bBack->setObjectName(QString::fromUtf8("bBack"));
        bBack->setMaximumSize(QSize(16777215, 20));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/extui/back.png"), QSize(), QIcon::Normal, QIcon::Off);
        bBack->setIcon(icon);
        bBack->setIconSize(QSize(65, 20));

        gridLayout->addWidget(bBack, 0, 0, 1, 2);

        bMove = new QToolButton(QG_CadToolBarModify);
        bMove->setObjectName(QString::fromUtf8("bMove"));
        bMove->setMaximumSize(QSize(32, 32));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/extui/modifymove.png"), QSize(), QIcon::Normal, QIcon::Off);
        bMove->setIcon(icon1);
        bMove->setIconSize(QSize(32, 32));

        gridLayout->addWidget(bMove, 1, 0, 1, 1);

        bRotate = new QToolButton(QG_CadToolBarModify);
        bRotate->setObjectName(QString::fromUtf8("bRotate"));
        bRotate->setMaximumSize(QSize(32, 32));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/extui/modifyrotate.png"), QSize(), QIcon::Normal, QIcon::Off);
        bRotate->setIcon(icon2);
        bRotate->setIconSize(QSize(32, 32));

        gridLayout->addWidget(bRotate, 1, 1, 1, 1);

        bScale = new QToolButton(QG_CadToolBarModify);
        bScale->setObjectName(QString::fromUtf8("bScale"));
        bScale->setMaximumSize(QSize(32, 32));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/extui/modifyscale.png"), QSize(), QIcon::Normal, QIcon::Off);
        bScale->setIcon(icon3);
        bScale->setIconSize(QSize(32, 32));

        gridLayout->addWidget(bScale, 2, 0, 1, 1);

        bMirror = new QToolButton(QG_CadToolBarModify);
        bMirror->setObjectName(QString::fromUtf8("bMirror"));
        bMirror->setMaximumSize(QSize(32, 32));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/extui/modifymirror.png"), QSize(), QIcon::Normal, QIcon::Off);
        bMirror->setIcon(icon4);
        bMirror->setIconSize(QSize(32, 32));

        gridLayout->addWidget(bMirror, 2, 1, 1, 1);

        bMoveRotate = new QToolButton(QG_CadToolBarModify);
        bMoveRotate->setObjectName(QString::fromUtf8("bMoveRotate"));
        bMoveRotate->setMaximumSize(QSize(32, 32));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/extui/modifymoverotate.png"), QSize(), QIcon::Normal, QIcon::Off);
        bMoveRotate->setIcon(icon5);
        bMoveRotate->setIconSize(QSize(32, 32));

        gridLayout->addWidget(bMoveRotate, 3, 0, 1, 1);

        bRotate2 = new QToolButton(QG_CadToolBarModify);
        bRotate2->setObjectName(QString::fromUtf8("bRotate2"));
        bRotate2->setMaximumSize(QSize(32, 32));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/extui/modifyrotate2.png"), QSize(), QIcon::Normal, QIcon::Off);
        bRotate2->setIcon(icon6);
        bRotate2->setIconSize(QSize(32, 32));

        gridLayout->addWidget(bRotate2, 3, 1, 1, 1);

        bTrim = new QToolButton(QG_CadToolBarModify);
        bTrim->setObjectName(QString::fromUtf8("bTrim"));
        bTrim->setMaximumSize(QSize(32, 32));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/extui/modifytrim.png"), QSize(), QIcon::Normal, QIcon::Off);
        bTrim->setIcon(icon7);
        bTrim->setIconSize(QSize(32, 32));

        gridLayout->addWidget(bTrim, 4, 0, 1, 1);

        bTrim2 = new QToolButton(QG_CadToolBarModify);
        bTrim2->setObjectName(QString::fromUtf8("bTrim2"));
        bTrim2->setMaximumSize(QSize(32, 32));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/extui/modifytrim2.png"), QSize(), QIcon::Normal, QIcon::Off);
        bTrim2->setIcon(icon8);
        bTrim2->setIconSize(QSize(32, 32));

        gridLayout->addWidget(bTrim2, 4, 1, 1, 1);

        bTrimAmount = new QToolButton(QG_CadToolBarModify);
        bTrimAmount->setObjectName(QString::fromUtf8("bTrimAmount"));
        bTrimAmount->setMaximumSize(QSize(32, 32));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/extui/modifytrimamount.png"), QSize(), QIcon::Normal, QIcon::Off);
        bTrimAmount->setIcon(icon9);
        bTrimAmount->setIconSize(QSize(32, 32));

        gridLayout->addWidget(bTrimAmount, 5, 0, 1, 1);

        bOffset = new QToolButton(QG_CadToolBarModify);
        bOffset->setObjectName(QString::fromUtf8("bOffset"));
        bOffset->setMaximumSize(QSize(32, 32));
        bOffset->setText(QString::fromUtf8(""));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/extui/arcspara.png"), QSize(), QIcon::Normal, QIcon::Off);
        bOffset->setIcon(icon10);
        bOffset->setIconSize(QSize(32, 32));
        bOffset->setCheckable(true);
        bOffset->setAutoExclusive(true);

        gridLayout->addWidget(bOffset, 5, 1, 1, 1);

        bBevel = new QToolButton(QG_CadToolBarModify);
        bBevel->setObjectName(QString::fromUtf8("bBevel"));
        bBevel->setMaximumSize(QSize(32, 32));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/extui/modifybevel.png"), QSize(), QIcon::Normal, QIcon::Off);
        bBevel->setIcon(icon11);
        bBevel->setIconSize(QSize(32, 32));

        gridLayout->addWidget(bBevel, 6, 0, 1, 1);

        bRound = new QToolButton(QG_CadToolBarModify);
        bRound->setObjectName(QString::fromUtf8("bRound"));
        bRound->setMaximumSize(QSize(32, 32));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/extui/modifyround.png"), QSize(), QIcon::Normal, QIcon::Off);
        bRound->setIcon(icon12);
        bRound->setIconSize(QSize(32, 32));

        gridLayout->addWidget(bRound, 6, 1, 1, 1);

        bCut = new QToolButton(QG_CadToolBarModify);
        bCut->setObjectName(QString::fromUtf8("bCut"));
        bCut->setMaximumSize(QSize(32, 32));
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/extui/modifycut.png"), QSize(), QIcon::Normal, QIcon::Off);
        bCut->setIcon(icon13);
        bCut->setIconSize(QSize(32, 32));

        gridLayout->addWidget(bCut, 7, 0, 1, 1);

        bStretch = new QToolButton(QG_CadToolBarModify);
        bStretch->setObjectName(QString::fromUtf8("bStretch"));
        bStretch->setMaximumSize(QSize(32, 32));
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/extui/modifystretch.png"), QSize(), QIcon::Normal, QIcon::Off);
        bStretch->setIcon(icon14);
        bStretch->setIconSize(QSize(32, 32));

        gridLayout->addWidget(bStretch, 7, 1, 1, 1);

        bEntity = new QToolButton(QG_CadToolBarModify);
        bEntity->setObjectName(QString::fromUtf8("bEntity"));
        bEntity->setMaximumSize(QSize(32, 32));
        QIcon icon15;
        icon15.addFile(QString::fromUtf8(":/extui/modifyentity.png"), QSize(), QIcon::Normal, QIcon::Off);
        bEntity->setIcon(icon15);
        bEntity->setIconSize(QSize(32, 32));

        gridLayout->addWidget(bEntity, 8, 0, 1, 1);

        bAttributes = new QToolButton(QG_CadToolBarModify);
        bAttributes->setObjectName(QString::fromUtf8("bAttributes"));
        bAttributes->setMaximumSize(QSize(32, 32));
        QIcon icon16;
        icon16.addFile(QString::fromUtf8(":/extui/modifyattributes.png"), QSize(), QIcon::Normal, QIcon::Off);
        bAttributes->setIcon(icon16);
        bAttributes->setIconSize(QSize(32, 32));

        gridLayout->addWidget(bAttributes, 8, 1, 1, 1);

        bDelete = new QToolButton(QG_CadToolBarModify);
        bDelete->setObjectName(QString::fromUtf8("bDelete"));
        bDelete->setMaximumSize(QSize(32, 32));
        QIcon icon17;
        icon17.addFile(QString::fromUtf8(":/extui/modifydelete.png"), QSize(), QIcon::Normal, QIcon::Off);
        bDelete->setIcon(icon17);
        bDelete->setIconSize(QSize(32, 32));

        gridLayout->addWidget(bDelete, 9, 0, 1, 1);

        bExplode = new QToolButton(QG_CadToolBarModify);
        bExplode->setObjectName(QString::fromUtf8("bExplode"));
        bExplode->setMaximumSize(QSize(32, 32));
        QIcon icon18;
        icon18.addFile(QString::fromUtf8(":/extui/modifyexplode.png"), QSize(), QIcon::Normal, QIcon::Off);
        bExplode->setIcon(icon18);
        bExplode->setIconSize(QSize(32, 32));

        gridLayout->addWidget(bExplode, 9, 1, 1, 1);

        bExplodeText = new QToolButton(QG_CadToolBarModify);
        bExplodeText->setObjectName(QString::fromUtf8("bExplodeText"));
        bExplodeText->setMaximumSize(QSize(32, 32));
        QIcon icon19;
        icon19.addFile(QString::fromUtf8(":/extui/modifyexplodetext.png"), QSize(), QIcon::Normal, QIcon::Off);
        bExplodeText->setIcon(icon19);
        bExplodeText->setIconSize(QSize(32, 32));

        gridLayout->addWidget(bExplodeText, 10, 0, 1, 1);

        bEntityText = new QToolButton(QG_CadToolBarModify);
        bEntityText->setObjectName(QString::fromUtf8("bEntityText"));
        bEntityText->setMaximumSize(QSize(32, 32));
        QIcon icon20;
        icon20.addFile(QString::fromUtf8(":/extui/modifytext.png"), QSize(), QIcon::Normal, QIcon::Off);
        bEntityText->setIcon(icon20);
        bEntityText->setIconSize(QSize(32, 32));

        gridLayout->addWidget(bEntityText, 10, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 11, 0, 1, 1);


        retranslateUi(QG_CadToolBarModify);
        QObject::connect(bMove, SIGNAL(clicked()), QG_CadToolBarModify, SLOT(modifyMove()));
        QObject::connect(bRotate, SIGNAL(clicked()), QG_CadToolBarModify, SLOT(modifyRotate()));
        QObject::connect(bEntity, SIGNAL(clicked()), QG_CadToolBarModify, SLOT(modifyEntity()));
        QObject::connect(bScale, SIGNAL(clicked()), QG_CadToolBarModify, SLOT(modifyScale()));
        QObject::connect(bDelete, SIGNAL(clicked()), QG_CadToolBarModify, SLOT(modifyDelete()));
        QObject::connect(bTrim, SIGNAL(clicked()), QG_CadToolBarModify, SLOT(modifyTrim()));
        QObject::connect(bMirror, SIGNAL(clicked()), QG_CadToolBarModify, SLOT(modifyMirror()));
        QObject::connect(bTrim2, SIGNAL(clicked()), QG_CadToolBarModify, SLOT(modifyTrim2()));
        QObject::connect(bMoveRotate, SIGNAL(clicked()), QG_CadToolBarModify, SLOT(modifyMoveRotate()));
        QObject::connect(bTrimAmount, SIGNAL(clicked()), QG_CadToolBarModify, SLOT(modifyTrimAmount()));
        QObject::connect(bCut, SIGNAL(clicked()), QG_CadToolBarModify, SLOT(modifyCut()));
        QObject::connect(bRotate2, SIGNAL(clicked()), QG_CadToolBarModify, SLOT(modifyRotate2()));
        QObject::connect(bStretch, SIGNAL(clicked()), QG_CadToolBarModify, SLOT(modifyStretch()));
        QObject::connect(bBevel, SIGNAL(clicked()), QG_CadToolBarModify, SLOT(modifyBevel()));
        QObject::connect(bRound, SIGNAL(clicked()), QG_CadToolBarModify, SLOT(modifyRound()));
        QObject::connect(bExplode, SIGNAL(clicked()), QG_CadToolBarModify, SLOT(modifyExplode()));
        QObject::connect(bAttributes, SIGNAL(clicked()), QG_CadToolBarModify, SLOT(modifyAttributes()));
        QObject::connect(bExplodeText, SIGNAL(clicked()), QG_CadToolBarModify, SLOT(modifyExplodeText()));
        QObject::connect(bEntityText, SIGNAL(clicked()), QG_CadToolBarModify, SLOT(modifyEntity()));
        QObject::connect(bOffset, SIGNAL(clicked()), QG_CadToolBarModify, SLOT(modifyOffset()));

        QMetaObject::connectSlotsByName(QG_CadToolBarModify);
    } // setupUi

    void retranslateUi(QWidget *QG_CadToolBarModify)
    {
        QG_CadToolBarModify->setWindowTitle(QApplication::translate("QG_CadToolBarModify", "Modify", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        bBack->setToolTip(QApplication::translate("QG_CadToolBarModify", "Back to main menu", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bBack->setText(QString());
#ifndef QT_NO_TOOLTIP
        bMove->setToolTip(QApplication::translate("QG_CadToolBarModify", "Move / Copy", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bMove->setText(QString());
#ifndef QT_NO_TOOLTIP
        bRotate->setToolTip(QApplication::translate("QG_CadToolBarModify", "Rotate", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bRotate->setText(QString());
#ifndef QT_NO_TOOLTIP
        bScale->setToolTip(QApplication::translate("QG_CadToolBarModify", "Scale", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bScale->setText(QString());
#ifndef QT_NO_TOOLTIP
        bMirror->setToolTip(QApplication::translate("QG_CadToolBarModify", "Mirror", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bMirror->setText(QString());
#ifndef QT_NO_TOOLTIP
        bMoveRotate->setToolTip(QApplication::translate("QG_CadToolBarModify", "Move and Rotate", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bMoveRotate->setText(QString());
#ifndef QT_NO_TOOLTIP
        bRotate2->setToolTip(QApplication::translate("QG_CadToolBarModify", "Rotate Two", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bRotate2->setText(QString());
#ifndef QT_NO_TOOLTIP
        bTrim->setToolTip(QApplication::translate("QG_CadToolBarModify", "Trim / Extend", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bTrim->setText(QString());
#ifndef QT_NO_TOOLTIP
        bTrim2->setToolTip(QApplication::translate("QG_CadToolBarModify", "Trim / Extend two", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bTrim2->setText(QString());
#ifndef QT_NO_TOOLTIP
        bTrimAmount->setToolTip(QApplication::translate("QG_CadToolBarModify", "Trim by amount", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bTrimAmount->setText(QString());
#ifndef QT_NO_TOOLTIP
        bOffset->setToolTip(QApplication::translate("QG_CadToolBarModify", "Offset (Experimental Feature, work in progress)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        bBevel->setToolTip(QApplication::translate("QG_CadToolBarModify", "Bevel", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bBevel->setText(QString());
#ifndef QT_NO_TOOLTIP
        bRound->setToolTip(QApplication::translate("QG_CadToolBarModify", "Round", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bRound->setText(QString());
#ifndef QT_NO_TOOLTIP
        bCut->setToolTip(QApplication::translate("QG_CadToolBarModify", "Divide", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bCut->setText(QString());
#ifndef QT_NO_TOOLTIP
        bStretch->setToolTip(QApplication::translate("QG_CadToolBarModify", "Stretch", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bStretch->setText(QString());
#ifndef QT_NO_TOOLTIP
        bEntity->setToolTip(QApplication::translate("QG_CadToolBarModify", "Edit Entity Geometry", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bEntity->setText(QString());
#ifndef QT_NO_TOOLTIP
        bAttributes->setToolTip(QApplication::translate("QG_CadToolBarModify", "Edit Entity Attributes", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bAttributes->setText(QString());
#ifndef QT_NO_TOOLTIP
        bDelete->setToolTip(QApplication::translate("QG_CadToolBarModify", "Delete", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bDelete->setText(QString());
#ifndef QT_NO_TOOLTIP
        bExplode->setToolTip(QApplication::translate("QG_CadToolBarModify", "Explode", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bExplode->setText(QString());
#ifndef QT_NO_TOOLTIP
        bExplodeText->setToolTip(QApplication::translate("QG_CadToolBarModify", "Explode Text into Letters", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bExplodeText->setText(QString());
#ifndef QT_NO_TOOLTIP
        bEntityText->setToolTip(QApplication::translate("QG_CadToolBarModify", "Edit Text", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bEntityText->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class QG_CadToolBarModify: public Ui_QG_CadToolBarModify {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QG_CADTOOLBARMODIFY_H
