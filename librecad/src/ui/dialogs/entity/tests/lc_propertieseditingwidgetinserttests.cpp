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

// The Insert Properties dialog (Modify > Properties on a block reference): the Columns field that was wired to the
// Rows slot, the crossed spacing pick buttons, and the counts and the scale factors that emptied or mirrored the
// reference.

#include <catch2/catch_test_macros.hpp>

#include <memory>

#include <QLineEdit>
#include <QStringList>
#include <QSpinBox>
#include <QToolButton>

#include "lc_actiontestsupport.h"
#include "lc_propertieseditingwidget_insert.h"
#include "rs_block.h"
#include "rs_graphic.h"
#include "rs_insert.h"
#include "rs_line.h"

namespace {

/// the application comes first: a drawing reads its settings when it is built
struct TestApplication {
    TestApplication() {
        (void)lc::test::application();
    }
};

/// the dialog's panel over a clone of a reference of block B (a line from 0,0 to 10,0): the clone is what it edits
struct Dialog : TestApplication {
    RS_Graphic graphic;
    lc::test::TestGraphicView view;
    RS_Insert* clone = nullptr;
    std::unique_ptr<LC_PropertiesEditingWidgetInsert> widget;

    Dialog() {
        graphic.initForNewDocument();
        auto* block = new RS_Block(&graphic, RS_BlockData("B", RS_Vector(0, 0), false));
        block->addEntity(new RS_Line(block, RS_LineData(RS_Vector(0, 0), RS_Vector(10, 0))));
        graphic.addBlock(block);
        auto* insert = new RS_Insert(&graphic, RS_InsertData("B", RS_Vector(0, 0), RS_Vector(1, 1), 0., 1, 1, RS_Vector(0, 0)));
        graphic.addEntity(insert);
        insert->update();
        view.setDocument(&graphic);

        clone = static_cast<RS_Insert*>(insert->clone());
        widget = std::make_unique<LC_PropertiesEditingWidgetInsert>(nullptr);
        widget->setGraphicViewport(view.getViewPort());
        widget->setEntity(clone);
        static_cast<LC_EntityPropertiesEditorWidget&>(*widget).setupInteractiveInputWidgets(); // the dialog calls it when it opens
    }

    ~Dialog() {
        widget.reset();
        delete clone;
    }

    QLineEdit* field(const char* name) const {
        auto* edit = widget->findChild<QLineEdit*>(name);
        REQUIRE(edit != nullptr);
        return edit;
    }

    QSpinBox* countField(const char* name) const {
        auto* spinBox = widget->findChild<QSpinBox*>(name);
        REQUIRE(spinBox != nullptr);
        return spinBox;
    }

    /// what the user does to a field: enters \p text and leaves it
    void type(const char* name, const char* text) const {
        QLineEdit* edit = field(name);
        edit->setText(QString::fromLatin1(text));
        REQUIRE(QMetaObject::invokeMethod(edit, "editingFinished"));
    }
};

}

TEST_CASE("The Insert Properties dialog applies the Columns field", "[gui][dialog][insert]") {
    Dialog d;

    d.countField("sbCols")->setValue(4);

    CHECK(d.clone->getCols() == 4);
    CHECK(d.clone->getRows() == 1);
}

TEST_CASE("Each spacing pick button of the Insert Properties dialog asks for its own field", "[gui][dialog][insert]") {
    Dialog d;
    QStringList asked;
    QObject::connect(d.widget.get(), &LC_EntityPropertiesEditorWidget::interactiveInputRequested,
                     [&asked](LC_ActionContext::InteractiveInputInfo::InputType, const QString& tag) {
                         asked.append(tag);
                     });
    auto* rowButton = d.widget->findChild<QToolButton*>("tbPickRowSpacing");
    auto* columnButton = d.widget->findChild<QToolButton*>("tbPickColumnSpacing");
    REQUIRE(rowButton != nullptr);
    REQUIRE(columnButton != nullptr);

    rowButton->click();
    CHECK(asked.join(",").toStdString() == "rowSpacing");

    asked.clear();
    columnButton->click();
    CHECK(asked.join(",").toStdString() == "colSpacing");
}

TEST_CASE("The Insert Properties dialog uses bounded spin boxes for array counts", "[gui][dialog][insert]") {
    Dialog d;
    auto* rows = d.countField("sbRows");
    auto* columns = d.countField("sbCols");

    CHECK(rows->minimum() == 1);
    CHECK(rows->maximum() == RS_InsertData::MaxArrayCount);
    CHECK(columns->minimum() == 1);
    CHECK(columns->maximum() == RS_InsertData::MaxArrayCount);

    rows->setValue(0);
    columns->setValue(RS_InsertData::MaxArrayCount + 1);
    CHECK(rows->value() == 1);
    CHECK(columns->value() == RS_InsertData::MaxArrayCount);
    CHECK(d.clone->getRows() == 1);
    CHECK(d.clone->getCols() == RS_InsertData::MaxArrayCount);
}

TEST_CASE("The Insert Properties dialog takes only a positive scale factor", "[gui][dialog][insert]") {
    // zero and infinity would empty the reference, a negative factor would mirror it: the reference keeps its factors
    // and the fields show them (an entry that is no number is not taken either)
    for (const char* text : {"0", "1/0", "0/0", "-1/0", "-2", "-0.5", "-1/2", "-1e-9", "1e-11", "abc", ""}) {
        Dialog d;
        d.type("leScaleX", "3");
        d.type("leScaleY", "0.5");
        INFO("typed '" << text << "'");

        d.type("leScaleX", text);
        d.type("leScaleY", text);
        d.clone->update();

        CHECK(d.clone->getScale().x == 3.0);
        CHECK(d.clone->getScale().y == 0.5);
        CHECK(d.clone->count() == 1);
        CHECK(d.field("leScaleX")->text().toStdString() == "3");
        CHECK(d.field("leScaleY")->text().toStdString() == "0.5");
    }

    Dialog d;
    d.type("leScaleX", "2.5");
    CHECK(d.clone->getScale().x == 2.5);
}

TEST_CASE("The Insert Properties dialog keeps the sign of a mirrored reference", "[gui][dialog][insert]") {
    Dialog d;
    d.clone->setScale(RS_Vector(-1, 1)); // a file or Modify > Mirror made it
    d.widget->setEntity(d.clone);

    d.type("leScaleY", "2");
    CHECK(d.clone->getScale().x == -1.0);
    CHECK(d.clone->getScale().y == 2.0);

    for (const char* text : {"-3", "0", "1/0"}) {
        INFO("typed '" << text << "'");
        d.type("leScaleX", text);
        CHECK(d.clone->getScale().x == -1.0);
        CHECK(d.field("leScaleX")->text().toStdString() == "-1");
    }

    d.type("leScaleX", "3"); // a positive entry is the way out of the mirror
    CHECK(d.clone->getScale().x == 3.0);
}

TEST_CASE("The Insert Properties dialog keeps the spacings when a field is left without a change",
          "[gui][dialog][insert]") {
    // a field emits editingFinished when it loses the focus, edited or not, and the slot stores what the fields show
    Dialog d;
    d.clone->setSpacing(RS_Vector(1234.5678, 0.123456789));
    d.widget->setEntity(d.clone);

    REQUIRE(QMetaObject::invokeMethod(d.field("leRowSpacing"), "editingFinished"));

    CHECK(d.clone->getSpacing().x == 1234.5678);
    CHECK(d.clone->getSpacing().y == 0.123456789);
}

TEST_CASE("The Insert Properties dialog keeps a spacing it cannot read", "[gui][dialog][insert]") {
    Dialog d;
    d.clone->setSpacing(RS_Vector(5, 7));
    d.widget->setEntity(d.clone);

    // text that is no number leaves the spacing as it was, and the field shows it
    for (const char* text : {"abc", ""}) {
        INFO("typed '" << text << "'");
        d.type("leColSpacing", text);
        d.type("leRowSpacing", text);

        CHECK(d.clone->getSpacing().x == 5.0);
        CHECK(d.clone->getSpacing().y == 7.0);
        CHECK(d.field("leColSpacing")->text().toStdString() == "5");
        CHECK(d.field("leRowSpacing")->text().toStdString() == "7");
    }

    d.type("leColSpacing", "1/4");
    d.type("leRowSpacing", "2*3");
    CHECK(d.clone->getSpacing().x == 0.25);
    CHECK(d.clone->getSpacing().y == 6.0);
}

TEST_CASE("The Insert Properties dialog keeps the position, the angle and the spacings it cannot use",
          "[gui][dialog][insert]") {
    // infinity and NaN empty the reference, and Save as DXF refuses to write them
    for (const char* text : {"1/0", "0/0", "-1/0"}) {
        Dialog d;
        d.clone->setInsertionPoint(RS_Vector(5, 6));
        d.clone->setAngle(0.5);
        d.clone->setSpacing(RS_Vector(7, 8));
        d.widget->setEntity(d.clone);
        INFO("typed '" << text << "'");

        for (const char* name : {"leInsertionPointX", "leInsertionPointY", "leAngle", "leColSpacing",
                                 "leRowSpacing"}) {
            INFO("in " << name);
            d.type(name, text);
        }
        d.clone->update();

        CHECK(d.clone->getInsertionPoint().x == 5.0);
        CHECK(d.clone->getInsertionPoint().y == 6.0);
        CHECK(d.clone->getAngle() == 0.5);
        CHECK(d.clone->getSpacing().x == 7.0);
        CHECK(d.clone->getSpacing().y == 8.0);
        CHECK(d.clone->count() == 1);
    }
}

TEST_CASE("A value picked for a spacing of the Insert Properties dialog lands in its own field",
          "[gui][dialog][insert]") {
    Dialog d;
    d.clone->setSpacing(RS_Vector(7, 8));
    d.widget->setEntity(d.clone);

    d.widget->interactiveInputUpdate(LC_ActionContext::InteractiveInputInfo::DISTANCE, "colSpacing", 12.5, 0.);
    CHECK(d.clone->getSpacing().x == 12.5);
    CHECK(d.clone->getSpacing().y == 8.0);

    d.widget->interactiveInputUpdate(LC_ActionContext::InteractiveInputInfo::DISTANCE, "rowSpacing", 3.25, 0.);
    CHECK(d.clone->getSpacing().x == 12.5);
    CHECK(d.clone->getSpacing().y == 3.25);
}
