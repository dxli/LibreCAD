/********************************************************************************
** Form generated from reading UI file 'qg_cadtoolbarellipses.ui'
**
** Created: Sat Feb 18 16:41:50 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QG_CADTOOLBARELLIPSES_H
#define UI_QG_CADTOOLBARELLIPSES_H

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

class Ui_QG_CadToolBarEllipses
{
public:
    QGridLayout *gridLayout;
    QToolButton *bBack;
    QToolButton *bEllipseAxes;
    QToolButton *bEllipseArcAxes;
    QToolButton *bEllipseFociPoint;
    QToolButton *bEllipse4Points;
    QToolButton *bEllipseCenter3Points;
    QToolButton *bEllipseInscribe;
    QSpacerItem *verticalSpacer;
    QToolButton *bHidden;

    void setupUi(QWidget *QG_CadToolBarEllipses)
    {
        if (QG_CadToolBarEllipses->objectName().isEmpty())
            QG_CadToolBarEllipses->setObjectName(QString::fromUtf8("QG_CadToolBarEllipses"));
        QG_CadToolBarEllipses->resize(65, 336);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QG_CadToolBarEllipses->sizePolicy().hasHeightForWidth());
        QG_CadToolBarEllipses->setSizePolicy(sizePolicy);
        QG_CadToolBarEllipses->setMinimumSize(QSize(65, 336));
        gridLayout = new QGridLayout(QG_CadToolBarEllipses);
        gridLayout->setSpacing(1);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        bBack = new QToolButton(QG_CadToolBarEllipses);
        bBack->setObjectName(QString::fromUtf8("bBack"));
        bBack->setMinimumSize(QSize(65, 20));
        bBack->setMaximumSize(QSize(65, 20));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/extui/back.png"), QSize(), QIcon::Normal, QIcon::Off);
        bBack->setIcon(icon);
        bBack->setIconSize(QSize(65, 20));

        gridLayout->addWidget(bBack, 0, 0, 1, 3);

        bEllipseAxes = new QToolButton(QG_CadToolBarEllipses);
        bEllipseAxes->setObjectName(QString::fromUtf8("bEllipseAxes"));
        bEllipseAxes->setMaximumSize(QSize(32, 32));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/extui/ellipsesaxes.png"), QSize(), QIcon::Normal, QIcon::Off);
        bEllipseAxes->setIcon(icon1);
        bEllipseAxes->setIconSize(QSize(32, 32));
        bEllipseAxes->setCheckable(true);
        bEllipseAxes->setAutoExclusive(true);

        gridLayout->addWidget(bEllipseAxes, 1, 0, 1, 2);

        bEllipseArcAxes = new QToolButton(QG_CadToolBarEllipses);
        bEllipseArcAxes->setObjectName(QString::fromUtf8("bEllipseArcAxes"));
        bEllipseArcAxes->setMaximumSize(QSize(32, 32));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/extui/ellipsearcsaxes.png"), QSize(), QIcon::Normal, QIcon::Off);
        bEllipseArcAxes->setIcon(icon2);
        bEllipseArcAxes->setIconSize(QSize(32, 32));
        bEllipseArcAxes->setCheckable(true);
        bEllipseArcAxes->setAutoExclusive(true);

        gridLayout->addWidget(bEllipseArcAxes, 1, 2, 1, 1);

        bEllipseFociPoint = new QToolButton(QG_CadToolBarEllipses);
        bEllipseFociPoint->setObjectName(QString::fromUtf8("bEllipseFociPoint"));
        bEllipseFociPoint->setMaximumSize(QSize(32, 32));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/extui/ellipsefocipoint.png"), QSize(), QIcon::Normal, QIcon::Off);
        bEllipseFociPoint->setIcon(icon3);
        bEllipseFociPoint->setIconSize(QSize(32, 32));
        bEllipseFociPoint->setCheckable(true);
        bEllipseFociPoint->setAutoExclusive(true);

        gridLayout->addWidget(bEllipseFociPoint, 2, 0, 1, 2);

        bEllipse4Points = new QToolButton(QG_CadToolBarEllipses);
        bEllipse4Points->setObjectName(QString::fromUtf8("bEllipse4Points"));
        bEllipse4Points->setMaximumSize(QSize(32, 32));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/extui/ellipse4points.png"), QSize(), QIcon::Normal, QIcon::Off);
        bEllipse4Points->setIcon(icon4);
        bEllipse4Points->setIconSize(QSize(32, 32));
        bEllipse4Points->setCheckable(true);
        bEllipse4Points->setAutoExclusive(true);

        gridLayout->addWidget(bEllipse4Points, 2, 2, 1, 1);

        bEllipseCenter3Points = new QToolButton(QG_CadToolBarEllipses);
        bEllipseCenter3Points->setObjectName(QString::fromUtf8("bEllipseCenter3Points"));
        bEllipseCenter3Points->setMaximumSize(QSize(32, 32));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/extui/ellipsecenter3points.png"), QSize(), QIcon::Normal, QIcon::Off);
        bEllipseCenter3Points->setIcon(icon5);
        bEllipseCenter3Points->setIconSize(QSize(32, 32));
        bEllipseCenter3Points->setCheckable(true);
        bEllipseCenter3Points->setAutoExclusive(true);

        gridLayout->addWidget(bEllipseCenter3Points, 3, 0, 1, 1);

        bEllipseInscribe = new QToolButton(QG_CadToolBarEllipses);
        bEllipseInscribe->setObjectName(QString::fromUtf8("bEllipseInscribe"));
        bEllipseInscribe->setMaximumSize(QSize(32, 32));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/extui/ellipseinscribed.png"), QSize(), QIcon::Normal, QIcon::Off);
        bEllipseInscribe->setIcon(icon6);
        bEllipseInscribe->setIconSize(QSize(32, 32));
        bEllipseInscribe->setCheckable(true);
        bEllipseInscribe->setAutoExclusive(true);

        gridLayout->addWidget(bEllipseInscribe, 3, 1, 1, 2);

        verticalSpacer = new QSpacerItem(20, 245, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 4, 0, 1, 2);

        bHidden = new QToolButton(QG_CadToolBarEllipses);
        bHidden->setObjectName(QString::fromUtf8("bHidden"));
        bHidden->setMaximumSize(QSize(0, 0));
        bHidden->setText(QString::fromUtf8(""));
        bHidden->setIconSize(QSize(0, 0));
        bHidden->setCheckable(true);
        bHidden->setAutoExclusive(true);

        gridLayout->addWidget(bHidden, 5, 0, 1, 1);


        retranslateUi(QG_CadToolBarEllipses);
        QObject::connect(bEllipseAxes, SIGNAL(clicked()), QG_CadToolBarEllipses, SLOT(drawEllipseAxis()));
        QObject::connect(bEllipseArcAxes, SIGNAL(clicked()), QG_CadToolBarEllipses, SLOT(drawEllipseArcAxis()));
        QObject::connect(bEllipseFociPoint, SIGNAL(clicked()), QG_CadToolBarEllipses, SLOT(drawEllipseFociPoint()));
        QObject::connect(bEllipse4Points, SIGNAL(clicked()), QG_CadToolBarEllipses, SLOT(drawEllipse4Points()));
        QObject::connect(bEllipseInscribe, SIGNAL(clicked()), QG_CadToolBarEllipses, SLOT(drawEllipseInscribe()));
        QObject::connect(bEllipseCenter3Points, SIGNAL(clicked()), QG_CadToolBarEllipses, SLOT(drawEllipseCenter3Points()));

        QMetaObject::connectSlotsByName(QG_CadToolBarEllipses);
    } // setupUi

    void retranslateUi(QWidget *QG_CadToolBarEllipses)
    {
        QG_CadToolBarEllipses->setWindowTitle(QApplication::translate("QG_CadToolBarEllipses", "Ellipses", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        bBack->setToolTip(QApplication::translate("QG_CadToolBarEllipses", "Back to main menu", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bBack->setText(QString());
#ifndef QT_NO_TOOLTIP
        bEllipseAxes->setToolTip(QApplication::translate("QG_CadToolBarEllipses", "Ellipse with Center and two points", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bEllipseAxes->setText(QString());
#ifndef QT_NO_TOOLTIP
        bEllipseArcAxes->setToolTip(QApplication::translate("QG_CadToolBarEllipses", "Ellipse arc with center, two points and angles", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bEllipseArcAxes->setText(QString());
#ifndef QT_NO_TOOLTIP
        bEllipseFociPoint->setToolTip(QApplication::translate("QG_CadToolBarEllipses", "Draw Ellipse by foci and a point on ellipse", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bEllipseFociPoint->setText(QString());
#ifndef QT_NO_TOOLTIP
        bEllipse4Points->setToolTip(QApplication::translate("QG_CadToolBarEllipses", "Draw Ellipse with axes in x-/y-directions by four points on ellipse", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bEllipse4Points->setText(QString());
#ifndef QT_NO_TOOLTIP
        bEllipseCenter3Points->setToolTip(QApplication::translate("QG_CadToolBarEllipses", "<html><head/><body><p>Draw Ellipse by center and 3 points on ellipse</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bEllipseCenter3Points->setText(QString());
#ifndef QT_NO_TOOLTIP
        bEllipseInscribe->setToolTip(QApplication::translate("QG_CadToolBarEllipses", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Draw Ellipse inscribed in a quadrilateral, <span style=\" font-style:italic;\">i.e.</span>, tangential to four lines</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bEllipseInscribe->setText(QString());
#ifndef QT_NO_TOOLTIP
        bHidden->setToolTip(QApplication::translate("QG_CadToolBarEllipses", "Concentric", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
    } // retranslateUi

};

namespace Ui {
    class QG_CadToolBarEllipses: public Ui_QG_CadToolBarEllipses {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QG_CADTOOLBARELLIPSES_H
