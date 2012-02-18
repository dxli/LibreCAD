/********************************************************************************
** Form generated from reading UI file 'qg_selectionwidget.ui'
**
** Created: Sat Feb 18 16:41:51 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QG_SELECTIONWIDGET_H
#define UI_QG_SELECTIONWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QG_SelectionWidget
{
public:
    QGridLayout *gridLayout;
    QLabel *lLabel;
    QLabel *lLabelLength;
    QLabel *lEntities;
    QLabel *lTotalLength;

    void setupUi(QWidget *QG_SelectionWidget)
    {
        if (QG_SelectionWidget->objectName().isEmpty())
            QG_SelectionWidget->setObjectName(QString::fromUtf8("QG_SelectionWidget"));
        QG_SelectionWidget->resize(110, 29);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QG_SelectionWidget->sizePolicy().hasHeightForWidth());
        QG_SelectionWidget->setSizePolicy(sizePolicy);
        QG_SelectionWidget->setMinimumSize(QSize(100, 27));
        QG_SelectionWidget->setMaximumSize(QSize(160, 50));
        gridLayout = new QGridLayout(QG_SelectionWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setHorizontalSpacing(6);
        gridLayout->setVerticalSpacing(0);
        lLabel = new QLabel(QG_SelectionWidget);
        lLabel->setObjectName(QString::fromUtf8("lLabel"));
        lLabel->setMaximumSize(QSize(42, 16777215));
        QFont font;
        font.setFamily(QString::fromUtf8("Helvetica"));
        font.setPointSize(7);
        lLabel->setFont(font);
        lLabel->setFrameShape(QFrame::NoFrame);
        lLabel->setFrameShadow(QFrame::Plain);
        lLabel->setWordWrap(false);

        gridLayout->addWidget(lLabel, 0, 0, 1, 1);

        lLabelLength = new QLabel(QG_SelectionWidget);
        lLabelLength->setObjectName(QString::fromUtf8("lLabelLength"));
        lLabelLength->setMinimumSize(QSize(60, 0));
        lLabelLength->setMaximumSize(QSize(60, 16777215));
        lLabelLength->setFont(font);
        lLabelLength->setFrameShape(QFrame::NoFrame);
        lLabelLength->setFrameShadow(QFrame::Plain);
        lLabelLength->setWordWrap(false);

        gridLayout->addWidget(lLabelLength, 0, 1, 1, 1);

        lEntities = new QLabel(QG_SelectionWidget);
        lEntities->setObjectName(QString::fromUtf8("lEntities"));
        lEntities->setMaximumSize(QSize(42, 16777215));
        lEntities->setFont(font);
        lEntities->setFocusPolicy(Qt::ClickFocus);
        lEntities->setWordWrap(false);
        lEntities->setIndent(0);
        lEntities->setTextInteractionFlags(Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);

        gridLayout->addWidget(lEntities, 1, 0, 1, 1);

        lTotalLength = new QLabel(QG_SelectionWidget);
        lTotalLength->setObjectName(QString::fromUtf8("lTotalLength"));
        lTotalLength->setMaximumSize(QSize(42, 16777215));
        lTotalLength->setFont(font);
        lTotalLength->setFocusPolicy(Qt::ClickFocus);
        lTotalLength->setWordWrap(false);
        lTotalLength->setTextInteractionFlags(Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);

        gridLayout->addWidget(lTotalLength, 1, 1, 1, 1);


        retranslateUi(QG_SelectionWidget);

        QMetaObject::connectSlotsByName(QG_SelectionWidget);
    } // setupUi

    void retranslateUi(QWidget *QG_SelectionWidget)
    {
        QG_SelectionWidget->setWindowTitle(QApplication::translate("QG_SelectionWidget", "Selection", 0, QApplication::UnicodeUTF8));
        lLabel->setText(QApplication::translate("QG_SelectionWidget", "Selected", 0, QApplication::UnicodeUTF8));
        lLabelLength->setText(QApplication::translate("QG_SelectionWidget", "Total Length", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        lEntities->setToolTip(QApplication::translate("QG_SelectionWidget", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Helvetica'; font-size:7pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Number of selected entities</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        lEntities->setText(QApplication::translate("QG_SelectionWidget", "0", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        lTotalLength->setToolTip(QApplication::translate("QG_SelectionWidget", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Helvetica'; font-size:7pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Total length of selected entities</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        lTotalLength->setText(QApplication::translate("QG_SelectionWidget", "0", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QG_SelectionWidget: public Ui_QG_SelectionWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QG_SELECTIONWIDGET_H
