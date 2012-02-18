/********************************************************************************
** Form generated from reading UI file 'qg_cadtoolbarlines.ui'
**
** Created: Sat Feb 18 16:41:50 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QG_CADTOOLBARLINES_H
#define UI_QG_CADTOOLBARLINES_H

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

class Ui_QG_CadToolBarLines
{
public:
    QGridLayout *gridLayout;
    QToolButton *bBack;
    QToolButton *bNormal;
    QToolButton *bAngle;
    QToolButton *bHorizontal;
    QToolButton *bVertical;
    QToolButton *bRectangle;
    QToolButton *bBisector;
    QToolButton *bParallel;
    QToolButton *bParallelThrough;
    QToolButton *bTangent1;
    QToolButton *bTangent2;
    QToolButton *bOrthTan;
    QToolButton *bOrthogonal;
    QToolButton *bRelAngle;
    QToolButton *bPolygon;
    QToolButton *bPolygon2;
    QToolButton *bFree;
    QToolButton *bHidden;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *QG_CadToolBarLines)
    {
        if (QG_CadToolBarLines->objectName().isEmpty())
            QG_CadToolBarLines->setObjectName(QString::fromUtf8("QG_CadToolBarLines"));
        QG_CadToolBarLines->resize(65, 338);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QG_CadToolBarLines->sizePolicy().hasHeightForWidth());
        QG_CadToolBarLines->setSizePolicy(sizePolicy);
        QG_CadToolBarLines->setMinimumSize(QSize(65, 336));
        gridLayout = new QGridLayout(QG_CadToolBarLines);
        gridLayout->setSpacing(1);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        bBack = new QToolButton(QG_CadToolBarLines);
        bBack->setObjectName(QString::fromUtf8("bBack"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(bBack->sizePolicy().hasHeightForWidth());
        bBack->setSizePolicy(sizePolicy1);
        bBack->setMaximumSize(QSize(65, 20));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/extui/back.png"), QSize(), QIcon::Normal, QIcon::Off);
        bBack->setIcon(icon);
        bBack->setIconSize(QSize(65, 20));

        gridLayout->addWidget(bBack, 0, 0, 1, 2);

        bNormal = new QToolButton(QG_CadToolBarLines);
        bNormal->setObjectName(QString::fromUtf8("bNormal"));
        bNormal->setMaximumSize(QSize(32, 32));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/extui/linesnormal.png"), QSize(), QIcon::Normal, QIcon::Off);
        bNormal->setIcon(icon1);
        bNormal->setIconSize(QSize(32, 32));
        bNormal->setCheckable(true);
        bNormal->setAutoExclusive(true);

        gridLayout->addWidget(bNormal, 1, 0, 1, 1);

        bAngle = new QToolButton(QG_CadToolBarLines);
        bAngle->setObjectName(QString::fromUtf8("bAngle"));
        bAngle->setMaximumSize(QSize(32, 32));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/extui/linesangle.png"), QSize(), QIcon::Normal, QIcon::Off);
        bAngle->setIcon(icon2);
        bAngle->setIconSize(QSize(32, 32));
        bAngle->setCheckable(true);
        bAngle->setAutoExclusive(true);

        gridLayout->addWidget(bAngle, 1, 1, 1, 1);

        bHorizontal = new QToolButton(QG_CadToolBarLines);
        bHorizontal->setObjectName(QString::fromUtf8("bHorizontal"));
        bHorizontal->setMaximumSize(QSize(32, 32));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/extui/lineshor.png"), QSize(), QIcon::Normal, QIcon::Off);
        bHorizontal->setIcon(icon3);
        bHorizontal->setIconSize(QSize(32, 32));
        bHorizontal->setCheckable(true);
        bHorizontal->setAutoExclusive(true);

        gridLayout->addWidget(bHorizontal, 2, 0, 1, 1);

        bVertical = new QToolButton(QG_CadToolBarLines);
        bVertical->setObjectName(QString::fromUtf8("bVertical"));
        bVertical->setMaximumSize(QSize(32, 32));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/extui/linesver.png"), QSize(), QIcon::Normal, QIcon::Off);
        bVertical->setIcon(icon4);
        bVertical->setIconSize(QSize(32, 32));
        bVertical->setCheckable(true);
        bVertical->setAutoExclusive(true);

        gridLayout->addWidget(bVertical, 2, 1, 1, 1);

        bRectangle = new QToolButton(QG_CadToolBarLines);
        bRectangle->setObjectName(QString::fromUtf8("bRectangle"));
        bRectangle->setMaximumSize(QSize(32, 32));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/extui/linesrect.png"), QSize(), QIcon::Normal, QIcon::Off);
        bRectangle->setIcon(icon5);
        bRectangle->setIconSize(QSize(32, 32));
        bRectangle->setCheckable(true);
        bRectangle->setAutoExclusive(true);

        gridLayout->addWidget(bRectangle, 3, 0, 1, 1);

        bBisector = new QToolButton(QG_CadToolBarLines);
        bBisector->setObjectName(QString::fromUtf8("bBisector"));
        bBisector->setMaximumSize(QSize(32, 32));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/extui/linesbisector.png"), QSize(), QIcon::Normal, QIcon::Off);
        bBisector->setIcon(icon6);
        bBisector->setIconSize(QSize(32, 32));
        bBisector->setCheckable(true);
        bBisector->setAutoExclusive(true);

        gridLayout->addWidget(bBisector, 3, 1, 1, 1);

        bParallel = new QToolButton(QG_CadToolBarLines);
        bParallel->setObjectName(QString::fromUtf8("bParallel"));
        bParallel->setMaximumSize(QSize(32, 32));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/extui/linespara.png"), QSize(), QIcon::Normal, QIcon::Off);
        bParallel->setIcon(icon7);
        bParallel->setIconSize(QSize(32, 32));
        bParallel->setCheckable(true);
        bParallel->setAutoExclusive(true);

        gridLayout->addWidget(bParallel, 4, 0, 1, 1);

        bParallelThrough = new QToolButton(QG_CadToolBarLines);
        bParallelThrough->setObjectName(QString::fromUtf8("bParallelThrough"));
        bParallelThrough->setMaximumSize(QSize(32, 32));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/extui/linesparathrough.png"), QSize(), QIcon::Normal, QIcon::Off);
        bParallelThrough->setIcon(icon8);
        bParallelThrough->setIconSize(QSize(32, 32));
        bParallelThrough->setCheckable(true);
        bParallelThrough->setAutoExclusive(true);

        gridLayout->addWidget(bParallelThrough, 4, 1, 1, 1);

        bTangent1 = new QToolButton(QG_CadToolBarLines);
        bTangent1->setObjectName(QString::fromUtf8("bTangent1"));
        bTangent1->setMaximumSize(QSize(32, 32));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/extui/linestan1.png"), QSize(), QIcon::Normal, QIcon::Off);
        bTangent1->setIcon(icon9);
        bTangent1->setIconSize(QSize(32, 32));
        bTangent1->setCheckable(true);
        bTangent1->setAutoExclusive(true);

        gridLayout->addWidget(bTangent1, 5, 0, 1, 1);

        bTangent2 = new QToolButton(QG_CadToolBarLines);
        bTangent2->setObjectName(QString::fromUtf8("bTangent2"));
        bTangent2->setMaximumSize(QSize(32, 32));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/extui/linestan2.png"), QSize(), QIcon::Normal, QIcon::Off);
        bTangent2->setIcon(icon10);
        bTangent2->setIconSize(QSize(32, 32));
        bTangent2->setCheckable(true);
        bTangent2->setAutoExclusive(true);

        gridLayout->addWidget(bTangent2, 5, 1, 1, 1);

        bOrthTan = new QToolButton(QG_CadToolBarLines);
        bOrthTan->setObjectName(QString::fromUtf8("bOrthTan"));
        bOrthTan->setMaximumSize(QSize(32, 32));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/extui/linesorthtan.png"), QSize(), QIcon::Normal, QIcon::Off);
        bOrthTan->setIcon(icon11);
        bOrthTan->setIconSize(QSize(32, 32));
        bOrthTan->setCheckable(true);
        bOrthTan->setAutoExclusive(true);

        gridLayout->addWidget(bOrthTan, 6, 0, 1, 1);

        bOrthogonal = new QToolButton(QG_CadToolBarLines);
        bOrthogonal->setObjectName(QString::fromUtf8("bOrthogonal"));
        bOrthogonal->setMaximumSize(QSize(32, 32));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/extui/linesorthogonal.png"), QSize(), QIcon::Normal, QIcon::Off);
        bOrthogonal->setIcon(icon12);
        bOrthogonal->setIconSize(QSize(32, 32));
        bOrthogonal->setCheckable(true);
        bOrthogonal->setAutoExclusive(true);

        gridLayout->addWidget(bOrthogonal, 6, 1, 1, 1);

        bRelAngle = new QToolButton(QG_CadToolBarLines);
        bRelAngle->setObjectName(QString::fromUtf8("bRelAngle"));
        bRelAngle->setMaximumSize(QSize(32, 32));
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/extui/linesrelativeangle.png"), QSize(), QIcon::Normal, QIcon::Off);
        bRelAngle->setIcon(icon13);
        bRelAngle->setIconSize(QSize(32, 32));
        bRelAngle->setCheckable(true);
        bRelAngle->setAutoExclusive(true);

        gridLayout->addWidget(bRelAngle, 7, 0, 1, 1);

        bPolygon = new QToolButton(QG_CadToolBarLines);
        bPolygon->setObjectName(QString::fromUtf8("bPolygon"));
        bPolygon->setMaximumSize(QSize(32, 32));
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/extui/linespolygon.png"), QSize(), QIcon::Normal, QIcon::Off);
        bPolygon->setIcon(icon14);
        bPolygon->setIconSize(QSize(32, 32));
        bPolygon->setCheckable(true);
        bPolygon->setAutoExclusive(true);

        gridLayout->addWidget(bPolygon, 7, 1, 1, 1);

        bPolygon2 = new QToolButton(QG_CadToolBarLines);
        bPolygon2->setObjectName(QString::fromUtf8("bPolygon2"));
        bPolygon2->setMaximumSize(QSize(32, 32));
        QIcon icon15;
        icon15.addFile(QString::fromUtf8(":/extui/linespolygon2.png"), QSize(), QIcon::Normal, QIcon::Off);
        bPolygon2->setIcon(icon15);
        bPolygon2->setIconSize(QSize(32, 32));
        bPolygon2->setCheckable(true);
        bPolygon2->setAutoExclusive(true);

        gridLayout->addWidget(bPolygon2, 8, 0, 1, 1);

        bFree = new QToolButton(QG_CadToolBarLines);
        bFree->setObjectName(QString::fromUtf8("bFree"));
        bFree->setMaximumSize(QSize(32, 32));
        QIcon icon16;
        icon16.addFile(QString::fromUtf8(":/extui/linesfree.png"), QSize(), QIcon::Normal, QIcon::Off);
        bFree->setIcon(icon16);
        bFree->setIconSize(QSize(32, 32));
        bFree->setCheckable(true);
        bFree->setAutoExclusive(true);

        gridLayout->addWidget(bFree, 8, 1, 1, 1);

        bHidden = new QToolButton(QG_CadToolBarLines);
        bHidden->setObjectName(QString::fromUtf8("bHidden"));
        bHidden->setMaximumSize(QSize(0, 0));
        bHidden->setText(QString::fromUtf8(""));
        bHidden->setIconSize(QSize(32, 32));
        bHidden->setCheckable(true);
        bHidden->setAutoExclusive(true);

        gridLayout->addWidget(bHidden, 9, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 9, 1, 1, 1);


        retranslateUi(QG_CadToolBarLines);
        QObject::connect(bNormal, SIGNAL(clicked()), QG_CadToolBarLines, SLOT(drawLine()));
        QObject::connect(bFree, SIGNAL(clicked()), QG_CadToolBarLines, SLOT(drawLineFree()));
        QObject::connect(bParallel, SIGNAL(clicked()), QG_CadToolBarLines, SLOT(drawLineParallel()));
        QObject::connect(bAngle, SIGNAL(clicked()), QG_CadToolBarLines, SLOT(drawLineAngle()));
        QObject::connect(bHorizontal, SIGNAL(clicked()), QG_CadToolBarLines, SLOT(drawLineHorizontal()));
        QObject::connect(bBisector, SIGNAL(clicked()), QG_CadToolBarLines, SLOT(drawLineBisector()));
        QObject::connect(bTangent1, SIGNAL(clicked()), QG_CadToolBarLines, SLOT(drawLineTangent1()));
        QObject::connect(bTangent2, SIGNAL(clicked()), QG_CadToolBarLines, SLOT(drawLineTangent2()));
        QObject::connect(bRectangle, SIGNAL(clicked()), QG_CadToolBarLines, SLOT(drawLineRectangle()));
        QObject::connect(bRelAngle, SIGNAL(clicked()), QG_CadToolBarLines, SLOT(drawLineRelAngle()));
        QObject::connect(bVertical, SIGNAL(clicked()), QG_CadToolBarLines, SLOT(drawLineVertical()));
        QObject::connect(bOrthTan, SIGNAL(clicked()), QG_CadToolBarLines, SLOT(drawLineOrthTan()));
        QObject::connect(bOrthogonal, SIGNAL(clicked()), QG_CadToolBarLines, SLOT(drawLineOrthogonal()));
        QObject::connect(bPolygon, SIGNAL(clicked()), QG_CadToolBarLines, SLOT(drawLinePolygon()));
        QObject::connect(bPolygon2, SIGNAL(clicked()), QG_CadToolBarLines, SLOT(drawLinePolygon2()));
        QObject::connect(bParallelThrough, SIGNAL(clicked()), QG_CadToolBarLines, SLOT(drawLineParallelThrough()));

        QMetaObject::connectSlotsByName(QG_CadToolBarLines);
    } // setupUi

    void retranslateUi(QWidget *QG_CadToolBarLines)
    {
        QG_CadToolBarLines->setWindowTitle(QApplication::translate("QG_CadToolBarLines", "Lines", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        bBack->setToolTip(QApplication::translate("QG_CadToolBarLines", "Back to main menu", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bBack->setText(QString());
#ifndef QT_NO_TOOLTIP
        bNormal->setToolTip(QApplication::translate("QG_CadToolBarLines", "Line with two points", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bNormal->setText(QString());
#ifndef QT_NO_TOOLTIP
        bAngle->setToolTip(QApplication::translate("QG_CadToolBarLines", "Line with given angle", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bAngle->setText(QString());
#ifndef QT_NO_TOOLTIP
        bHorizontal->setToolTip(QApplication::translate("QG_CadToolBarLines", "Horizontal lines", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bHorizontal->setText(QString());
#ifndef QT_NO_TOOLTIP
        bVertical->setToolTip(QApplication::translate("QG_CadToolBarLines", "Vertical lines", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bVertical->setText(QString());
#ifndef QT_NO_TOOLTIP
        bRectangle->setToolTip(QApplication::translate("QG_CadToolBarLines", "Rectangles", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bRectangle->setText(QString());
#ifndef QT_NO_TOOLTIP
        bBisector->setToolTip(QApplication::translate("QG_CadToolBarLines", "Bisectors", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bBisector->setText(QString());
#ifndef QT_NO_TOOLTIP
        bParallel->setToolTip(QApplication::translate("QG_CadToolBarLines", "Parallels with distance", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bParallel->setText(QString());
#ifndef QT_NO_TOOLTIP
        bParallelThrough->setToolTip(QApplication::translate("QG_CadToolBarLines", "Parallels through point", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bParallelThrough->setText(QString());
#ifndef QT_NO_TOOLTIP
        bTangent1->setToolTip(QApplication::translate("QG_CadToolBarLines", "Tangents from point to circle", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bTangent1->setText(QString());
#ifndef QT_NO_TOOLTIP
        bTangent2->setToolTip(QApplication::translate("QG_CadToolBarLines", "Tangents from circle to circle", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bTangent2->setText(QString());
#ifndef QT_NO_TOOLTIP
        bOrthTan->setToolTip(QApplication::translate("QG_CadToolBarLines", "Tangents to a circle, orthogonal to a line", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bOrthTan->setText(QString());
#ifndef QT_NO_TOOLTIP
        bOrthogonal->setToolTip(QApplication::translate("QG_CadToolBarLines", "Orthogonal lines", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bOrthogonal->setText(QString());
#ifndef QT_NO_TOOLTIP
        bRelAngle->setToolTip(QApplication::translate("QG_CadToolBarLines", "Lines with relative angles", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bRelAngle->setText(QString());
#ifndef QT_NO_TOOLTIP
        bPolygon->setToolTip(QApplication::translate("QG_CadToolBarLines", "Polygons with Center and Corner", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bPolygon->setText(QString());
#ifndef QT_NO_TOOLTIP
        bPolygon2->setToolTip(QApplication::translate("QG_CadToolBarLines", "Polygons with two Corners", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bPolygon2->setText(QString());
#ifndef QT_NO_TOOLTIP
        bFree->setToolTip(QApplication::translate("QG_CadToolBarLines", "Freehand lines", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bFree->setText(QString());
#ifndef QT_NO_TOOLTIP
        bHidden->setToolTip(QApplication::translate("QG_CadToolBarLines", "Line with two points", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
    } // retranslateUi

};

namespace Ui {
    class QG_CadToolBarLines: public Ui_QG_CadToolBarLines {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QG_CADTOOLBARLINES_H
