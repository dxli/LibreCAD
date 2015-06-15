/****************************************************************************
**
** This file is part of the LibreCAD project, a 2D CAD program
**
** Copyright (C) 2010 R. van Twisk (librecad@rvt.dds.nl)
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
**
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software 
** Foundation and appearing in the file gpl-2.0.txt included in the
** packaging of this file.
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
** This copyright notice MUST APPEAR in all copies of the script!  
**
**********************************************************************/


#ifndef RS_CREATION_H
#define RS_CREATION_H

#include <memory>
#include <QString>
#include "rs_vector.h"

class RS_Document;
class RS_EntityContainer;
class RS_GraphicView;
class RS_Graphic;
class RS_Entity;
class RS_Arc;
class RS_Circle;
class RS_Ellipse;
class RS_Line;
class LC_SplinePoints;
struct RS_BlockData;
struct RS_ImageData;
class RS_Image;
struct RS_InsertData;
class RS_Insert;
class RS_Block;

/**
 * Data needed to insert library items.
 */
struct RS_LibraryInsertData {
        QString file;
	RS_Vector insertionPoint;
	double factor;
	double angle;
};


/**
 * Class for the creation of new entities.
 * This class is bound to an entity container in which the
 * entities are stored. 
 *
 * @author Andrew Mustun
 */
class RS_Creation {
public:
    RS_Creation(RS_EntityContainer* container,
				RS_GraphicView* graphicView=nullptr,
                bool handleUndo=true);
	~RS_Creation()=default;

    std::shared_ptr<RS_Entity> createParallelThrough(const RS_Vector& coord,
                              int number,
                              std::shared_ptr<RS_Entity> const& e);

    std::shared_ptr<RS_Entity> createParallel(const RS_Vector& coord,
                              double distance,
                              int number,
                              std::shared_ptr<RS_Entity> const& e);

    std::shared_ptr<RS_Entity> createParallelLine(const RS_Vector& coord,
                                double distance, int number,
                                std::shared_ptr<RS_Entity> const& e);

    std::shared_ptr<RS_Entity> createParallelArc(const RS_Vector& coord,
                              double distance, int number,
                              std::shared_ptr<RS_Entity> const& e);

    std::shared_ptr<RS_Entity> createParallelCircle(const RS_Vector& coord,
                                    double distance, int number,
                                    std::shared_ptr<RS_Entity> const& e);

    std::shared_ptr<RS_Entity> createParallelSplinePoints(const RS_Vector& coord,
                                    double distance, int number,
                                    std::shared_ptr<RS_Entity> const& e);

    std::shared_ptr<RS_Entity> createBisector(const RS_Vector& coord1,
                            const RS_Vector& coord2,
                            double length,
                            int num,
                            std::shared_ptr<RS_Entity> const& l1,
                            std::shared_ptr<RS_Entity> const& l2);

    std::shared_ptr<RS_Entity> createTangent1(const RS_Vector& coord,
                            const RS_Vector& point,
                            std::shared_ptr<RS_Entity> const& circle);
/**
 * create a tangent line which is orthogonal to the given RS_Line(normal)
 */
    std::shared_ptr<RS_Entity> createLineOrthTan(const RS_Vector& coord,
                            std::shared_ptr<RS_Entity> const& normal,
                            std::shared_ptr<RS_Entity> const& circle);
    std::shared_ptr<RS_Entity> createTangent2(const RS_Vector& coord,
                            std::shared_ptr<RS_Entity> circle1,
                            std::shared_ptr<RS_Entity> circle2);
    /**
      * create the path of centers of common tangent circles of the two given circles
	  *@ return nullptr, if failed
      *@ at success return either an ellipse or hyperbola
      */
    std::vector<std::shared_ptr<RS_Entity>> createCircleTangent2(std::shared_ptr<RS_Entity> const& circle1,
                                                 std::shared_ptr<RS_Entity> const& circle2);

    std::shared_ptr<RS_Entity> createLineRelAngle(const RS_Vector& coord,
                                std::shared_ptr<RS_Entity> const& entity,
                                double angle,
                                double length);

    std::shared_ptr<RS_Entity> createPolygon(const RS_Vector& center,
                           const RS_Vector& corner,
                           int number);

    std::shared_ptr<RS_Entity> createPolygon2(const RS_Vector& corner1,
                            const RS_Vector& corner2,
                            int number);

    std::shared_ptr<RS_Entity> createInsert(const RS_InsertData* pdata);
	
    std::shared_ptr<RS_Entity> createImage(const RS_ImageData* pdata);

    std::shared_ptr<RS_Entity> createBlock(const RS_BlockData* data,
                          const RS_Vector& referencePoint,
                          const bool remove);
						  
    std::shared_ptr<RS_Entity> createLibraryInsert(RS_LibraryInsertData& data);

protected:
    RS_EntityContainer* container;
	RS_Graphic* graphic;
	RS_Document* document;
    RS_GraphicView* graphicView;
    bool handleUndo;
private:
    void setEntity(std::shared_ptr<RS_Entity> const& en) const;
};

#endif
