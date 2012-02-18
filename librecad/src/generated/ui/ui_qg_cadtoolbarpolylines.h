/********************************************************************************
** Form generated from reading UI file 'qg_cadtoolbarpolylines.ui'
**
** Created: Sat Feb 18 16:41:50 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QG_CADTOOLBARPOLYLINES_H
#define UI_QG_CADTOOLBARPOLYLINES_H

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

class Ui_QG_CadToolBarPolylines
{
public:
    QGridLayout *gridLayout;
    QToolButton *bBack;
    QToolButton *bPolyline;
    QToolButton *bPolylineAdd;
    QToolButton *bPolylineAppend;
    QToolButton *bPolylineDel;
    QToolButton *bPolylineDelBetween;
    QToolButton *bPolylineTrim;
    QToolButton *bPolylineEquidistant;
    QToolButton *bPolylineSegment;
    QSpacerItem *verticalSpacer;
    QToolButton *bHidden;

    void setupUi(QWidget *QG_CadToolBarPolylines)
    {
        if (QG_CadToolBarPolylines->objectName().isEmpty())
            QG_CadToolBarPolylines->setObjectName(QString::fromUtf8("QG_CadToolBarPolylines"));
        QG_CadToolBarPolylines->resize(65, 336);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QG_CadToolBarPolylines->sizePolicy().hasHeightForWidth());
        QG_CadToolBarPolylines->setSizePolicy(sizePolicy);
        QG_CadToolBarPolylines->setMinimumSize(QSize(65, 336));
        gridLayout = new QGridLayout(QG_CadToolBarPolylines);
        gridLayout->setSpacing(1);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        bBack = new QToolButton(QG_CadToolBarPolylines);
        bBack->setObjectName(QString::fromUtf8("bBack"));
        sizePolicy.setHeightForWidth(bBack->sizePolicy().hasHeightForWidth());
        bBack->setSizePolicy(sizePolicy);
        bBack->setMinimumSize(QSize(0, 20));
        bBack->setMaximumSize(QSize(16777215, 20));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/extui/back.png"), QSize(), QIcon::Normal, QIcon::Off);
        bBack->setIcon(icon);
        bBack->setIconSize(QSize(65, 20));

        gridLayout->addWidget(bBack, 0, 0, 1, 2);

        bPolyline = new QToolButton(QG_CadToolBarPolylines);
        bPolyline->setObjectName(QString::fromUtf8("bPolyline"));
        bPolyline->setMaximumSize(QSize(32, 32));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/extui/polyline.png"), QSize(), QIcon::Normal, QIcon::Off);
        bPolyline->setIcon(icon1);
        bPolyline->setIconSize(QSize(32, 32));
        bPolyline->setCheckable(true);
        bPolyline->setAutoExclusive(true);

        gridLayout->addWidget(bPolyline, 1, 0, 1, 1);

        bPolylineAdd = new QToolButton(QG_CadToolBarPolylines);
        bPolylineAdd->setObjectName(QString::fromUtf8("bPolylineAdd"));
        bPolylineAdd->setMaximumSize(QSize(32, 32));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/extui/polylineadd.png"), QSize(), QIcon::Normal, QIcon::Off);
        bPolylineAdd->setIcon(icon2);
        bPolylineAdd->setIconSize(QSize(32, 32));
        bPolylineAdd->setCheckable(true);
        bPolylineAdd->setAutoExclusive(true);

        gridLayout->addWidget(bPolylineAdd, 1, 1, 1, 1);

        bPolylineAppend = new QToolButton(QG_CadToolBarPolylines);
        bPolylineAppend->setObjectName(QString::fromUtf8("bPolylineAppend"));
        bPolylineAppend->setMaximumSize(QSize(32, 32));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/extui/polylineappend.png"), QSize(), QIcon::Normal, QIcon::Off);
        bPolylineAppend->setIcon(icon3);
        bPolylineAppend->setIconSize(QSize(32, 32));
        bPolylineAppend->setCheckable(true);
        bPolylineAppend->setAutoExclusive(true);

        gridLayout->addWidget(bPolylineAppend, 2, 0, 1, 1);

        bPolylineDel = new QToolButton(QG_CadToolBarPolylines);
        bPolylineDel->setObjectName(QString::fromUtf8("bPolylineDel"));
        bPolylineDel->setMaximumSize(QSize(32, 32));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/extui/polylinedel.png"), QSize(), QIcon::Normal, QIcon::Off);
        bPolylineDel->setIcon(icon4);
        bPolylineDel->setIconSize(QSize(32, 32));
        bPolylineDel->setCheckable(true);
        bPolylineDel->setAutoExclusive(true);

        gridLayout->addWidget(bPolylineDel, 2, 1, 1, 1);

        bPolylineDelBetween = new QToolButton(QG_CadToolBarPolylines);
        bPolylineDelBetween->setObjectName(QString::fromUtf8("bPolylineDelBetween"));
        bPolylineDelBetween->setMaximumSize(QSize(32, 32));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/extui/polylinedelbetween.png"), QSize(), QIcon::Normal, QIcon::Off);
        bPolylineDelBetween->setIcon(icon5);
        bPolylineDelBetween->setIconSize(QSize(32, 32));
        bPolylineDelBetween->setCheckable(true);
        bPolylineDelBetween->setAutoExclusive(true);

        gridLayout->addWidget(bPolylineDelBetween, 3, 0, 1, 1);

        bPolylineTrim = new QToolButton(QG_CadToolBarPolylines);
        bPolylineTrim->setObjectName(QString::fromUtf8("bPolylineTrim"));
        bPolylineTrim->setMaximumSize(QSize(32, 32));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/extui/polylinetrim.png"), QSize(), QIcon::Normal, QIcon::Off);
        bPolylineTrim->setIcon(icon6);
        bPolylineTrim->setIconSize(QSize(32, 32));
        bPolylineTrim->setCheckable(true);
        bPolylineTrim->setAutoExclusive(true);

        gridLayout->addWidget(bPolylineTrim, 3, 1, 1, 1);

        bPolylineEquidistant = new QToolButton(QG_CadToolBarPolylines);
        bPolylineEquidistant->setObjectName(QString::fromUtf8("bPolylineEquidistant"));
        bPolylineEquidistant->setMaximumSize(QSize(32, 32));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/extui/polylineequidstant.png"), QSize(), QIcon::Normal, QIcon::Off);
        bPolylineEquidistant->setIcon(icon7);
        bPolylineEquidistant->setIconSize(QSize(32, 32));
        bPolylineEquidistant->setCheckable(true);
        bPolylineEquidistant->setAutoExclusive(true);

        gridLayout->addWidget(bPolylineEquidistant, 4, 0, 1, 1);

        bPolylineSegment = new QToolButton(QG_CadToolBarPolylines);
        bPolylineSegment->setObjectName(QString::fromUtf8("bPolylineSegment"));
        bPolylineSegment->setMaximumSize(QSize(32, 32));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/extui/polylinesegment.png"), QSize(), QIcon::Normal, QIcon::Off);
        bPolylineSegment->setIcon(icon8);
        bPolylineSegment->setIconSize(QSize(32, 32));
        bPolylineSegment->setCheckable(true);
        bPolylineSegment->setAutoExclusive(true);

        gridLayout->addWidget(bPolylineSegment, 4, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 5, 0, 1, 1);

        bHidden = new QToolButton(QG_CadToolBarPolylines);
        bHidden->setObjectName(QString::fromUtf8("bHidden"));
        bHidden->setMaximumSize(QSize(0, 0));
        bHidden->setText(QString::fromUtf8(""));
        bHidden->setIconSize(QSize(0, 0));
        bHidden->setCheckable(true);
        bHidden->setAutoExclusive(true);

        gridLayout->addWidget(bHidden, 6, 0, 1, 1);


        retranslateUi(QG_CadToolBarPolylines);
        QObject::connect(bPolyline, SIGNAL(clicked()), QG_CadToolBarPolylines, SLOT(drawPolyline()));
        QObject::connect(bPolylineAdd, SIGNAL(clicked()), QG_CadToolBarPolylines, SLOT(polylineAdd()));
        QObject::connect(bPolylineDel, SIGNAL(clicked()), QG_CadToolBarPolylines, SLOT(polylineDel()));
        QObject::connect(bPolylineDelBetween, SIGNAL(clicked()), QG_CadToolBarPolylines, SLOT(polylineDelBetween()));
        QObject::connect(bPolylineTrim, SIGNAL(clicked()), QG_CadToolBarPolylines, SLOT(polylineTrim()));
        QObject::connect(bPolylineAppend, SIGNAL(clicked()), QG_CadToolBarPolylines, SLOT(polylineAppend()));
        QObject::connect(bPolylineEquidistant, SIGNAL(clicked()), QG_CadToolBarPolylines, SLOT(polylineEquidistant()));
        QObject::connect(bPolylineSegment, SIGNAL(clicked()), QG_CadToolBarPolylines, SLOT(polylineSegment()));

        QMetaObject::connectSlotsByName(QG_CadToolBarPolylines);
    } // setupUi

    void retranslateUi(QWidget *QG_CadToolBarPolylines)
    {
        QG_CadToolBarPolylines->setWindowTitle(QApplication::translate("QG_CadToolBarPolylines", "Polylines", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        bBack->setToolTip(QApplication::translate("QG_CadToolBarPolylines", "Back to main menu", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bBack->setText(QString());
#ifndef QT_NO_TOOLTIP
        bPolyline->setToolTip(QApplication::translate("QG_CadToolBarPolylines", "Create Polyline", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bPolyline->setText(QString());
#ifndef QT_NO_TOOLTIP
        bPolylineAdd->setToolTip(QApplication::translate("QG_CadToolBarPolylines", "Add node", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bPolylineAdd->setText(QString());
#ifndef QT_NO_TOOLTIP
        bPolylineAppend->setToolTip(QApplication::translate("QG_CadToolBarPolylines", "Append node", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bPolylineAppend->setText(QString());
#ifndef QT_NO_TOOLTIP
        bPolylineDel->setToolTip(QApplication::translate("QG_CadToolBarPolylines", "Delete node", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bPolylineDel->setText(QString());
#ifndef QT_NO_TOOLTIP
        bPolylineDelBetween->setToolTip(QApplication::translate("QG_CadToolBarPolylines", "Delete between two nodes", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bPolylineDelBetween->setText(QString());
#ifndef QT_NO_TOOLTIP
        bPolylineTrim->setToolTip(QApplication::translate("QG_CadToolBarPolylines", "Trim segments", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bPolylineTrim->setText(QString());
#ifndef QT_NO_TOOLTIP
        bPolylineEquidistant->setToolTip(QApplication::translate("QG_CadToolBarPolylines", "Polyline equidistant", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bPolylineEquidistant->setText(QString());
#ifndef QT_NO_TOOLTIP
        bPolylineSegment->setToolTip(QApplication::translate("QG_CadToolBarPolylines", "Create Polyline from Existing Segments", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bPolylineSegment->setText(QString());
#ifndef QT_NO_TOOLTIP
        bHidden->setToolTip(QApplication::translate("QG_CadToolBarPolylines", "Concentric", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
    } // retranslateUi

};

namespace Ui {
    class QG_CadToolBarPolylines: public Ui_QG_CadToolBarPolylines {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QG_CADTOOLBARPOLYLINES_H
