/********************************************************************************
** Form generated from reading UI file 'qg_printpreviewoptions.ui'
**
** Created: Sat Feb 18 16:41:51 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QG_PRINTPREVIEWOPTIONS_H
#define UI_QG_PRINTPREVIEWOPTIONS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QToolButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QG_PrintPreviewOptions
{
public:
    QHBoxLayout *hboxLayout;
    QComboBox *cbScale;
    QToolButton *bBlackWhite;
    QToolButton *bCenter;
    QToolButton *bFit;
    QFrame *sep1_2;

    void setupUi(QWidget *QG_PrintPreviewOptions)
    {
        if (QG_PrintPreviewOptions->objectName().isEmpty())
            QG_PrintPreviewOptions->setObjectName(QString::fromUtf8("QG_PrintPreviewOptions"));
        QG_PrintPreviewOptions->resize(200, 22);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QG_PrintPreviewOptions->sizePolicy().hasHeightForWidth());
        QG_PrintPreviewOptions->setSizePolicy(sizePolicy);
        QG_PrintPreviewOptions->setMinimumSize(QSize(200, 22));
        hboxLayout = new QHBoxLayout(QG_PrintPreviewOptions);
        hboxLayout->setSpacing(6);
        hboxLayout->setContentsMargins(1, 1, 1, 1);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        cbScale = new QComboBox(QG_PrintPreviewOptions);
        cbScale->setObjectName(QString::fromUtf8("cbScale"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(cbScale->sizePolicy().hasHeightForWidth());
        cbScale->setSizePolicy(sizePolicy1);
        cbScale->setMinimumSize(QSize(110, 0));
        cbScale->setEditable(true);

        hboxLayout->addWidget(cbScale);

        bBlackWhite = new QToolButton(QG_PrintPreviewOptions);
        bBlackWhite->setObjectName(QString::fromUtf8("bBlackWhite"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(bBlackWhite->sizePolicy().hasHeightForWidth());
        bBlackWhite->setSizePolicy(sizePolicy2);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/extui/printbw.png"), QSize(), QIcon::Normal, QIcon::Off);
        bBlackWhite->setIcon(icon);
        bBlackWhite->setCheckable(true);

        hboxLayout->addWidget(bBlackWhite);

        bCenter = new QToolButton(QG_PrintPreviewOptions);
        bCenter->setObjectName(QString::fromUtf8("bCenter"));
        sizePolicy2.setHeightForWidth(bCenter->sizePolicy().hasHeightForWidth());
        bCenter->setSizePolicy(sizePolicy2);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/extui/printcenter.png"), QSize(), QIcon::Normal, QIcon::Off);
        bCenter->setIcon(icon1);

        hboxLayout->addWidget(bCenter);

        bFit = new QToolButton(QG_PrintPreviewOptions);
        bFit->setObjectName(QString::fromUtf8("bFit"));
        sizePolicy2.setHeightForWidth(bFit->sizePolicy().hasHeightForWidth());
        bFit->setSizePolicy(sizePolicy2);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/extui/printfit.png"), QSize(), QIcon::Normal, QIcon::Off);
        bFit->setIcon(icon2);

        hboxLayout->addWidget(bFit);

        sep1_2 = new QFrame(QG_PrintPreviewOptions);
        sep1_2->setObjectName(QString::fromUtf8("sep1_2"));
        sizePolicy2.setHeightForWidth(sep1_2->sizePolicy().hasHeightForWidth());
        sep1_2->setSizePolicy(sizePolicy2);
        sep1_2->setFrameShape(QFrame::VLine);
        sep1_2->setFrameShadow(QFrame::Sunken);

        hboxLayout->addWidget(sep1_2);


        retranslateUi(QG_PrintPreviewOptions);
        QObject::connect(bCenter, SIGNAL(clicked()), QG_PrintPreviewOptions, SLOT(center()));
        QObject::connect(bBlackWhite, SIGNAL(toggled(bool)), QG_PrintPreviewOptions, SLOT(setBlackWhite(bool)));
        QObject::connect(cbScale, SIGNAL(currentIndexChanged(QString)), QG_PrintPreviewOptions, SLOT(scale(QString)));
        QObject::connect(bFit, SIGNAL(clicked()), QG_PrintPreviewOptions, SLOT(fit()));

        QMetaObject::connectSlotsByName(QG_PrintPreviewOptions);
    } // setupUi

    void retranslateUi(QWidget *QG_PrintPreviewOptions)
    {
        QG_PrintPreviewOptions->setWindowTitle(QApplication::translate("QG_PrintPreviewOptions", "Print Preview Options", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        QG_PrintPreviewOptions->setToolTip(QString());
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        bBlackWhite->setToolTip(QApplication::translate("QG_PrintPreviewOptions", "Toggle Black / White mode", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bBlackWhite->setText(QString());
#ifndef QT_NO_TOOLTIP
        bCenter->setToolTip(QApplication::translate("QG_PrintPreviewOptions", "Center to page", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bCenter->setText(QString());
#ifndef QT_NO_TOOLTIP
        bFit->setToolTip(QApplication::translate("QG_PrintPreviewOptions", "Fit to page", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        bFit->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class QG_PrintPreviewOptions: public Ui_QG_PrintPreviewOptions {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QG_PRINTPREVIEWOPTIONS_H
