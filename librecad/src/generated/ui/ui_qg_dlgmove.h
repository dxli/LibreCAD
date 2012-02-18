/********************************************************************************
** Form generated from reading UI file 'qg_dlgmove.ui'
**
** Created: Sat Feb 18 16:41:50 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QG_DLGMOVE_H
#define UI_QG_DLGMOVE_H

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
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_QG_DlgMove
{
public:
    QGridLayout *gridLayout;
    QGroupBox *bgNumber;
    QVBoxLayout *vboxLayout;
    QRadioButton *rbMove;
    QRadioButton *rbCopy;
    QRadioButton *rbMultiCopy;
    QLineEdit *leNumber;
    QSpacerItem *spacer7;
    QCheckBox *cbCurrentAttributes;
    QCheckBox *cbCurrentLayer;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacer;
    QPushButton *bOk;
    QPushButton *bCancel;
    QToolButton *toolButton;

    void setupUi(QDialog *QG_DlgMove)
    {
        if (QG_DlgMove->objectName().isEmpty())
            QG_DlgMove->setObjectName(QString::fromUtf8("QG_DlgMove"));
        QG_DlgMove->resize(398, 218);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QG_DlgMove->sizePolicy().hasHeightForWidth());
        QG_DlgMove->setSizePolicy(sizePolicy);
        QG_DlgMove->setMinimumSize(QSize(300, 190));
        gridLayout = new QGridLayout(QG_DlgMove);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        bgNumber = new QGroupBox(QG_DlgMove);
        bgNumber->setObjectName(QString::fromUtf8("bgNumber"));
        vboxLayout = new QVBoxLayout(bgNumber);
        vboxLayout->setSpacing(6);
        vboxLayout->setContentsMargins(11, 11, 11, 11);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        rbMove = new QRadioButton(bgNumber);
        rbMove->setObjectName(QString::fromUtf8("rbMove"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(rbMove->sizePolicy().hasHeightForWidth());
        rbMove->setSizePolicy(sizePolicy1);
        rbMove->setMinimumSize(QSize(0, 18));

        vboxLayout->addWidget(rbMove);

        rbCopy = new QRadioButton(bgNumber);
        rbCopy->setObjectName(QString::fromUtf8("rbCopy"));
        sizePolicy1.setHeightForWidth(rbCopy->sizePolicy().hasHeightForWidth());
        rbCopy->setSizePolicy(sizePolicy1);
        rbCopy->setMinimumSize(QSize(0, 18));

        vboxLayout->addWidget(rbCopy);

        rbMultiCopy = new QRadioButton(bgNumber);
        rbMultiCopy->setObjectName(QString::fromUtf8("rbMultiCopy"));
        sizePolicy1.setHeightForWidth(rbMultiCopy->sizePolicy().hasHeightForWidth());
        rbMultiCopy->setSizePolicy(sizePolicy1);
        rbMultiCopy->setMinimumSize(QSize(0, 18));

        vboxLayout->addWidget(rbMultiCopy);

        leNumber = new QLineEdit(bgNumber);
        leNumber->setObjectName(QString::fromUtf8("leNumber"));

        vboxLayout->addWidget(leNumber);

        spacer7 = new QSpacerItem(20, 130, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout->addItem(spacer7);


        gridLayout->addWidget(bgNumber, 0, 0, 3, 1);

        cbCurrentAttributes = new QCheckBox(QG_DlgMove);
        cbCurrentAttributes->setObjectName(QString::fromUtf8("cbCurrentAttributes"));

        gridLayout->addWidget(cbCurrentAttributes, 1, 1, 1, 1);

        cbCurrentLayer = new QCheckBox(QG_DlgMove);
        cbCurrentLayer->setObjectName(QString::fromUtf8("cbCurrentLayer"));

        gridLayout->addWidget(cbCurrentLayer, 2, 1, 1, 1);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacer);

        bOk = new QPushButton(QG_DlgMove);
        bOk->setObjectName(QString::fromUtf8("bOk"));
        bOk->setDefault(true);

        hboxLayout->addWidget(bOk);

        bCancel = new QPushButton(QG_DlgMove);
        bCancel->setObjectName(QString::fromUtf8("bCancel"));

        hboxLayout->addWidget(bCancel);


        gridLayout->addLayout(hboxLayout, 3, 0, 1, 2);

        toolButton = new QToolButton(QG_DlgMove);
        toolButton->setObjectName(QString::fromUtf8("toolButton"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/extui/dlgmove1.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton->setIcon(icon);
        toolButton->setIconSize(QSize(80, 80));

        gridLayout->addWidget(toolButton, 0, 1, 1, 1);


        retranslateUi(QG_DlgMove);
        QObject::connect(rbMove, SIGNAL(toggled(bool)), leNumber, SLOT(setDisabled(bool)));
        QObject::connect(rbCopy, SIGNAL(toggled(bool)), leNumber, SLOT(setDisabled(bool)));
        QObject::connect(rbMultiCopy, SIGNAL(toggled(bool)), leNumber, SLOT(setEnabled(bool)));
        QObject::connect(bOk, SIGNAL(clicked()), QG_DlgMove, SLOT(accept()));
        QObject::connect(bCancel, SIGNAL(clicked()), QG_DlgMove, SLOT(reject()));

        QMetaObject::connectSlotsByName(QG_DlgMove);
    } // setupUi

    void retranslateUi(QDialog *QG_DlgMove)
    {
        QG_DlgMove->setWindowTitle(QApplication::translate("QG_DlgMove", "Move/Copy Options", 0, QApplication::UnicodeUTF8));
        bgNumber->setTitle(QApplication::translate("QG_DlgMove", "Number of copies", 0, QApplication::UnicodeUTF8));
        rbMove->setText(QApplication::translate("QG_DlgMove", "&Delete Original", 0, QApplication::UnicodeUTF8));
        rbCopy->setText(QApplication::translate("QG_DlgMove", "&Keep Original", 0, QApplication::UnicodeUTF8));
        rbMultiCopy->setText(QApplication::translate("QG_DlgMove", "&Multiple Copies", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        leNumber->setToolTip(QApplication::translate("QG_DlgMove", "Number of copies. Maximum 100", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        cbCurrentAttributes->setText(QApplication::translate("QG_DlgMove", "Use current &attributes", 0, QApplication::UnicodeUTF8));
        cbCurrentLayer->setText(QApplication::translate("QG_DlgMove", "Use current &layer", 0, QApplication::UnicodeUTF8));
        bOk->setText(QApplication::translate("QG_DlgMove", "&OK", 0, QApplication::UnicodeUTF8));
        bOk->setShortcut(QApplication::translate("QG_DlgMove", "Alt+O", 0, QApplication::UnicodeUTF8));
        bCancel->setText(QApplication::translate("QG_DlgMove", "Cancel", 0, QApplication::UnicodeUTF8));
        bCancel->setShortcut(QApplication::translate("QG_DlgMove", "Esc", 0, QApplication::UnicodeUTF8));
        toolButton->setText(QApplication::translate("QG_DlgMove", "...", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QG_DlgMove: public Ui_QG_DlgMove {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QG_DLGMOVE_H
