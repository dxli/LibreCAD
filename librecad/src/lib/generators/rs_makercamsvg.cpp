/****************************************************************************
**
** This file is part of the LibreCAD project, a 2D CAD program
**
**  Copyright (C) 2014 Christian Luginbühl (dinkel@pimprecords.com}
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License as published by the Free Software
** Foundation either version 2 of the License, or (at your option)
**  any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**
**********************************************************************/

#include "rs_makercamsvg.h"

#include <stdio.h>
#include <string.h>

#include <libxml++/libxml++.h>

#include "rs_dimaligned.h"
#include "rs_dimangular.h"
#include "rs_dimdiametric.h"
#include "rs_dimlinear.h"
#include "rs_dimradial.h"
#include "rs_hatch.h"
#include "rs_image.h"
#include "rs_insert.h"
#include "rs_layer.h"
#include "rs_point.h"
#include "rs_line.h"
#include "rs_leader.h"
#include "rs_spline.h"
#include "lc_splinepoints.h"
#include "rs_system.h"
#include "rs_graphic.h"
#include "rs_graphicview.h"
#include "rs_grid.h"
#include "rs_dialogfactory.h"
#include "rs_units.h"

RS_MakerCamSVG::RS_MakerCamSVG(bool writeInvisibleLayers,
                               bool writeConstructionLayers,
                               bool writeBlocksInline,
                               bool convertEllipsesToPaths) {

    RS_DEBUG->print("RS_MakerCamSVG::RS_MakerCamSVG()");

    this->writeInvisibleLayers = writeInvisibleLayers;
    this->writeConstructionLayers = writeConstructionLayers;
    this->writeBlocksInline = writeBlocksInline;
    this->convertEllipsesToPaths = convertEllipsesToPaths;

    this->doc = new xmlpp::Document();
}

RS_MakerCamSVG::~RS_MakerCamSVG() {
    delete doc;    
}

bool RS_MakerCamSVG::generate(RS_Graphic* graphic) {

    write(graphic);
    
    return true;
}

std::string RS_MakerCamSVG::resultAsString() {

    return doc->write_to_string_formatted();
}

void RS_MakerCamSVG::write(RS_Graphic* graphic) {

    RS_DEBUG->print("RS_MakerCamSVG::write: Writing root node ...");

    xmlpp::Element* e = doc->create_root_node("svg", "http://www.w3.org/2000/svg");

    graphic->calculateBorders();

    min = graphic->getMin();
    max = graphic->getMax();

    RS2::Unit raw_unit = graphic->getUnit();

    switch (raw_unit) {
        case RS2::Millimeter:
            unit = "mm";
            break;
        case RS2::Centimeter:
            unit = "cm";
            break;
        case RS2::Inch:
            unit = "in";
            break;

        default:
            unit = "";
            break;
    }

    e->set_attribute("width", numXml(max.x - min.x) + unit);

    e->set_attribute("height", numXml(max.y - min.y) + unit);

    e->set_attribute("viewBox", "0 0 "+ numXml(max.x - min.x) + " " + numXml(max.y - min.y));

    e->set_namespace_declaration("http://www.librecad.org", "lc");
    e->set_namespace_declaration("http://www.w3.org/1999/xlink", "xlink");

    writeBlocks(e, graphic);
    writeLayers(e, graphic);
}

void RS_MakerCamSVG::writeBlocks(xmlpp::Element* parent, RS_Document* document) {

    RS_DEBUG->print("RS_MakerCamSVG::writeBlocks: Writing blocks ...");

    RS_BlockList* blocklist = document->getBlockList();

    for (int i = 0; i < blocklist->count(); i++) {

        writeBlock(parent, blocklist->at(i));
    }
}

void RS_MakerCamSVG::writeBlock(xmlpp::Element* parent, RS_Block* block) {

    RS_DEBUG->print("RS_MakerCamSVG::writeBlock: Writing block with name '%s'",
                    qPrintable(block->getName()));

    xmlpp::Element* defs = parent->add_child("defs");

    defs->set_attribute("blockname", qPrintable(block->getName()), "lc");

    xmlpp::Element* g = defs->add_child("g");

    g->set_attribute("id", std::to_string(block->getId()));
    
    writeLayers(g, block);
}

void RS_MakerCamSVG::writeLayers(xmlpp::Element* parent, RS_Document* document) {

    RS_DEBUG->print("RS_MakerCamSVG::writeLayers: Writing layers ...");

    RS_LayerList* layerlist = document->getLayerList();

    for (unsigned int i = 0; i < layerlist->count(); i++) {

        writeLayer(parent, document, layerlist->at(i));
    }
}

void RS_MakerCamSVG::writeLayer(xmlpp::Element* parent, RS_Document* document, RS_Layer* layer) {

    if (writeInvisibleLayers || !layer->isFrozen()) {
    
        if (writeConstructionLayers || !layer->isConstructionLayer()) {
        
            RS_DEBUG->print("RS_MakerCamSVG::writeLayer: Writing layer with name '%s'",
                            qPrintable(layer->getName()));

            xmlpp::Element* g = parent->add_child("g");
            
            g->set_attribute("layername", qPrintable(layer->getName()), "lc");
            g->set_attribute("is_locked", (layer->isLocked() ? "true" : "false"), "lc");
            g->set_attribute("is_construction", (layer->isConstructionLayer() ? "true" : "false"), "lc");

            if (layer->isFrozen())
            {
                g->set_attribute("style", "display: none;");
            }

            g->set_attribute("fill", "none");
            g->set_attribute("stroke", "black");
            g->set_attribute("stroke-width", "1");

            writeEntities(g, document, layer);
        } else {

            RS_DEBUG->print("RS_MakerCamSVG::writeLayer: Omitting construction layer with name '%s'",
                            qPrintable(layer->getName()));
        }
    } else {
    
        RS_DEBUG->print("RS_MakerCamSVG::writeLayer: Omitting invisible layer with name '%s'",
                        qPrintable(layer->getName()));
    }
}

void RS_MakerCamSVG::writeEntities(xmlpp::Element* parent, RS_Document* document, RS_Layer* layer) {

    RS_DEBUG->print("RS_MakerCamSVG::writeEntitiesFromBlock: Writing entities from layer ...");

    for (RS_Entity *e = document->firstEntity(RS2::ResolveNone); e != NULL; e = document->nextEntity(RS2::ResolveNone)) {

        if (e->getLayer() == layer) {

            if (!(e->getFlag(RS2::FlagUndone))) {

                writeEntity(parent, e);
            }
        }
    }
}

void RS_MakerCamSVG::writeEntity(xmlpp::Element* parent, RS_Entity* entity) {

    RS_DEBUG->print("RS_MakerCamSVG::writeEntity: Found entity ...");

    switch (entity->rtti()) {
        case RS2::EntityInsert:
            writeInsert(parent, (RS_Insert*)entity);
            break;
        case RS2::EntityPoint:
            writePoint(parent, (RS_Point*)entity);
            break;
        case RS2::EntityLine:
            writeLine(parent, (RS_Line*)entity);
            break;
        case RS2::EntityCircle:
            writeCircle(parent, (RS_Circle*)entity);
            break;
        case RS2::EntityArc:
            writeArc(parent, (RS_Arc*)entity);
            break;
        case RS2::EntityEllipse:
            writeEllipse(parent, (RS_Ellipse*)entity);
            break;

        default:
            RS_DEBUG->print("RS_MakerCamSVG::writeEntity: Entity with type '%d' not yet implemented",
                            (int)entity->rtti());
            break;
    }
}

void RS_MakerCamSVG::writeInsert(xmlpp::Element* parent, RS_Insert* insert) {

    RS_DEBUG->print("RS_MakerCamSVG::writeInsert: Writing insert ...");

    xmlpp::Element* e = parent->add_child("use");

    RS_Vector insertionpoint = convertToSvg(insert->getInsertionPoint());
    
    RS_Block* block = insert->getBlockForInsert();    
    
    e->set_attribute("x", numXml(insertionpoint.x));
    e->set_attribute("y", numXml(insertionpoint.y - (max.y - min.y)));
    e->set_attribute("href", "#" + std::to_string(block->getId()), "xlink");
    
}

void RS_MakerCamSVG::writePoint(xmlpp::Element* parent, RS_Point* point) {

    RS_DEBUG->print("RS_MakerCamSVG::writePoint: Writing point ...");

    // NOTE: There is no "point" element in SVG, therefore creating a circle
    //       with minimal radius.

    xmlpp::Element* e = parent->add_child("circle");

    RS_Vector center = convertToSvg(point->getPos());

    e->set_attribute("cx", numXml(center.x));
    e->set_attribute("cy", numXml(center.y));
    e->set_attribute("r", numXml(0.1));
}

void RS_MakerCamSVG::writeLine(xmlpp::Element* parent, RS_Line* line) {

    RS_DEBUG->print("RS_MakerCamSVG::writeLine: Writing line ...");

    xmlpp::Element* e = parent->add_child("line");

    RS_Vector startpoint = convertToSvg(line->getStartpoint());
    RS_Vector endpoint = convertToSvg(line->getEndpoint());

    e->set_attribute("x1", numXml(startpoint.x));
    e->set_attribute("y1", numXml(startpoint.y));
    e->set_attribute("x2", numXml(endpoint.x));
    e->set_attribute("y2", numXml(endpoint.y));
}

void RS_MakerCamSVG::writeCircle(xmlpp::Element* parent, RS_Circle* circle) {

    RS_DEBUG->print("RS_MakerCamSVG::writeCircle: Writing circle ...");

    xmlpp::Element* e = parent->add_child("circle");

    RS_Vector center = convertToSvg(circle->getCenter());

    e->set_attribute("cx", numXml(center.x));
    e->set_attribute("cy", numXml(center.y));
    e->set_attribute("r", numXml(circle->getRadius()));
}

void RS_MakerCamSVG::writeArc(xmlpp::Element* parent, RS_Arc* arc) {

    RS_DEBUG->print("RS_MakerCamSVG::writeArc: Writing arc ...");

    xmlpp::Element* e = parent->add_child("path");

    RS_Vector startpoint = convertToSvg(arc->getStartpoint());
    RS_Vector endpoint = convertToSvg(arc->getEndpoint());
    double radius = arc->getRadius();

    double startangle = arc->getAngle1();
    double endangle = arc->getAngle2();

    
    if (endangle < startangle) {
        endangle += (2.0 * 3.14159265);
    }

    bool large_arc_flag = ((endangle - startangle) > 3.14159265);
    bool sweep_flag = false;

    if (arc->isReversed())
    {
        large_arc_flag = !large_arc_flag;
        sweep_flag = !sweep_flag;
    }

    std::string path = "M" + numXml(startpoint.x) + "," + numXml(startpoint.y) + " " +
                       "A" + numXml(radius) + "," + numXml(radius) + " " +
                       "0 " + 
                       (large_arc_flag ? "1" : "0") + " " +
                       (sweep_flag ? "1" : "0") + " " +
                       numXml(endpoint.x) + "," + numXml(endpoint.y);

    e->set_attribute("d", path);
}

void RS_MakerCamSVG::writeEllipse(xmlpp::Element* parent, RS_Ellipse* ellipse) {

    RS_DEBUG->print("RS_MakerCamSVG::writeEllipse: Writing ellipse ...");

    xmlpp::Element* e = parent->add_child("ellipse");

    RS_Vector center = convertToSvg(ellipse->getCenter());

    double minorradius = ellipse->getMinorRadius();
    double majorradius = ellipse->getMajorRadius();

    double majorangle = 180 - (180.0 / 3.14159265 * ellipse->getAngle() - 90);

    std::string transform = "translate(" + numXml(center.x) + ", " + numXml(center.y) + ") " +
                            "rotate(" + numXml(majorangle) + ")";

    e->set_attribute("rx", numXml(minorradius));
    e->set_attribute("ry", numXml(majorradius));
    e->set_attribute("transform", transform);
}

std::string RS_MakerCamSVG::numXml(double value) {
    
    char buffer[30];
    
    snprintf(buffer, sizeof(buffer), "%10.8f", value);

    std::string formatted(buffer);

    // TODO: How to not memory leak ... (delete[] buffer doen't work)

    return formatted;
}

RS_Vector RS_MakerCamSVG::convertToSvg(RS_Vector vector) {

    RS_Vector translated((vector.x - min.x), (max.y - vector.y));

    return translated;
}
