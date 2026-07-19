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
 * @brief RAII wrapper for shapelib SHPHandle.
 *
 * Automatically calls SHPClose() when the scope exits, preventing resource
 * leaks on early returns or exceptions.
 */
struct ScopedSHPHandle {
    SHPHandle sh{nullptr};
    ~ScopedSHPHandle() { if (sh) SHPClose(sh); }
    ScopedSHPHandle(const ScopedSHPHandle&) = delete;
    ScopedSHPHandle& operator=(const ScopedSHPHandle&) = delete;
};

/**
 * @brief RAII wrapper for shapelib DBFHandle.
 *
 * Automatically calls DBFClose() when the scope exits.
 */
struct ScopedDBFHandle {
    DBFHandle dh{nullptr};
    ~ScopedDBFHandle() { if (dh) DBFClose(dh); }
    ScopedDBFHandle(const ScopedDBFHandle&) = delete;
    ScopedDBFHandle& operator=(const ScopedDBFHandle&) = delete;
};

/**
 * @brief Resolve a DBF field index by name.
 *
 * @param radioFromData  Radio button controlling whether to use a DBF field.
 * @param comboBox       Combo box listing available DBF field names.
 * @param dh             DBF handle for field lookup.
 * @param[out] fieldIndex  Set to field index if resolved, -1 otherwise.
 * @return true if a field was successfully resolved.
 */
static bool resolveFieldIndex(
    QRadioButton* radioFromData,
    QComboBox* comboBox,
    DBFHandle dh,
    int& fieldIndex)
{
    if (!radioFromData->isChecked() || comboBox->currentIndex() < 0) {
        fieldIndex = -1;
        return false;
    }
    const QByteArray fieldName = QFile::encodeName(comboBox->currentText());
    fieldIndex = DBFGetFieldIndex(dh, fieldName.constData());
    return fieldIndex >= 0;
}

/**
 * @brief Process shape parts or rings into polylines.
 *
 * Shared implementation for readPolyline() and readPolygon().
 * Each part/ring becomes a separate polyline entity.
 *
 * @param dh        DBF handle for attribute reading.
 * @param recordIndex  Current shape record index.
 * @param closed    true for polygon rings (closed), false for arcs (open).
 * @param minVerts  Minimum vertices per part to create an entity (default 3).
 */
void processParts(DBFHandle dh, int recordIndex, bool closed, int minVerts = 3);

/**
 * @brief Shapefile import dialog.
 *
 * Provides UI for selecting a .shp file, choosing which DBF field maps to
 * layer, and importing the geometry.
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

    // Geometry readers
    void readPoint(DBFHandle dh, int recordIndex);
    void readPolyline(DBFHandle dh, int recordIndex);
    void readPolygon(DBFHandle dh, int recordIndex);
    void readMultiPoint(DBFHandle dh, int recordIndex);
    void readAttributes(DBFHandle dh, int recordIndex);

    // processParts — shared ring/part iteration
    void processParts(DBFHandle dh, int recordIndex, bool closed, int minVerts = 3);

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

    // Attribute group boxes (owned by layout)
    QGroupBox* layBox{};
    QGroupBox* colBox{};
    QGroupBox* ltypeBox{};
    QGroupBox* lwidthBox{};

    // DBF field indices (resolved from combo box selections)
    int layerF{-1}, colorF{-1}, ltypeF{-1}, lwidthF{-1}, pointF{-1};

    // SHP object (class member — accessed synchronously within loop)
    SHPObject* sObject{nullptr};

    // Layer state
    QString currentLayer;

    // Document interface (set by ImportShp::execComm)
    Document_Interface* currDoc{nullptr};
};

#endif // IMPORTSHP_H
