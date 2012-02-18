/********************************************************************************
** Form generated from reading UI file 'qg_cadtoolbarmain.ui'
**
** Created: Sat Feb 18 16:41:50 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QG_CADTOOLBARMAIN_H
#define UI_QG_CADTOOLBARMAIN_H

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

class Ui_QG_CadToolBarMain
{
public:
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer;
    QToolButton *bMenuLine;
    QToolButton *bMenuArc;
    QToolButton *bMenuCircle;
    QToolButton *bMenuEllipse;
    QToolButton *bMenuPolyline;
    QToolButton *bMenuSpline;
    QToolButton *bMenuPoint;
    QToolButton *bMenuText;
    QToolButton *bMenuDim;
    QToolButton *bMenuHatch;
    QToolButton *bMenuImage;
    QToolButton *bMenuModify;
    QToolButton *bMenuInfo;
    QToolButton *bMenuBlock;
    QToolButton *bMenuSelect;
    QSpacerItem *verticalSpacer;
    QToolButton *bHidden;

    void setupUi(QWidget *QG_CadToolBarMain)
    {
        if (QG_CadToolBarMain->objectName().isEmpty())
            QG_CadToolBarMain->setObjectName(QString::fromUtf8("QG_CadToolBarMain"));
        QG_CadToolBarMain->resize(65, 362);
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QG_CadToolBarMain->sizePolicy().hasHeightForWidth());
        QG_CadToolBarMain->setSizePolicy(sizePolicy);
        QG_CadToolBarMain->setMinimumSize(QSize(65, 336));
        QG_CadToolBarMain->setMaximumSize(QSize(65, 16777215));
        gridLayout = new QGridLayout(QG_CadToolBarMain);
        gridLayout->setSpacing(1);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalSpacer = new QSpacerItem(62, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 0, 1, 3);

        bMenuLine = new QToolButton(QG_CadToolBarMain);
        bMenuLine->setObjectName(QString::fromUtf8("bMenuLine"));
        bMenuLine->setMinimumSize(QSize(0, 0));
        bMenuLine->setMaximumSize(QSize(32, 32));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/extui/menuline.png"), QSize(), QIcon::Normal, QIcon::Off);
        bMenuLine->setIcon(icon);
        bMenuLine->setIconSize(QSize(32, 32));
        bMenuLine->setCheckable(true);
        bMenuLine->setAutoExclusive(true);

        gridLayout->addWidget(bMenuLine, 1, 0, 1, 2);

        bMenuArc = new QToolButton(QG_CadToolBarMain);
        bMenuArc->setObjectName(QString::fromUtf8("bMenuArc"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(bMenuArc->sizePolicy().hasHeightForWidth());
        bMenuArc->setSizePolicy(sizePolicy1);
        bMenuArc->setMinimumSize(QSize(0, 0));
        bMenuArc->setMaximumSize(QSize(32, 32));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/extui/menuarc.png"), QSize(), QIcon::Normal, QIcon::Off);
        bMenuArc->setIcon(icon1);
        bMenuArc->setIconSize(QSize(32, 32));
        bMenuArc->setCheckable(true);
        bMenuArc->setAutoExclusive(true);

        gridLayout->addWidget(bMenuArc, 1, 2, 1, 1);

        bMenuCircle = new QToolButton(QG_CadToolBarMain);
        bMenuCircle->setObjectName(QString::fromUtf8("bMenuCircle"));
        bMenuCircle->setMaximumSize(QSize(32, 32));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/extui/menucircle.png"), QSize(), QIcon::Normal, QIcon::Off);
        bMenuCircle->setIcon(icon2);
        bMenuCircle->setIconSize(QSize(32, 32));
        bMenuCircle->setCheckable(true);
        bMenuCircle->setAutoExclusive(true);

        gridLayout->addWidget(bMenuCircle, 2, 0, 1, 2);

        bMenuEllipse = new QToolButton(QG_CadToolBarMain);
        bMenuEllipse->setObjectName(QString::fromUtf8("bMenuEllipse"));
        bMenuEllipse->setMaximumSize(QSize(32, 32));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/extui/menuellipse.png"), QSize(), QIcon::Normal, QIcon::Off);
        bMenuEllipse->setIcon(icon3);
        bMenuEllipse->setIconSize(QSize(32, 32));
        bMenuEllipse->setCheckable(true);
        bMenuEllipse->setAutoExclusive(true);

        gridLayout->addWidget(bMenuEllipse, 2, 2, 1, 1);

        bMenuPolyline = new QToolButton(QG_CadToolBarMain);
        bMenuPolyline->setObjectName(QString::fromUtf8("bMenuPolyline"));
        bMenuPolyline->setMaximumSize(QSize(32, 32));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/extui/menupolyline.png"), QSize(), QIcon::Normal, QIcon::Off);
        bMenuPolyline->setIcon(icon4);
        bMenuPolyline->setIconSize(QSize(32, 32));
        bMenuPolyline->setCheckable(true);
        bMenuPolyline->setAutoExclusive(true);

        gridLayout->addWidget(bMenuPolyline, 3, 0, 1, 2);

        bMenuSpline = new QToolButton(QG_CadToolBarMain);
        bMenuSpline->setObjectName(QString::fromUtf8("bMenuSpline"));
        bMenuSpline->setMaximumSize(QSize(32, 32));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/extui/menuspline.png"), QSize(), QIcon::Normal, QIcon::Off);
        bMenuSpline->setIcon(icon5);
        bMenuSpline->setIconSize(QSize(32, 32));
        bMenuSpline->setCheckable(true);
        bMenuSpline->setAutoExclusive(true);

        gridLayout->addWidget(bMenuSpline, 3, 2, 1, 1);

        bMenuPoint = new QToolButton(QG_CadToolBarMain);
        bMenuPoint->setObjectName(QString::fromUtf8("bMenuPoint"));
        bMenuPoint->setMaximumSize(QSize(32, 32));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/extui/points.png"), QSize(), QIcon::Normal, QIcon::Off);
        bMenuPoint->setIcon(icon6);
        bMenuPoint->setIconSize(QSize(32, 32));
        bMenuPoint->setCheckable(true);
        bMenuPoint->setAutoExclusive(true);

        gridLayout->addWidget(bMenuPoint, 4, 0, 1, 2);

        bMenuText = new QToolButton(QG_CadToolBarMain);
        bMenuText->setObjectName(QString::fromUtf8("bMenuText"));
        bMenuText->setMaximumSize(QSize(32, 32));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/extui/menutext.png"), QSize(), QIcon::Normal, QIcon::Off);
        bMenuText->setIcon(icon7);
        bMenuText->setIconSize(QSize(32, 32));
        bMenuText->setCheckable(false);
        bMenuText->setAutoExclusive(false);

        gridLayout->addWidget(bMenuText, 5, 0, 1, 2);

        bMenuDim = new QToolButton(QG_CadToolBarMain);
        bMenuDim->setObjectName(QString::fromUtf8("bMenuDim"));
        bMenuDim->setMaximumSize(QSize(32, 32));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/extui/dimhor.png"), QSize(), QIcon::Normal, QIcon::Off);
        bMenuDim->setIcon(icon8);
        bMenuDim->setIconSize(QSize(32, 32));
        bMenuDim->setCheckable(true);
        bMenuDim->setAutoExclusive(true);

        gridLayout->addWidget(bMenuDim, 5, 2, 1, 1);

        bMenuHatch = new QToolButton(QG_CadToolBarMain);
        bMenuHatch->setObjectName(QString::fromUtf8("bMenuHatch"));
        bMenuHatch->setMaximumSize(QSize(32, 32));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/extui/menuhatch.png"), QSize(), QIcon::Normal, QIcon::Off);
        bMenuHatch->setIcon(icon9);
        bMenuHatch->setIconSize(QSize(32, 32));
        bMenuHatch->setCheckable(true);
        bMenuHatch->setAutoExclusive(true);

        gridLayout->addWidget(bMenuHatch, 6, 0, 1, 2);

        bMenuImage = new QToolButton(QG_CadToolBarMain);
        bMenuImage->setObjectName(QString::fromUtf8("bMenuImage"));
        bMenuImage->setMaximumSize(QSize(32, 32));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/extui/menuimage.png"), QSize(), QIcon::Normal, QIcon::Off);
        bMenuImage->setIcon(icon10);
        bMenuImage->setIconSize(QSize(32, 32));
        bMenuImage->setCheckable(false);
        bMenuImage->setAutoExclusive(false);

        gridLayout->addWidget(bMenuImage, 6, 2, 1, 1);

        bMenuModify = new QToolButton(QG_CadToolBarMain);
        bMenuModify->setObjectName(QString::fromUtf8("bMenuModify"));
        bMenuModify->setMaximumSize(QSize(32, 32));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/extui/menuedit.png"), QSize(), QIcon::Normal, QIcon::Off);
        bMenuModify->setIcon(icon11);
        bMenuModify->setIconSize(QSize(32, 32));
        bMenuModify->setCheckable(true);
        bMenuModify->setAutoExclusive(true);

        gridLayout->addWidget(bMenuModify, 7, 0, 1, 2);

        bMenuInfo = new QToolButton(QG_CadToolBarMain);
        bMenuInfo->setObjectName(QString::fromUtf8("bMenuInfo"));
        bMenuInfo->setMaximumSize(QSize(32, 32));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/extui/menumeasure.png"), QSize(), QIcon::Normal, QIcon::Off);
        bMenuInfo->setIcon(icon12);
        bMenuInfo->setIconSize(QSize(32, 32));
        bMenuInfo->setCheckable(true);
        bMenuInfo->setAutoExclusive(true);

        gridLayout->addWidget(bMenuInfo, 7, 2, 1, 1);

        bMenuBlock = new QToolButton(QG_CadToolBarMain);
        bMenuBlock->setObjectName(QString::fromUtf8("bMenuBlock"));
        bMenuBlock->setMaximumSize(QSize(32, 32));
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/extui/menublock.png"), QSize(), QIcon::Normal, QIcon::Off);
        bMenuBlock->setIcon(icon13);
        bMenuBlock->setIconSize(QSize(32, 32));
        bMenuBlock->setCheckable(true);
        bMenuBlock->setAutoExclusive(true);

        gridLayout->addWidget(bMenuBlock, 8, 0, 1, 2);

        bMenuSelect = new QToolButton(QG_CadToolBarMain);
        bMenuSelect->setObjectName(QString::fromUtf8("bMenuSelect"));
        bMenuSelect->setMaximumSize(QSize(32, 32));
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/extui/menuselect.png"), QSize(), QIcon::Normal, QIcon::Off);
        bMenuSelect->setIcon(icon14);
        bMenuSelect->setIconSize(QSize(32, 32));
        bMenuSelect->setCheckable(true);
        bMenuSelect->setAutoExclusive(true);

        gridLayout->addWidget(bMenuSelect, 8, 2, 1, 1);

        verticalSpacer = new QSpacerItem(20, 70, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 9, 1, 1, 1);

        bHidden = new QToolButton(QG_CadToolBarMain);
        bHidden->setObjectName(QString::fromUtf8("bHidden"));
        bHidden->setMaximumSize(QSize(0, 0));
        bHidden->setIconSize(QSize(0, 0));
        bHidden->setCheckable(true);
        bHidden->setAutoExclusive(true);

        gridLayout->addWidget(bHidden, 10, 0, 1, 1);

        QWidget::setTabOrder(bMenuPolyline, bMenuSpline);

        retranslateUi(QG_CadToolBarMain);

        QMetaObject::connectSlotsByName(QG_CadToolBarMain);
    } // setupUi

    void retranslateUi(QWidget *QG_CadToolBarMain)
    {
        QG_CadToolBarMain->setWindowTitle(QApplication::translate("QG_CadToolBarMain", "Main", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        bMenuLine->setToolTip(QApplication::translate("QG_CadToolBarMain", "Show toolbar \"Lines\"", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bMenuLine->setText(QString());
#ifndef QT_NO_TOOLTIP
        bMenuArc->setToolTip(QApplication::translate("QG_CadToolBarMain", "Show toolbar \"Arcs\"", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bMenuArc->setText(QString());
#ifndef QT_NO_TOOLTIP
        bMenuCircle->setToolTip(QApplication::translate("QG_CadToolBarMain", "Show toolbar \"Circles\"", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bMenuCircle->setText(QString());
#ifndef QT_NO_TOOLTIP
        bMenuEllipse->setToolTip(QApplication::translate("QG_CadToolBarMain", "Show toolbar \"Ellipses\"", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bMenuEllipse->setText(QString());
#ifndef QT_NO_TOOLTIP
        bMenuPolyline->setToolTip(QApplication::translate("QG_CadToolBarMain", "Show toolbar \"Polylines\"", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bMenuPolyline->setText(QString());
#ifndef QT_NO_TOOLTIP
        bMenuSpline->setToolTip(QApplication::translate("QG_CadToolBarMain", "Draw Splines", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bMenuSpline->setText(QString());
#ifndef QT_NO_TOOLTIP
        bMenuPoint->setToolTip(QApplication::translate("QG_CadToolBarMain", "Draw Points", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bMenuPoint->setText(QString());
#ifndef QT_NO_TOOLTIP
        bMenuText->setToolTip(QApplication::translate("QG_CadToolBarMain", "Insert Text", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bMenuText->setText(QString());
#ifndef QT_NO_TOOLTIP
        bMenuDim->setToolTip(QApplication::translate("QG_CadToolBarMain", "Show toolbar \"Dimensions\"", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bMenuDim->setText(QString());
#ifndef QT_NO_TOOLTIP
        bMenuHatch->setToolTip(QApplication::translate("QG_CadToolBarMain", "Create Hatch", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bMenuHatch->setText(QString());
#ifndef QT_NO_TOOLTIP
        bMenuImage->setToolTip(QApplication::translate("QG_CadToolBarMain", "Insert Image", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bMenuImage->setText(QString());
#ifndef QT_NO_TOOLTIP
        bMenuModify->setToolTip(QApplication::translate("QG_CadToolBarMain", "Show toolbar \"Modify\"", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bMenuModify->setText(QString());
#ifndef QT_NO_TOOLTIP
        bMenuInfo->setToolTip(QApplication::translate("QG_CadToolBarMain", "Show toolbar \"Info\"", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bMenuInfo->setText(QString());
#ifndef QT_NO_TOOLTIP
        bMenuBlock->setToolTip(QApplication::translate("QG_CadToolBarMain", "Create Block", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bMenuBlock->setText(QString());
#ifndef QT_NO_TOOLTIP
        bMenuSelect->setToolTip(QApplication::translate("QG_CadToolBarMain", "Show toolbar \"Select\"", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bMenuSelect->setText(QString());
#ifndef QT_NO_TOOLTIP
        bHidden->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        bHidden->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class QG_CadToolBarMain: public Ui_QG_CadToolBarMain {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QG_CADTOOLBARMAIN_H
