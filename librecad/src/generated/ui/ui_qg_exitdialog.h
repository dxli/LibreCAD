/********************************************************************************
** Form generated from reading UI file 'qg_exitdialog.ui'
**
** Created: Sat Feb 18 16:41:51 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QG_EXITDIALOG_H
#define UI_QG_EXITDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_QG_ExitDialog
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *hboxLayout;
    QSpacerItem *Horizontal_Spacing2;
    QPushButton *bLeave;
    QPushButton *bSave;
    QPushButton *bSaveAs;
    QPushButton *bCancel;
    QSpacerItem *spacer2;
    QLabel *lQuestion;
    QLabel *l_icon;

    void setupUi(QDialog *QG_ExitDialog)
    {
        if (QG_ExitDialog->objectName().isEmpty())
            QG_ExitDialog->setObjectName(QString::fromUtf8("QG_ExitDialog"));
        QG_ExitDialog->resize(450, 106);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QG_ExitDialog->sizePolicy().hasHeightForWidth());
        QG_ExitDialog->setSizePolicy(sizePolicy);
        QG_ExitDialog->setMinimumSize(QSize(450, 0));
        QG_ExitDialog->setSizeGripEnabled(false);
        gridLayout = new QGridLayout(QG_ExitDialog);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        Horizontal_Spacing2 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(Horizontal_Spacing2);

        bLeave = new QPushButton(QG_ExitDialog);
        bLeave->setObjectName(QString::fromUtf8("bLeave"));
        bLeave->setAutoDefault(true);
        bLeave->setDefault(false);

        hboxLayout->addWidget(bLeave);

        bSave = new QPushButton(QG_ExitDialog);
        bSave->setObjectName(QString::fromUtf8("bSave"));

        hboxLayout->addWidget(bSave);

        bSaveAs = new QPushButton(QG_ExitDialog);
        bSaveAs->setObjectName(QString::fromUtf8("bSaveAs"));

        hboxLayout->addWidget(bSaveAs);

        bCancel = new QPushButton(QG_ExitDialog);
        bCancel->setObjectName(QString::fromUtf8("bCancel"));
        bCancel->setAutoDefault(true);
        bCancel->setDefault(true);

        hboxLayout->addWidget(bCancel);

        spacer2 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacer2);


        gridLayout->addLayout(hboxLayout, 1, 0, 1, 2);

        lQuestion = new QLabel(QG_ExitDialog);
        lQuestion->setObjectName(QString::fromUtf8("lQuestion"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lQuestion->sizePolicy().hasHeightForWidth());
        lQuestion->setSizePolicy(sizePolicy1);
        lQuestion->setAlignment(Qt::AlignCenter);
        lQuestion->setWordWrap(true);

        gridLayout->addWidget(lQuestion, 0, 1, 1, 1);

        l_icon = new QLabel(QG_ExitDialog);
        l_icon->setObjectName(QString::fromUtf8("l_icon"));
        l_icon->setWordWrap(false);

        gridLayout->addWidget(l_icon, 0, 0, 1, 1);

        QWidget::setTabOrder(bCancel, bLeave);
        QWidget::setTabOrder(bLeave, bSave);
        QWidget::setTabOrder(bSave, bSaveAs);

        retranslateUi(QG_ExitDialog);
        QObject::connect(bLeave, SIGNAL(clicked()), QG_ExitDialog, SLOT(accept()));
        QObject::connect(bCancel, SIGNAL(clicked()), QG_ExitDialog, SLOT(reject()));
        QObject::connect(bSave, SIGNAL(clicked()), QG_ExitDialog, SLOT(slotSave()));
        QObject::connect(bSaveAs, SIGNAL(clicked()), QG_ExitDialog, SLOT(slotSaveAs()));

        QMetaObject::connectSlotsByName(QG_ExitDialog);
    } // setupUi

    void retranslateUi(QDialog *QG_ExitDialog)
    {
        QG_ExitDialog->setWindowTitle(QApplication::translate("QG_ExitDialog", "QCad", 0, QApplication::UnicodeUTF8));
        bLeave->setText(QApplication::translate("QG_ExitDialog", "C&lose", 0, QApplication::UnicodeUTF8));
        bSave->setText(QApplication::translate("QG_ExitDialog", "&Save", 0, QApplication::UnicodeUTF8));
        bSaveAs->setText(QApplication::translate("QG_ExitDialog", "Save &As..", 0, QApplication::UnicodeUTF8));
        bCancel->setText(QApplication::translate("QG_ExitDialog", "&Cancel", 0, QApplication::UnicodeUTF8));
        lQuestion->setText(QApplication::translate("QG_ExitDialog", "again Still No Text supplied.", 0, QApplication::UnicodeUTF8));
        l_icon->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class QG_ExitDialog: public Ui_QG_ExitDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QG_EXITDIALOG_H
