/********************************************************************************
** Form generated from reading UI file 'qg_cadtoolbararcs.ui'
**
** Created: Sat Feb 18 16:41:50 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QG_CADTOOLBARARCS_H
#define UI_QG_CADTOOLBARARCS_H

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

class Ui_QG_CadToolBarArcs
{
public:
    QGridLayout *gridLayout;
    QToolButton *bBack;
    QToolButton *bArc;
    QToolButton *bArc3P;
    QToolButton *bArcParallel;
    QToolButton *bArcTangential;
    QSpacerItem *verticalSpacer;
    QToolButton *bHidden;

    void setupUi(QWidget *QG_CadToolBarArcs)
    {
        if (QG_CadToolBarArcs->objectName().isEmpty())
            QG_CadToolBarArcs->setObjectName(QString::fromUtf8("QG_CadToolBarArcs"));
        QG_CadToolBarArcs->resize(65, 336);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QG_CadToolBarArcs->sizePolicy().hasHeightForWidth());
        QG_CadToolBarArcs->setSizePolicy(sizePolicy);
        QG_CadToolBarArcs->setMinimumSize(QSize(65, 336));
        gridLayout = new QGridLayout(QG_CadToolBarArcs);
        gridLayout->setSpacing(1);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        bBack = new QToolButton(QG_CadToolBarArcs);
        bBack->setObjectName(QString::fromUtf8("bBack"));
        bBack->setMaximumSize(QSize(65, 20));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/extui/back.png"), QSize(), QIcon::Normal, QIcon::Off);
        bBack->setIcon(icon);
        bBack->setIconSize(QSize(65, 20));

        gridLayout->addWidget(bBack, 0, 0, 1, 2);

        bArc = new QToolButton(QG_CadToolBarArcs);
        bArc->setObjectName(QString::fromUtf8("bArc"));
        bArc->setMaximumSize(QSize(32, 32));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/extui/arcscraa.png"), QSize(), QIcon::Normal, QIcon::Off);
        bArc->setIcon(icon1);
        bArc->setCheckable(true);
        bArc->setAutoExclusive(true);

        gridLayout->addWidget(bArc, 1, 0, 1, 1);

        bArc3P = new QToolButton(QG_CadToolBarArcs);
        bArc3P->setObjectName(QString::fromUtf8("bArc3P"));
        bArc3P->setMaximumSize(QSize(32, 32));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/extui/arcs3p.png"), QSize(), QIcon::Normal, QIcon::Off);
        bArc3P->setIcon(icon2);
        bArc3P->setCheckable(true);
        bArc3P->setAutoExclusive(true);

        gridLayout->addWidget(bArc3P, 1, 1, 1, 1);

        bArcParallel = new QToolButton(QG_CadToolBarArcs);
        bArcParallel->setObjectName(QString::fromUtf8("bArcParallel"));
        bArcParallel->setMaximumSize(QSize(32, 32));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/extui/arcspara.png"), QSize(), QIcon::Normal, QIcon::Off);
        bArcParallel->setIcon(icon3);
        bArcParallel->setCheckable(true);
        bArcParallel->setAutoExclusive(true);

        gridLayout->addWidget(bArcParallel, 2, 0, 1, 1);

        bArcTangential = new QToolButton(QG_CadToolBarArcs);
        bArcTangential->setObjectName(QString::fromUtf8("bArcTangential"));
        bArcTangential->setMaximumSize(QSize(32, 32));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/extui/arcstangential.png"), QSize(), QIcon::Normal, QIcon::Off);
        bArcTangential->setIcon(icon4);
        bArcTangential->setCheckable(true);
        bArcTangential->setAutoExclusive(true);

        gridLayout->addWidget(bArcTangential, 2, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 4, 0, 1, 1);

        bHidden = new QToolButton(QG_CadToolBarArcs);
        bHidden->setObjectName(QString::fromUtf8("bHidden"));
        bHidden->setMaximumSize(QSize(0, 0));
        bHidden->setText(QString::fromUtf8(""));
        bHidden->setIconSize(QSize(0, 0));
        bHidden->setCheckable(true);
        bHidden->setAutoExclusive(true);

        gridLayout->addWidget(bHidden, 3, 0, 1, 1);

        QWidget::setTabOrder(bBack, bArc);
        QWidget::setTabOrder(bArc, bArc3P);
        QWidget::setTabOrder(bArc3P, bArcParallel);

        retranslateUi(QG_CadToolBarArcs);
        QObject::connect(bArc, SIGNAL(clicked()), QG_CadToolBarArcs, SLOT(drawArc()));
        QObject::connect(bArc3P, SIGNAL(clicked()), QG_CadToolBarArcs, SLOT(drawArc3P()));
        QObject::connect(bArcParallel, SIGNAL(clicked()), QG_CadToolBarArcs, SLOT(drawArcParallel()));
        QObject::connect(bArcTangential, SIGNAL(clicked()), QG_CadToolBarArcs, SLOT(drawArcTangential()));

        QMetaObject::connectSlotsByName(QG_CadToolBarArcs);
    } // setupUi

    void retranslateUi(QWidget *QG_CadToolBarArcs)
    {
        QG_CadToolBarArcs->setWindowTitle(QApplication::translate("QG_CadToolBarArcs", "Arcs", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        bBack->setToolTip(QApplication::translate("QG_CadToolBarArcs", "Back to main menu", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bBack->setText(QString());
#ifndef QT_NO_TOOLTIP
        bArc->setToolTip(QApplication::translate("QG_CadToolBarArcs", "Arc with Center, Point, Angles", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bArc->setText(QString());
#ifndef QT_NO_TOOLTIP
        bArc3P->setToolTip(QApplication::translate("QG_CadToolBarArcs", "Arc with three points", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bArc3P->setText(QString());
#ifndef QT_NO_TOOLTIP
        bArcParallel->setToolTip(QApplication::translate("QG_CadToolBarArcs", "Concentric", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bArcParallel->setText(QString());
#ifndef QT_NO_TOOLTIP
        bArcTangential->setToolTip(QApplication::translate("QG_CadToolBarArcs", "Arc tangential to base entity with radius", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bArcTangential->setText(QString());
#ifndef QT_NO_TOOLTIP
        bHidden->setToolTip(QApplication::translate("QG_CadToolBarArcs", "Arc with Center, Point, Angles", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
    } // retranslateUi

};

namespace Ui {
    class QG_CadToolBarArcs: public Ui_QG_CadToolBarArcs {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QG_CADTOOLBARARCS_H
