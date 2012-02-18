/********************************************************************************
** Form generated from reading UI file 'qg_cadtoolbarcircles.ui'
**
** Created: Sat Feb 18 16:41:50 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QG_CADTOOLBARCIRCLES_H
#define UI_QG_CADTOOLBARCIRCLES_H

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

class Ui_QG_CadToolBarCircles
{
public:
    QGridLayout *gridLayout;
    QToolButton *bBack;
    QToolButton *bCircle;
    QToolButton *bCircleCR;
    QToolButton *bCircle2P;
    QToolButton *bCircle3P;
    QToolButton *bCircleParallel;
    QSpacerItem *verticalSpacer;
    QToolButton *bHidden;
    QToolButton *bCircleInscribe;

    void setupUi(QWidget *QG_CadToolBarCircles)
    {
        if (QG_CadToolBarCircles->objectName().isEmpty())
            QG_CadToolBarCircles->setObjectName(QString::fromUtf8("QG_CadToolBarCircles"));
        QG_CadToolBarCircles->resize(65, 336);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QG_CadToolBarCircles->sizePolicy().hasHeightForWidth());
        QG_CadToolBarCircles->setSizePolicy(sizePolicy);
        QG_CadToolBarCircles->setMinimumSize(QSize(65, 336));
        gridLayout = new QGridLayout(QG_CadToolBarCircles);
        gridLayout->setSpacing(1);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        bBack = new QToolButton(QG_CadToolBarCircles);
        bBack->setObjectName(QString::fromUtf8("bBack"));
        bBack->setMaximumSize(QSize(65, 20));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/extui/back.png"), QSize(), QIcon::Normal, QIcon::Off);
        bBack->setIcon(icon);
        bBack->setIconSize(QSize(65, 20));

        gridLayout->addWidget(bBack, 0, 0, 1, 3);

        bCircle = new QToolButton(QG_CadToolBarCircles);
        bCircle->setObjectName(QString::fromUtf8("bCircle"));
        bCircle->setMaximumSize(QSize(32, 32));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/extui/circles.png"), QSize(), QIcon::Normal, QIcon::Off);
        bCircle->setIcon(icon1);
        bCircle->setIconSize(QSize(32, 32));
        bCircle->setCheckable(true);
        bCircle->setAutoExclusive(true);

        gridLayout->addWidget(bCircle, 1, 0, 1, 2);

        bCircleCR = new QToolButton(QG_CadToolBarCircles);
        bCircleCR->setObjectName(QString::fromUtf8("bCircleCR"));
        bCircleCR->setMaximumSize(QSize(32, 32));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/extui/circlescr.png"), QSize(), QIcon::Normal, QIcon::Off);
        bCircleCR->setIcon(icon2);
        bCircleCR->setIconSize(QSize(32, 32));
        bCircleCR->setCheckable(true);
        bCircleCR->setAutoExclusive(true);

        gridLayout->addWidget(bCircleCR, 1, 2, 1, 1);

        bCircle2P = new QToolButton(QG_CadToolBarCircles);
        bCircle2P->setObjectName(QString::fromUtf8("bCircle2P"));
        bCircle2P->setMaximumSize(QSize(32, 32));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/extui/circles2p.png"), QSize(), QIcon::Normal, QIcon::Off);
        bCircle2P->setIcon(icon3);
        bCircle2P->setIconSize(QSize(32, 32));
        bCircle2P->setCheckable(true);
        bCircle2P->setAutoExclusive(true);

        gridLayout->addWidget(bCircle2P, 2, 0, 2, 2);

        bCircle3P = new QToolButton(QG_CadToolBarCircles);
        bCircle3P->setObjectName(QString::fromUtf8("bCircle3P"));
        bCircle3P->setMaximumSize(QSize(32, 32));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/extui/circles3p.png"), QSize(), QIcon::Normal, QIcon::Off);
        bCircle3P->setIcon(icon4);
        bCircle3P->setIconSize(QSize(32, 32));
        bCircle3P->setCheckable(true);
        bCircle3P->setAutoExclusive(true);

        gridLayout->addWidget(bCircle3P, 2, 2, 2, 1);

        bCircleParallel = new QToolButton(QG_CadToolBarCircles);
        bCircleParallel->setObjectName(QString::fromUtf8("bCircleParallel"));
        bCircleParallel->setMaximumSize(QSize(32, 32));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/extui/circlespara.png"), QSize(), QIcon::Normal, QIcon::Off);
        bCircleParallel->setIcon(icon5);
        bCircleParallel->setIconSize(QSize(32, 32));
        bCircleParallel->setCheckable(true);
        bCircleParallel->setAutoExclusive(true);

        gridLayout->addWidget(bCircleParallel, 4, 0, 1, 2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 5, 0, 1, 1);

        bHidden = new QToolButton(QG_CadToolBarCircles);
        bHidden->setObjectName(QString::fromUtf8("bHidden"));
        bHidden->setMaximumSize(QSize(0, 0));
        bHidden->setText(QString::fromUtf8(""));
        bHidden->setIconSize(QSize(0, 0));
        bHidden->setCheckable(true);
        bHidden->setAutoExclusive(true);

        gridLayout->addWidget(bHidden, 6, 0, 1, 1);

        bCircleInscribe = new QToolButton(QG_CadToolBarCircles);
        bCircleInscribe->setObjectName(QString::fromUtf8("bCircleInscribe"));
        bCircleInscribe->setMaximumSize(QSize(32, 32));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/extui/circleinscribe.png"), QSize(), QIcon::Normal, QIcon::Off);
        bCircleInscribe->setIcon(icon6);
        bCircleInscribe->setIconSize(QSize(32, 32));
        bCircleInscribe->setCheckable(true);
        bCircleInscribe->setAutoExclusive(true);

        gridLayout->addWidget(bCircleInscribe, 4, 2, 1, 1);

        QWidget::setTabOrder(bBack, bCircle);
        QWidget::setTabOrder(bCircle, bCircleCR);
        QWidget::setTabOrder(bCircleCR, bCircle2P);
        QWidget::setTabOrder(bCircle2P, bCircle3P);
        QWidget::setTabOrder(bCircle3P, bCircleParallel);

        retranslateUi(QG_CadToolBarCircles);
        QObject::connect(bCircle, SIGNAL(clicked()), QG_CadToolBarCircles, SLOT(drawCircle()));
        QObject::connect(bCircleCR, SIGNAL(clicked()), QG_CadToolBarCircles, SLOT(drawCircleCR()));
        QObject::connect(bCircle2P, SIGNAL(clicked()), QG_CadToolBarCircles, SLOT(drawCircle2P()));
        QObject::connect(bCircle3P, SIGNAL(clicked()), QG_CadToolBarCircles, SLOT(drawCircle3P()));
        QObject::connect(bCircleParallel, SIGNAL(clicked()), QG_CadToolBarCircles, SLOT(drawCircleParallel()));
        QObject::connect(bCircleInscribe, SIGNAL(clicked()), QG_CadToolBarCircles, SLOT(drawCircleInscribe()));

        QMetaObject::connectSlotsByName(QG_CadToolBarCircles);
    } // setupUi

    void retranslateUi(QWidget *QG_CadToolBarCircles)
    {
        QG_CadToolBarCircles->setWindowTitle(QApplication::translate("QG_CadToolBarCircles", "Circles", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        bBack->setToolTip(QApplication::translate("QG_CadToolBarCircles", "Back to main menu", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bBack->setText(QString());
#ifndef QT_NO_TOOLTIP
        bCircle->setToolTip(QApplication::translate("QG_CadToolBarCircles", "Circle with center and point", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bCircle->setText(QString());
#ifndef QT_NO_TOOLTIP
        bCircleCR->setToolTip(QApplication::translate("QG_CadToolBarCircles", "Circle with center and radius", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bCircleCR->setText(QString());
#ifndef QT_NO_TOOLTIP
        bCircle2P->setToolTip(QApplication::translate("QG_CadToolBarCircles", "Circle with two opposite points", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bCircle2P->setText(QString());
#ifndef QT_NO_TOOLTIP
        bCircle3P->setToolTip(QApplication::translate("QG_CadToolBarCircles", "Circle with three points", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bCircle3P->setText(QString());
#ifndef QT_NO_TOOLTIP
        bCircleParallel->setToolTip(QApplication::translate("QG_CadToolBarCircles", "Concentric", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bCircleParallel->setText(QString());
#ifndef QT_NO_TOOLTIP
        bHidden->setToolTip(QApplication::translate("QG_CadToolBarCircles", "Concentric", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        bCircleInscribe->setToolTip(QApplication::translate("QG_CadToolBarCircles", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Draw Circle inscribed in a triangle, <span style=\" font-style:italic;\">i.e.</span>, tangential to 3 lines</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bCircleInscribe->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class QG_CadToolBarCircles: public Ui_QG_CadToolBarCircles {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QG_CADTOOLBARCIRCLES_H
