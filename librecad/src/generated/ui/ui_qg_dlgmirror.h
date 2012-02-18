/********************************************************************************
** Form generated from reading UI file 'qg_dlgmirror.ui'
**
** Created: Sat Feb 18 16:41:50 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QG_DLGMIRROR_H
#define UI_QG_DLGMIRROR_H

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
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_QG_DlgMirror
{
public:
    QGridLayout *gridLayout;
    QGroupBox *bgNumber;
    QVBoxLayout *vboxLayout;
    QRadioButton *rbMove;
    QRadioButton *rbCopy;
    QSpacerItem *spacer7;
    QCheckBox *cbCurrentAttributes;
    QCheckBox *cbCurrentLayer;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacer;
    QPushButton *bOk;
    QPushButton *bCancel;
    QToolButton *toolButton;

    void setupUi(QDialog *QG_DlgMirror)
    {
        if (QG_DlgMirror->objectName().isEmpty())
            QG_DlgMirror->setObjectName(QString::fromUtf8("QG_DlgMirror"));
        QG_DlgMirror->resize(357, 195);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QG_DlgMirror->sizePolicy().hasHeightForWidth());
        QG_DlgMirror->setSizePolicy(sizePolicy);
        QG_DlgMirror->setMinimumSize(QSize(300, 190));
        gridLayout = new QGridLayout(QG_DlgMirror);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        bgNumber = new QGroupBox(QG_DlgMirror);
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

        spacer7 = new QSpacerItem(20, 21, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout->addItem(spacer7);


        gridLayout->addWidget(bgNumber, 0, 0, 4, 1);

        cbCurrentAttributes = new QCheckBox(QG_DlgMirror);
        cbCurrentAttributes->setObjectName(QString::fromUtf8("cbCurrentAttributes"));

        gridLayout->addWidget(cbCurrentAttributes, 2, 1, 1, 1);

        cbCurrentLayer = new QCheckBox(QG_DlgMirror);
        cbCurrentLayer->setObjectName(QString::fromUtf8("cbCurrentLayer"));

        gridLayout->addWidget(cbCurrentLayer, 3, 1, 1, 1);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacer);

        bOk = new QPushButton(QG_DlgMirror);
        bOk->setObjectName(QString::fromUtf8("bOk"));
        bOk->setDefault(true);

        hboxLayout->addWidget(bOk);

        bCancel = new QPushButton(QG_DlgMirror);
        bCancel->setObjectName(QString::fromUtf8("bCancel"));

        hboxLayout->addWidget(bCancel);


        gridLayout->addLayout(hboxLayout, 4, 0, 1, 2);

        toolButton = new QToolButton(QG_DlgMirror);
        toolButton->setObjectName(QString::fromUtf8("toolButton"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/extui/dlgmodifymirror.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton->setIcon(icon);
        toolButton->setIconSize(QSize(80, 80));

        gridLayout->addWidget(toolButton, 0, 1, 1, 1);


        retranslateUi(QG_DlgMirror);
        QObject::connect(bOk, SIGNAL(clicked()), QG_DlgMirror, SLOT(accept()));
        QObject::connect(bCancel, SIGNAL(clicked()), QG_DlgMirror, SLOT(reject()));

        QMetaObject::connectSlotsByName(QG_DlgMirror);
    } // setupUi

    void retranslateUi(QDialog *QG_DlgMirror)
    {
        QG_DlgMirror->setWindowTitle(QApplication::translate("QG_DlgMirror", "Mirroring Options", 0, QApplication::UnicodeUTF8));
        bgNumber->setTitle(QApplication::translate("QG_DlgMirror", "Number of copies", 0, QApplication::UnicodeUTF8));
        rbMove->setText(QApplication::translate("QG_DlgMirror", "&Delete Original", 0, QApplication::UnicodeUTF8));
        rbCopy->setText(QApplication::translate("QG_DlgMirror", "&Keep Original", 0, QApplication::UnicodeUTF8));
        cbCurrentAttributes->setText(QApplication::translate("QG_DlgMirror", "Use current &attributes", 0, QApplication::UnicodeUTF8));
        cbCurrentLayer->setText(QApplication::translate("QG_DlgMirror", "Use current &layer", 0, QApplication::UnicodeUTF8));
        bOk->setText(QApplication::translate("QG_DlgMirror", "&OK", 0, QApplication::UnicodeUTF8));
        bCancel->setText(QApplication::translate("QG_DlgMirror", "Cancel", 0, QApplication::UnicodeUTF8));
        bCancel->setShortcut(QApplication::translate("QG_DlgMirror", "Esc", 0, QApplication::UnicodeUTF8));
        toolButton->setText(QApplication::translate("QG_DlgMirror", "...", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QG_DlgMirror: public Ui_QG_DlgMirror {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QG_DLGMIRROR_H
