/********************************************************************************
** Form generated from reading UI file 'qg_dimoptions.ui'
**
** Created: Sat Feb 18 16:41:50 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QG_DIMOPTIONS_H
#define UI_QG_DIMOPTIONS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QG_DimOptions
{
public:
    QHBoxLayout *hboxLayout;
    QLabel *lLabel;
    QToolButton *bDiameter;
    QLineEdit *leLabel;
    QComboBox *cbSymbol;
    QToolButton *toolButton;
    QLineEdit *leTol1;
    QToolButton *toolButton_2;
    QLineEdit *leTol2;
    QFrame *sep1;

    void setupUi(QWidget *QG_DimOptions)
    {
        if (QG_DimOptions->objectName().isEmpty())
            QG_DimOptions->setObjectName(QString::fromUtf8("QG_DimOptions"));
        QG_DimOptions->resize(420, 22);
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QG_DimOptions->sizePolicy().hasHeightForWidth());
        QG_DimOptions->setSizePolicy(sizePolicy);
        QG_DimOptions->setMinimumSize(QSize(420, 22));
        QG_DimOptions->setMaximumSize(QSize(420, 22));
        hboxLayout = new QHBoxLayout(QG_DimOptions);
        hboxLayout->setSpacing(6);
        hboxLayout->setContentsMargins(1, 1, 1, 1);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        lLabel = new QLabel(QG_DimOptions);
        lLabel->setObjectName(QString::fromUtf8("lLabel"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lLabel->sizePolicy().hasHeightForWidth());
        lLabel->setSizePolicy(sizePolicy1);
        lLabel->setFrameShape(QFrame::NoFrame);
        lLabel->setFrameShadow(QFrame::Plain);
        lLabel->setWordWrap(false);

        hboxLayout->addWidget(lLabel);

        bDiameter = new QToolButton(QG_DimOptions);
        bDiameter->setObjectName(QString::fromUtf8("bDiameter"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(bDiameter->sizePolicy().hasHeightForWidth());
        bDiameter->setSizePolicy(sizePolicy2);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/extui/char_diameter.png"), QSize(), QIcon::Normal, QIcon::Off);
        bDiameter->setIcon(icon);
        bDiameter->setCheckable(true);

        hboxLayout->addWidget(bDiameter);

        leLabel = new QLineEdit(QG_DimOptions);
        leLabel->setObjectName(QString::fromUtf8("leLabel"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(leLabel->sizePolicy().hasHeightForWidth());
        leLabel->setSizePolicy(sizePolicy3);

        hboxLayout->addWidget(leLabel);

        cbSymbol = new QComboBox(QG_DimOptions);
        cbSymbol->setObjectName(QString::fromUtf8("cbSymbol"));
        QSizePolicy sizePolicy4(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(cbSymbol->sizePolicy().hasHeightForWidth());
        cbSymbol->setSizePolicy(sizePolicy4);

        hboxLayout->addWidget(cbSymbol);

        toolButton = new QToolButton(QG_DimOptions);
        toolButton->setObjectName(QString::fromUtf8("toolButton"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/extui/tolerance1.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton->setIcon(icon1);

        hboxLayout->addWidget(toolButton);

        leTol1 = new QLineEdit(QG_DimOptions);
        leTol1->setObjectName(QString::fromUtf8("leTol1"));
        sizePolicy3.setHeightForWidth(leTol1->sizePolicy().hasHeightForWidth());
        leTol1->setSizePolicy(sizePolicy3);

        hboxLayout->addWidget(leTol1);

        toolButton_2 = new QToolButton(QG_DimOptions);
        toolButton_2->setObjectName(QString::fromUtf8("toolButton_2"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/extui/tolerance2.png"), QSize(), QIcon::Normal, QIcon::Off);
        toolButton_2->setIcon(icon2);

        hboxLayout->addWidget(toolButton_2);

        leTol2 = new QLineEdit(QG_DimOptions);
        leTol2->setObjectName(QString::fromUtf8("leTol2"));
        sizePolicy3.setHeightForWidth(leTol2->sizePolicy().hasHeightForWidth());
        leTol2->setSizePolicy(sizePolicy3);

        hboxLayout->addWidget(leTol2);

        sep1 = new QFrame(QG_DimOptions);
        sep1->setObjectName(QString::fromUtf8("sep1"));
        QSizePolicy sizePolicy5(QSizePolicy::Fixed, QSizePolicy::Minimum);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(sep1->sizePolicy().hasHeightForWidth());
        sep1->setSizePolicy(sizePolicy5);
        sep1->setFrameShape(QFrame::VLine);
        sep1->setFrameShadow(QFrame::Sunken);

        hboxLayout->addWidget(sep1);


        retranslateUi(QG_DimOptions);
        QObject::connect(leLabel, SIGNAL(textChanged(QString)), QG_DimOptions, SLOT(updateLabel()));
        QObject::connect(bDiameter, SIGNAL(toggled(bool)), QG_DimOptions, SLOT(updateLabel()));
        QObject::connect(leTol1, SIGNAL(textChanged(QString)), QG_DimOptions, SLOT(updateLabel()));
        QObject::connect(leTol2, SIGNAL(textChanged(QString)), QG_DimOptions, SLOT(updateLabel()));
        QObject::connect(cbSymbol, SIGNAL(activated(QString)), QG_DimOptions, SLOT(insertSign(QString)));

        QMetaObject::connectSlotsByName(QG_DimOptions);
    } // setupUi

    void retranslateUi(QWidget *QG_DimOptions)
    {
        QG_DimOptions->setWindowTitle(QApplication::translate("QG_DimOptions", "Dimension Options", 0, QApplication::UnicodeUTF8));
        lLabel->setText(QApplication::translate("QG_DimOptions", "Label:", 0, QApplication::UnicodeUTF8));
        bDiameter->setText(QString());
        cbSymbol->clear();
        cbSymbol->insertItems(0, QStringList()
         << QApplication::translate("QG_DimOptions", "\303\270", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DimOptions", "\302\260", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DimOptions", "\302\261", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DimOptions", "\302\266", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DimOptions", "\303\227", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DimOptions", "\303\267", 0, QApplication::UnicodeUTF8)
        );
        toolButton->setText(QApplication::translate("QG_DimOptions", "...", 0, QApplication::UnicodeUTF8));
        toolButton_2->setText(QApplication::translate("QG_DimOptions", "...", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QG_DimOptions: public Ui_QG_DimOptions {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QG_DIMOPTIONS_H
