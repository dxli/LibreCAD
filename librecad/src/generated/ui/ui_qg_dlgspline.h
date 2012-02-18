/********************************************************************************
** Form generated from reading UI file 'qg_dlgspline.ui'
**
** Created: Sat Feb 18 16:41:51 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QG_DLGSPLINE_H
#define UI_QG_DLGSPLINE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
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

class Ui_QG_DlgSpline
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
    QLabel *lDegree;
    QCheckBox *cbClosed;
    QComboBox *cbDegree;
    QHBoxLayout *hboxLayout2;
    QSpacerItem *spacer;
    QPushButton *bOk;
    QPushButton *bCancel;

    void setupUi(QDialog *QG_DlgSpline)
    {
        if (QG_DlgSpline->objectName().isEmpty())
            QG_DlgSpline->setObjectName(QString::fromUtf8("QG_DlgSpline"));
        QG_DlgSpline->resize(406, 234);
        QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(1), static_cast<QSizePolicy::Policy>(1));
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QG_DlgSpline->sizePolicy().hasHeightForWidth());
        QG_DlgSpline->setSizePolicy(sizePolicy);
        QG_DlgSpline->setMinimumSize(QSize(300, 190));
        vboxLayout = new QVBoxLayout(QG_DlgSpline);
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
        lLayer = new QLabel(QG_DlgSpline);
        lLayer->setObjectName(QString::fromUtf8("lLayer"));
        QSizePolicy sizePolicy1(static_cast<QSizePolicy::Policy>(5), static_cast<QSizePolicy::Policy>(5));
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lLayer->sizePolicy().hasHeightForWidth());
        lLayer->setSizePolicy(sizePolicy1);
        lLayer->setWordWrap(false);

        hboxLayout1->addWidget(lLayer);

        cbLayer = new QG_LayerBox(QG_DlgSpline);
        cbLayer->setObjectName(QString::fromUtf8("cbLayer"));

        hboxLayout1->addWidget(cbLayer);


        vboxLayout1->addLayout(hboxLayout1);

        wPen = new QG_WidgetPen(QG_DlgSpline);
        wPen->setObjectName(QString::fromUtf8("wPen"));
        QSizePolicy sizePolicy2(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(3));
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(wPen->sizePolicy().hasHeightForWidth());
        wPen->setSizePolicy(sizePolicy2);

        vboxLayout1->addWidget(wPen);


        hboxLayout->addLayout(vboxLayout1);

        buttonGroup8 = new QGroupBox(QG_DlgSpline);
        buttonGroup8->setObjectName(QString::fromUtf8("buttonGroup8"));
        QSizePolicy sizePolicy3(static_cast<QSizePolicy::Policy>(5), static_cast<QSizePolicy::Policy>(5));
        sizePolicy3.setHorizontalStretch(1);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(buttonGroup8->sizePolicy().hasHeightForWidth());
        buttonGroup8->setSizePolicy(sizePolicy3);
        gridLayout = new QGridLayout(buttonGroup8);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        spacer58 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(spacer58, 2, 0, 1, 1);

        spacer61 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(spacer61, 2, 1, 1, 1);

        lDegree = new QLabel(buttonGroup8);
        lDegree->setObjectName(QString::fromUtf8("lDegree"));
        lDegree->setWordWrap(false);

        gridLayout->addWidget(lDegree, 0, 0, 1, 1);

        cbClosed = new QCheckBox(buttonGroup8);
        cbClosed->setObjectName(QString::fromUtf8("cbClosed"));

        gridLayout->addWidget(cbClosed, 1, 0, 1, 2);

        cbDegree = new QComboBox(buttonGroup8);
        cbDegree->setObjectName(QString::fromUtf8("cbDegree"));
        cbDegree->setMinimumSize(QSize(64, 0));

        gridLayout->addWidget(cbDegree, 0, 1, 1, 1);


        hboxLayout->addWidget(buttonGroup8);


        vboxLayout->addLayout(hboxLayout);

        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setSpacing(6);
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout2->addItem(spacer);

        bOk = new QPushButton(QG_DlgSpline);
        bOk->setObjectName(QString::fromUtf8("bOk"));
        bOk->setDefault(true);

        hboxLayout2->addWidget(bOk);

        bCancel = new QPushButton(QG_DlgSpline);
        bCancel->setObjectName(QString::fromUtf8("bCancel"));

        hboxLayout2->addWidget(bCancel);


        vboxLayout->addLayout(hboxLayout2);

        QWidget::setTabOrder(bOk, bCancel);

        retranslateUi(QG_DlgSpline);
        QObject::connect(bOk, SIGNAL(clicked()), QG_DlgSpline, SLOT(accept()));
        QObject::connect(bCancel, SIGNAL(clicked()), QG_DlgSpline, SLOT(reject()));

        QMetaObject::connectSlotsByName(QG_DlgSpline);
    } // setupUi

    void retranslateUi(QDialog *QG_DlgSpline)
    {
        QG_DlgSpline->setWindowTitle(QApplication::translate("QG_DlgSpline", "Spline", 0, QApplication::UnicodeUTF8));
        lLayer->setText(QApplication::translate("QG_DlgSpline", "Layer:", 0, QApplication::UnicodeUTF8));
        buttonGroup8->setTitle(QApplication::translate("QG_DlgSpline", "Geometry", 0, QApplication::UnicodeUTF8));
        lDegree->setText(QApplication::translate("QG_DlgSpline", "Degree:", 0, QApplication::UnicodeUTF8));
        cbClosed->setText(QApplication::translate("QG_DlgSpline", "Closed", 0, QApplication::UnicodeUTF8));
        cbDegree->clear();
        cbDegree->insertItems(0, QStringList()
         << QApplication::translate("QG_DlgSpline", "1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgSpline", "2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgSpline", "3", 0, QApplication::UnicodeUTF8)
        );
        bOk->setText(QApplication::translate("QG_DlgSpline", "&OK", 0, QApplication::UnicodeUTF8));
        bOk->setShortcut(QApplication::translate("QG_DlgSpline", "Alt+O", 0, QApplication::UnicodeUTF8));
        bCancel->setText(QApplication::translate("QG_DlgSpline", "Cancel", 0, QApplication::UnicodeUTF8));
        bCancel->setShortcut(QApplication::translate("QG_DlgSpline", "Esc", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QG_DlgSpline: public Ui_QG_DlgSpline {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QG_DLGSPLINE_H
