/********************************************************************************
** Form generated from reading UI file 'qg_dlgtext.ui'
**
** Created: Sat Feb 18 16:41:51 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QG_DLGTEXT_H
#define UI_QG_DLGTEXT_H

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
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextEdit>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include "qg_fontbox.h"

QT_BEGIN_NAMESPACE

class Ui_QG_DlgText
{
public:
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QVBoxLayout *vboxLayout1;
    QGroupBox *bgFont;
    QGridLayout *gridLayout;
    QLineEdit *leHeight;
    QLabel *lHeight;
    QSpacerItem *spacer2_2;
    QG_FontBox *cbFont;
    QSpacerItem *spacer2;
    QLabel *lLineSpacingFactor;
    QLineEdit *leLineSpacingFactor;
    QCheckBox *cbDefault;
    QHBoxLayout *hboxLayout1;
    QGroupBox *bgAlignment;
    QGridLayout *gridLayout1;
    QToolButton *bTR;
    QToolButton *bTL;
    QToolButton *bML;
    QToolButton *bMC;
    QToolButton *bMR;
    QToolButton *bBL;
    QToolButton *bBR;
    QToolButton *bBC;
    QToolButton *bTC;
    QGroupBox *bgShape;
    QVBoxLayout *vboxLayout2;
    QLineEdit *leAngle;
    QSpacerItem *spacer13;
    QVBoxLayout *vboxLayout3;
    QHBoxLayout *hboxLayout2;
    QLabel *lText;
    QSpacerItem *spacer4;
    QToolButton *bClear;
    QToolButton *bLoad;
    QToolButton *bSave;
    QToolButton *bCut;
    QToolButton *bCopy;
    QToolButton *bPaste;
    QTextEdit *teText;
    QHBoxLayout *hboxLayout3;
    QGroupBox *buttonGroup7;
    QVBoxLayout *vboxLayout4;
    QComboBox *cbSymbol;
    QGroupBox *buttonGroup5;
    QGridLayout *gridLayout2;
    QLabel *textLabel1;
    QLabel *textLabel1_2;
    QComboBox *cbUniPage;
    QComboBox *cbUniChar;
    QToolButton *bUnicode;
    QHBoxLayout *hboxLayout4;
    QSpacerItem *Horizontal_Spacing2;
    QPushButton *buttonOk;
    QPushButton *buttonCancel;

    void setupUi(QDialog *QG_DlgText)
    {
        if (QG_DlgText->objectName().isEmpty())
            QG_DlgText->setObjectName(QString::fromUtf8("QG_DlgText"));
        QG_DlgText->resize(619, 454);
        QG_DlgText->setSizeGripEnabled(true);
        vboxLayout = new QVBoxLayout(QG_DlgText);
        vboxLayout->setSpacing(6);
        vboxLayout->setContentsMargins(11, 11, 11, 11);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        vboxLayout1 = new QVBoxLayout();
        vboxLayout1->setSpacing(6);
        vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
        bgFont = new QGroupBox(QG_DlgText);
        bgFont->setObjectName(QString::fromUtf8("bgFont"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(bgFont->sizePolicy().hasHeightForWidth());
        bgFont->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(bgFont);
        gridLayout->setSpacing(2);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        leHeight = new QLineEdit(bgFont);
        leHeight->setObjectName(QString::fromUtf8("leHeight"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(leHeight->sizePolicy().hasHeightForWidth());
        leHeight->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(leHeight, 1, 1, 1, 1);

        lHeight = new QLabel(bgFont);
        lHeight->setObjectName(QString::fromUtf8("lHeight"));
        lHeight->setWordWrap(false);

        gridLayout->addWidget(lHeight, 1, 0, 1, 1);

        spacer2_2 = new QSpacerItem(20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(spacer2_2, 5, 1, 1, 1);

        cbFont = new QG_FontBox(bgFont);
        cbFont->setObjectName(QString::fromUtf8("cbFont"));
        sizePolicy1.setHeightForWidth(cbFont->sizePolicy().hasHeightForWidth());
        cbFont->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(cbFont, 0, 0, 1, 2);

        spacer2 = new QSpacerItem(20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(spacer2, 5, 0, 1, 1);

        lLineSpacingFactor = new QLabel(bgFont);
        lLineSpacingFactor->setObjectName(QString::fromUtf8("lLineSpacingFactor"));
        lLineSpacingFactor->setWordWrap(false);

        gridLayout->addWidget(lLineSpacingFactor, 3, 0, 1, 1);

        leLineSpacingFactor = new QLineEdit(bgFont);
        leLineSpacingFactor->setObjectName(QString::fromUtf8("leLineSpacingFactor"));
        sizePolicy1.setHeightForWidth(leLineSpacingFactor->sizePolicy().hasHeightForWidth());
        leLineSpacingFactor->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(leLineSpacingFactor, 3, 1, 1, 1);

        cbDefault = new QCheckBox(bgFont);
        cbDefault->setObjectName(QString::fromUtf8("cbDefault"));
        sizePolicy1.setHeightForWidth(cbDefault->sizePolicy().hasHeightForWidth());
        cbDefault->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(cbDefault, 2, 0, 1, 2);


        vboxLayout1->addWidget(bgFont);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(6);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        bgAlignment = new QGroupBox(QG_DlgText);
        bgAlignment->setObjectName(QString::fromUtf8("bgAlignment"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(bgAlignment->sizePolicy().hasHeightForWidth());
        bgAlignment->setSizePolicy(sizePolicy2);
        gridLayout1 = new QGridLayout(bgAlignment);
        gridLayout1->setSpacing(6);
        gridLayout1->setContentsMargins(11, 11, 11, 11);
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        bTR = new QToolButton(bgAlignment);
        bTR->setObjectName(QString::fromUtf8("bTR"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(bTR->sizePolicy().hasHeightForWidth());
        bTR->setSizePolicy(sizePolicy3);
        bTR->setMinimumSize(QSize(25, 25));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/extui/align_tr.png"), QSize(), QIcon::Normal, QIcon::Off);
        bTR->setIcon(icon);
        bTR->setCheckable(true);

        gridLayout1->addWidget(bTR, 0, 2, 1, 1);

        bTL = new QToolButton(bgAlignment);
        bTL->setObjectName(QString::fromUtf8("bTL"));
        sizePolicy3.setHeightForWidth(bTL->sizePolicy().hasHeightForWidth());
        bTL->setSizePolicy(sizePolicy3);
        bTL->setMinimumSize(QSize(25, 25));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/extui/align_tl.png"), QSize(), QIcon::Normal, QIcon::Off);
        bTL->setIcon(icon1);
        bTL->setCheckable(true);

        gridLayout1->addWidget(bTL, 0, 0, 1, 1);

        bML = new QToolButton(bgAlignment);
        bML->setObjectName(QString::fromUtf8("bML"));
        sizePolicy3.setHeightForWidth(bML->sizePolicy().hasHeightForWidth());
        bML->setSizePolicy(sizePolicy3);
        bML->setMinimumSize(QSize(25, 25));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/extui/align_ml.png"), QSize(), QIcon::Normal, QIcon::Off);
        bML->setIcon(icon2);
        bML->setCheckable(true);

        gridLayout1->addWidget(bML, 1, 0, 1, 1);

        bMC = new QToolButton(bgAlignment);
        bMC->setObjectName(QString::fromUtf8("bMC"));
        sizePolicy3.setHeightForWidth(bMC->sizePolicy().hasHeightForWidth());
        bMC->setSizePolicy(sizePolicy3);
        bMC->setMinimumSize(QSize(25, 25));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/extui/align_mc.png"), QSize(), QIcon::Normal, QIcon::Off);
        bMC->setIcon(icon3);
        bMC->setCheckable(true);

        gridLayout1->addWidget(bMC, 1, 1, 1, 1);

        bMR = new QToolButton(bgAlignment);
        bMR->setObjectName(QString::fromUtf8("bMR"));
        sizePolicy3.setHeightForWidth(bMR->sizePolicy().hasHeightForWidth());
        bMR->setSizePolicy(sizePolicy3);
        bMR->setMinimumSize(QSize(25, 25));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/extui/align_mr.png"), QSize(), QIcon::Normal, QIcon::Off);
        bMR->setIcon(icon4);
        bMR->setCheckable(true);

        gridLayout1->addWidget(bMR, 1, 2, 1, 1);

        bBL = new QToolButton(bgAlignment);
        bBL->setObjectName(QString::fromUtf8("bBL"));
        sizePolicy3.setHeightForWidth(bBL->sizePolicy().hasHeightForWidth());
        bBL->setSizePolicy(sizePolicy3);
        bBL->setMinimumSize(QSize(25, 25));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/extui/align_bl.png"), QSize(), QIcon::Normal, QIcon::Off);
        bBL->setIcon(icon5);
        bBL->setCheckable(true);

        gridLayout1->addWidget(bBL, 2, 0, 1, 1);

        bBR = new QToolButton(bgAlignment);
        bBR->setObjectName(QString::fromUtf8("bBR"));
        sizePolicy3.setHeightForWidth(bBR->sizePolicy().hasHeightForWidth());
        bBR->setSizePolicy(sizePolicy3);
        bBR->setMinimumSize(QSize(25, 25));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/extui/align_br.png"), QSize(), QIcon::Normal, QIcon::Off);
        bBR->setIcon(icon6);
        bBR->setCheckable(true);

        gridLayout1->addWidget(bBR, 2, 2, 1, 1);

        bBC = new QToolButton(bgAlignment);
        bBC->setObjectName(QString::fromUtf8("bBC"));
        sizePolicy3.setHeightForWidth(bBC->sizePolicy().hasHeightForWidth());
        bBC->setSizePolicy(sizePolicy3);
        bBC->setMinimumSize(QSize(25, 25));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/extui/align_bc.png"), QSize(), QIcon::Normal, QIcon::Off);
        bBC->setIcon(icon7);
        bBC->setCheckable(true);

        gridLayout1->addWidget(bBC, 2, 1, 1, 1);

        bTC = new QToolButton(bgAlignment);
        bTC->setObjectName(QString::fromUtf8("bTC"));
        sizePolicy3.setHeightForWidth(bTC->sizePolicy().hasHeightForWidth());
        bTC->setSizePolicy(sizePolicy3);
        bTC->setMinimumSize(QSize(25, 25));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/extui/align_tc.png"), QSize(), QIcon::Normal, QIcon::Off);
        bTC->setIcon(icon8);
        bTC->setCheckable(true);

        gridLayout1->addWidget(bTC, 0, 1, 1, 1);


        hboxLayout1->addWidget(bgAlignment);

        bgShape = new QGroupBox(QG_DlgText);
        bgShape->setObjectName(QString::fromUtf8("bgShape"));
        QSizePolicy sizePolicy4(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(bgShape->sizePolicy().hasHeightForWidth());
        bgShape->setSizePolicy(sizePolicy4);
        bgShape->setMinimumSize(QSize(80, 0));
        vboxLayout2 = new QVBoxLayout(bgShape);
        vboxLayout2->setSpacing(6);
        vboxLayout2->setContentsMargins(11, 11, 11, 11);
        vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
        leAngle = new QLineEdit(bgShape);
        leAngle->setObjectName(QString::fromUtf8("leAngle"));
        sizePolicy1.setHeightForWidth(leAngle->sizePolicy().hasHeightForWidth());
        leAngle->setSizePolicy(sizePolicy1);

        vboxLayout2->addWidget(leAngle);

        spacer13 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout2->addItem(spacer13);


        hboxLayout1->addWidget(bgShape);


        vboxLayout1->addLayout(hboxLayout1);


        hboxLayout->addLayout(vboxLayout1);

        vboxLayout3 = new QVBoxLayout();
        vboxLayout3->setSpacing(6);
        vboxLayout3->setObjectName(QString::fromUtf8("vboxLayout3"));
        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setSpacing(6);
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        lText = new QLabel(QG_DlgText);
        lText->setObjectName(QString::fromUtf8("lText"));
        lText->setWordWrap(false);

        hboxLayout2->addWidget(lText);

        spacer4 = new QSpacerItem(110, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout2->addItem(spacer4);

        bClear = new QToolButton(QG_DlgText);
        bClear->setObjectName(QString::fromUtf8("bClear"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/extui/filenew.png"), QSize(), QIcon::Normal, QIcon::Off);
        bClear->setIcon(icon9);

        hboxLayout2->addWidget(bClear);

        bLoad = new QToolButton(QG_DlgText);
        bLoad->setObjectName(QString::fromUtf8("bLoad"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/extui/fileopen.png"), QSize(), QIcon::Normal, QIcon::Off);
        bLoad->setIcon(icon10);

        hboxLayout2->addWidget(bLoad);

        bSave = new QToolButton(QG_DlgText);
        bSave->setObjectName(QString::fromUtf8("bSave"));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/extui/filesave.png"), QSize(), QIcon::Normal, QIcon::Off);
        bSave->setIcon(icon11);

        hboxLayout2->addWidget(bSave);

        bCut = new QToolButton(QG_DlgText);
        bCut->setObjectName(QString::fromUtf8("bCut"));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/extui/editcut.png"), QSize(), QIcon::Normal, QIcon::Off);
        bCut->setIcon(icon12);

        hboxLayout2->addWidget(bCut);

        bCopy = new QToolButton(QG_DlgText);
        bCopy->setObjectName(QString::fromUtf8("bCopy"));
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/extui/editcopy.png"), QSize(), QIcon::Normal, QIcon::Off);
        bCopy->setIcon(icon13);

        hboxLayout2->addWidget(bCopy);

        bPaste = new QToolButton(QG_DlgText);
        bPaste->setObjectName(QString::fromUtf8("bPaste"));
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/extui/editpaste.png"), QSize(), QIcon::Normal, QIcon::Off);
        bPaste->setIcon(icon14);

        hboxLayout2->addWidget(bPaste);


        vboxLayout3->addLayout(hboxLayout2);

        teText = new QTextEdit(QG_DlgText);
        teText->setObjectName(QString::fromUtf8("teText"));
        teText->setProperty("linkUnderline", QVariant(false));

        vboxLayout3->addWidget(teText);


        hboxLayout->addLayout(vboxLayout3);


        vboxLayout->addLayout(hboxLayout);

        hboxLayout3 = new QHBoxLayout();
        hboxLayout3->setSpacing(6);
        hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
        buttonGroup7 = new QGroupBox(QG_DlgText);
        buttonGroup7->setObjectName(QString::fromUtf8("buttonGroup7"));
        sizePolicy.setHeightForWidth(buttonGroup7->sizePolicy().hasHeightForWidth());
        buttonGroup7->setSizePolicy(sizePolicy);
        buttonGroup7->setMinimumSize(QSize(148, 0));
        vboxLayout4 = new QVBoxLayout(buttonGroup7);
        vboxLayout4->setSpacing(6);
        vboxLayout4->setContentsMargins(11, 11, 11, 11);
        vboxLayout4->setObjectName(QString::fromUtf8("vboxLayout4"));
        cbSymbol = new QComboBox(buttonGroup7);
        cbSymbol->setObjectName(QString::fromUtf8("cbSymbol"));
        QSizePolicy sizePolicy5(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(cbSymbol->sizePolicy().hasHeightForWidth());
        cbSymbol->setSizePolicy(sizePolicy5);

        vboxLayout4->addWidget(cbSymbol);


        hboxLayout3->addWidget(buttonGroup7);

        buttonGroup5 = new QGroupBox(QG_DlgText);
        buttonGroup5->setObjectName(QString::fromUtf8("buttonGroup5"));
        gridLayout2 = new QGridLayout(buttonGroup5);
        gridLayout2->setSpacing(6);
        gridLayout2->setContentsMargins(11, 11, 11, 11);
        gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
        textLabel1 = new QLabel(buttonGroup5);
        textLabel1->setObjectName(QString::fromUtf8("textLabel1"));
        textLabel1->setWordWrap(false);

        gridLayout2->addWidget(textLabel1, 0, 0, 1, 1);

        textLabel1_2 = new QLabel(buttonGroup5);
        textLabel1_2->setObjectName(QString::fromUtf8("textLabel1_2"));
        textLabel1_2->setWordWrap(false);

        gridLayout2->addWidget(textLabel1_2, 1, 0, 1, 1);

        cbUniPage = new QComboBox(buttonGroup5);
        cbUniPage->setObjectName(QString::fromUtf8("cbUniPage"));
        sizePolicy5.setHeightForWidth(cbUniPage->sizePolicy().hasHeightForWidth());
        cbUniPage->setSizePolicy(sizePolicy5);
        cbUniPage->setMinimumSize(QSize(280, 0));

        gridLayout2->addWidget(cbUniPage, 0, 1, 1, 1);

        cbUniChar = new QComboBox(buttonGroup5);
        cbUniChar->setObjectName(QString::fromUtf8("cbUniChar"));
        sizePolicy5.setHeightForWidth(cbUniChar->sizePolicy().hasHeightForWidth());
        cbUniChar->setSizePolicy(sizePolicy5);
        cbUniChar->setMinimumSize(QSize(92, 0));

        gridLayout2->addWidget(cbUniChar, 1, 1, 1, 1);

        bUnicode = new QToolButton(buttonGroup5);
        bUnicode->setObjectName(QString::fromUtf8("bUnicode"));
        sizePolicy3.setHeightForWidth(bUnicode->sizePolicy().hasHeightForWidth());
        bUnicode->setSizePolicy(sizePolicy3);
        bUnicode->setMinimumSize(QSize(25, 25));
        bUnicode->setMaximumSize(QSize(25, 25));

        gridLayout2->addWidget(bUnicode, 0, 2, 1, 1);


        hboxLayout3->addWidget(buttonGroup5);


        vboxLayout->addLayout(hboxLayout3);

        hboxLayout4 = new QHBoxLayout();
        hboxLayout4->setSpacing(6);
        hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
        Horizontal_Spacing2 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout4->addItem(Horizontal_Spacing2);

        buttonOk = new QPushButton(QG_DlgText);
        buttonOk->setObjectName(QString::fromUtf8("buttonOk"));
        buttonOk->setAutoDefault(true);
        buttonOk->setDefault(true);

        hboxLayout4->addWidget(buttonOk);

        buttonCancel = new QPushButton(QG_DlgText);
        buttonCancel->setObjectName(QString::fromUtf8("buttonCancel"));
        buttonCancel->setAutoDefault(true);

        hboxLayout4->addWidget(buttonCancel);


        vboxLayout->addLayout(hboxLayout4);

#ifndef QT_NO_SHORTCUT
        lHeight->setBuddy(leHeight);
        lLineSpacingFactor->setBuddy(leLineSpacingFactor);
#endif // QT_NO_SHORTCUT

        retranslateUi(QG_DlgText);
        QObject::connect(buttonOk, SIGNAL(clicked()), QG_DlgText, SLOT(accept()));
        QObject::connect(buttonCancel, SIGNAL(clicked()), QG_DlgText, SLOT(reject()));
        QObject::connect(bTL, SIGNAL(clicked()), QG_DlgText, SLOT(setAlignmentTL()));
        QObject::connect(bTC, SIGNAL(clicked()), QG_DlgText, SLOT(setAlignmentTC()));
        QObject::connect(bTR, SIGNAL(clicked()), QG_DlgText, SLOT(setAlignmentTR()));
        QObject::connect(bML, SIGNAL(clicked()), QG_DlgText, SLOT(setAlignmentML()));
        QObject::connect(bMC, SIGNAL(clicked()), QG_DlgText, SLOT(setAlignmentMC()));
        QObject::connect(bMR, SIGNAL(clicked()), QG_DlgText, SLOT(setAlignmentMR()));
        QObject::connect(bBL, SIGNAL(clicked()), QG_DlgText, SLOT(setAlignmentBL()));
        QObject::connect(bBC, SIGNAL(clicked()), QG_DlgText, SLOT(setAlignmentBC()));
        QObject::connect(bBR, SIGNAL(clicked()), QG_DlgText, SLOT(setAlignmentBR()));
        QObject::connect(cbDefault, SIGNAL(toggled(bool)), leLineSpacingFactor, SLOT(setDisabled(bool)));
        QObject::connect(cbDefault, SIGNAL(toggled(bool)), QG_DlgText, SLOT(defaultChanged(bool)));
        QObject::connect(bClear, SIGNAL(clicked()), teText, SLOT(clear()));
        QObject::connect(bCut, SIGNAL(clicked()), teText, SLOT(cut()));
        QObject::connect(bCopy, SIGNAL(clicked()), teText, SLOT(copy()));
        QObject::connect(bPaste, SIGNAL(clicked()), teText, SLOT(paste()));
        QObject::connect(bLoad, SIGNAL(clicked()), QG_DlgText, SLOT(loadText()));
        QObject::connect(bSave, SIGNAL(clicked()), QG_DlgText, SLOT(saveText()));
        QObject::connect(cbUniPage, SIGNAL(activated(int)), QG_DlgText, SLOT(updateUniCharComboBox(int)));
        QObject::connect(bUnicode, SIGNAL(clicked()), QG_DlgText, SLOT(insertChar()));
        QObject::connect(cbUniPage, SIGNAL(activated(int)), QG_DlgText, SLOT(updateUniCharButton(int)));
        QObject::connect(cbUniChar, SIGNAL(activated(int)), QG_DlgText, SLOT(updateUniCharButton(int)));
        QObject::connect(cbSymbol, SIGNAL(activated(int)), QG_DlgText, SLOT(insertSymbol(int)));

        QMetaObject::connectSlotsByName(QG_DlgText);
    } // setupUi

    void retranslateUi(QDialog *QG_DlgText)
    {
        QG_DlgText->setWindowTitle(QApplication::translate("QG_DlgText", "Text", 0, QApplication::UnicodeUTF8));
        bgFont->setTitle(QApplication::translate("QG_DlgText", "Font", 0, QApplication::UnicodeUTF8));
        lHeight->setText(QApplication::translate("QG_DlgText", "&Height:", 0, QApplication::UnicodeUTF8));
        lLineSpacingFactor->setText(QApplication::translate("QG_DlgText", "Line &spacing:", 0, QApplication::UnicodeUTF8));
        cbDefault->setText(QApplication::translate("QG_DlgText", "&Default line spacing", 0, QApplication::UnicodeUTF8));
        cbDefault->setShortcut(QApplication::translate("QG_DlgText", "Alt+D", 0, QApplication::UnicodeUTF8));
        bgAlignment->setTitle(QApplication::translate("QG_DlgText", "Alignment", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        bTR->setToolTip(QApplication::translate("QG_DlgText", "Top Right", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bTR->setText(QString());
#ifndef QT_NO_TOOLTIP
        bTL->setToolTip(QApplication::translate("QG_DlgText", "Top Left", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bTL->setText(QString());
#ifndef QT_NO_TOOLTIP
        bML->setToolTip(QApplication::translate("QG_DlgText", "Middle Left", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bML->setText(QString());
#ifndef QT_NO_TOOLTIP
        bMC->setToolTip(QApplication::translate("QG_DlgText", "Middle Center", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bMC->setText(QString());
#ifndef QT_NO_TOOLTIP
        bMR->setToolTip(QApplication::translate("QG_DlgText", "Middle Right", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bMR->setText(QString());
#ifndef QT_NO_TOOLTIP
        bBL->setToolTip(QApplication::translate("QG_DlgText", "Bottom Left", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bBL->setText(QString());
#ifndef QT_NO_TOOLTIP
        bBR->setToolTip(QApplication::translate("QG_DlgText", "Bottom Right", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bBR->setText(QString());
#ifndef QT_NO_TOOLTIP
        bBC->setToolTip(QApplication::translate("QG_DlgText", "Bottom Center", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bBC->setText(QString());
#ifndef QT_NO_TOOLTIP
        bTC->setToolTip(QApplication::translate("QG_DlgText", "Top Center", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bTC->setText(QString());
        bgShape->setTitle(QApplication::translate("QG_DlgText", "Angle", 0, QApplication::UnicodeUTF8));
        lText->setText(QApplication::translate("QG_DlgText", "Text:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        bClear->setToolTip(QApplication::translate("QG_DlgText", "Clear Text", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bClear->setText(QString());
#ifndef QT_NO_TOOLTIP
        bLoad->setToolTip(QApplication::translate("QG_DlgText", "Load Text From File", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bLoad->setText(QString());
#ifndef QT_NO_TOOLTIP
        bSave->setToolTip(QApplication::translate("QG_DlgText", "Save Text To File", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bSave->setText(QString());
#ifndef QT_NO_TOOLTIP
        bCut->setToolTip(QApplication::translate("QG_DlgText", "Cut", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bCut->setText(QString());
#ifndef QT_NO_TOOLTIP
        bCopy->setToolTip(QApplication::translate("QG_DlgText", "Copy", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bCopy->setText(QString());
#ifndef QT_NO_TOOLTIP
        bPaste->setToolTip(QApplication::translate("QG_DlgText", "Paste", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bPaste->setText(QString());
        buttonGroup7->setTitle(QApplication::translate("QG_DlgText", "Insert Symbol", 0, QApplication::UnicodeUTF8));
        cbSymbol->clear();
        cbSymbol->insertItems(0, QStringList()
         << QApplication::translate("QG_DlgText", "Diameter (\303\270)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "Degree (\302\260)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "Plus / Minus (\302\261)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "At (@)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "Hash (#)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "Dollar ($)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "Copyright (\302\251)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "Registered (\302\256)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "Paragraph (\302\247)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "Pi (\317\200)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "Pound (\302\243)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "Yen (\302\245)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "Times (\303\227)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "Division (\303\267)", 0, QApplication::UnicodeUTF8)
        );
        buttonGroup5->setTitle(QApplication::translate("QG_DlgText", "Insert Unicode", 0, QApplication::UnicodeUTF8));
        textLabel1->setText(QApplication::translate("QG_DlgText", "Page:", 0, QApplication::UnicodeUTF8));
        textLabel1_2->setText(QApplication::translate("QG_DlgText", "Char:", 0, QApplication::UnicodeUTF8));
        cbUniPage->clear();
        cbUniPage->insertItems(0, QStringList()
         << QApplication::translate("QG_DlgText", "[0000-007F] Basic Latin", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[0080-00FF] Latin-1 Supplementary", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[0100-017F] Latin Extended-A", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[0180-024F] Latin Extended-B", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[0250-02AF] IPA Extensions", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[02B0-02FF] Spacing Modifier Letters", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[0300-036F] Combining Diacritical Marks", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[0370-03FF] Greek and Coptic", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[0400-04FF] Cyrillic", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[0500-052F] Cyrillic Supplementary", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[0530-058F] Armenian", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[0590-05FF] Hebrew", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[0600-06FF] Arabic", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[0700-074F] Syriac", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[0780-07BF] Thaana", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[0900-097F] Devanagari", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[0980-09FF] Bengali", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[0A00-0A7F] Gurmukhi", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[0A80-0AFF] Gujarati", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[0B00-0B7F] Oriya", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[0B80-0BFF] Tamil", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[0C00-0C7F] Telugu", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[0C80-0CFF] Kannada", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[0D00-0D7F] Malayalam", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[0D80-0DFF] Sinhala", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[0E00-0E7F] Thai", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[0E80-0EFF] Lao", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[0F00-0FFF] Tibetan", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[1000-109F] Myanmar", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[10A0-10FF] Georgian", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[1100-11FF] Hangul Jamo", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[1200-137F] Ethiopic", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[13A0-13FF] Cherokee", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[1400-167F] Unified Canadian Aboriginal Syllabic", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[1680-169F] Ogham", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[16A0-16FF] Runic", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[1700-171F] Tagalog", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[1720-173F] Hanunoo", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[1740-175F] Buhid", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[1760-177F] Tagbanwa", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[1780-17FF] Khmer", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[1800-18AF] Mongolian", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[1E00-1EFF] Latin Extended Additional", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[1F00-1FFF] Greek Extended", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[2000-206F] General Punctuation", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[2070-209F] Superscripts and Subscripts", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[20A0-20CF] Currency Symbols", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[20D0-20FF] Combining Marks for Symbols", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[2100-214F] Letterlike Symbols", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[2150-218F] Number Forms", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[2190-21FF] Arrows", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[2200-22FF] Mathematical Operators", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[2300-23FF] Miscellaneous Technical", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[2400-243F] Control Pictures", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[2440-245F] Optical Character Recognition", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[2460-24FF] Enclosed Alphanumerics", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[2500-257F] Box Drawing", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[2580-259F] Block Elements", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[25A0-25FF] Geometric Shapes", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[2600-26FF] Miscellaneous Symbols", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[2700-27BF] Dingbats", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[27C0-27EF] Miscellaneous Mathematical Symbols-A", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[27F0-27FF] Supplemental Arrows-A", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[2800-28FF] Braille Patterns", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[2900-297F] Supplemental Arrows-B", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[2980-29FF] Miscellaneous Mathematical Symbols-B", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[2A00-2AFF] Supplemental Mathematical Operators", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[2E80-2EFF] CJK Radicals Supplement", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[2F00-2FDF] Kangxi Radicals", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[2FF0-2FFF] Ideographic Description Characters", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[3000-303F] CJK Symbols and Punctuation", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[3040-309F] Hiragana", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[30A0-30FF] Katakana", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[3100-312F] Bopomofo", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[3130-318F] Hangul Compatibility Jamo", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[3190-319F] Kanbun", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[31A0-31BF] Bopomofo Extended", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[3200-32FF] Enclosed CJK Letters and Months", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[3300-33FF] CJK Compatibility", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[3400-4DBF] CJK Unified Ideographs Extension A", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[4E00-9FAF] CJK Unified Ideographs", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[A000-A48F] Yi Syllables", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[A490-A4CF] Yi Radicals", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[AC00-D7AF] Hangul Syllables", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[D800-DBFF] High Surrogates", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[DC00-DFFF] Low Surrogate Area", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[E000-F8FF] Private Use Area", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[F900-FAFF] CJK Compatibility Ideographs", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[FB00-FB4F] Alphabetic Presentation Forms", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[FB50-FDFF] Arabic Presentation Forms-A", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[FE00-FE0F] Variation Selectors", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[FE20-FE2F] Combining Half Marks", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[FE30-FE4F] CJK Compatibility Forms", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[FE50-FE6F] Small Form Variants", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[FE70-FEFF] Arabic Presentation Forms-B", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[FF00-FFEF] Halfwidth and Fullwidth Forms", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[FFF0-FFFF] Specials", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[10300-1032F] Old Italic", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[10330-1034F] Gothic", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[10400-1044F] Deseret", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[1D000-1D0FF] Byzantine Musical Symbols", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[1D100-1D1FF] Musical Symbols", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[1D400-1D7FF] Mathematical Alphanumeric Symbols", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[20000-2A6DF] CJK Unified Ideographs Extension B", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[2F800-2FA1F] CJK Compatibility Ideographs Supplement", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[E0000-E007F] Tags", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[F0000-FFFFD] Supplementary Private Use Area-A", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgText", "[100000-10FFFD] Supplementary Private Use Area-B", 0, QApplication::UnicodeUTF8)
        );
        bUnicode->setText(QString());
        buttonOk->setText(QApplication::translate("QG_DlgText", "&OK", 0, QApplication::UnicodeUTF8));
        buttonCancel->setText(QApplication::translate("QG_DlgText", "Cancel", 0, QApplication::UnicodeUTF8));
        buttonCancel->setShortcut(QApplication::translate("QG_DlgText", "Esc", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QG_DlgText: public Ui_QG_DlgText {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QG_DLGTEXT_H
