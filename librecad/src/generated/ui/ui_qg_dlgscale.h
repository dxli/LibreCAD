/********************************************************************************
** Form generated from reading UI file 'qg_dlgscale.ui'
**
** Created: Sat Feb 18 16:41:51 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QG_DLGSCALE_H
#define UI_QG_DLGSCALE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
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
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QG_DlgScale
{
public:
    QGroupBox *bgNumber;
    QVBoxLayout *vboxLayout;
    QRadioButton *rbMove;
    QRadioButton *rbCopy;
    QRadioButton *rbMultiCopy;
    QLineEdit *leNumber;
    QSpacerItem *spacer7;
    QWidget *layoutWidget;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacer;
    QPushButton *bOk;
    QPushButton *bCancel;
    QToolButton *toolButton;
    QWidget *layoutWidget_1;
    QHBoxLayout *hboxLayout1;
    QCheckBox *cbIsotropic;
    QCheckBox *cbCurrentLayer;
    QCheckBox *cbCurrentAttributes;
    QWidget *layoutWidget_2;
    QHBoxLayout *_2;
    QLabel *lFactor;
    QLineEdit *leFactorX;
    QSpacerItem *horizontalSpacer;
    QLabel *lFactor_2;
    QLineEdit *leFactorY;

    void setupUi(QDialog *QG_DlgScale)
    {
        if (QG_DlgScale->objectName().isEmpty())
            QG_DlgScale->setObjectName(QString::fromUtf8("QG_DlgScale"));
        QG_DlgScale->resize(422, 277);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QG_DlgScale->sizePolicy().hasHeightForWidth());
        QG_DlgScale->setSizePolicy(sizePolicy);
        QG_DlgScale->setMinimumSize(QSize(300, 190));
        bgNumber = new QGroupBox(QG_DlgScale);
        bgNumber->setObjectName(QString::fromUtf8("bgNumber"));
        bgNumber->setGeometry(QRect(9, 9, 159, 165));
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

        spacer7 = new QSpacerItem(20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout->addItem(spacer7);

        layoutWidget = new QWidget(QG_DlgScale);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(130, 240, 224, 29));
        hboxLayout = new QHBoxLayout(layoutWidget);
        hboxLayout->setSpacing(6);
        hboxLayout->setContentsMargins(11, 11, 11, 11);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacer);

        bOk = new QPushButton(layoutWidget);
        bOk->setObjectName(QString::fromUtf8("bOk"));
        bOk->setDefault(true);

        hboxLayout->addWidget(bOk);

        bCancel = new QPushButton(layoutWidget);
        bCancel->setObjectName(QString::fromUtf8("bCancel"));

        hboxLayout->addWidget(bCancel);

        toolButton = new QToolButton(QG_DlgScale);
        toolButton->setObjectName(QString::fromUtf8("toolButton"));
        toolButton->setGeometry(QRect(175, 10, 86, 88));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/extui/dlgmodifyscale.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton->setIcon(icon);
        toolButton->setIconSize(QSize(80, 80));
        layoutWidget_1 = new QWidget(QG_DlgScale);
        layoutWidget_1->setObjectName(QString::fromUtf8("layoutWidget_1"));
        layoutWidget_1->setGeometry(QRect(180, 100, 237, 41));
        hboxLayout1 = new QHBoxLayout(layoutWidget_1);
        hboxLayout1->setSpacing(6);
        hboxLayout1->setContentsMargins(11, 11, 11, 11);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        hboxLayout1->setContentsMargins(0, 0, 0, 0);
        cbIsotropic = new QCheckBox(layoutWidget_1);
        cbIsotropic->setObjectName(QString::fromUtf8("cbIsotropic"));
        cbIsotropic->setChecked(true);

        hboxLayout1->addWidget(cbIsotropic);

        cbCurrentLayer = new QCheckBox(QG_DlgScale);
        cbCurrentLayer->setObjectName(QString::fromUtf8("cbCurrentLayer"));
        cbCurrentLayer->setGeometry(QRect(175, 210, 146, 21));
        cbCurrentAttributes = new QCheckBox(QG_DlgScale);
        cbCurrentAttributes->setObjectName(QString::fromUtf8("cbCurrentAttributes"));
        cbCurrentAttributes->setGeometry(QRect(175, 190, 179, 21));
        layoutWidget_2 = new QWidget(QG_DlgScale);
        layoutWidget_2->setObjectName(QString::fromUtf8("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(180, 140, 237, 41));
        _2 = new QHBoxLayout(layoutWidget_2);
        _2->setSpacing(6);
        _2->setContentsMargins(11, 11, 11, 11);
        _2->setObjectName(QString::fromUtf8("_2"));
        _2->setContentsMargins(0, 0, 0, 0);
        lFactor = new QLabel(layoutWidget_2);
        lFactor->setObjectName(QString::fromUtf8("lFactor"));
        lFactor->setWordWrap(false);

        _2->addWidget(lFactor);

        leFactorX = new QLineEdit(layoutWidget_2);
        leFactorX->setObjectName(QString::fromUtf8("leFactorX"));

        _2->addWidget(leFactorX);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        _2->addItem(horizontalSpacer);

        lFactor_2 = new QLabel(layoutWidget_2);
        lFactor_2->setObjectName(QString::fromUtf8("lFactor_2"));
        lFactor_2->setWordWrap(false);

        _2->addWidget(lFactor_2);

        leFactorY = new QLineEdit(layoutWidget_2);
        leFactorY->setObjectName(QString::fromUtf8("leFactorY"));
        leFactorY->setReadOnly(true);

        _2->addWidget(leFactorY);


        retranslateUi(QG_DlgScale);
        QObject::connect(rbMove, SIGNAL(toggled(bool)), leNumber, SLOT(setDisabled(bool)));
        QObject::connect(rbCopy, SIGNAL(toggled(bool)), leNumber, SLOT(setDisabled(bool)));
        QObject::connect(rbMultiCopy, SIGNAL(toggled(bool)), leNumber, SLOT(setEnabled(bool)));
        QObject::connect(bOk, SIGNAL(clicked()), QG_DlgScale, SLOT(accept()));
        QObject::connect(bCancel, SIGNAL(clicked()), QG_DlgScale, SLOT(reject()));

        QMetaObject::connectSlotsByName(QG_DlgScale);
    } // setupUi

    void retranslateUi(QDialog *QG_DlgScale)
    {
        QG_DlgScale->setWindowTitle(QApplication::translate("QG_DlgScale", "Scaling Options", 0, QApplication::UnicodeUTF8));
        bgNumber->setTitle(QApplication::translate("QG_DlgScale", "Number of copies", 0, QApplication::UnicodeUTF8));
        rbMove->setText(QApplication::translate("QG_DlgScale", "&Delete Original", 0, QApplication::UnicodeUTF8));
        rbCopy->setText(QApplication::translate("QG_DlgScale", "&Keep Original", 0, QApplication::UnicodeUTF8));
        rbMultiCopy->setText(QApplication::translate("QG_DlgScale", "&Multiple Copies", 0, QApplication::UnicodeUTF8));
        bOk->setText(QApplication::translate("QG_DlgScale", "&OK", 0, QApplication::UnicodeUTF8));
        bCancel->setText(QApplication::translate("QG_DlgScale", "&Cancel", 0, QApplication::UnicodeUTF8));
        bCancel->setShortcut(QApplication::translate("QG_DlgScale", "Esc", 0, QApplication::UnicodeUTF8));
        toolButton->setText(QApplication::translate("QG_DlgScale", "...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        cbIsotropic->setToolTip(QApplication::translate("QG_DlgScale", "Scale by the same factor at both x- and y- directions", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        cbIsotropic->setText(QApplication::translate("QG_DlgScale", "Isotropic Scaling", 0, QApplication::UnicodeUTF8));
        cbCurrentLayer->setText(QApplication::translate("QG_DlgScale", "Use current &layer", 0, QApplication::UnicodeUTF8));
        cbCurrentAttributes->setText(QApplication::translate("QG_DlgScale", "Use current &attributes", 0, QApplication::UnicodeUTF8));
        lFactor->setText(QApplication::translate("QG_DlgScale", "X", 0, QApplication::UnicodeUTF8));
        leFactorX->setText(QApplication::translate("QG_DlgScale", "1.0", 0, QApplication::UnicodeUTF8));
        lFactor_2->setText(QApplication::translate("QG_DlgScale", "Y", 0, QApplication::UnicodeUTF8));
        leFactorY->setText(QApplication::translate("QG_DlgScale", "1.0", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QG_DlgScale: public Ui_QG_DlgScale {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QG_DLGSCALE_H
