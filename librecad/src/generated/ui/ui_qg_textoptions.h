/********************************************************************************
** Form generated from reading UI file 'qg_textoptions.ui'
**
** Created: Sat Feb 18 16:41:51 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QG_TEXTOPTIONS_H
#define UI_QG_TEXTOPTIONS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QG_TextOptions
{
public:
    QHBoxLayout *hboxLayout;
    QLabel *lText;
    QTextEdit *teText;
    QLabel *lAngle;
    QLineEdit *leAngle;
    QFrame *sep1;

    void setupUi(QWidget *QG_TextOptions)
    {
        if (QG_TextOptions->objectName().isEmpty())
            QG_TextOptions->setObjectName(QString::fromUtf8("QG_TextOptions"));
        QG_TextOptions->resize(300, 24);
        QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(0));
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QG_TextOptions->sizePolicy().hasHeightForWidth());
        QG_TextOptions->setSizePolicy(sizePolicy);
        QG_TextOptions->setMinimumSize(QSize(200, 22));
        QG_TextOptions->setMaximumSize(QSize(300, 32767));
        hboxLayout = new QHBoxLayout(QG_TextOptions);
        hboxLayout->setSpacing(6);
        hboxLayout->setContentsMargins(1, 1, 1, 1);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        lText = new QLabel(QG_TextOptions);
        lText->setObjectName(QString::fromUtf8("lText"));
        lText->setWordWrap(false);

        hboxLayout->addWidget(lText);

        teText = new QTextEdit(QG_TextOptions);
        teText->setObjectName(QString::fromUtf8("teText"));
        QSizePolicy sizePolicy1(static_cast<QSizePolicy::Policy>(5), static_cast<QSizePolicy::Policy>(0));
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(teText->sizePolicy().hasHeightForWidth());
        teText->setSizePolicy(sizePolicy1);
        teText->setMinimumSize(QSize(0, 22));
        teText->setMaximumSize(QSize(32767, 22));
        teText->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        teText->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        hboxLayout->addWidget(teText);

        lAngle = new QLabel(QG_TextOptions);
        lAngle->setObjectName(QString::fromUtf8("lAngle"));
        lAngle->setWordWrap(false);

        hboxLayout->addWidget(lAngle);

        leAngle = new QLineEdit(QG_TextOptions);
        leAngle->setObjectName(QString::fromUtf8("leAngle"));

        hboxLayout->addWidget(leAngle);

        sep1 = new QFrame(QG_TextOptions);
        sep1->setObjectName(QString::fromUtf8("sep1"));
        QSizePolicy sizePolicy2(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(1));
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(sep1->sizePolicy().hasHeightForWidth());
        sep1->setSizePolicy(sizePolicy2);
        sep1->setFrameShape(QFrame::VLine);
        sep1->setFrameShadow(QFrame::Sunken);

        hboxLayout->addWidget(sep1);


        retranslateUi(QG_TextOptions);
        QObject::connect(teText, SIGNAL(textChanged()), QG_TextOptions, SLOT(updateText()));
        QObject::connect(leAngle, SIGNAL(textChanged(QString)), QG_TextOptions, SLOT(updateAngle()));

        QMetaObject::connectSlotsByName(QG_TextOptions);
    } // setupUi

    void retranslateUi(QWidget *QG_TextOptions)
    {
        QG_TextOptions->setWindowTitle(QApplication::translate("QG_TextOptions", "Text Options", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        QG_TextOptions->setProperty("toolTip", QVariant(QString()));
#endif // QT_NO_TOOLTIP
        lText->setText(QApplication::translate("QG_TextOptions", "Text:", 0, QApplication::UnicodeUTF8));
        lAngle->setText(QApplication::translate("QG_TextOptions", "Angle:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QG_TextOptions: public Ui_QG_TextOptions {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QG_TEXTOPTIONS_H
