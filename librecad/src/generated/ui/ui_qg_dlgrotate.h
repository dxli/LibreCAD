/********************************************************************************
** Form generated from reading UI file 'qg_dlgrotate.ui'
**
** Created: Sat Feb 18 16:41:51 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QG_DLGROTATE_H
#define UI_QG_DLGROTATE_H

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
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_QG_DlgRotate
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacer;
    QPushButton *bOk;
    QPushButton *bCancel;
    QGroupBox *bgNumber;
    QVBoxLayout *vboxLayout;
    QRadioButton *rbMove;
    QRadioButton *rbCopy;
    QRadioButton *rbMultiCopy;
    QLineEdit *leNumber;
    QSpacerItem *spacer7;
    QVBoxLayout *vboxLayout1;
    QToolButton *toolButton;
    QHBoxLayout *hboxLayout1;
    QLabel *lAngle;
    QSpacerItem *spacer12;
    QLineEdit *leAngle;
    QCheckBox *cbCurrentAttributes;
    QCheckBox *cbCurrentLayer;

    void setupUi(QDialog *QG_DlgRotate)
    {
        if (QG_DlgRotate->objectName().isEmpty())
            QG_DlgRotate->setObjectName(QString::fromUtf8("QG_DlgRotate"));
        QG_DlgRotate->resize(403, 252);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QG_DlgRotate->sizePolicy().hasHeightForWidth());
        QG_DlgRotate->setSizePolicy(sizePolicy);
        QG_DlgRotate->setMinimumSize(QSize(300, 190));
        gridLayout = new QGridLayout(QG_DlgRotate);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacer);

        bOk = new QPushButton(QG_DlgRotate);
        bOk->setObjectName(QString::fromUtf8("bOk"));
        bOk->setDefault(true);

        hboxLayout->addWidget(bOk);

        bCancel = new QPushButton(QG_DlgRotate);
        bCancel->setObjectName(QString::fromUtf8("bCancel"));

        hboxLayout->addWidget(bCancel);


        gridLayout->addLayout(hboxLayout, 1, 0, 1, 2);

        bgNumber = new QGroupBox(QG_DlgRotate);
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

        spacer7 = new QSpacerItem(20, 146, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout->addItem(spacer7);


        gridLayout->addWidget(bgNumber, 0, 0, 1, 1);

        vboxLayout1 = new QVBoxLayout();
        vboxLayout1->setSpacing(6);
        vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
        toolButton = new QToolButton(QG_DlgRotate);
        toolButton->setObjectName(QString::fromUtf8("toolButton"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/extui/dlgmodifyrotate.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton->setIcon(icon);
        toolButton->setIconSize(QSize(80, 80));

        vboxLayout1->addWidget(toolButton);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(6);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        lAngle = new QLabel(QG_DlgRotate);
        lAngle->setObjectName(QString::fromUtf8("lAngle"));
        lAngle->setWordWrap(false);

        hboxLayout1->addWidget(lAngle);

        spacer12 = new QSpacerItem(31, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout1->addItem(spacer12);

        leAngle = new QLineEdit(QG_DlgRotate);
        leAngle->setObjectName(QString::fromUtf8("leAngle"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(leAngle->sizePolicy().hasHeightForWidth());
        leAngle->setSizePolicy(sizePolicy2);

        hboxLayout1->addWidget(leAngle);


        vboxLayout1->addLayout(hboxLayout1);

        cbCurrentAttributes = new QCheckBox(QG_DlgRotate);
        cbCurrentAttributes->setObjectName(QString::fromUtf8("cbCurrentAttributes"));

        vboxLayout1->addWidget(cbCurrentAttributes);

        cbCurrentLayer = new QCheckBox(QG_DlgRotate);
        cbCurrentLayer->setObjectName(QString::fromUtf8("cbCurrentLayer"));

        vboxLayout1->addWidget(cbCurrentLayer);


        gridLayout->addLayout(vboxLayout1, 0, 1, 1, 1);

#ifndef QT_NO_SHORTCUT
        lAngle->setBuddy(leAngle);
#endif // QT_NO_SHORTCUT

        retranslateUi(QG_DlgRotate);
        QObject::connect(rbMove, SIGNAL(toggled(bool)), leNumber, SLOT(setDisabled(bool)));
        QObject::connect(rbCopy, SIGNAL(toggled(bool)), leNumber, SLOT(setDisabled(bool)));
        QObject::connect(rbMultiCopy, SIGNAL(toggled(bool)), leNumber, SLOT(setEnabled(bool)));
        QObject::connect(bOk, SIGNAL(clicked()), QG_DlgRotate, SLOT(accept()));
        QObject::connect(bCancel, SIGNAL(clicked()), QG_DlgRotate, SLOT(reject()));

        QMetaObject::connectSlotsByName(QG_DlgRotate);
    } // setupUi

    void retranslateUi(QDialog *QG_DlgRotate)
    {
        QG_DlgRotate->setWindowTitle(QApplication::translate("QG_DlgRotate", "Rotation Options", 0, QApplication::UnicodeUTF8));
        bOk->setText(QApplication::translate("QG_DlgRotate", "&OK", 0, QApplication::UnicodeUTF8));
        bCancel->setText(QApplication::translate("QG_DlgRotate", "&Cancel", 0, QApplication::UnicodeUTF8));
        bCancel->setShortcut(QApplication::translate("QG_DlgRotate", "Esc", 0, QApplication::UnicodeUTF8));
        bgNumber->setTitle(QApplication::translate("QG_DlgRotate", "Number of copies", 0, QApplication::UnicodeUTF8));
        rbMove->setText(QApplication::translate("QG_DlgRotate", "&Delete Original", 0, QApplication::UnicodeUTF8));
        rbCopy->setText(QApplication::translate("QG_DlgRotate", "&Keep Original", 0, QApplication::UnicodeUTF8));
        rbMultiCopy->setText(QApplication::translate("QG_DlgRotate", "&Multiple Copies:", 0, QApplication::UnicodeUTF8));
        toolButton->setText(QApplication::translate("QG_DlgRotate", "...", 0, QApplication::UnicodeUTF8));
        lAngle->setText(QApplication::translate("QG_DlgRotate", "&Angle (a):", 0, QApplication::UnicodeUTF8));
        cbCurrentAttributes->setText(QApplication::translate("QG_DlgRotate", "Use current &attributes", 0, QApplication::UnicodeUTF8));
        cbCurrentLayer->setText(QApplication::translate("QG_DlgRotate", "Use current &layer", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QG_DlgRotate: public Ui_QG_DlgRotate {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QG_DLGROTATE_H
