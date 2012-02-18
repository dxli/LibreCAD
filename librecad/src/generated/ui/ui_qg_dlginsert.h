/********************************************************************************
** Form generated from reading UI file 'qg_dlginsert.ui'
**
** Created: Sat Feb 18 16:41:50 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QG_DLGINSERT_H
#define UI_QG_DLGINSERT_H

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

class Ui_QG_DlgInsert
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
    QSpacerItem *spacer61;
    QSpacerItem *spacer58;
    QLineEdit *leInsertionPointY;
    QLineEdit *leInsertionPointX;
    QLabel *lInsertionPointX;
    QLabel *lInsertionPointY;
    QLineEdit *leScale;
    QLabel *lFactor;
    QLabel *lAngle;
    QLineEdit *leAngle;
    QLabel *lRows;
    QLineEdit *leRows;
    QLineEdit *leCols;
    QLabel *lCols;
    QLabel *lRowSpacing;
    QLineEdit *leRowSpacing;
    QLineEdit *leColSpacing;
    QLabel *lColSpacing;
    QHBoxLayout *hboxLayout2;
    QSpacerItem *spacer;
    QPushButton *bOk;
    QPushButton *bCancel;

    void setupUi(QDialog *QG_DlgInsert)
    {
        if (QG_DlgInsert->objectName().isEmpty())
            QG_DlgInsert->setObjectName(QString::fromUtf8("QG_DlgInsert"));
        QG_DlgInsert->resize(457, 310);
        QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(1), static_cast<QSizePolicy::Policy>(1));
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QG_DlgInsert->sizePolicy().hasHeightForWidth());
        QG_DlgInsert->setSizePolicy(sizePolicy);
        QG_DlgInsert->setMinimumSize(QSize(300, 190));
        vboxLayout = new QVBoxLayout(QG_DlgInsert);
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
        lLayer = new QLabel(QG_DlgInsert);
        lLayer->setObjectName(QString::fromUtf8("lLayer"));
        QSizePolicy sizePolicy1(static_cast<QSizePolicy::Policy>(5), static_cast<QSizePolicy::Policy>(5));
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lLayer->sizePolicy().hasHeightForWidth());
        lLayer->setSizePolicy(sizePolicy1);
        lLayer->setWordWrap(false);

        hboxLayout1->addWidget(lLayer);

        cbLayer = new QG_LayerBox(QG_DlgInsert);
        cbLayer->setObjectName(QString::fromUtf8("cbLayer"));

        hboxLayout1->addWidget(cbLayer);


        vboxLayout1->addLayout(hboxLayout1);

        wPen = new QG_WidgetPen(QG_DlgInsert);
        wPen->setObjectName(QString::fromUtf8("wPen"));
        QSizePolicy sizePolicy2(static_cast<QSizePolicy::Policy>(3), static_cast<QSizePolicy::Policy>(3));
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(wPen->sizePolicy().hasHeightForWidth());
        wPen->setSizePolicy(sizePolicy2);

        vboxLayout1->addWidget(wPen);


        hboxLayout->addLayout(vboxLayout1);

        buttonGroup8 = new QGroupBox(QG_DlgInsert);
        buttonGroup8->setObjectName(QString::fromUtf8("buttonGroup8"));
        gridLayout = new QGridLayout(buttonGroup8);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        spacer61 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(spacer61, 8, 1, 1, 1);

        spacer58 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(spacer58, 8, 0, 1, 1);

        leInsertionPointY = new QLineEdit(buttonGroup8);
        leInsertionPointY->setObjectName(QString::fromUtf8("leInsertionPointY"));
        leInsertionPointY->setMinimumSize(QSize(64, 0));

        gridLayout->addWidget(leInsertionPointY, 1, 1, 1, 1);

        leInsertionPointX = new QLineEdit(buttonGroup8);
        leInsertionPointX->setObjectName(QString::fromUtf8("leInsertionPointX"));
        leInsertionPointX->setMinimumSize(QSize(64, 0));

        gridLayout->addWidget(leInsertionPointX, 0, 1, 1, 1);

        lInsertionPointX = new QLabel(buttonGroup8);
        lInsertionPointX->setObjectName(QString::fromUtf8("lInsertionPointX"));
        lInsertionPointX->setWordWrap(false);

        gridLayout->addWidget(lInsertionPointX, 0, 0, 1, 1);

        lInsertionPointY = new QLabel(buttonGroup8);
        lInsertionPointY->setObjectName(QString::fromUtf8("lInsertionPointY"));
        lInsertionPointY->setWordWrap(false);

        gridLayout->addWidget(lInsertionPointY, 1, 0, 1, 1);

        leScale = new QLineEdit(buttonGroup8);
        leScale->setObjectName(QString::fromUtf8("leScale"));
        leScale->setMinimumSize(QSize(64, 0));

        gridLayout->addWidget(leScale, 2, 1, 1, 1);

        lFactor = new QLabel(buttonGroup8);
        lFactor->setObjectName(QString::fromUtf8("lFactor"));
        lFactor->setWordWrap(false);

        gridLayout->addWidget(lFactor, 2, 0, 1, 1);

        lAngle = new QLabel(buttonGroup8);
        lAngle->setObjectName(QString::fromUtf8("lAngle"));
        lAngle->setWordWrap(false);

        gridLayout->addWidget(lAngle, 3, 0, 1, 1);

        leAngle = new QLineEdit(buttonGroup8);
        leAngle->setObjectName(QString::fromUtf8("leAngle"));
        leAngle->setMinimumSize(QSize(64, 0));

        gridLayout->addWidget(leAngle, 3, 1, 1, 1);

        lRows = new QLabel(buttonGroup8);
        lRows->setObjectName(QString::fromUtf8("lRows"));
        lRows->setWordWrap(false);

        gridLayout->addWidget(lRows, 4, 0, 1, 1);

        leRows = new QLineEdit(buttonGroup8);
        leRows->setObjectName(QString::fromUtf8("leRows"));
        leRows->setMinimumSize(QSize(64, 0));

        gridLayout->addWidget(leRows, 4, 1, 1, 1);

        leCols = new QLineEdit(buttonGroup8);
        leCols->setObjectName(QString::fromUtf8("leCols"));
        leCols->setMinimumSize(QSize(64, 0));

        gridLayout->addWidget(leCols, 5, 1, 1, 1);

        lCols = new QLabel(buttonGroup8);
        lCols->setObjectName(QString::fromUtf8("lCols"));
        lCols->setWordWrap(false);

        gridLayout->addWidget(lCols, 5, 0, 1, 1);

        lRowSpacing = new QLabel(buttonGroup8);
        lRowSpacing->setObjectName(QString::fromUtf8("lRowSpacing"));
        lRowSpacing->setWordWrap(false);

        gridLayout->addWidget(lRowSpacing, 6, 0, 1, 1);

        leRowSpacing = new QLineEdit(buttonGroup8);
        leRowSpacing->setObjectName(QString::fromUtf8("leRowSpacing"));
        leRowSpacing->setMinimumSize(QSize(64, 0));

        gridLayout->addWidget(leRowSpacing, 6, 1, 1, 1);

        leColSpacing = new QLineEdit(buttonGroup8);
        leColSpacing->setObjectName(QString::fromUtf8("leColSpacing"));
        leColSpacing->setMinimumSize(QSize(64, 0));

        gridLayout->addWidget(leColSpacing, 7, 1, 1, 1);

        lColSpacing = new QLabel(buttonGroup8);
        lColSpacing->setObjectName(QString::fromUtf8("lColSpacing"));
        lColSpacing->setWordWrap(false);

        gridLayout->addWidget(lColSpacing, 7, 0, 1, 1);


        hboxLayout->addWidget(buttonGroup8);


        vboxLayout->addLayout(hboxLayout);

        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setSpacing(6);
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout2->addItem(spacer);

        bOk = new QPushButton(QG_DlgInsert);
        bOk->setObjectName(QString::fromUtf8("bOk"));
        bOk->setDefault(true);

        hboxLayout2->addWidget(bOk);

        bCancel = new QPushButton(QG_DlgInsert);
        bCancel->setObjectName(QString::fromUtf8("bCancel"));

        hboxLayout2->addWidget(bCancel);


        vboxLayout->addLayout(hboxLayout2);

        QWidget::setTabOrder(leInsertionPointX, leInsertionPointY);
        QWidget::setTabOrder(leInsertionPointY, leScale);
        QWidget::setTabOrder(leScale, leAngle);
        QWidget::setTabOrder(leAngle, leRows);
        QWidget::setTabOrder(leRows, leCols);
        QWidget::setTabOrder(leCols, leRowSpacing);
        QWidget::setTabOrder(leRowSpacing, leColSpacing);
        QWidget::setTabOrder(leColSpacing, bOk);
        QWidget::setTabOrder(bOk, bCancel);

        retranslateUi(QG_DlgInsert);
        QObject::connect(bOk, SIGNAL(clicked()), QG_DlgInsert, SLOT(accept()));
        QObject::connect(bCancel, SIGNAL(clicked()), QG_DlgInsert, SLOT(reject()));

        QMetaObject::connectSlotsByName(QG_DlgInsert);
    } // setupUi

    void retranslateUi(QDialog *QG_DlgInsert)
    {
        QG_DlgInsert->setWindowTitle(QApplication::translate("QG_DlgInsert", "Insert", 0, QApplication::UnicodeUTF8));
        lLayer->setText(QApplication::translate("QG_DlgInsert", "Layer:", 0, QApplication::UnicodeUTF8));
        buttonGroup8->setTitle(QApplication::translate("QG_DlgInsert", "Geometry", 0, QApplication::UnicodeUTF8));
        lInsertionPointX->setText(QApplication::translate("QG_DlgInsert", "Insertion point (x):", 0, QApplication::UnicodeUTF8));
        lInsertionPointY->setText(QApplication::translate("QG_DlgInsert", "Insertion point (y):", 0, QApplication::UnicodeUTF8));
        lFactor->setText(QApplication::translate("QG_DlgInsert", "Scale:", 0, QApplication::UnicodeUTF8));
        lAngle->setText(QApplication::translate("QG_DlgInsert", "Angle:", 0, QApplication::UnicodeUTF8));
        lRows->setText(QApplication::translate("QG_DlgInsert", "Rows:", 0, QApplication::UnicodeUTF8));
        lCols->setText(QApplication::translate("QG_DlgInsert", "Columns:", 0, QApplication::UnicodeUTF8));
        lRowSpacing->setText(QApplication::translate("QG_DlgInsert", "Row Spacing:", 0, QApplication::UnicodeUTF8));
        lColSpacing->setText(QApplication::translate("QG_DlgInsert", "Column Spacing:", 0, QApplication::UnicodeUTF8));
        bOk->setText(QApplication::translate("QG_DlgInsert", "&OK", 0, QApplication::UnicodeUTF8));
        bCancel->setText(QApplication::translate("QG_DlgInsert", "Cancel", 0, QApplication::UnicodeUTF8));
        bCancel->setShortcut(QApplication::translate("QG_DlgInsert", "Esc", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QG_DlgInsert: public Ui_QG_DlgInsert {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QG_DLGINSERT_H
