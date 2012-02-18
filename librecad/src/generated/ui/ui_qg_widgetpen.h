/********************************************************************************
** Form generated from reading UI file 'qg_widgetpen.ui'
**
** Created: Sat Feb 18 16:41:50 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QG_WIDGETPEN_H
#define UI_QG_WIDGETPEN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QWidget>
#include "qg_colorbox.h"
#include "qg_linetypebox.h"
#include "qg_widthbox.h"

QT_BEGIN_NAMESPACE

class Ui_QG_WidgetPen
{
public:
    QGridLayout *gridLayout;
    QGroupBox *bgPen;
    QGridLayout *gridLayout1;
    QSpacerItem *spacer9;
    QLabel *lLineType;
    QSpacerItem *spacer8;
    QLabel *lWidth;
    QG_LineTypeBox *cbLineType;
    QG_ColorBox *cbColor;
    QLabel *lColor;
    QG_WidthBox *cbWidth;

    void setupUi(QWidget *QG_WidgetPen)
    {
        if (QG_WidgetPen->objectName().isEmpty())
            QG_WidgetPen->setObjectName(QString::fromUtf8("QG_WidgetPen"));
        QG_WidgetPen->resize(303, 180);
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QG_WidgetPen->sizePolicy().hasHeightForWidth());
        QG_WidgetPen->setSizePolicy(sizePolicy);
        QG_WidgetPen->setMinimumSize(QSize(0, 0));
        gridLayout = new QGridLayout(QG_WidgetPen);
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        bgPen = new QGroupBox(QG_WidgetPen);
        bgPen->setObjectName(QString::fromUtf8("bgPen"));
        gridLayout1 = new QGridLayout(bgPen);
        gridLayout1->setSpacing(6);
        gridLayout1->setContentsMargins(11, 11, 11, 11);
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        spacer9 = new QSpacerItem(21, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout1->addItem(spacer9, 3, 1, 1, 1);

        lLineType = new QLabel(bgPen);
        lLineType->setObjectName(QString::fromUtf8("lLineType"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lLineType->sizePolicy().hasHeightForWidth());
        lLineType->setSizePolicy(sizePolicy1);
        lLineType->setWordWrap(false);

        gridLayout1->addWidget(lLineType, 2, 0, 1, 1);

        spacer8 = new QSpacerItem(21, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout1->addItem(spacer8, 3, 0, 1, 1);

        lWidth = new QLabel(bgPen);
        lWidth->setObjectName(QString::fromUtf8("lWidth"));
        sizePolicy1.setHeightForWidth(lWidth->sizePolicy().hasHeightForWidth());
        lWidth->setSizePolicy(sizePolicy1);
        lWidth->setWordWrap(false);

        gridLayout1->addWidget(lWidth, 1, 0, 1, 1);

        cbLineType = new QG_LineTypeBox(bgPen);
        cbLineType->setObjectName(QString::fromUtf8("cbLineType"));

        gridLayout1->addWidget(cbLineType, 2, 1, 1, 1);

        cbColor = new QG_ColorBox(bgPen);
        cbColor->setObjectName(QString::fromUtf8("cbColor"));

        gridLayout1->addWidget(cbColor, 0, 1, 1, 1);

        lColor = new QLabel(bgPen);
        lColor->setObjectName(QString::fromUtf8("lColor"));
        sizePolicy1.setHeightForWidth(lColor->sizePolicy().hasHeightForWidth());
        lColor->setSizePolicy(sizePolicy1);
        lColor->setWordWrap(false);

        gridLayout1->addWidget(lColor, 0, 0, 1, 1);

        cbWidth = new QG_WidthBox(bgPen);
        cbWidth->setObjectName(QString::fromUtf8("cbWidth"));

        gridLayout1->addWidget(cbWidth, 1, 1, 1, 1);


        gridLayout->addWidget(bgPen, 0, 0, 1, 1);


        retranslateUi(QG_WidgetPen);

        QMetaObject::connectSlotsByName(QG_WidgetPen);
    } // setupUi

    void retranslateUi(QWidget *QG_WidgetPen)
    {
        QG_WidgetPen->setWindowTitle(QApplication::translate("QG_WidgetPen", "Pen", 0, QApplication::UnicodeUTF8));
        bgPen->setTitle(QApplication::translate("QG_WidgetPen", "Pen", 0, QApplication::UnicodeUTF8));
        lLineType->setText(QApplication::translate("QG_WidgetPen", "Line type:", 0, QApplication::UnicodeUTF8));
        lWidth->setText(QApplication::translate("QG_WidgetPen", "Width:", 0, QApplication::UnicodeUTF8));
        lColor->setText(QApplication::translate("QG_WidgetPen", "Color:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QG_WidgetPen: public Ui_QG_WidgetPen {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QG_WIDGETPEN_H
