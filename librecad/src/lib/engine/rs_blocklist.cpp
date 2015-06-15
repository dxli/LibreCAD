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

#include <QString>
#include <QRegExp>
#include "rs_debug.h"
#include "rs_blocklist.h"
#include "rs_block.h"
#include "rs_blocklistlistener.h"

#if QT_VERSION < 0x040400
#include "emu_qt44.h"
#endif

/**
 * Constructor.
 * 
 * @param owner true if this is the owner of the blocks added.
 *              If so, the blocks will be deleted when the block
 *              list is deleted.
 */
RS_BlockList::RS_BlockList(bool /*owner*/)
{
    //blocks.setAutoDelete(owner);
	setModified(false);
}


/**
 * Removes all blocks in the blocklist.
 */
void RS_BlockList::clear() {
    blocks.clear();
    activeBlock.reset();
	setModified(true);
}


/**
 * Activates the given block.
 * Listeners are notified.
 */
void RS_BlockList::activate(const QString& name) {
    RS_DEBUG->print("RS_BlockList::activateBlock");

    activate(find(name));
}

/**
 * Activates the given block.
 * Listeners are notified.
 */
void RS_BlockList::activate(std::shared_ptr<RS_Entity> const& block) {
    RS_DEBUG->print("RS_BlockList::activateBlock");
	activeBlock = block;
}

bool RS_BlockList::add(RS_Entity* block, bool notify)
{
    return add(std::shared_ptr<RS_Entity>{block}, notify);
}

/**
 * Adds a block to the block list. If a block with the same name
 * exists already, the given block will be deleted if the blocklist
 * owns the blocks.
 *
 * @param notify true if you want listeners to be notified.
 *
 * @return false: block already existed and was deleted.
 */
bool RS_BlockList::add(std::shared_ptr<RS_Entity> const& block, bool notify) {
    RS_DEBUG->print("RS_BlockList::add()");

    if (!block) return false;
    if (block->rtti()!= RS2::EntityBlock) return false;

    // check if block already exists:
    RS_Block* blockP=static_cast<RS_Block*>(block.get());
    std::shared_ptr<RS_Entity> b = find(blockP->getName());
    if (!b) {
        blocks.append(block);

        if (notify) {
            addNotification();
        }
		setModified(true);

		return true;
    } else {
//        if (owner) {
//            delete block;
//			block = nullptr;
//        }
		return false;
    }

}



/**
 * Notifies the listeners about blocks that were added. This can be
 * used after adding a lot of blocks without auto-update or simply
 * to force an update of GUI blocklists.
 */
void RS_BlockList::addNotification() {
	for(auto l: blockListListeners){
		l->blockAdded(nullptr);
	}
}



/**
 * Removes a block from the list.
 * Listeners are notified after the block was removed from 
 * the list but before it gets deleted.
 */
void RS_BlockList::remove(std::shared_ptr<RS_Entity> const& block) {
    RS_DEBUG->print("RS_BlockList::removeBlock()");

    // here the block is removed from the list but not deleted
#if QT_VERSION < 0x040400
    emu_qt44_removeOne(blocks, block);
#else
    blocks.removeOne(block);
#endif
	for(auto l: blockListListeners){
		l->blockRemoved(block);
	}
		
	setModified(true);

    // / *
    // activate an other block if necessary:
    if (activeBlock==block) {
    	//activate(blocks.first());
        activeBlock.reset();
	}
    // * /

    // now it's save to delete the block
//    if (owner) {
//        delete block;
//    }
}



/**
 * Tries to rename the given block to 'name'. Block names are unique in the
 * block list.
 *
 * @retval true block was successfully renamed.
 * @retval false block couldn't be renamed.
 */
bool RS_BlockList::rename(std::shared_ptr<RS_Entity>& block, const QString& name) {
	if (block) {
        if (!find(name)) {
            static_cast<RS_Block*>(block.get())->setName(name);
			setModified(true);
			return true;
		}
	}

	return false;
}


/**
 * Changes a block's attributes. The attributes of block 'block'
 * are copied from block 'source'.
 * Listeners are notified.
 */
/*
void RS_BlockList::editBlock(RS_Block* block, const RS_Block& source) {
	*block = source;
	
    for (unsigned i=0; i<blockListListeners.count(); ++i) {
		RS_BlockListListener* l = blockListListeners.at(i);
 
		l->blockEdited(block);
	}
}
*/

/**
 * @return Pointer to the block with the given name or
 * \p nullptr if no such block was found.
 */
std::shared_ptr<RS_Entity> RS_BlockList::find(const QString& name) {
    //RS_DEBUG->print("RS_BlockList::find");
	// Todo : reduce this from O(N) to O(log(N)) complexity based on sorted list or hash
    for(auto const& b: blocks){
        if (static_cast<RS_Block*>(b.get())->getName()==name) {
			return b;
		}
	}

	return nullptr;
}

/**
 * Finds a new unique block name.
 *
 * @param suggestion Suggested name the new name will be based on.
 */
QString RS_BlockList::newName(const QString& suggestion) {
//	qDebug()<<"suggestion: "<<suggestion;
	if(!find(suggestion))
		return suggestion;
	QString name=suggestion;
	QRegExp const rx(R"(\b(-?\d+)\b$)");
	int index=name.lastIndexOf(rx);
	int i=-1;
	if(index>0){
		i=name.mid(index+1).toInt();
		name=name.mid(0, index-1);
	}

    for(auto const& b: blocks){
        RS_Block* bP=static_cast<RS_Block*>(b.get());
        index=bP->getName().lastIndexOf(rx);
        if(bP->getName().mid(0, index-1) != name) continue;
        index=bP->getName().lastIndexOf(rx);
		if(index<0) continue;
        i=std::max(bP->getName().mid(index).toInt(),i);
	}
//	qDebug()<<QString("%1-%2").arg(name).arg(i+1);
	return QString("%1-%2").arg(name).arg(i+1);
}

/**
 * Switches on / off the given block. 
 * Listeners are notified.
 */
void RS_BlockList::toggle(const QString& name) {
    toggle(find(name));
}

/**
 * Switches on / off the given block. 
 * Listeners are notified.
 */
void RS_BlockList::toggle(std::shared_ptr<RS_Entity> const& block) {
    if (!block) return;

    static_cast<RS_Block*>(block.get())->toggle();
    // TODO LordOfBikes: when block attributes are saved, activate this
    //setModified(true);

    // Notify listeners:
	for(auto l: blockListListeners){
		l->blockToggled(block);
	}
}

/**
 * Freezes or defreezes all blocks.
 *
 * @param freeze true: freeze, false: defreeze
 */
void RS_BlockList::freezeAll(bool freeze)
{
    for(auto& b: blocks){
        static_cast<RS_Block*>(b.get())->freeze(freeze);
    }
    // TODO LordOfBikes: when block attributes are saved, activate this
    //setModified(true);

	for(auto l: blockListListeners){
		l->blockToggled(nullptr);
	}
}


/**
 * Switches on / off the given block. 
 * Listeners are notified.
 */
/*
void RS_BlockList::toggleBlock(const QString& name) {
	RS_Block* block = findBlock(name);
	block->toggle();
	
    // Notify listeners:
    for (unsigned i=0; i<blockListListeners.count(); ++i) {
		RS_BlockListListener* l = blockListListeners.at(i);
 
		l->blockToggled(block);
	}
}
*/


/**
 * adds a BlockListListener to the list of listeners. Listeners
 * are notified when the block list changes.
 */
void RS_BlockList::addListener(RS_BlockListListener* listener) {
    blockListListeners.append(listener);
}



/**
 * removes a BlockListListener from the list of listeners. 
 */
void RS_BlockList::removeListener(RS_BlockListListener* listener) {
#if QT_VERSION < 0x040400
    emu_qt44_removeOne(blockListListeners, listener);
#else
    blockListListeners.removeOne(listener);
#endif
}

int RS_BlockList::count() const{
	return blocks.count();
}

/**
 * @return Block at given position or nullptr if i is out of range.
 */
std::shared_ptr<RS_Entity> RS_BlockList::at(int i) {
	return blocks.at(i);
}
std::shared_ptr<RS_Entity> RS_BlockList::at(int i) const{
	return blocks.at(i);
}
QList<std::shared_ptr<RS_Entity>>::iterator RS_BlockList::begin()
{
	return blocks.begin();
}

QList<std::shared_ptr<RS_Entity>>::iterator RS_BlockList::end()
{
	return blocks.end();
}

QList<std::shared_ptr<RS_Entity>>::const_iterator RS_BlockList::begin()const
{
	return blocks.begin();
}

QList<std::shared_ptr<RS_Entity>>::const_iterator RS_BlockList::end()const
{
	return blocks.end();
}

//! @return The active block of nullptr if no block is activated.
std::shared_ptr<RS_Entity> RS_BlockList::getActive() {
	return activeBlock;
}

/**
 * Sets the layer lists modified status to 'm'.
 */
void RS_BlockList::setModified(bool m) {
	modified = m;
}

/**
 * @retval true The layer list has been modified.
 * @retval false The layer list has not been modified.
 */
bool RS_BlockList::isModified() const {
	return modified;
}

/**
 * Dumps the blocks to stdout.
 */
std::ostream& operator << (std::ostream& os, RS_BlockList& b) {

    os << "Blocklist: \n";
    for(auto& b: b.blocks){
        RS_Block* blk = static_cast<RS_Block*>(b.get());

        os << *blk << "\n";
    }

    return os;
}

