#ifndef IMPORTSHP_H
#define IMPORTSHP_H

#include <QLabel>
#include <QGroupBox>
#include <QLineEdit>
#include <QComboBox>
#include <QDialog>
#include <QRadioButton>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QFileInfo>
#include <QSettings>
#include <QMessageBox>

#include "qc_plugininterface.h"
#include "document_interface.h"
#include "shapefil.h"

/**
 * @brief Plugin entry point — registered via Q_PLUGIN_METADATA.
 */
class ImportShp : public QObject, QC_PluginInterface {
    Q_OBJECT
    Q_INTERFACES(QC_PluginInterface)
    Q_PLUGIN_METADATA(IID LC_DocumentInterface_iid FILE "importshp.json")

public:
    [[nodiscard]] PluginCapabilities getCapabilities() const override;
    [[nodiscard]] QString name() const override;
    void execComm(Document_Interface* doc, QWidget* parent, QString cmd) override;
};

/**
 * @brief Attribute data extracted from DBF fields.
 */
class AttribData {
public:
    AttribData() noexcept
        : layer{"0"}, lineType{"BYLAYER"}, width{"BYLAYER"}, color{-1} {}

    QString layer;
    QString lineType;
    QString width;
    int color;
};

/**
 * @brief Shapefile import dialog.
 */
class dibSHP : public QDialog {
    Q_OBJECT

public:
    explicit dibSHP(QWidget* parent = nullptr);
    ~dibSHP() override = default;
    void procesFile(Document_Interface* doc);

public slots:
    void getFile();
    void checkAccept();
    void updateFile();

private:
    void readSettings();
    void writeSettings() const;

    void readPoint(DBFHandle dh, int recordIndex);
    void readPolyline(DBFHandle dh, int recordIndex);
    void readPolylineC(DBFHandle dh, int recordIndex);
    void readMultiPolyline(DBFHandle dh, int recordIndex);
    void readAttributes(DBFHandle dh, int recordIndex);

    // Allow ImportShp::execComm to set currDoc
    friend class ImportShp;

private:
    // UI controls
    QLineEdit* fileEdit{};
    QComboBox* layerData{};
    QComboBox* colorData{};
    QComboBox* ltypeData{};
    QComboBox* lwidthData{};
    QComboBox* pointData{};
    QGroupBox* pointBox{};
    QRadioButton* radioLayerCurrent{};
    QRadioButton* radioLayerFromData{};
    QRadioButton* radioColorCurrent{};
    QRadioButton* radioColorFromData{};
    QRadioButton* radioLtypeCurrent{};
    QRadioButton* radioLtypeFromData{};
    QRadioButton* radioLwidthCurrent{};
    QRadioButton* radioLwidthFromData{};
    QRadioButton* radioPointAsPoint{};
    QRadioButton* radioPointAsLabel{};
    QLabel* formatType{};
    QPushButton* fileButton{};
    QPushButton* acceptButton{};
    QPushButton* cancelButton{};

    // DBF field indices (resolved from combo box selections)
    int layerF{-1}, colorF{-1}, ltypeF{-1}, lwidthF{-1}, pointF{-1};

    // Attribute data for current record
    AttribData attData;

    // SHP object (class member — accessed synchronously within loop)
    SHPObject* sObject{nullptr};

    // Layer state
    QString currentLayer;

    // Document interface (set by ImportShp::execComm)
    Document_Interface* currDoc{nullptr};
};

#endif // IMPORTSHP_H
