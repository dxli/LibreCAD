/********************************************************************************
** Form generated from reading UI file 'qg_dlgline.ui'
**
** Created: Sat Feb 18 16:41:50 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QG_DLGLINE_H
#define UI_QG_DLGLINE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include "qg_layerbox.h"
#include "qg_widgetpen.h"

QT_BEGIN_NAMESPACE

class Ui_QG_DlgLine
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
    QLabel *lEndX;
    QLineEdit *leEndX;
    QLineEdit *leEndY;
    QLabel *lEndY;
    QLabel *lStartY;
    QLineEdit *leStartY;
    QLabel *lStartX;
    QLineEdit *leStartX;
    QSpacerItem *spacer61;
    QSpacerItem *spacer58;
    QHBoxLayout *hboxLayout2;
    QSpacerItem *spacer;
    QPushButton *bOk;
    QPushButton *bCancel;

    void setupUi(QDialog *QG_DlgLine)
    {
        if (QG_DlgLine->objectName().isEmpty())
            QG_DlgLine->setObjectName(QString::fromUtf8("QG_DlgLine"));
        QG_DlgLine->setEnabled(true);
        QG_DlgLine->resize(548, 270);
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QG_DlgLine->sizePolicy().hasHeightForWidth());
        QG_DlgLine->setSizePolicy(sizePolicy);
        QG_DlgLine->setMinimumSize(QSize(0, 0));
        vboxLayout = new QVBoxLayout(QG_DlgLine);
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
        lLayer = new QLabel(QG_DlgLine);
        lLayer->setObjectName(QString::fromUtf8("lLayer"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lLayer->sizePolicy().hasHeightForWidth());
        lLayer->setSizePolicy(sizePolicy1);
        lLayer->setWordWrap(false);

        hboxLayout1->addWidget(lLayer);

        cbLayer = new QG_LayerBox(QG_DlgLine);
        cbLayer->setObjectName(QString::fromUtf8("cbLayer"));

        hboxLayout1->addWidget(cbLayer);


        vboxLayout1->addLayout(hboxLayout1);

        wPen = new QG_WidgetPen(QG_DlgLine);
        wPen->setObjectName(QString::fromUtf8("wPen"));
        sizePolicy.setHeightForWidth(wPen->sizePolicy().hasHeightForWidth());
        wPen->setSizePolicy(sizePolicy);
        wPen->setMinimumSize(QSize(230, 140));

        vboxLayout1->addWidget(wPen);


        hboxLayout->addLayout(vboxLayout1);

        buttonGroup8 = new QGroupBox(QG_DlgLine);
        buttonGroup8->setObjectName(QString::fromUtf8("buttonGroup8"));
        gridLayout = new QGridLayout(buttonGroup8);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        lEndX = new QLabel(buttonGroup8);
        lEndX->setObjectName(QString::fromUtf8("lEndX"));
        lEndX->setWordWrap(false);

        gridLayout->addWidget(lEndX, 5, 0, 1, 1);

        leEndX = new QLineEdit(buttonGroup8);
        leEndX->setObjectName(QString::fromUtf8("leEndX"));
        leEndX->setMinimumSize(QSize(64, 0));

        gridLayout->addWidget(leEndX, 5, 1, 1, 1);

        leEndY = new QLineEdit(buttonGroup8);
        leEndY->setObjectName(QString::fromUtf8("leEndY"));
        leEndY->setMinimumSize(QSize(64, 0));

        gridLayout->addWidget(leEndY, 7, 1, 1, 1);

        lEndY = new QLabel(buttonGroup8);
        lEndY->setObjectName(QString::fromUtf8("lEndY"));
        lEndY->setWordWrap(false);

        gridLayout->addWidget(lEndY, 7, 0, 1, 1);

        lStartY = new QLabel(buttonGroup8);
        lStartY->setObjectName(QString::fromUtf8("lStartY"));
        lStartY->setWordWrap(false);

        gridLayout->addWidget(lStartY, 3, 0, 1, 1);

        leStartY = new QLineEdit(buttonGroup8);
        leStartY->setObjectName(QString::fromUtf8("leStartY"));
        leStartY->setMinimumSize(QSize(64, 0));

        gridLayout->addWidget(leStartY, 3, 1, 1, 1);

        lStartX = new QLabel(buttonGroup8);
        lStartX->setObjectName(QString::fromUtf8("lStartX"));
        lStartX->setWordWrap(false);

        gridLayout->addWidget(lStartX, 1, 0, 1, 1);

        leStartX = new QLineEdit(buttonGroup8);
        leStartX->setObjectName(QString::fromUtf8("leStartX"));
        leStartX->setMinimumSize(QSize(64, 0));

        gridLayout->addWidget(leStartX, 1, 1, 1, 1);

        spacer61 = new QSpacerItem(20, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(spacer61, 8, 1, 1, 1);

        spacer58 = new QSpacerItem(20, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(spacer58, 8, 0, 1, 1);


        hboxLayout->addWidget(buttonGroup8);


        vboxLayout->addLayout(hboxLayout);

        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setSpacing(6);
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout2->addItem(spacer);

        bOk = new QPushButton(QG_DlgLine);
        bOk->setObjectName(QString::fromUtf8("bOk"));
        bOk->setDefault(true);

        hboxLayout2->addWidget(bOk);

        bCancel = new QPushButton(QG_DlgLine);
        bCancel->setObjectName(QString::fromUtf8("bCancel"));

        hboxLayout2->addWidget(bCancel);


        vboxLayout->addLayout(hboxLayout2);

        QWidget::setTabOrder(leStartX, leStartY);
        QWidget::setTabOrder(leStartY, leEndX);
        QWidget::setTabOrder(leEndX, leEndY);
        QWidget::setTabOrder(leEndY, bOk);
        QWidget::setTabOrder(bOk, bCancel);

        retranslateUi(QG_DlgLine);
        QObject::connect(bOk, SIGNAL(clicked()), QG_DlgLine, SLOT(accept()));
        QObject::connect(bCancel, SIGNAL(clicked()), QG_DlgLine, SLOT(reject()));

        QMetaObject::connectSlotsByName(QG_DlgLine);
    } // setupUi

    void retranslateUi(QDialog *QG_DlgLine)
    {
        QG_DlgLine->setWindowTitle(QApplication::translate("QG_DlgLine", "Line", 0, QApplication::UnicodeUTF8));
        lLayer->setText(QApplication::translate("QG_DlgLine", "Layer:", 0, QApplication::UnicodeUTF8));
        buttonGroup8->setTitle(QApplication::translate("QG_DlgLine", "Geometry", 0, QApplication::UnicodeUTF8));
        lEndX->setText(QApplication::translate("QG_DlgLine", "End point (x):", 0, QApplication::UnicodeUTF8));
        lEndY->setText(QApplication::translate("QG_DlgLine", "End point (y):", 0, QApplication::UnicodeUTF8));
        lStartY->setText(QApplication::translate("QG_DlgLine", "Start point (y):", 0, QApplication::UnicodeUTF8));
        lStartX->setText(QApplication::translate("QG_DlgLine", "Start point (x):", 0, QApplication::UnicodeUTF8));
        bOk->setText(QApplication::translate("QG_DlgLine", "&OK", 0, QApplication::UnicodeUTF8));
        bCancel->setText(QApplication::translate("QG_DlgLine", "Cancel", 0, QApplication::UnicodeUTF8));
        bCancel->setShortcut(QApplication::translate("QG_DlgLine", "Esc", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QG_DlgLine: public Ui_QG_DlgLine {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QG_DLGLINE_H
