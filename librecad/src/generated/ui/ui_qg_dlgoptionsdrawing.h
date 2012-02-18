/********************************************************************************
** Form generated from reading UI file 'qg_dlgoptionsdrawing.ui'
**
** Created: Sat Feb 18 16:41:50 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QG_DLGOPTIONSDRAWING_H
#define UI_QG_DLGOPTIONSDRAWING_H

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
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QG_DlgOptionsDrawing
{
public:
    QGridLayout *gridLayout_2;
    QTabWidget *tabWidget;
    QWidget *tab1;
    QVBoxLayout *vboxLayout;
    QGroupBox *buttonGroup3;
    QVBoxLayout *vboxLayout1;
    QHBoxLayout *hboxLayout;
    QComboBox *cbPaperFormat;
    QSpacerItem *spacer9;
    QHBoxLayout *hboxLayout1;
    QRadioButton *rbLandscape;
    QRadioButton *rbPortrait;
    QSpacerItem *spacer6;
    QGridLayout *gridLayout_3;
    QSpacerItem *spacer8;
    QLineEdit *lePaperHeight;
    QLabel *lPageHeight;
    QLabel *lPageWidth;
    QLineEdit *lePaperWidth;
    QSpacerItem *spacer7;
    QWidget *tab2;
    QVBoxLayout *vboxLayout2;
    QGroupBox *bgUnit;
    QHBoxLayout *hboxLayout2;
    QLabel *lUnit;
    QComboBox *cbUnit;
    QSpacerItem *spacer19;
    QHBoxLayout *hboxLayout3;
    QVBoxLayout *vboxLayout3;
    QGroupBox *bgLength;
    QGridLayout *gridLayout_4;
    QLabel *lLengthFormat;
    QComboBox *cbLengthFormat;
    QComboBox *cbLengthPrecision;
    QLabel *lLengthPrecision;
    QSpacerItem *spacer15;
    QSpacerItem *spacer15_2;
    QGroupBox *bgLengthPreview;
    QHBoxLayout *hboxLayout4;
    QLabel *lLinear;
    QVBoxLayout *vboxLayout4;
    QGroupBox *bgAngle;
    QGridLayout *gridLayout_5;
    QLabel *lAngleFormat;
    QSpacerItem *spacer15_3;
    QSpacerItem *spacer15_2_2;
    QComboBox *cbAngleFormat;
    QLabel *lAnglePrecision;
    QComboBox *cbAnglePrecision;
    QGroupBox *bgAnglePreview;
    QHBoxLayout *hboxLayout5;
    QLabel *lAngular;
    QWidget *tab3;
    QGroupBox *bgGrid;
    QGridLayout *gridLayout_1;
    QCheckBox *cbGridOn;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QLabel *lCrosshair;
    QRadioButton *rbCrosshairLeft;
    QRadioButton *rbCrosshairTop;
    QRadioButton *rbCrosshairRight;
    QRadioButton *rbOrthogonalGrid;
    QRadioButton *rbIsometricGrid;
    QLabel *lXSpacing;
    QComboBox *cbYSpacing;
    QLabel *lYSpacing;
    QComboBox *cbXSpacing;
    QWidget *tab4;
    QGridLayout *gridLayout;
    QLabel *lDimTextHeight;
    QComboBox *cbDimTextHeight;
    QLabel *lDimUnit1;
    QLabel *lDimExe;
    QComboBox *cbDimExe;
    QLabel *lDimUnit2;
    QLabel *lDimExo;
    QComboBox *cbDimExo;
    QLabel *lDimUnit3;
    QLabel *lDimGap;
    QComboBox *cbDimGap;
    QLabel *lDimUnit4;
    QLabel *lDimAsz;
    QComboBox *cbDimAsz;
    QLabel *lDimUnit5;
    QLabel *lDimTsz;
    QComboBox *cbDimTsz;
    QLabel *lDimUnit5_2;
    QSpacerItem *spacer11;
    QSpacerItem *spacer11_2;
    QSpacerItem *spacer11_2_2;
    QWidget *tab5;
    QVBoxLayout *vboxLayout5;
    QHBoxLayout *hboxLayout6;
    QLabel *lSplineSegs;
    QComboBox *cbSplineSegs;
    QSpacerItem *spacer19_2;
    QHBoxLayout *hboxLayout7;
    QSpacerItem *Horizontal_Spacing2;
    QPushButton *buttonOk;
    QPushButton *buttonCancel;

    void setupUi(QDialog *QG_DlgOptionsDrawing)
    {
        if (QG_DlgOptionsDrawing->objectName().isEmpty())
            QG_DlgOptionsDrawing->setObjectName(QString::fromUtf8("QG_DlgOptionsDrawing"));
        QG_DlgOptionsDrawing->resize(505, 372);
        QG_DlgOptionsDrawing->setSizeGripEnabled(true);
        gridLayout_2 = new QGridLayout(QG_DlgOptionsDrawing);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        tabWidget = new QTabWidget(QG_DlgOptionsDrawing);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab1 = new QWidget();
        tab1->setObjectName(QString::fromUtf8("tab1"));
        vboxLayout = new QVBoxLayout(tab1);
        vboxLayout->setSpacing(6);
        vboxLayout->setContentsMargins(11, 11, 11, 11);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        buttonGroup3 = new QGroupBox(tab1);
        buttonGroup3->setObjectName(QString::fromUtf8("buttonGroup3"));
        vboxLayout1 = new QVBoxLayout(buttonGroup3);
        vboxLayout1->setSpacing(6);
        vboxLayout1->setContentsMargins(11, 11, 11, 11);
        vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        cbPaperFormat = new QComboBox(buttonGroup3);
        cbPaperFormat->setObjectName(QString::fromUtf8("cbPaperFormat"));

        hboxLayout->addWidget(cbPaperFormat);

        spacer9 = new QSpacerItem(211, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacer9);


        vboxLayout1->addLayout(hboxLayout);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(6);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        rbLandscape = new QRadioButton(buttonGroup3);
        rbLandscape->setObjectName(QString::fromUtf8("rbLandscape"));

        hboxLayout1->addWidget(rbLandscape);

        rbPortrait = new QRadioButton(buttonGroup3);
        rbPortrait->setObjectName(QString::fromUtf8("rbPortrait"));

        hboxLayout1->addWidget(rbPortrait);

        spacer6 = new QSpacerItem(231, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout1->addItem(spacer6);


        vboxLayout1->addLayout(hboxLayout1);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setSpacing(6);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        spacer8 = new QSpacerItem(221, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(spacer8, 2, 2, 1, 1);

        lePaperHeight = new QLineEdit(buttonGroup3);
        lePaperHeight->setObjectName(QString::fromUtf8("lePaperHeight"));

        gridLayout_3->addWidget(lePaperHeight, 1, 1, 2, 1);

        lPageHeight = new QLabel(buttonGroup3);
        lPageHeight->setObjectName(QString::fromUtf8("lPageHeight"));
        lPageHeight->setWordWrap(false);

        gridLayout_3->addWidget(lPageHeight, 2, 0, 1, 1);

        lPageWidth = new QLabel(buttonGroup3);
        lPageWidth->setObjectName(QString::fromUtf8("lPageWidth"));
        lPageWidth->setWordWrap(false);

        gridLayout_3->addWidget(lPageWidth, 0, 0, 1, 1);

        lePaperWidth = new QLineEdit(buttonGroup3);
        lePaperWidth->setObjectName(QString::fromUtf8("lePaperWidth"));

        gridLayout_3->addWidget(lePaperWidth, 0, 1, 1, 1);

        spacer7 = new QSpacerItem(201, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(spacer7, 0, 2, 2, 1);


        vboxLayout1->addLayout(gridLayout_3);


        vboxLayout->addWidget(buttonGroup3);

        tabWidget->addTab(tab1, QString());
        tab2 = new QWidget();
        tab2->setObjectName(QString::fromUtf8("tab2"));
        vboxLayout2 = new QVBoxLayout(tab2);
        vboxLayout2->setSpacing(6);
        vboxLayout2->setContentsMargins(11, 11, 11, 11);
        vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
        bgUnit = new QGroupBox(tab2);
        bgUnit->setObjectName(QString::fromUtf8("bgUnit"));
        hboxLayout2 = new QHBoxLayout(bgUnit);
        hboxLayout2->setSpacing(6);
        hboxLayout2->setContentsMargins(11, 11, 11, 11);
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        lUnit = new QLabel(bgUnit);
        lUnit->setObjectName(QString::fromUtf8("lUnit"));
        lUnit->setWordWrap(false);

        hboxLayout2->addWidget(lUnit);

        cbUnit = new QComboBox(bgUnit);
        cbUnit->setObjectName(QString::fromUtf8("cbUnit"));

        hboxLayout2->addWidget(cbUnit);

        spacer19 = new QSpacerItem(71, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout2->addItem(spacer19);


        vboxLayout2->addWidget(bgUnit);

        hboxLayout3 = new QHBoxLayout();
        hboxLayout3->setSpacing(6);
        hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
        vboxLayout3 = new QVBoxLayout();
        vboxLayout3->setSpacing(6);
        vboxLayout3->setObjectName(QString::fromUtf8("vboxLayout3"));
        bgLength = new QGroupBox(tab2);
        bgLength->setObjectName(QString::fromUtf8("bgLength"));
        gridLayout_4 = new QGridLayout(bgLength);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        lLengthFormat = new QLabel(bgLength);
        lLengthFormat->setObjectName(QString::fromUtf8("lLengthFormat"));
        lLengthFormat->setWordWrap(false);

        gridLayout_4->addWidget(lLengthFormat, 0, 0, 1, 1);

        cbLengthFormat = new QComboBox(bgLength);
        cbLengthFormat->setObjectName(QString::fromUtf8("cbLengthFormat"));

        gridLayout_4->addWidget(cbLengthFormat, 0, 1, 1, 1);

        cbLengthPrecision = new QComboBox(bgLength);
        cbLengthPrecision->setObjectName(QString::fromUtf8("cbLengthPrecision"));

        gridLayout_4->addWidget(cbLengthPrecision, 1, 1, 1, 1);

        lLengthPrecision = new QLabel(bgLength);
        lLengthPrecision->setObjectName(QString::fromUtf8("lLengthPrecision"));
        lLengthPrecision->setWordWrap(false);

        gridLayout_4->addWidget(lLengthPrecision, 1, 0, 1, 1);

        spacer15 = new QSpacerItem(20, 31, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(spacer15, 2, 1, 1, 1);

        spacer15_2 = new QSpacerItem(20, 31, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(spacer15_2, 2, 0, 1, 1);


        vboxLayout3->addWidget(bgLength);

        bgLengthPreview = new QGroupBox(tab2);
        bgLengthPreview->setObjectName(QString::fromUtf8("bgLengthPreview"));
        hboxLayout4 = new QHBoxLayout(bgLengthPreview);
        hboxLayout4->setSpacing(6);
        hboxLayout4->setContentsMargins(11, 11, 11, 11);
        hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
        lLinear = new QLabel(bgLengthPreview);
        lLinear->setObjectName(QString::fromUtf8("lLinear"));
        lLinear->setWordWrap(false);

        hboxLayout4->addWidget(lLinear);


        vboxLayout3->addWidget(bgLengthPreview);


        hboxLayout3->addLayout(vboxLayout3);

        vboxLayout4 = new QVBoxLayout();
        vboxLayout4->setSpacing(6);
        vboxLayout4->setObjectName(QString::fromUtf8("vboxLayout4"));
        bgAngle = new QGroupBox(tab2);
        bgAngle->setObjectName(QString::fromUtf8("bgAngle"));
        gridLayout_5 = new QGridLayout(bgAngle);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        lAngleFormat = new QLabel(bgAngle);
        lAngleFormat->setObjectName(QString::fromUtf8("lAngleFormat"));
        lAngleFormat->setWordWrap(false);

        gridLayout_5->addWidget(lAngleFormat, 0, 0, 1, 1);

        spacer15_3 = new QSpacerItem(20, 31, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_5->addItem(spacer15_3, 2, 1, 1, 1);

        spacer15_2_2 = new QSpacerItem(20, 31, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_5->addItem(spacer15_2_2, 2, 0, 1, 1);

        cbAngleFormat = new QComboBox(bgAngle);
        cbAngleFormat->setObjectName(QString::fromUtf8("cbAngleFormat"));

        gridLayout_5->addWidget(cbAngleFormat, 0, 1, 1, 1);

        lAnglePrecision = new QLabel(bgAngle);
        lAnglePrecision->setObjectName(QString::fromUtf8("lAnglePrecision"));
        lAnglePrecision->setWordWrap(false);

        gridLayout_5->addWidget(lAnglePrecision, 1, 0, 1, 1);

        cbAnglePrecision = new QComboBox(bgAngle);
        cbAnglePrecision->setObjectName(QString::fromUtf8("cbAnglePrecision"));

        gridLayout_5->addWidget(cbAnglePrecision, 1, 1, 1, 1);


        vboxLayout4->addWidget(bgAngle);

        bgAnglePreview = new QGroupBox(tab2);
        bgAnglePreview->setObjectName(QString::fromUtf8("bgAnglePreview"));
        hboxLayout5 = new QHBoxLayout(bgAnglePreview);
        hboxLayout5->setSpacing(6);
        hboxLayout5->setContentsMargins(11, 11, 11, 11);
        hboxLayout5->setObjectName(QString::fromUtf8("hboxLayout5"));
        lAngular = new QLabel(bgAnglePreview);
        lAngular->setObjectName(QString::fromUtf8("lAngular"));
        lAngular->setWordWrap(false);

        hboxLayout5->addWidget(lAngular);


        vboxLayout4->addWidget(bgAnglePreview);


        hboxLayout3->addLayout(vboxLayout4);


        vboxLayout2->addLayout(hboxLayout3);

        tabWidget->addTab(tab2, QString());
        tab3 = new QWidget();
        tab3->setObjectName(QString::fromUtf8("tab3"));
        bgGrid = new QGroupBox(tab3);
        bgGrid->setObjectName(QString::fromUtf8("bgGrid"));
        bgGrid->setGeometry(QRect(9, 9, 414, 126));
        bgGrid->setMinimumSize(QSize(120, 60));
        gridLayout_1 = new QGridLayout(bgGrid);
        gridLayout_1->setSpacing(6);
        gridLayout_1->setContentsMargins(11, 11, 11, 11);
        gridLayout_1->setObjectName(QString::fromUtf8("gridLayout_1"));
        cbGridOn = new QCheckBox(bgGrid);
        cbGridOn->setObjectName(QString::fromUtf8("cbGridOn"));

        gridLayout_1->addWidget(cbGridOn, 0, 0, 1, 1);

        widget = new QWidget(bgGrid);
        widget->setObjectName(QString::fromUtf8("widget"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        widget->setMinimumSize(QSize(0, 0));
        widget->setMaximumSize(QSize(250, 16777215));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, -1, 0, -1);
        lCrosshair = new QLabel(widget);
        lCrosshair->setObjectName(QString::fromUtf8("lCrosshair"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lCrosshair->sizePolicy().hasHeightForWidth());
        lCrosshair->setSizePolicy(sizePolicy1);
        lCrosshair->setMinimumSize(QSize(70, 0));

        horizontalLayout->addWidget(lCrosshair);

        rbCrosshairLeft = new QRadioButton(widget);
        rbCrosshairLeft->setObjectName(QString::fromUtf8("rbCrosshairLeft"));
        sizePolicy1.setHeightForWidth(rbCrosshairLeft->sizePolicy().hasHeightForWidth());
        rbCrosshairLeft->setSizePolicy(sizePolicy1);
        rbCrosshairLeft->setMinimumSize(QSize(55, 0));

        horizontalLayout->addWidget(rbCrosshairLeft);

        rbCrosshairTop = new QRadioButton(widget);
        rbCrosshairTop->setObjectName(QString::fromUtf8("rbCrosshairTop"));
        sizePolicy1.setHeightForWidth(rbCrosshairTop->sizePolicy().hasHeightForWidth());
        rbCrosshairTop->setSizePolicy(sizePolicy1);
        rbCrosshairTop->setMinimumSize(QSize(50, 0));

        horizontalLayout->addWidget(rbCrosshairTop);

        rbCrosshairRight = new QRadioButton(widget);
        rbCrosshairRight->setObjectName(QString::fromUtf8("rbCrosshairRight"));
        QSizePolicy sizePolicy2(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(rbCrosshairRight->sizePolicy().hasHeightForWidth());
        rbCrosshairRight->setSizePolicy(sizePolicy2);
        rbCrosshairRight->setMinimumSize(QSize(60, 0));

        horizontalLayout->addWidget(rbCrosshairRight);


        gridLayout_1->addWidget(widget, 3, 1, 1, 2);

        rbOrthogonalGrid = new QRadioButton(bgGrid);
        rbOrthogonalGrid->setObjectName(QString::fromUtf8("rbOrthogonalGrid"));
        rbOrthogonalGrid->setAutoExclusive(true);

        gridLayout_1->addWidget(rbOrthogonalGrid, 1, 0, 1, 1);

        rbIsometricGrid = new QRadioButton(bgGrid);
        rbIsometricGrid->setObjectName(QString::fromUtf8("rbIsometricGrid"));
        rbIsometricGrid->setAutoExclusive(false);

        gridLayout_1->addWidget(rbIsometricGrid, 1, 1, 1, 1);

        lXSpacing = new QLabel(tab3);
        lXSpacing->setObjectName(QString::fromUtf8("lXSpacing"));
        lXSpacing->setGeometry(QRect(11, 151, 65, 16));
        lXSpacing->setWordWrap(false);
        cbYSpacing = new QComboBox(tab3);
        cbYSpacing->setObjectName(QString::fromUtf8("cbYSpacing"));
        cbYSpacing->setGeometry(QRect(82, 181, 90, 24));
        cbYSpacing->setEditable(true);
        lYSpacing = new QLabel(tab3);
        lYSpacing->setObjectName(QString::fromUtf8("lYSpacing"));
        lYSpacing->setGeometry(QRect(11, 181, 65, 16));
        lYSpacing->setWordWrap(false);
        cbXSpacing = new QComboBox(tab3);
        cbXSpacing->setObjectName(QString::fromUtf8("cbXSpacing"));
        cbXSpacing->setGeometry(QRect(82, 151, 90, 24));
        cbXSpacing->setEditable(true);
        tabWidget->addTab(tab3, QString());
        tab4 = new QWidget();
        tab4->setObjectName(QString::fromUtf8("tab4"));
        gridLayout = new QGridLayout(tab4);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        lDimTextHeight = new QLabel(tab4);
        lDimTextHeight->setObjectName(QString::fromUtf8("lDimTextHeight"));
        lDimTextHeight->setWordWrap(false);

        gridLayout->addWidget(lDimTextHeight, 0, 0, 1, 1);

        cbDimTextHeight = new QComboBox(tab4);
        cbDimTextHeight->setObjectName(QString::fromUtf8("cbDimTextHeight"));
        cbDimTextHeight->setEditable(true);

        gridLayout->addWidget(cbDimTextHeight, 0, 1, 1, 1);

        lDimUnit1 = new QLabel(tab4);
        lDimUnit1->setObjectName(QString::fromUtf8("lDimUnit1"));
        lDimUnit1->setWordWrap(false);

        gridLayout->addWidget(lDimUnit1, 0, 2, 1, 1);

        lDimExe = new QLabel(tab4);
        lDimExe->setObjectName(QString::fromUtf8("lDimExe"));
        lDimExe->setWordWrap(false);

        gridLayout->addWidget(lDimExe, 1, 0, 1, 1);

        cbDimExe = new QComboBox(tab4);
        cbDimExe->setObjectName(QString::fromUtf8("cbDimExe"));
        cbDimExe->setEditable(true);

        gridLayout->addWidget(cbDimExe, 1, 1, 1, 1);

        lDimUnit2 = new QLabel(tab4);
        lDimUnit2->setObjectName(QString::fromUtf8("lDimUnit2"));
        lDimUnit2->setWordWrap(false);

        gridLayout->addWidget(lDimUnit2, 1, 2, 1, 1);

        lDimExo = new QLabel(tab4);
        lDimExo->setObjectName(QString::fromUtf8("lDimExo"));
        lDimExo->setWordWrap(false);

        gridLayout->addWidget(lDimExo, 2, 0, 1, 1);

        cbDimExo = new QComboBox(tab4);
        cbDimExo->setObjectName(QString::fromUtf8("cbDimExo"));
        cbDimExo->setEditable(true);

        gridLayout->addWidget(cbDimExo, 2, 1, 1, 1);

        lDimUnit3 = new QLabel(tab4);
        lDimUnit3->setObjectName(QString::fromUtf8("lDimUnit3"));
        lDimUnit3->setWordWrap(false);

        gridLayout->addWidget(lDimUnit3, 2, 2, 1, 1);

        lDimGap = new QLabel(tab4);
        lDimGap->setObjectName(QString::fromUtf8("lDimGap"));
        lDimGap->setWordWrap(false);

        gridLayout->addWidget(lDimGap, 3, 0, 1, 1);

        cbDimGap = new QComboBox(tab4);
        cbDimGap->setObjectName(QString::fromUtf8("cbDimGap"));
        cbDimGap->setEditable(true);

        gridLayout->addWidget(cbDimGap, 3, 1, 1, 1);

        lDimUnit4 = new QLabel(tab4);
        lDimUnit4->setObjectName(QString::fromUtf8("lDimUnit4"));
        lDimUnit4->setWordWrap(false);

        gridLayout->addWidget(lDimUnit4, 3, 2, 1, 1);

        lDimAsz = new QLabel(tab4);
        lDimAsz->setObjectName(QString::fromUtf8("lDimAsz"));
        lDimAsz->setWordWrap(false);

        gridLayout->addWidget(lDimAsz, 4, 0, 1, 1);

        cbDimAsz = new QComboBox(tab4);
        cbDimAsz->setObjectName(QString::fromUtf8("cbDimAsz"));
        cbDimAsz->setEditable(true);

        gridLayout->addWidget(cbDimAsz, 4, 1, 1, 1);

        lDimUnit5 = new QLabel(tab4);
        lDimUnit5->setObjectName(QString::fromUtf8("lDimUnit5"));
        lDimUnit5->setWordWrap(false);

        gridLayout->addWidget(lDimUnit5, 4, 2, 1, 1);

        lDimTsz = new QLabel(tab4);
        lDimTsz->setObjectName(QString::fromUtf8("lDimTsz"));
        lDimTsz->setWordWrap(false);

        gridLayout->addWidget(lDimTsz, 5, 0, 1, 1);

        cbDimTsz = new QComboBox(tab4);
        cbDimTsz->setObjectName(QString::fromUtf8("cbDimTsz"));
        cbDimTsz->setEditable(true);

        gridLayout->addWidget(cbDimTsz, 5, 1, 1, 1);

        lDimUnit5_2 = new QLabel(tab4);
        lDimUnit5_2->setObjectName(QString::fromUtf8("lDimUnit5_2"));
        lDimUnit5_2->setWordWrap(false);

        gridLayout->addWidget(lDimUnit5_2, 5, 2, 1, 1);

        spacer11 = new QSpacerItem(20, 30, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(spacer11, 6, 0, 1, 1);

        spacer11_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(spacer11_2, 6, 1, 1, 1);

        spacer11_2_2 = new QSpacerItem(20, 30, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(spacer11_2_2, 6, 2, 1, 1);

        tabWidget->addTab(tab4, QString());
        tab5 = new QWidget();
        tab5->setObjectName(QString::fromUtf8("tab5"));
        vboxLayout5 = new QVBoxLayout(tab5);
        vboxLayout5->setSpacing(6);
        vboxLayout5->setContentsMargins(11, 11, 11, 11);
        vboxLayout5->setObjectName(QString::fromUtf8("vboxLayout5"));
        hboxLayout6 = new QHBoxLayout();
        hboxLayout6->setSpacing(6);
        hboxLayout6->setObjectName(QString::fromUtf8("hboxLayout6"));
        lSplineSegs = new QLabel(tab5);
        lSplineSegs->setObjectName(QString::fromUtf8("lSplineSegs"));
        lSplineSegs->setWordWrap(false);

        hboxLayout6->addWidget(lSplineSegs);

        cbSplineSegs = new QComboBox(tab5);
        cbSplineSegs->setObjectName(QString::fromUtf8("cbSplineSegs"));
        cbSplineSegs->setEditable(true);

        hboxLayout6->addWidget(cbSplineSegs);


        vboxLayout5->addLayout(hboxLayout6);

        spacer19_2 = new QSpacerItem(20, 111, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout5->addItem(spacer19_2);

        tabWidget->addTab(tab5, QString());

        gridLayout_2->addWidget(tabWidget, 0, 0, 1, 1);

        hboxLayout7 = new QHBoxLayout();
        hboxLayout7->setSpacing(6);
        hboxLayout7->setObjectName(QString::fromUtf8("hboxLayout7"));
        Horizontal_Spacing2 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout7->addItem(Horizontal_Spacing2);

        buttonOk = new QPushButton(QG_DlgOptionsDrawing);
        buttonOk->setObjectName(QString::fromUtf8("buttonOk"));
        buttonOk->setAutoDefault(true);
        buttonOk->setDefault(true);

        hboxLayout7->addWidget(buttonOk);

        buttonCancel = new QPushButton(QG_DlgOptionsDrawing);
        buttonCancel->setObjectName(QString::fromUtf8("buttonCancel"));
        buttonCancel->setAutoDefault(true);

        hboxLayout7->addWidget(buttonCancel);


        gridLayout_2->addLayout(hboxLayout7, 1, 0, 1, 1);

#ifndef QT_NO_SHORTCUT
        lPageHeight->setBuddy(lePaperHeight);
        lPageWidth->setBuddy(lePaperWidth);
        lUnit->setBuddy(cbUnit);
        lLengthFormat->setBuddy(cbLengthFormat);
        lLengthPrecision->setBuddy(cbLengthPrecision);
        lAngleFormat->setBuddy(cbAngleFormat);
        lAnglePrecision->setBuddy(cbAnglePrecision);
#endif // QT_NO_SHORTCUT
        QWidget::setTabOrder(tabWidget, cbPaperFormat);
        QWidget::setTabOrder(cbPaperFormat, rbLandscape);
        QWidget::setTabOrder(rbLandscape, rbPortrait);
        QWidget::setTabOrder(rbPortrait, lePaperWidth);
        QWidget::setTabOrder(lePaperWidth, lePaperHeight);
        QWidget::setTabOrder(lePaperHeight, buttonOk);
        QWidget::setTabOrder(buttonOk, buttonCancel);
        QWidget::setTabOrder(buttonCancel, cbUnit);
        QWidget::setTabOrder(cbUnit, cbLengthFormat);
        QWidget::setTabOrder(cbLengthFormat, cbLengthPrecision);
        QWidget::setTabOrder(cbLengthPrecision, cbAngleFormat);
        QWidget::setTabOrder(cbAngleFormat, cbAnglePrecision);
        QWidget::setTabOrder(cbAnglePrecision, cbXSpacing);
        QWidget::setTabOrder(cbXSpacing, cbYSpacing);
        QWidget::setTabOrder(cbYSpacing, cbDimTextHeight);
        QWidget::setTabOrder(cbDimTextHeight, cbDimExe);
        QWidget::setTabOrder(cbDimExe, cbDimExo);
        QWidget::setTabOrder(cbDimExo, cbDimGap);
        QWidget::setTabOrder(cbDimGap, cbDimAsz);

        retranslateUi(QG_DlgOptionsDrawing);
        QObject::connect(buttonOk, SIGNAL(clicked()), QG_DlgOptionsDrawing, SLOT(validate()));
        QObject::connect(buttonCancel, SIGNAL(clicked()), QG_DlgOptionsDrawing, SLOT(reject()));
        QObject::connect(cbLengthFormat, SIGNAL(activated(int)), QG_DlgOptionsDrawing, SLOT(updateLengthPrecision()));
        QObject::connect(cbAngleFormat, SIGNAL(activated(int)), QG_DlgOptionsDrawing, SLOT(updateAnglePrecision()));
        QObject::connect(cbUnit, SIGNAL(activated(int)), QG_DlgOptionsDrawing, SLOT(updatePreview()));
        QObject::connect(cbAngleFormat, SIGNAL(activated(int)), QG_DlgOptionsDrawing, SLOT(updatePreview()));
        QObject::connect(cbLengthFormat, SIGNAL(activated(QString)), QG_DlgOptionsDrawing, SLOT(updatePreview()));
        QObject::connect(cbAnglePrecision, SIGNAL(activated(int)), QG_DlgOptionsDrawing, SLOT(updatePreview()));
        QObject::connect(cbLengthPrecision, SIGNAL(activated(int)), QG_DlgOptionsDrawing, SLOT(updatePreview()));
        QObject::connect(cbPaperFormat, SIGNAL(activated(int)), QG_DlgOptionsDrawing, SLOT(updatePaperSize()));
        QObject::connect(cbUnit, SIGNAL(activated(int)), QG_DlgOptionsDrawing, SLOT(updateUnitLabels()));

        tabWidget->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(QG_DlgOptionsDrawing);
    } // setupUi

    void retranslateUi(QDialog *QG_DlgOptionsDrawing)
    {
        QG_DlgOptionsDrawing->setWindowTitle(QApplication::translate("QG_DlgOptionsDrawing", "Drawing Preferences", 0, QApplication::UnicodeUTF8));
        buttonGroup3->setTitle(QApplication::translate("QG_DlgOptionsDrawing", "Paper Format", 0, QApplication::UnicodeUTF8));
        rbLandscape->setText(QApplication::translate("QG_DlgOptionsDrawing", "&Landscape", 0, QApplication::UnicodeUTF8));
        rbPortrait->setText(QApplication::translate("QG_DlgOptionsDrawing", "P&ortrait", 0, QApplication::UnicodeUTF8));
        lPageHeight->setText(QApplication::translate("QG_DlgOptionsDrawing", "Paper &Height:", 0, QApplication::UnicodeUTF8));
        lPageWidth->setText(QApplication::translate("QG_DlgOptionsDrawing", "Paper &Width:", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab1), QApplication::translate("QG_DlgOptionsDrawing", "&Paper", 0, QApplication::UnicodeUTF8));
        bgUnit->setTitle(QApplication::translate("QG_DlgOptionsDrawing", "Main Unit", 0, QApplication::UnicodeUTF8));
        lUnit->setText(QApplication::translate("QG_DlgOptionsDrawing", "&Main drawing unit:", 0, QApplication::UnicodeUTF8));
        bgLength->setTitle(QApplication::translate("QG_DlgOptionsDrawing", "Length", 0, QApplication::UnicodeUTF8));
        lLengthFormat->setText(QApplication::translate("QG_DlgOptionsDrawing", "&Format:", 0, QApplication::UnicodeUTF8));
        lLengthPrecision->setText(QApplication::translate("QG_DlgOptionsDrawing", "P&recision:", 0, QApplication::UnicodeUTF8));
        bgLengthPreview->setTitle(QApplication::translate("QG_DlgOptionsDrawing", "Preview", 0, QApplication::UnicodeUTF8));
        lLinear->setText(QApplication::translate("QG_DlgOptionsDrawing", "linear", 0, QApplication::UnicodeUTF8));
        bgAngle->setTitle(QApplication::translate("QG_DlgOptionsDrawing", "Angle", 0, QApplication::UnicodeUTF8));
        lAngleFormat->setText(QApplication::translate("QG_DlgOptionsDrawing", "F&ormat:", 0, QApplication::UnicodeUTF8));
        lAnglePrecision->setText(QApplication::translate("QG_DlgOptionsDrawing", "Pre&cision:", 0, QApplication::UnicodeUTF8));
        bgAnglePreview->setTitle(QApplication::translate("QG_DlgOptionsDrawing", "Preview", 0, QApplication::UnicodeUTF8));
        lAngular->setText(QApplication::translate("QG_DlgOptionsDrawing", "angular", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab2), QApplication::translate("QG_DlgOptionsDrawing", "&Units", 0, QApplication::UnicodeUTF8));
        bgGrid->setTitle(QApplication::translate("QG_DlgOptionsDrawing", "Grid Settings", 0, QApplication::UnicodeUTF8));
        cbGridOn->setText(QApplication::translate("QG_DlgOptionsDrawing", "Show Grid", 0, QApplication::UnicodeUTF8));
        lCrosshair->setText(QApplication::translate("QG_DlgOptionsDrawing", "Crosshair", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        rbCrosshairLeft->setToolTip(QApplication::translate("QG_DlgOptionsDrawing", "Left crosshair with isometric grid", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        rbCrosshairLeft->setText(QApplication::translate("QG_DlgOptionsDrawing", "Left", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        rbCrosshairTop->setToolTip(QApplication::translate("QG_DlgOptionsDrawing", "Top crosshair with isometric grid", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        rbCrosshairTop->setText(QApplication::translate("QG_DlgOptionsDrawing", "Top", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        rbCrosshairRight->setToolTip(QApplication::translate("QG_DlgOptionsDrawing", "Right crosshair with isometric grid", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        rbCrosshairRight->setText(QApplication::translate("QG_DlgOptionsDrawing", "Right", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        rbOrthogonalGrid->setToolTip(QApplication::translate("QG_DlgOptionsDrawing", "Use orthogonal grid", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        rbOrthogonalGrid->setText(QApplication::translate("QG_DlgOptionsDrawing", "Orthogonal Grid", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        rbIsometricGrid->setToolTip(QApplication::translate("QG_DlgOptionsDrawing", "Use isometric grid", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        rbIsometricGrid->setText(QApplication::translate("QG_DlgOptionsDrawing", "Isometric Grid", 0, QApplication::UnicodeUTF8));
        lXSpacing->setText(QApplication::translate("QG_DlgOptionsDrawing", "X Spacing:", 0, QApplication::UnicodeUTF8));
        cbYSpacing->clear();
        cbYSpacing->insertItems(0, QStringList()
         << QApplication::translate("QG_DlgOptionsDrawing", "auto", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsDrawing", "0.01", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsDrawing", "0.1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsDrawing", "1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsDrawing", "10", 0, QApplication::UnicodeUTF8)
        );
        lYSpacing->setText(QApplication::translate("QG_DlgOptionsDrawing", "Y Spacing:", 0, QApplication::UnicodeUTF8));
        cbXSpacing->clear();
        cbXSpacing->insertItems(0, QStringList()
         << QApplication::translate("QG_DlgOptionsDrawing", "auto", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsDrawing", "0.01", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsDrawing", "0.1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsDrawing", "1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsDrawing", "10", 0, QApplication::UnicodeUTF8)
        );
        tabWidget->setTabText(tabWidget->indexOf(tab3), QApplication::translate("QG_DlgOptionsDrawing", "&Grid", 0, QApplication::UnicodeUTF8));
        lDimTextHeight->setText(QApplication::translate("QG_DlgOptionsDrawing", "Text Height:", 0, QApplication::UnicodeUTF8));
        cbDimTextHeight->clear();
        cbDimTextHeight->insertItems(0, QStringList()
         << QApplication::translate("QG_DlgOptionsDrawing", "1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsDrawing", "2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsDrawing", "5", 0, QApplication::UnicodeUTF8)
        );
        lDimUnit1->setText(QApplication::translate("QG_DlgOptionsDrawing", "units", 0, QApplication::UnicodeUTF8));
        lDimExe->setText(QApplication::translate("QG_DlgOptionsDrawing", "Extension line extension:", 0, QApplication::UnicodeUTF8));
        cbDimExe->clear();
        cbDimExe->insertItems(0, QStringList()
         << QApplication::translate("QG_DlgOptionsDrawing", "1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsDrawing", "2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsDrawing", "5", 0, QApplication::UnicodeUTF8)
        );
        lDimUnit2->setText(QApplication::translate("QG_DlgOptionsDrawing", "units", 0, QApplication::UnicodeUTF8));
        lDimExo->setText(QApplication::translate("QG_DlgOptionsDrawing", "Extension line offset:", 0, QApplication::UnicodeUTF8));
        cbDimExo->clear();
        cbDimExo->insertItems(0, QStringList()
         << QApplication::translate("QG_DlgOptionsDrawing", "1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsDrawing", "2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsDrawing", "5", 0, QApplication::UnicodeUTF8)
        );
        lDimUnit3->setText(QApplication::translate("QG_DlgOptionsDrawing", "units", 0, QApplication::UnicodeUTF8));
        lDimGap->setText(QApplication::translate("QG_DlgOptionsDrawing", "Dimension line gap:", 0, QApplication::UnicodeUTF8));
        cbDimGap->clear();
        cbDimGap->insertItems(0, QStringList()
         << QApplication::translate("QG_DlgOptionsDrawing", "1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsDrawing", "2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsDrawing", "5", 0, QApplication::UnicodeUTF8)
        );
        lDimUnit4->setText(QApplication::translate("QG_DlgOptionsDrawing", "units", 0, QApplication::UnicodeUTF8));
        lDimAsz->setText(QApplication::translate("QG_DlgOptionsDrawing", "Arrow size:", 0, QApplication::UnicodeUTF8));
        cbDimAsz->clear();
        cbDimAsz->insertItems(0, QStringList()
         << QApplication::translate("QG_DlgOptionsDrawing", "1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsDrawing", "2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsDrawing", "5", 0, QApplication::UnicodeUTF8)
        );
        lDimUnit5->setText(QApplication::translate("QG_DlgOptionsDrawing", "units", 0, QApplication::UnicodeUTF8));
        lDimTsz->setText(QApplication::translate("QG_DlgOptionsDrawing", "Tick size:", 0, QApplication::UnicodeUTF8));
        cbDimTsz->clear();
        cbDimTsz->insertItems(0, QStringList()
         << QApplication::translate("QG_DlgOptionsDrawing", "0", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsDrawing", "1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsDrawing", "2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsDrawing", "5", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_TOOLTIP
        cbDimTsz->setToolTip(QApplication::translate("QG_DlgOptionsDrawing", "<html><head/><body><p>Dimension Tick size. Set it to 0 will disable the dimension tick. Dimension arrow won't be drawn when dimension tick is enabled.</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        lDimUnit5_2->setText(QApplication::translate("QG_DlgOptionsDrawing", "units", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab4), QApplication::translate("QG_DlgOptionsDrawing", "&Dimensions", 0, QApplication::UnicodeUTF8));
        lSplineSegs->setText(QApplication::translate("QG_DlgOptionsDrawing", "Number of line segments per spline patch:", 0, QApplication::UnicodeUTF8));
        cbSplineSegs->clear();
        cbSplineSegs->insertItems(0, QStringList()
         << QApplication::translate("QG_DlgOptionsDrawing", "2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsDrawing", "4", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsDrawing", "8", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsDrawing", "16", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsDrawing", "32", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsDrawing", "64", 0, QApplication::UnicodeUTF8)
        );
        tabWidget->setTabText(tabWidget->indexOf(tab5), QApplication::translate("QG_DlgOptionsDrawing", "Splines", 0, QApplication::UnicodeUTF8));
        buttonOk->setText(QApplication::translate("QG_DlgOptionsDrawing", "&OK", 0, QApplication::UnicodeUTF8));
        buttonOk->setShortcut(QApplication::translate("QG_DlgOptionsDrawing", "Alt+O", 0, QApplication::UnicodeUTF8));
        buttonCancel->setText(QApplication::translate("QG_DlgOptionsDrawing", "Cancel", 0, QApplication::UnicodeUTF8));
        buttonCancel->setShortcut(QApplication::translate("QG_DlgOptionsDrawing", "Esc", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QG_DlgOptionsDrawing: public Ui_QG_DlgOptionsDrawing {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QG_DLGOPTIONSDRAWING_H
