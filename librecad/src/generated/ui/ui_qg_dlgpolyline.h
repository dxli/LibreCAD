/********************************************************************************
** Form generated from reading UI file 'qg_dlgpolyline.ui'
**
** Created: Sat Feb 18 16:41:50 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QG_DLGPOLYLINE_H
#define UI_QG_DLGPOLYLINE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include "qg_layerbox.h"
#include "qg_widgetpen.h"

QT_BEGIN_NAMESPACE

class Ui_QG_DlgPolyline
{
public:
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QVBoxLayout *vboxLayout1;
    QHBoxLayout *hboxLayout1;
    QLabel *lLayer;
    QG_LayerBox *cbLayer;
    QG_WidgetPen *wPen;
    QGroupBox *buttonGroup8;
    QGridLayout *gridLayout;
    QSpacerItem *spacer58;
    QSpacerItem *spacer61;
    QCheckBox *cbClosed;
    QHBoxLayout *hboxLayout2;
    QSpacerItem *spacer;
    QPushButton *bOk;
    QPushButton *bCancel;

    void setupUi(QDialog *QG_DlgPolyline)
    {
        if (QG_DlgPolyline->objectName().isEmpty())
            QG_DlgPolyline->setObjectName(QString::fromUtf8("QG_DlgPolyline"));
        QG_DlgPolyline->resize(406, 234);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QG_DlgPolyline->sizePolicy().hasHeightForWidth());
        QG_DlgPolyline->setSizePolicy(sizePolicy);
        QG_DlgPolyline->setMinimumSize(QSize(300, 190));
        vboxLayout = new QVBoxLayout(QG_DlgPolyline);
        vboxLayout->setSpacing(6);
        vboxLayout->setContentsMargins(11, 11, 11, 11);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        vboxLayout1 = new QVBoxLayout();
        vboxLayout1->setSpacing(6);
        vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(6);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        lLayer = new QLabel(QG_DlgPolyline);
        lLayer->setObjectName(QString::fromUtf8("lLayer"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lLayer->sizePolicy().hasHeightForWidth());
        lLayer->setSizePolicy(sizePolicy1);
        lLayer->setWordWrap(false);

        hboxLayout1->addWidget(lLayer);

        cbLayer = new QG_LayerBox(QG_DlgPolyline);
        cbLayer->setObjectName(QString::fromUtf8("cbLayer"));

        hboxLayout1->addWidget(cbLayer);


        vboxLayout1->addLayout(hboxLayout1);

        wPen = new QG_WidgetPen(QG_DlgPolyline);
        wPen->setObjectName(QString::fromUtf8("wPen"));
        QSizePolicy sizePolicy2(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(wPen->sizePolicy().hasHeightForWidth());
        wPen->setSizePolicy(sizePolicy2);

        vboxLayout1->addWidget(wPen);


        hboxLayout->addLayout(vboxLayout1);

        buttonGroup8 = new QGroupBox(QG_DlgPolyline);
        buttonGroup8->setObjectName(QString::fromUtf8("buttonGroup8"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(1);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(buttonGroup8->sizePolicy().hasHeightForWidth());
        buttonGroup8->setSizePolicy(sizePolicy3);
        gridLayout = new QGridLayout(buttonGroup8);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        spacer58 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(spacer58, 1, 0, 1, 1);

        spacer61 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(spacer61, 1, 1, 1, 1);

        cbClosed = new QCheckBox(buttonGroup8);
        cbClosed->setObjectName(QString::fromUtf8("cbClosed"));

        gridLayout->addWidget(cbClosed, 0, 0, 1, 2);


        hboxLayout->addWidget(buttonGroup8);


        vboxLayout->addLayout(hboxLayout);

        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setSpacing(6);
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout2->addItem(spacer);

        bOk = new QPushButton(QG_DlgPolyline);
        bOk->setObjectName(QString::fromUtf8("bOk"));
        bOk->setDefault(true);

        hboxLayout2->addWidget(bOk);

        bCancel = new QPushButton(QG_DlgPolyline);
        bCancel->setObjectName(QString::fromUtf8("bCancel"));

        hboxLayout2->addWidget(bCancel);


        vboxLayout->addLayout(hboxLayout2);

        QWidget::setTabOrder(bOk, bCancel);

        retranslateUi(QG_DlgPolyline);
        QObject::connect(bOk, SIGNAL(clicked()), QG_DlgPolyline, SLOT(accept()));
        QObject::connect(bCancel, SIGNAL(clicked()), QG_DlgPolyline, SLOT(reject()));

        QMetaObject::connectSlotsByName(QG_DlgPolyline);
    } // setupUi

    void retranslateUi(QDialog *QG_DlgPolyline)
    {
        QG_DlgPolyline->setWindowTitle(QApplication::translate("QG_DlgPolyline", "Polyline", 0, QApplication::UnicodeUTF8));
        lLayer->setText(QApplication::translate("QG_DlgPolyline", "Layer:", 0, QApplication::UnicodeUTF8));
        buttonGroup8->setTitle(QApplication::translate("QG_DlgPolyline", "Geometry", 0, QApplication::UnicodeUTF8));
        cbClosed->setText(QApplication::translate("QG_DlgPolyline", "Closed", 0, QApplication::UnicodeUTF8));
        bOk->setText(QApplication::translate("QG_DlgPolyline", "&OK", 0, QApplication::UnicodeUTF8));
        bOk->setShortcut(QApplication::translate("QG_DlgPolyline", "Alt+O", 0, QApplication::UnicodeUTF8));
        bCancel->setText(QApplication::translate("QG_DlgPolyline", "Cancel", 0, QApplication::UnicodeUTF8));
        bCancel->setShortcut(QApplication::translate("QG_DlgPolyline", "Esc", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QG_DlgPolyline: public Ui_QG_DlgPolyline {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QG_DLGPOLYLINE_H
