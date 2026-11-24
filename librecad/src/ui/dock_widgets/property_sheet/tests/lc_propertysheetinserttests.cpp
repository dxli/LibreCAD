/****************************************************************************
**
** This file is part of the LibreCAD project, a 2D CAD program
**
** Copyright (C) 2026 LibreCAD.org
** Copyright (C) 2026 Dongxu Li (github.com/dxli)
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**
****************************************************************************/

// The Insert panel of the property sheet: the Rows row that kept the wrong focus, the Block list that offered
// blocks a reference cannot expand, the count rows without an upper limit, and a zero or negative scale factor.
// A negative entry is refused by the spin box of the Scale rows, so those tests type into the editor the sheet opens.

#include <catch2/catch_test_macros.hpp>

#include <memory>
#include <string>
#include <vector>

#include <QCoreApplication>
#include <QDoubleSpinBox>
#include <QEventLoop>
#include <QKeyEvent>
#include <QLineEdit>
#include <QTimer>
#include <QWidget>

#include "lc_actioncontext.h"
#include "lc_actiongroupmanager.h"
#include "lc_actiontestsupport.h"
#include "lc_properties_sheet.h"
#include "lc_property_container.h"
#include "lc_property_double.h"
#include "lc_property_int.h"
#include "lc_property_int_spinbox_view.h"
#include "lc_property_multi.h"
#include "lc_property_qstring_list_combobox_view.h"
#include "lc_property_view_factory.h"
#include "lc_propertysheet_widget_options.h"
#include "lc_propertysheetwidget.h"
#include "rs_block.h"
#include "rs_graphic.h"
#include "rs_insert.h"
#include "rs_line.h"

namespace {

/// the row shown under this name, searched through all sections of the sheet
LC_Property* findByDisplayName(LC_PropertyContainer* container, const QString& displayName) {
    for (LC_Property* property : container->childProperties()) {
        if (property->getDisplayName() == displayName) {
            return property;
        }
        if (LC_PropertyContainer* section = property->asContainer()) {
            if (LC_Property* found = findByDisplayName(section, displayName)) {
                return found;
            }
        }
    }
    return nullptr;
}

/// runs the event loop for \p milliseconds (a property edit is applied by a 30 ms timer)
void waitMs(const int milliseconds) {
    QEventLoop loop;
    QTimer::singleShot(milliseconds, &loop, &QEventLoop::quit);
    loop.exec();
}

/// the one reference in \p container that has not been replaced by an edit
RS_Insert* liveInsert(RS_EntityContainer& container) {
    RS_Insert* found = nullptr;
    int count = 0;
    for (RS_Entity* e : container) {
        if (e != nullptr && e->rtti() == RS2::EntityInsert && !e->isDeleted()) {
            found = static_cast<RS_Insert*>(e);
            ++count;
        }
    }
    REQUIRE(count == 1);
    return found;
}

/// the references of \p container that an edit has not replaced, in the order they were added
std::vector<RS_Insert*> liveInserts(RS_EntityContainer& container) {
    std::vector<RS_Insert*> found;
    for (RS_Entity* e : container) {
        if (e != nullptr && e->rtti() == RS2::EntityInsert && !e->isDeleted()) {
            found.push_back(static_cast<RS_Insert*>(e));
        }
    }
    return found;
}

/// the application comes first: a drawing reads its settings when it is built
struct TestApplication {
    TestApplication() {
        (void)lc::test::application();
    }
};

/// a property sheet over a drawing with block B (a line from 0,0 to 10,0)
struct Sheet : TestApplication {
    LC_ActionContext context;
    LC_ActionGroupManager actions{nullptr};
    LC_PropertySheetWidget sheet{nullptr, &context, &actions}; // before the drawing, so that it is destroyed after it
    RS_Graphic graphic;
    lc::test::TestGraphicView view;

    Sheet() {
        sheet.getOptions()->noSelectionWorkspace = false;
        sheet.getOptions()->noSelectionGraphicView = false;
        graphic.initForNewDocument(); // the layer 0, which the sheet paints in its Layer row
        auto* block = new RS_Block(&graphic, RS_BlockData("B", RS_Vector(0, 0), false));
        block->addEntity(new RS_Line(block, RS_LineData(RS_Vector(0, 0), RS_Vector(10, 0))));
        graphic.addBlock(block);
    }

    ~Sheet() {
        sheet.setGraphicView(nullptr);
    }

    /// a reference of block B in \p container, with the scale factors \p scale
    RS_Insert* addInsert(RS_EntityContainer* container, const RS_Vector& scale = RS_Vector(1, 1)) {
        auto* insert = new RS_Insert(container, RS_InsertData("B", RS_Vector(0, 0), scale, 0., 1, 1, RS_Vector(0, 0)));
        container->addEntity(insert);
        insert->update();
        return insert;
    }

    /// shows \p selected, an entity of \p document, in the sheet
    void open(RS_Document* document, RS_Entity* selected) {
        open(document, QList<RS_Entity*>{selected});
    }

    /// shows the row of every entity of \p selected, which are entities of \p document
    void open(RS_Document* document, const QList<RS_Entity*>& selected) {
        document->select(selected, true);
        view.setDocument(document);
        context.setDocumentAndView(document, &view);
        sheet.show();
        QCoreApplication::processEvents();
        sheet.setGraphicView(&view);
        QCoreApplication::processEvents();
    }

    LC_PropertiesSheet* properties() {
        auto* properties = sheet.findChild<LC_PropertiesSheet*>();
        REQUIRE(properties != nullptr);
        REQUIRE(properties->getPropertyContainer() != nullptr);
        return properties;
    }

    /// the row shown under this name: it wraps the property of each selected entity
    LC_PropertyMulti* row(const QString& name) {
        auto* multi = dynamic_cast<LC_PropertyMulti*>(findByDisplayName(properties()->getPropertyContainer(), name));
        INFO(name.toStdString());
        REQUIRE(multi != nullptr);
        return multi;
    }

    /// what the editor of a row does when the user enters \p value
    template <class PropertyClass, class Value>
    void edit(const QString& name, const Value value) {
        LC_PropertyMulti* multi = row(name);
        auto* typed = dynamic_cast<PropertyClass*>(multi->getFirstProperty());
        REQUIRE(typed != nullptr);
        REQUIRE(typed->setValue(value, LC_PropertyChangeReason(PropertyChangeReasonEdit) | PropertyChangeReasonValueNew));
        apply(multi);
    }

    /// what the sheet does when the editor of a row is closed: it applies the entry to the selected entities
    void apply(LC_PropertyMulti* multi) {
        sheet.onPropertyEdited(multi);
        waitMs(100);
    }

    /// the names the Block row offers, joined (a QStringList prints as {?} in a failed check)
    std::string blockNames() {
        const LC_PropertyViewDescriptor* descriptor = row("Block")->getFirstProperty()->getViewDescriptor();
        REQUIRE(descriptor != nullptr);
        return descriptor->attributes.value(LC_PropertyQStringListComboBoxView::ATTR_ITEMS).toStringList().join(",").toStdString();
    }
};

/// the spin box the sheet opens on a number row, made the way the sheet makes it, and the keys a user presses in it
struct SpinEditor {
    std::unique_ptr<LC_PropertyView> view; // after the editor, which is destroyed first
    QWidget parent;
    QDoubleSpinBox* spinBox = nullptr;
    QLineEdit* lineEdit = nullptr; // the text of the spin box

    explicit SpinEditor(LC_PropertyMulti* multi) {
        LC_PropertyAtomic* first = multi->getFirstProperty();
        view.reset(LC_PropertyViewFactory::staticInstance()->createView(first));
        view->setStateProperty(multi);
        // a row can have no attributes at all: the sheet then applies none
        const LC_PropertyViewDescriptor* attributes = first->getViewDescriptor();
        view->applyAttributes(attributes != nullptr ? *attributes : LC_PropertyViewDescriptor());
        auto* editable = dynamic_cast<LC_PropertyViewEditable*>(view.get());
        REQUIRE(editable != nullptr);
        spinBox = qobject_cast<QDoubleSpinBox*>(editable->createValueEditor(&parent, QRect(0, 0, 200, 24), nullptr));
        REQUIRE(spinBox != nullptr);
        lineEdit = spinBox->findChild<QLineEdit*>();
        REQUIRE(lineEdit != nullptr);
        multi->markEdited(true); // as the sheet does while the editor is open: an entry reaches every selected entity
    }

    /// what the editor shows
    std::string shown() const {
        return lineEdit->text().toStdString();
    }

    /// the user presses \p key in the editor (Up and Down step the value)
    void press(const int key) {
        QKeyEvent event(QEvent::KeyPress, key, Qt::NoModifier);
        QCoreApplication::sendEvent(lineEdit, &event);
    }

    /// the user types \p text over the shown value, key by key, and presses Return
    void enter(const char* text) {
        lineEdit->selectAll();
        for (const char* c = text; *c != '\0'; ++c) {
            const int key = *c == '-' ? Qt::Key_Minus : *c == '.' ? Qt::Key_Period : Qt::Key_0 + (*c - '0');
            QKeyEvent press(QEvent::KeyPress, key, Qt::NoModifier, QString(QChar(*c)));
            QCoreApplication::sendEvent(lineEdit, &press);
        }
        QKeyEvent ret(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
        QCoreApplication::sendEvent(spinBox, &ret);
    }
};

}

TEST_CASE("The Rows row of a block reference keeps the focus when it is edited", "[gui][properties][insert]") {
    Sheet s;
    s.open(&s.graphic, s.addInsert(&s.graphic));
    // the sheet finds the active row again by its name
    CHECK(s.row("Rows")->getName().toStdString() != s.row("Columns")->getName().toStdString());
    s.properties()->setActiveProperty(s.row("Rows"), true);
    QCoreApplication::processEvents();

    s.edit<LC_PropertyInt>("Rows", 2);

    CHECK(liveInsert(s.graphic)->getRows() == 2);
    REQUIRE(s.properties()->getActiveProperty() != nullptr);
    CHECK(s.properties()->getActiveProperty()->getDisplayName().toStdString() == "Rows");
}

TEST_CASE("The Block list of a block reference leaves out removed blocks and lists the others once",
          "[gui][properties][insert]") {
    Sheet s;
    // an anonymous block expands as any
    s.graphic.addBlock(new RS_Block(&s.graphic, RS_BlockData("*U1", RS_Vector(0, 0), false)));
    auto* removed = new RS_Block(&s.graphic, RS_BlockData("R", RS_Vector(0, 0), false));
    s.graphic.addBlock(removed);
    removed->setFlag(RS2::FlagDeleted); // Blocks > Remove keeps the block in the list, flagged, for undo
    s.open(&s.graphic, s.addInsert(&s.graphic));

    CHECK(s.blockNames() == "*U1,B");

    // a reference of a block that was removed keeps showing it
    Sheet orphan;
    auto* gone = new RS_Block(&orphan.graphic, RS_BlockData("G", RS_Vector(0, 0), false));
    orphan.graphic.addBlock(gone);
    auto* insert = new RS_Insert(&orphan.graphic,
                                 RS_InsertData("G", RS_Vector(0, 0), RS_Vector(1, 1), 0., 1, 1, RS_Vector(0, 0)));
    orphan.graphic.addEntity(insert);
    gone->setFlag(RS2::FlagDeleted);
    orphan.open(&orphan.graphic, insert);

    CHECK(orphan.blockNames() == "B,G");

    // a block removed and made again under its name is listed once
    Sheet again;
    RS_Block* old = again.graphic.getBlockList()->find("B");
    REQUIRE(old != nullptr);
    old->setFlag(RS2::FlagDeleted);
    auto* fresh = new RS_Block(&again.graphic, RS_BlockData("B", RS_Vector(0, 0), false));
    fresh->addEntity(new RS_Line(fresh, RS_LineData(RS_Vector(0, 0), RS_Vector(5, 0))));
    REQUIRE(again.graphic.addBlock(fresh));
    again.open(&again.graphic, again.addInsert(&again.graphic));

    CHECK(again.blockNames() == "B");
}

TEST_CASE("In a block's editor the Block list leaves out that block and the blocks that contain it", "[gui][properties][insert]") {
    Sheet s;
    auto* a = new RS_Block(&s.graphic, RS_BlockData("A", RS_Vector(0, 0), false));
    s.graphic.addBlock(a);
    auto* c = new RS_Block(&s.graphic, RS_BlockData("C", RS_Vector(0, 0), false));
    s.graphic.addBlock(c);
    c->addEntity(new RS_Insert(c, RS_InsertData("A", RS_Vector(0, 0), RS_Vector(1, 1), 0., 1, 1, RS_Vector(0, 0))));
    s.graphic.addBlock(new RS_Block(&s.graphic, RS_BlockData("D", RS_Vector(0, 0), false)));
    s.open(a, s.addInsert(a)); // A is open, and holds a reference of B

    // a reference of A inside A, or of C (which holds A), would contain itself
    CHECK(s.blockNames() == "B,D");
}

TEST_CASE("The Columns and Rows rows stop at the largest array a DXF file reads back", "[gui][properties][insert]") {
    Sheet s;
    s.open(&s.graphic, s.addInsert(&s.graphic));

    for (const char* name : {"Columns", "Rows"}) {
        INFO(name);
        const LC_PropertyViewDescriptor* descriptor = s.row(name)->getFirstProperty()->getViewDescriptor();
        REQUIRE(descriptor != nullptr);
        // libdxfrw refuses the whole file when a count in groups 70 and 71 is above 32767
        CHECK(descriptor->attributes.value(LC_PropertyIntSpinBoxView::ATTR_MAX).toInt() == 32767);
    }
}

TEST_CASE("A zero scale factor in the Scale X row keeps the block reference expanded", "[gui][properties][insert]") {
    Sheet s;
    s.open(&s.graphic, s.addInsert(&s.graphic));

    s.edit<LC_PropertyDouble>("Scale X", 0.0); // the row's spin box takes a zero

    const RS_Insert* insert = liveInsert(s.graphic);
    CHECK(insert->getScale().x == 1.0);
    CHECK(insert->count() == 1);
}

TEST_CASE("The Scale X and Scale Y rows take no minus sign and ignore zero", "[gui][properties][insert]") {
    Sheet s;
    s.open(&s.graphic, s.addInsert(&s.graphic, RS_Vector(2, 3)));

    // a negative factor would mirror the block: the spin box does not take the minus sign,
    // so "-4" is 4 and "-0.5" is 0.5
    {
        LC_PropertyMulti* row = s.row("Scale X");
        SpinEditor editor(row);
        CHECK(editor.spinBox->minimum() == RS_TOLERANCE);
        editor.enter("-4");
        CHECK(editor.shown() == "4");
        s.apply(row);
    }
    {
        LC_PropertyMulti* row = s.row("Scale Y");
        SpinEditor editor(row);
        editor.enter("-0.5");
        s.apply(row);
    }
    CHECK(liveInsert(s.graphic)->getScale().x == 4.0);
    CHECK(liveInsert(s.graphic)->getScale().y == 0.5);

    // zero is below the minimum: the entry is ignored and the reference keeps its factors
    for (const char* name : {"Scale X", "Scale Y"}) {
        INFO(name);
        LC_PropertyMulti* row = s.row(name);
        SpinEditor editor(row);
        editor.enter("0");
        s.apply(row);
    }
    CHECK(liveInsert(s.graphic)->getScale().x == 4.0);
    CHECK(liveInsert(s.graphic)->getScale().y == 0.5);
}

TEST_CASE("A step down in a Scale row does not land on the minimum", "[gui][properties][insert]") {
    Sheet s;
    s.open(&s.graphic, s.addInsert(&s.graphic));

    // the minimum is the tolerance: a step of 1 from the factor 1 would leave a reference that has all but vanished
    LC_PropertyMulti* row = s.row("Scale X");
    SpinEditor editor(row);
    editor.press(Qt::Key_Down);

    CHECK(editor.shown() == "0.9");
}

TEST_CASE("An ignored scale entry leaves every selected block reference as it was", "[gui][properties][insert]") {
    Sheet s;
    RS_Insert* first = s.addInsert(&s.graphic, RS_Vector(2, 1));
    RS_Insert* second = s.addInsert(&s.graphic, RS_Vector(3, 1));
    s.open(&s.graphic, QList<RS_Entity*>{first, second});

    // the row holds both factors: an entry that is not taken must not copy the first factor to the second
    {
        LC_PropertyMulti* row = s.row("Scale X");
        SpinEditor editor(row);
        editor.enter("0");
        s.apply(row);
    }
    auto inserts = liveInserts(s.graphic);
    REQUIRE(inserts.size() == 2);
    CHECK(inserts[0]->getScale().x == 2.0);
    CHECK(inserts[1]->getScale().x == 3.0);

    {
        LC_PropertyMulti* row = s.row("Scale X");
        SpinEditor editor(row);
        editor.enter("-4");
        s.apply(row);
    }
    inserts = liveInserts(s.graphic);
    REQUIRE(inserts.size() == 2);
    CHECK(inserts[0]->getScale().x == 4.0);
    CHECK(inserts[1]->getScale().x == 4.0);
}

TEST_CASE("A mirrored block reference keeps its sign until a factor is entered", "[gui][properties][insert]") {
    Sheet s;
    s.open(&s.graphic, s.addInsert(&s.graphic, RS_Vector(-1, 1))); // a file or Modify > Mirror made it

    s.edit<LC_PropertyDouble>("Scale Y", 2.0); // the other factor
    CHECK(liveInsert(s.graphic)->getScale().x == -1.0);
    CHECK(liveInsert(s.graphic)->getScale().y == 2.0);

    for (const char* entry : {"", "0"}) {
        // the spin box cannot show a negative factor: closing it with no entry, or with zero,
        // must not store what it shows
        INFO("entered '" << entry << "'");
        LC_PropertyMulti* row = s.row("Scale X");
        SpinEditor editor(row);
        editor.enter(entry);
        s.apply(row);
        CHECK(liveInsert(s.graphic)->getScale().x == -1.0);
    }

    {
        LC_PropertyMulti* row = s.row("Scale X");
        SpinEditor editor(row);
        editor.enter("3"); // a positive entry is the way out of the mirror
        s.apply(row);
    }
    CHECK(liveInsert(s.graphic)->getScale().x == 3.0);
}
