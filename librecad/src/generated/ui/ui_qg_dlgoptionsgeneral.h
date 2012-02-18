/********************************************************************************
** Form generated from reading UI file 'qg_dlgoptionsgeneral.ui'
**
** Created: Sat Feb 18 16:41:50 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QG_DLGOPTIONSGENERAL_H
#define UI_QG_DLGOPTIONSGENERAL_H

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
#include <QtGui/QSpinBox>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QG_DlgOptionsGeneral
{
public:
    QVBoxLayout *vboxLayout;
    QTabWidget *tabWidget;
    QWidget *tab_1;
    QGridLayout *gridLayout;
    QGroupBox *bgLanguage;
    QGridLayout *gridLayout1;
    QLabel *lLanguage;
    QLabel *lLanguageCmd;
    QComboBox *cbLanguageCmd;
    QComboBox *cbLanguage;
    QGroupBox *bgColors;
    QGridLayout *gridLayout2;
    QLabel *lBackground;
    QLabel *lGridColor;
    QComboBox *cbBackgroundColor;
    QComboBox *cbGridColor;
    QLabel *lMetaGridColor;
    QComboBox *cbMetaGridColor;
    QLabel *lSelectedColor;
    QComboBox *cbSelectedColor;
    QComboBox *cbHighlightedColor;
    QLabel *lHighlightedColor;
    QSpacerItem *spacer7;
    QSpacerItem *spacer8;
    QGroupBox *buttonGroup5_2;
    QGridLayout *gridLayout3;
    QLabel *lSizeStatus;
    QComboBox *cbSizeStatus;
    QSpacerItem *spacer11_3;
    QSpacerItem *spacer12;
    QGroupBox *bgGraphicView;
    QGridLayout *gridLayout4;
    QCheckBox *cbShowCrosshairs;
    QCheckBox *cbScaleGrid;
    QSpacerItem *spacer29;
    QSpacerItem *spacer29_2;
    QLabel *lMaxPreview;
    QComboBox *cbMaxPreview;
    QLabel *lMinGridSpacing;
    QComboBox *cbMinGridSpacing;
    QSpacerItem *spacer9;
    QSpacerItem *spacer4;
    QWidget *tab_2;
    QGridLayout *gridLayout5;
    QLabel *textLabel2;
    QLabel *textLabel3;
    QLabel *textLabel4;
    QLabel *textLabel5;
    QLabel *textLabel6;
    QLineEdit *lePathTranslations;
    QLineEdit *lePathHatch;
    QLineEdit *lePathFonts;
    QLineEdit *lePathScripts;
    QLineEdit *lePathLibrary;
    QSpacerItem *spacer5;
    QSpacerItem *spacer5_2;
    QWidget *tab_3;
    QGridLayout *gridLayout6;
    QGroupBox *buttonGroup5;
    QHBoxLayout *hboxLayout;
    QLabel *lUnit;
    QComboBox *cbUnit;
    QSpacerItem *spacer11_2;
    QSpacerItem *verticalSpacer;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpinBox *cbAutoSaveTime;
    QCheckBox *cbAutoBackup;
    QHBoxLayout *hboxLayout1;
    QSpacerItem *Horizontal_Spacing2;
    QPushButton *buttonOk;
    QPushButton *buttonCancel;

    void setupUi(QDialog *QG_DlgOptionsGeneral)
    {
        if (QG_DlgOptionsGeneral->objectName().isEmpty())
            QG_DlgOptionsGeneral->setObjectName(QString::fromUtf8("QG_DlgOptionsGeneral"));
        QG_DlgOptionsGeneral->resize(645, 404);
        QG_DlgOptionsGeneral->setSizeGripEnabled(true);
        vboxLayout = new QVBoxLayout(QG_DlgOptionsGeneral);
        vboxLayout->setSpacing(6);
        vboxLayout->setContentsMargins(11, 11, 11, 11);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        tabWidget = new QTabWidget(QG_DlgOptionsGeneral);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab_1 = new QWidget();
        tab_1->setObjectName(QString::fromUtf8("tab_1"));
        gridLayout = new QGridLayout(tab_1);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        bgLanguage = new QGroupBox(tab_1);
        bgLanguage->setObjectName(QString::fromUtf8("bgLanguage"));
        gridLayout1 = new QGridLayout(bgLanguage);
        gridLayout1->setSpacing(6);
        gridLayout1->setContentsMargins(11, 11, 11, 11);
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        lLanguage = new QLabel(bgLanguage);
        lLanguage->setObjectName(QString::fromUtf8("lLanguage"));
        lLanguage->setWordWrap(false);

        gridLayout1->addWidget(lLanguage, 0, 0, 1, 1);

        lLanguageCmd = new QLabel(bgLanguage);
        lLanguageCmd->setObjectName(QString::fromUtf8("lLanguageCmd"));
        lLanguageCmd->setWordWrap(false);

        gridLayout1->addWidget(lLanguageCmd, 1, 0, 1, 1);

        cbLanguageCmd = new QComboBox(bgLanguage);
        cbLanguageCmd->setObjectName(QString::fromUtf8("cbLanguageCmd"));

        gridLayout1->addWidget(cbLanguageCmd, 1, 1, 1, 1);

        cbLanguage = new QComboBox(bgLanguage);
        cbLanguage->setObjectName(QString::fromUtf8("cbLanguage"));

        gridLayout1->addWidget(cbLanguage, 0, 1, 1, 1);


        gridLayout->addWidget(bgLanguage, 0, 0, 1, 1);

        bgColors = new QGroupBox(tab_1);
        bgColors->setObjectName(QString::fromUtf8("bgColors"));
        gridLayout2 = new QGridLayout(bgColors);
        gridLayout2->setSpacing(6);
        gridLayout2->setContentsMargins(11, 11, 11, 11);
        gridLayout2->setObjectName(QString::fromUtf8("gridLayout2"));
        lBackground = new QLabel(bgColors);
        lBackground->setObjectName(QString::fromUtf8("lBackground"));
        lBackground->setWordWrap(false);

        gridLayout2->addWidget(lBackground, 0, 0, 1, 1);

        lGridColor = new QLabel(bgColors);
        lGridColor->setObjectName(QString::fromUtf8("lGridColor"));
        lGridColor->setWordWrap(false);

        gridLayout2->addWidget(lGridColor, 1, 0, 1, 1);

        cbBackgroundColor = new QComboBox(bgColors);
        cbBackgroundColor->setObjectName(QString::fromUtf8("cbBackgroundColor"));
        cbBackgroundColor->setEditable(true);

        gridLayout2->addWidget(cbBackgroundColor, 0, 1, 1, 1);

        cbGridColor = new QComboBox(bgColors);
        cbGridColor->setObjectName(QString::fromUtf8("cbGridColor"));
        cbGridColor->setEditable(true);

        gridLayout2->addWidget(cbGridColor, 1, 1, 1, 1);

        lMetaGridColor = new QLabel(bgColors);
        lMetaGridColor->setObjectName(QString::fromUtf8("lMetaGridColor"));
        lMetaGridColor->setWordWrap(false);

        gridLayout2->addWidget(lMetaGridColor, 2, 0, 1, 1);

        cbMetaGridColor = new QComboBox(bgColors);
        cbMetaGridColor->setObjectName(QString::fromUtf8("cbMetaGridColor"));
        cbMetaGridColor->setEditable(true);

        gridLayout2->addWidget(cbMetaGridColor, 2, 1, 1, 1);

        lSelectedColor = new QLabel(bgColors);
        lSelectedColor->setObjectName(QString::fromUtf8("lSelectedColor"));
        lSelectedColor->setWordWrap(false);

        gridLayout2->addWidget(lSelectedColor, 3, 0, 1, 1);

        cbSelectedColor = new QComboBox(bgColors);
        cbSelectedColor->setObjectName(QString::fromUtf8("cbSelectedColor"));
        cbSelectedColor->setEditable(true);

        gridLayout2->addWidget(cbSelectedColor, 3, 1, 1, 1);

        cbHighlightedColor = new QComboBox(bgColors);
        cbHighlightedColor->setObjectName(QString::fromUtf8("cbHighlightedColor"));
        cbHighlightedColor->setEditable(true);

        gridLayout2->addWidget(cbHighlightedColor, 4, 1, 1, 1);

        lHighlightedColor = new QLabel(bgColors);
        lHighlightedColor->setObjectName(QString::fromUtf8("lHighlightedColor"));
        lHighlightedColor->setWordWrap(false);

        gridLayout2->addWidget(lHighlightedColor, 4, 0, 1, 1);

        spacer7 = new QSpacerItem(21, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout2->addItem(spacer7, 5, 0, 1, 1);

        spacer8 = new QSpacerItem(21, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout2->addItem(spacer8, 5, 1, 1, 1);


        gridLayout->addWidget(bgColors, 0, 1, 2, 1);

        buttonGroup5_2 = new QGroupBox(tab_1);
        buttonGroup5_2->setObjectName(QString::fromUtf8("buttonGroup5_2"));
        gridLayout3 = new QGridLayout(buttonGroup5_2);
        gridLayout3->setSpacing(6);
        gridLayout3->setContentsMargins(11, 11, 11, 11);
        gridLayout3->setObjectName(QString::fromUtf8("gridLayout3"));
        lSizeStatus = new QLabel(buttonGroup5_2);
        lSizeStatus->setObjectName(QString::fromUtf8("lSizeStatus"));
        lSizeStatus->setWordWrap(false);

        gridLayout3->addWidget(lSizeStatus, 0, 0, 1, 1);

        cbSizeStatus = new QComboBox(buttonGroup5_2);
        cbSizeStatus->setObjectName(QString::fromUtf8("cbSizeStatus"));
        cbSizeStatus->setEditable(true);

        gridLayout3->addWidget(cbSizeStatus, 0, 1, 1, 1);

        spacer11_3 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout3->addItem(spacer11_3, 1, 0, 1, 1);

        spacer12 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout3->addItem(spacer12, 1, 1, 1, 1);


        gridLayout->addWidget(buttonGroup5_2, 2, 1, 1, 1);

        bgGraphicView = new QGroupBox(tab_1);
        bgGraphicView->setObjectName(QString::fromUtf8("bgGraphicView"));
        gridLayout4 = new QGridLayout(bgGraphicView);
        gridLayout4->setSpacing(6);
        gridLayout4->setContentsMargins(11, 11, 11, 11);
        gridLayout4->setObjectName(QString::fromUtf8("gridLayout4"));
        cbShowCrosshairs = new QCheckBox(bgGraphicView);
        cbShowCrosshairs->setObjectName(QString::fromUtf8("cbShowCrosshairs"));

        gridLayout4->addWidget(cbShowCrosshairs, 0, 0, 1, 2);

        cbScaleGrid = new QCheckBox(bgGraphicView);
        cbScaleGrid->setObjectName(QString::fromUtf8("cbScaleGrid"));

        gridLayout4->addWidget(cbScaleGrid, 1, 0, 1, 2);

        spacer29 = new QSpacerItem(21, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout4->addItem(spacer29, 4, 0, 1, 1);

        spacer29_2 = new QSpacerItem(21, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout4->addItem(spacer29_2, 4, 1, 1, 1);

        lMaxPreview = new QLabel(bgGraphicView);
        lMaxPreview->setObjectName(QString::fromUtf8("lMaxPreview"));
        lMaxPreview->setWordWrap(false);

        gridLayout4->addWidget(lMaxPreview, 3, 0, 1, 1);

        cbMaxPreview = new QComboBox(bgGraphicView);
        cbMaxPreview->setObjectName(QString::fromUtf8("cbMaxPreview"));
        cbMaxPreview->setEditable(true);

        gridLayout4->addWidget(cbMaxPreview, 3, 1, 1, 1);

        lMinGridSpacing = new QLabel(bgGraphicView);
        lMinGridSpacing->setObjectName(QString::fromUtf8("lMinGridSpacing"));
        lMinGridSpacing->setWordWrap(false);

        gridLayout4->addWidget(lMinGridSpacing, 2, 0, 1, 1);

        cbMinGridSpacing = new QComboBox(bgGraphicView);
        cbMinGridSpacing->setObjectName(QString::fromUtf8("cbMinGridSpacing"));
        cbMinGridSpacing->setEditable(true);

        gridLayout4->addWidget(cbMinGridSpacing, 2, 1, 1, 1);


        gridLayout->addWidget(bgGraphicView, 1, 0, 2, 1);

        spacer9 = new QSpacerItem(31, 16, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(spacer9, 3, 1, 1, 1);

        spacer4 = new QSpacerItem(31, 16, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(spacer4, 3, 0, 1, 1);

        tabWidget->addTab(tab_1, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        gridLayout5 = new QGridLayout(tab_2);
        gridLayout5->setSpacing(6);
        gridLayout5->setContentsMargins(11, 11, 11, 11);
        gridLayout5->setObjectName(QString::fromUtf8("gridLayout5"));
        textLabel2 = new QLabel(tab_2);
        textLabel2->setObjectName(QString::fromUtf8("textLabel2"));
        textLabel2->setWordWrap(false);

        gridLayout5->addWidget(textLabel2, 0, 0, 1, 1);

        textLabel3 = new QLabel(tab_2);
        textLabel3->setObjectName(QString::fromUtf8("textLabel3"));
        textLabel3->setWordWrap(false);

        gridLayout5->addWidget(textLabel3, 1, 0, 1, 1);

        textLabel4 = new QLabel(tab_2);
        textLabel4->setObjectName(QString::fromUtf8("textLabel4"));
        textLabel4->setWordWrap(false);

        gridLayout5->addWidget(textLabel4, 2, 0, 1, 1);

        textLabel5 = new QLabel(tab_2);
        textLabel5->setObjectName(QString::fromUtf8("textLabel5"));
        textLabel5->setWordWrap(false);

        gridLayout5->addWidget(textLabel5, 3, 0, 1, 1);

        textLabel6 = new QLabel(tab_2);
        textLabel6->setObjectName(QString::fromUtf8("textLabel6"));
        textLabel6->setWordWrap(false);

        gridLayout5->addWidget(textLabel6, 4, 0, 1, 1);

        lePathTranslations = new QLineEdit(tab_2);
        lePathTranslations->setObjectName(QString::fromUtf8("lePathTranslations"));

        gridLayout5->addWidget(lePathTranslations, 0, 1, 1, 1);

        lePathHatch = new QLineEdit(tab_2);
        lePathHatch->setObjectName(QString::fromUtf8("lePathHatch"));

        gridLayout5->addWidget(lePathHatch, 1, 1, 1, 1);

        lePathFonts = new QLineEdit(tab_2);
        lePathFonts->setObjectName(QString::fromUtf8("lePathFonts"));

        gridLayout5->addWidget(lePathFonts, 2, 1, 1, 1);

        lePathScripts = new QLineEdit(tab_2);
        lePathScripts->setObjectName(QString::fromUtf8("lePathScripts"));

        gridLayout5->addWidget(lePathScripts, 3, 1, 1, 1);

        lePathLibrary = new QLineEdit(tab_2);
        lePathLibrary->setObjectName(QString::fromUtf8("lePathLibrary"));

        gridLayout5->addWidget(lePathLibrary, 4, 1, 1, 1);

        spacer5 = new QSpacerItem(31, 71, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout5->addItem(spacer5, 5, 0, 1, 1);

        spacer5_2 = new QSpacerItem(31, 71, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout5->addItem(spacer5_2, 5, 1, 1, 1);

        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        gridLayout6 = new QGridLayout(tab_3);
        gridLayout6->setSpacing(6);
        gridLayout6->setContentsMargins(11, 11, 11, 11);
        gridLayout6->setObjectName(QString::fromUtf8("gridLayout6"));
        buttonGroup5 = new QGroupBox(tab_3);
        buttonGroup5->setObjectName(QString::fromUtf8("buttonGroup5"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(buttonGroup5->sizePolicy().hasHeightForWidth());
        buttonGroup5->setSizePolicy(sizePolicy);
        buttonGroup5->setMinimumSize(QSize(0, 70));
        hboxLayout = new QHBoxLayout(buttonGroup5);
        hboxLayout->setSpacing(6);
        hboxLayout->setContentsMargins(11, 11, 11, 11);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        lUnit = new QLabel(buttonGroup5);
        lUnit->setObjectName(QString::fromUtf8("lUnit"));
        sizePolicy.setHeightForWidth(lUnit->sizePolicy().hasHeightForWidth());
        lUnit->setSizePolicy(sizePolicy);
        lUnit->setWordWrap(false);

        hboxLayout->addWidget(lUnit);

        cbUnit = new QComboBox(buttonGroup5);
        cbUnit->setObjectName(QString::fromUtf8("cbUnit"));

        hboxLayout->addWidget(cbUnit);


        gridLayout6->addWidget(buttonGroup5, 0, 0, 1, 1);

        spacer11_2 = new QSpacerItem(211, 21, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout6->addItem(spacer11_2, 0, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout6->addItem(verticalSpacer, 3, 0, 1, 1);

        groupBox = new QGroupBox(tab_3);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        groupBox->setMinimumSize(QSize(0, 70));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        cbAutoSaveTime = new QSpinBox(groupBox);
        cbAutoSaveTime->setObjectName(QString::fromUtf8("cbAutoSaveTime"));
        cbAutoSaveTime->setMinimum(1);
        cbAutoSaveTime->setMaximum(60);

        horizontalLayout->addWidget(cbAutoSaveTime);


        verticalLayout->addLayout(horizontalLayout);

        cbAutoBackup = new QCheckBox(groupBox);
        cbAutoBackup->setObjectName(QString::fromUtf8("cbAutoBackup"));

        verticalLayout->addWidget(cbAutoBackup);


        gridLayout6->addWidget(groupBox, 1, 0, 1, 1);

        tabWidget->addTab(tab_3, QString());

        vboxLayout->addWidget(tabWidget);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(6);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        Horizontal_Spacing2 = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout1->addItem(Horizontal_Spacing2);

        buttonOk = new QPushButton(QG_DlgOptionsGeneral);
        buttonOk->setObjectName(QString::fromUtf8("buttonOk"));
        buttonOk->setAutoDefault(true);
        buttonOk->setDefault(true);

        hboxLayout1->addWidget(buttonOk);

        buttonCancel = new QPushButton(QG_DlgOptionsGeneral);
        buttonCancel->setObjectName(QString::fromUtf8("buttonCancel"));
        buttonCancel->setAutoDefault(true);

        hboxLayout1->addWidget(buttonCancel);


        vboxLayout->addLayout(hboxLayout1);

#ifndef QT_NO_SHORTCUT
        lLanguage->setBuddy(cbLanguage);
        lLanguageCmd->setBuddy(cbLanguageCmd);
        lBackground->setBuddy(cbBackgroundColor);
        lGridColor->setBuddy(cbGridColor);
        lMetaGridColor->setBuddy(cbMetaGridColor);
        lSelectedColor->setBuddy(cbMetaGridColor);
        lHighlightedColor->setBuddy(cbMetaGridColor);
        lMaxPreview->setBuddy(cbMaxPreview);
        lMinGridSpacing->setBuddy(cbMaxPreview);
        lUnit->setBuddy(cbUnit);
#endif // QT_NO_SHORTCUT
        QWidget::setTabOrder(tabWidget, cbLanguage);
        QWidget::setTabOrder(cbLanguage, cbLanguageCmd);
        QWidget::setTabOrder(cbLanguageCmd, cbShowCrosshairs);
        QWidget::setTabOrder(cbShowCrosshairs, cbScaleGrid);
        QWidget::setTabOrder(cbScaleGrid, cbMinGridSpacing);
        QWidget::setTabOrder(cbMinGridSpacing, cbMaxPreview);
        QWidget::setTabOrder(cbMaxPreview, cbBackgroundColor);
        QWidget::setTabOrder(cbBackgroundColor, cbGridColor);
        QWidget::setTabOrder(cbGridColor, cbMetaGridColor);
        QWidget::setTabOrder(cbMetaGridColor, cbSelectedColor);
        QWidget::setTabOrder(cbSelectedColor, cbHighlightedColor);
        QWidget::setTabOrder(cbHighlightedColor, cbSizeStatus);
        QWidget::setTabOrder(cbSizeStatus, buttonOk);
        QWidget::setTabOrder(buttonOk, buttonCancel);
        QWidget::setTabOrder(buttonCancel, lePathTranslations);
        QWidget::setTabOrder(lePathTranslations, lePathHatch);
        QWidget::setTabOrder(lePathHatch, lePathFonts);
        QWidget::setTabOrder(lePathFonts, lePathScripts);
        QWidget::setTabOrder(lePathScripts, lePathLibrary);
        QWidget::setTabOrder(lePathLibrary, cbUnit);

        retranslateUi(QG_DlgOptionsGeneral);
        QObject::connect(buttonOk, SIGNAL(clicked()), QG_DlgOptionsGeneral, SLOT(ok()));
        QObject::connect(buttonCancel, SIGNAL(clicked()), QG_DlgOptionsGeneral, SLOT(reject()));
        QObject::connect(cbSizeStatus, SIGNAL(activated(int)), QG_DlgOptionsGeneral, SLOT(setRestartNeeded()));
        QObject::connect(cbLanguageCmd, SIGNAL(activated(int)), QG_DlgOptionsGeneral, SLOT(setRestartNeeded()));
        QObject::connect(cbLanguage, SIGNAL(activated(int)), QG_DlgOptionsGeneral, SLOT(setRestartNeeded()));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(QG_DlgOptionsGeneral);
    } // setupUi

    void retranslateUi(QDialog *QG_DlgOptionsGeneral)
    {
        QG_DlgOptionsGeneral->setWindowTitle(QApplication::translate("QG_DlgOptionsGeneral", "Application Preferences", 0, QApplication::UnicodeUTF8));
        bgLanguage->setTitle(QApplication::translate("QG_DlgOptionsGeneral", "Language", 0, QApplication::UnicodeUTF8));
        lLanguage->setText(QApplication::translate("QG_DlgOptionsGeneral", "&GUI Language:", 0, QApplication::UnicodeUTF8));
        lLanguageCmd->setText(QApplication::translate("QG_DlgOptionsGeneral", "&Command Language:", 0, QApplication::UnicodeUTF8));
        bgColors->setTitle(QApplication::translate("QG_DlgOptionsGeneral", "Colors", 0, QApplication::UnicodeUTF8));
        lBackground->setText(QApplication::translate("QG_DlgOptionsGeneral", "Backgr&ound:", 0, QApplication::UnicodeUTF8));
        lGridColor->setText(QApplication::translate("QG_DlgOptionsGeneral", "G&rid Color:", 0, QApplication::UnicodeUTF8));
        cbBackgroundColor->clear();
        cbBackgroundColor->insertItems(0, QStringList()
         << QApplication::translate("QG_DlgOptionsGeneral", "#000000", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsGeneral", "#FFFFFF", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsGeneral", "#C0C0C0", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsGeneral", "#808080", 0, QApplication::UnicodeUTF8)
        );
        cbGridColor->clear();
        cbGridColor->insertItems(0, QStringList()
         << QApplication::translate("QG_DlgOptionsGeneral", "#C0C0C0", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsGeneral", "#FFFFFF", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsGeneral", "#000000", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsGeneral", "#808080", 0, QApplication::UnicodeUTF8)
        );
        lMetaGridColor->setText(QApplication::translate("QG_DlgOptionsGeneral", "&Meta Grid Color:", 0, QApplication::UnicodeUTF8));
        cbMetaGridColor->clear();
        cbMetaGridColor->insertItems(0, QStringList()
         << QApplication::translate("QG_DlgOptionsGeneral", "#404040", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsGeneral", "#000000", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsGeneral", "#FFFFFF", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsGeneral", "#C0C0C0", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsGeneral", "#808080", 0, QApplication::UnicodeUTF8)
        );
        lSelectedColor->setText(QApplication::translate("QG_DlgOptionsGeneral", "S&elected Color:", 0, QApplication::UnicodeUTF8));
        cbSelectedColor->clear();
        cbSelectedColor->insertItems(0, QStringList()
         << QApplication::translate("QG_DlgOptionsGeneral", "#A54747", 0, QApplication::UnicodeUTF8)
        );
        cbHighlightedColor->clear();
        cbHighlightedColor->insertItems(0, QStringList()
         << QApplication::translate("QG_DlgOptionsGeneral", "#739373", 0, QApplication::UnicodeUTF8)
        );
        lHighlightedColor->setText(QApplication::translate("QG_DlgOptionsGeneral", "&Highlighted Color:", 0, QApplication::UnicodeUTF8));
        buttonGroup5_2->setTitle(QApplication::translate("QG_DlgOptionsGeneral", "Fontsize", 0, QApplication::UnicodeUTF8));
        lSizeStatus->setText(QApplication::translate("QG_DlgOptionsGeneral", "Statusbar:", 0, QApplication::UnicodeUTF8));
        cbSizeStatus->clear();
        cbSizeStatus->insertItems(0, QStringList()
         << QApplication::translate("QG_DlgOptionsGeneral", "5", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsGeneral", "6", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsGeneral", "7", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsGeneral", "8", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsGeneral", "9", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsGeneral", "10", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsGeneral", "11", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsGeneral", "12", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsGeneral", "14", 0, QApplication::UnicodeUTF8)
        );
        bgGraphicView->setTitle(QApplication::translate("QG_DlgOptionsGeneral", "Graphic View", 0, QApplication::UnicodeUTF8));
        cbShowCrosshairs->setText(QApplication::translate("QG_DlgOptionsGeneral", "&Show large crosshairs", 0, QApplication::UnicodeUTF8));
        cbShowCrosshairs->setShortcut(QApplication::translate("QG_DlgOptionsGeneral", "Alt+S", 0, QApplication::UnicodeUTF8));
        cbScaleGrid->setText(QApplication::translate("QG_DlgOptionsGeneral", "A&utomatically scale grid", 0, QApplication::UnicodeUTF8));
        cbScaleGrid->setShortcut(QApplication::translate("QG_DlgOptionsGeneral", "Alt+U", 0, QApplication::UnicodeUTF8));
        lMaxPreview->setText(QApplication::translate("QG_DlgOptionsGeneral", "Number of p&review entities:", 0, QApplication::UnicodeUTF8));
        cbMaxPreview->clear();
        cbMaxPreview->insertItems(0, QStringList()
         << QApplication::translate("QG_DlgOptionsGeneral", "0", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsGeneral", "50", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsGeneral", "100", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsGeneral", "200", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsGeneral", "400", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsGeneral", "800", 0, QApplication::UnicodeUTF8)
        );
        lMinGridSpacing->setText(QApplication::translate("QG_DlgOptionsGeneral", "Minimal Grid Spacing (px):", 0, QApplication::UnicodeUTF8));
        cbMinGridSpacing->clear();
        cbMinGridSpacing->insertItems(0, QStringList()
         << QApplication::translate("QG_DlgOptionsGeneral", "0", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsGeneral", "4", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsGeneral", "5", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsGeneral", "6", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsGeneral", "8", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsGeneral", "10", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsGeneral", "15", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("QG_DlgOptionsGeneral", "20", 0, QApplication::UnicodeUTF8)
        );
        tabWidget->setTabText(tabWidget->indexOf(tab_1), QApplication::translate("QG_DlgOptionsGeneral", "&Appearance", 0, QApplication::UnicodeUTF8));
        textLabel2->setText(QApplication::translate("QG_DlgOptionsGeneral", "Translations:", 0, QApplication::UnicodeUTF8));
        textLabel3->setText(QApplication::translate("QG_DlgOptionsGeneral", "Hatch Patterns:", 0, QApplication::UnicodeUTF8));
        textLabel4->setText(QApplication::translate("QG_DlgOptionsGeneral", "Fonts:", 0, QApplication::UnicodeUTF8));
        textLabel5->setText(QApplication::translate("QG_DlgOptionsGeneral", "Scripts:", 0, QApplication::UnicodeUTF8));
        textLabel6->setText(QApplication::translate("QG_DlgOptionsGeneral", "Part Libraries:", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("QG_DlgOptionsGeneral", "&Paths", 0, QApplication::UnicodeUTF8));
        buttonGroup5->setTitle(QApplication::translate("QG_DlgOptionsGeneral", "Defaults for new drawings", 0, QApplication::UnicodeUTF8));
        lUnit->setText(QApplication::translate("QG_DlgOptionsGeneral", "&Unit:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        cbUnit->setToolTip(QApplication::translate("QG_DlgOptionsGeneral", "Drawing unit.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        groupBox->setTitle(QApplication::translate("QG_DlgOptionsGeneral", "Program Defaults", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("QG_DlgOptionsGeneral", "Auto save time:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        cbAutoSaveTime->setToolTip(QApplication::translate("QG_DlgOptionsGeneral", "Time between automatic saving of the document in minutes.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        cbAutoBackup->setToolTip(QApplication::translate("QG_DlgOptionsGeneral", "When set, LibreCAD will automatically generate a backup of your current drawing.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        cbAutoBackup->setText(QApplication::translate("QG_DlgOptionsGeneral", "Auto backup", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("QG_DlgOptionsGeneral", "&Defaults", 0, QApplication::UnicodeUTF8));
        buttonOk->setText(QApplication::translate("QG_DlgOptionsGeneral", "&OK", 0, QApplication::UnicodeUTF8));
        buttonOk->setShortcut(QApplication::translate("QG_DlgOptionsGeneral", "Alt+O", 0, QApplication::UnicodeUTF8));
        buttonCancel->setText(QApplication::translate("QG_DlgOptionsGeneral", "Cancel", 0, QApplication::UnicodeUTF8));
        buttonCancel->setShortcut(QApplication::translate("QG_DlgOptionsGeneral", "Esc", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class QG_DlgOptionsGeneral: public Ui_QG_DlgOptionsGeneral {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QG_DLGOPTIONSGENERAL_H
