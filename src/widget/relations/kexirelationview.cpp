/* This file is part of the KDE project
   Copyright (C) 2002   Lucijan Busch <lucijan@gmx.at>
   Copyright (C) 2003 Jaroslaw Staniek <js@iidea.pl>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#include <kdebug.h>

#include <qstringlist.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qheader.h>
#include <qevent.h>
#include <qpainter.h>
#include <qstyle.h>
#include <qlineedit.h>
#include <qpopupmenu.h>

#include <kdebug.h>
#include <klocale.h>
#include <kaction.h>
#include <kpopupmenu.h>
#include <kglobalsettings.h>
#include <kmessagebox.h>

#include <kexidb/tableschema.h>
#include <kexidb/connection.h>
#include <kexidb/indexschema.h>

#include "kexirelationview.h"
#include "kexirelationviewtable.h"
#include "kexirelationviewconnection.h"
#include "kexi_utils.h"

KexiRelationView::KexiRelationView(QWidget *parent, KexiDB::Connection *conn, const char *name)
 : QScrollView(parent, name, WStaticContents)
{
//	m_relation=relation;
//	m_relation->incUsageCount();
	m_selectedConnection = 0;
	m_readOnly=false;
	m_focusedTableView = 0;
	setFrameStyle(QFrame::WinPanel | QFrame::Sunken);

//	connect(relation, SIGNAL(relationListUpdated(QObject *)), this, SLOT(slotListUpdate(QObject *)));

	viewport()->setPaletteBackgroundColor(colorGroup().mid());
	setFocusPolicy(WheelFocus);
	setResizePolicy(Manual);
/*MOVED TO KexiRelationDialog
	//actions
	m_tableQueryPopup = new KPopupMenu(this, "m_popup");
	m_tableQueryPopup->insertTitle(i18n("Table"));
	m_connectionPopup = new KPopupMenu(this, "m_connectionPopup");
	m_connectionPopup->insertTitle(i18n("Relation"));
	m_areaPopup = new KPopupMenu(this, "m_areaPopup");
	
	plugSharedAction("edit_delete", i18n("Hide Table"), m_tableQueryPopup);
	plugSharedAction("edit_delete",m_connectionPopup);
	plugSharedAction("edit_delete",this, SLOT(removeSelectedObject()));
*/	
#if 0
	m_removeSelectedTableQueryAction = new KAction(i18n("&Hide Selected Table/Query"), "editdelete", "",
		this, SLOT(removeSelectedTableQuery()), parent->actionCollection(), "relationsview_removeSelectedTableQuery");
	m_removeSelectedConnectionAction = new KAction(i18n("&Remove Selected Relationship"), "button_cancel", "",
		this, SLOT(removeSelectedConnection()), parent->actionCollection(), "relationsview_removeSelectedConnection");
	m_openSelectedTableQueryAction = new KAction(i18n("&Open Selected Table/Query"), "", "",
		this, SLOT(openSelectedTableQuery()), 0/*parent->actionCollection()*/, "relationsview_openSelectedTableQuery");
#endif

//	invalidateActions();

#if 0


	m_popup = new KPopupMenu(this, "m_popup");
	m_openSelectedTableQueryAction->plug( m_popup );
	m_removeSelectedTableQueryAction->plug( m_popup );
	m_removeSelectedConnectionAction->plug( m_popup );

	invalidateActions();
#endif

	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding, true);
}

KexiRelationView::~KexiRelationView()
{
}

void
KexiRelationView::addTable(KexiDB::TableSchema *t)
{
	if(!t)
		return;

	kdDebug() << "KexiRelationView::addTable(): " << t->name() << ", " << viewport() << endl;

	/*
	if(m_tables.contains(t->name()))
	{
		kdDebug() << "KexiRelationView::addTable(): table already exists" << endl;
//		return;
	}
	*/

	KexiRelationViewTableContainer *c = new KexiRelationViewTableContainer(this, t);
	connect(c, SIGNAL(endDrag()), this, SLOT(slotTableViewEndDrag()));
	connect(c, SIGNAL(gotFocus()), this, SLOT(slotTableViewGotFocus()));
//	connect(c, SIGNAL(headerContextMenuRequest(const QPoint&)), 
//		this, SLOT(tableHeaderContextMenuRequest(const QPoint&)));
	connect(c, SIGNAL(contextMenuRequest(const QPoint&)), 
		this, SIGNAL(tableContextMenuRequest(const QPoint&)));
	
	addChild(c, 100,100);
//	c->setFixedSize(110, 160);
	c->show();
	updateGeometry();
	c->updateGeometry();
	c->resize(c->sizeHint());
	int x, y;

	if(m_tables.count() > 0)
	{
		int place = -10;
		QDictIterator<KexiRelationViewTableContainer> it(m_tables);
		for(; it.current(); ++it)
		{
			int right = (*it)->x() + (*it)->width();
			if(right > place)
				place = right;
		}

		x = place + 15;
	}
	else
	{
		x = 5;
	}

	y = 5;
	QPoint p = viewportToContents(QPoint(x, y));
	recalculateSize(p.x() + c->width(), p.y() + c->height());
	moveChild(c, x, y);

	m_tables.insert(t->name(), c);

	connect(c, SIGNAL(moved(KexiRelationViewTableContainer *)), this,
            SLOT(containerMoved(KexiRelationViewTableContainer *)));
	
	c->setFocus();
}

void
KexiRelationView::addConnection(SourceConnection conn, bool)
{
	kdDebug() << "KexiRelationView::addConnection()" << endl;

	KexiRelationViewTableContainer *master = m_tables[conn.masterTable];
	KexiRelationViewTableContainer *details = m_tables[conn.detailsTable];
	if (!master || !details)
		return;

	KexiDB::TableSchema *masterTable = master->table();
	KexiDB::TableSchema *detailsTable = details->table();
	if (!masterTable || !detailsTable)
		return;

	// ok, but we need to know where is the 'master' and where is the 'details' side:
	KexiDB::Field *masterFld = masterTable->field(conn.masterField);
	KexiDB::Field *detailsFld = detailsTable->field(conn.detailsField);
	if (!masterFld || !detailsFld)
		return;
	
	if (!masterFld->isUniqueKey()) {
		if (detailsFld->isUniqueKey()) {
			//SWAP:
			KexiDB::Field *tmpFld = masterFld;
			masterFld = detailsFld;
			detailsFld = tmpFld;
			KexiDB::TableSchema *tmpTable = masterTable;
			masterTable = detailsTable;
			detailsTable = tmpTable;
			KexiRelationViewTableContainer *tmp = master;
			master = details;
			details = tmp;
			QString tmp_masterTable = conn.masterTable;
			conn.masterTable = conn.detailsTable;
			conn.detailsTable = tmp_masterTable;
			QString tmp_masterField = conn.masterField;
			conn.masterField = conn.detailsField;
			conn.detailsField = tmp_masterField;
		}
	}

//	kdDebug() << "KexiRelationView::addConnection(): finalSRC = " << m_tables[conn.srcTable] << endl;

	KexiRelationViewConnection *connView = new KexiRelationViewConnection(master, details, conn, this);
	m_connectionViews.append(connView);
	updateContents(connView->connectionRect());

/*js: will be moved up to relation/query part as this is only visual class
	KexiDB::TableSchema *mtable = m_conn->tableSchema(conn.srcTable);
	KexiDB::TableSchema *ftable = m_conn->tableSchema(conn.rcvTable);
	KexiDB::IndexSchema *forign = new KexiDB::IndexSchema(ftable);

	forign->addField(mtable->field(conn.srcField));
	new KexiDB::Reference(forign, mtable->primaryKey());
*/
#if 0
	if(!interactive)
	{
		kdDebug() << "KexiRelationView::addConnection: adding self" << endl;
		RelationList l = m_relation->projectRelations();
		l.append(conn);
		m_relation->updateRelationList(this, l);
	}
#endif
}

void
KexiRelationView::drawContents(QPainter *p, int cx, int cy, int cw, int ch)
{
	KexiRelationViewConnection *cview;
//	p->translate(0, (double)contentsY());

	QRect clipping(cx, cy, cw, ch);
	for(cview = m_connectionViews.first(); cview; cview = m_connectionViews.next())
	{
		if(clipping.intersects(cview->connectionRect()))
			cview->drawConnection(p);
	}
}

void
KexiRelationView::slotTableScrolling(QString table)
{
	KexiRelationViewTableContainer *c = m_tables[table];

	if(c)
		containerMoved(c);
}

void
KexiRelationView::containerMoved(KexiRelationViewTableContainer *c)
{
	KexiRelationViewConnection *cview;
	for (cview = m_connectionViews.first(); cview; cview = m_connectionViews.next())
	{
		if(cview->masterTable() == c || cview->detailsTable() == c)
		{
			updateContents(cview->oldRect());
			updateContents(cview->connectionRect());
		}
	}

//	QRect w(c->x() - 5, c->y() - 5, c->width() + 5, c->height() + 5);
//	updateContents(w);

	QPoint p = viewportToContents(QPoint(c->x(), c->y()));
	recalculateSize(p.x() + c->width(), p.y() + c->height());
}

void
KexiRelationView::setReadOnly(bool b)
{
	m_readOnly=b;
//TODO
//	invalidateActions();
/*	for (TableList::iterator it=m_tables.begin();it!=m_tables.end();++it)
	{
//		(*it)->setReadOnly(b);
#ifndef Q_WS_WIN
		#warning readonly needed
#endif
	}*/
}

void
KexiRelationView::slotListUpdate(QObject *)
{
#if 0
	if(s != this)
	{
		m_connectionViews.clear();
		RelationList rl = m_relation->projectRelations();
		if(!rl.isEmpty())
		{
			for(RelationList::Iterator it = rl.begin(); it != rl.end(); it++)
			{
				addConnection((*it), true);
			}
		}
	}

	updateContents();
#endif
}

void
KexiRelationView::contentsMousePressEvent(QMouseEvent *ev)
{
	KexiRelationViewConnection *cview;
	for(cview = m_connectionViews.first(); cview; cview = m_connectionViews.next())
	{
		if(!cview->matchesPoint(ev->pos(), 3))
			continue;
		clearSelection();
		cview->setSelected(true);
		updateContents(cview->connectionRect());
		m_selectedConnection = cview;
		emit connectionViewGotFocus();
//		invalidateActions();

		if(ev->button() == RightButton) {//show popup
			kdDebug() << "KexiRelationView::contentsMousePressEvent(): context" << endl;
//			QPopupMenu m;
//				m_removeSelectedTableQueryAction->plug( &m );
//				m_removeSelectedConnectionAction->plug( &m );
			emit connectionContextMenuRequest( ev->globalPos() );
//			executePopup( ev->globalPos() );
		}
		return;
	}
	//connection not found
	clearSelection();
//	invalidateActions();
	if(ev->button() == RightButton) {//show popup on view background area
//		QPopupMenu m;
//			m_removeSelectedConnectionAction->plug( &m );
		emit emptyAreaContextMenuRequest( ev->globalPos() );
//		executePopup(ev->globalPos());
	}
	else {
		emit emptyAreaGotFocus();
	}
	setFocus();
//	QScrollView::contentsMousePressEvent(ev);
}

void KexiRelationView::clearSelection()
{
	if (m_focusedTableView) {
		m_focusedTableView->unsetFocus();
		m_focusedTableView = 0;
//		setFocus();
//		invalidateActions();
	}
	if (m_selectedConnection) {
		m_selectedConnection->setSelected(false);
		updateContents(m_selectedConnection->connectionRect());
		m_selectedConnection = 0;
//		invalidateActions();
	}
}

/*
void
KexiRelationView::keyPressEvent(QKeyEvent *ev)
{
	kdDebug() << "KexiRelationView::keyPressEvent()" << endl;

	if (ev->key()==KGlobalSettings::contextMenuKey()) {
//		m_popup->exec( mapToGlobal( m_focusedTableView ? m_focusedTableView->pos() + m_focusedTableView->rect().center() : rect().center() ) );
		executePopup();
	}

	if(ev->key() == Key_Delete)
		removeSelectedConnection();

}*/

void
KexiRelationView::recalculateSize(int width, int height)
{
	kdDebug() << "recalculateSize(" << width << ", " << height << ")" << endl;
	int newW = contentsWidth(), newH = contentsHeight();
	kdDebug() << "contentsSize(" << newW << ", " << newH << ")" << endl;

	if(newW < width)
		newW = width;

	if(newH < height)
		newH = height;

	resizeContents(newW, newH);
}

/*! Resizes contents to size exactly enough to fit tableViews.
	Executed on every tableView's drop event. 
*/
void
KexiRelationView::stretchExpandSize()
{
	int max_x=-1, max_y=-1;
	QDictIterator<KexiRelationViewTableContainer> it(m_tables);
	for (;it.current(); ++it) {
		if (it.current()->right()>max_x)
			max_x = it.current()->right();
		if (it.current()->bottom()>max_y)
			max_y = it.current()->bottom();
	}
	QPoint p = viewportToContents(QPoint(max_x, max_y) + QPoint(3,3)); //3 pixels margin
	resizeContents(p.x(), p.y());
}

void KexiRelationView::slotTableViewEndDrag()
{
	kdDebug() << "END DRAG!" <<endl;
	stretchExpandSize();

}

void
KexiRelationView::removeSelectedObject()
{
	if (m_selectedConnection) {
		removeConnection(m_selectedConnection);

#if 0
	RelationList l = m_relation->projectRelations();
	RelationList nl;
	for(RelationList::Iterator it = l.begin(); it != l.end(); ++it)
	{
		if((*it).srcTable == m_selectedConnection->connection().srcTable
			&& (*it).rcvTable == m_selectedConnection->connection().rcvTable
			&& (*it).srcField == m_selectedConnection->connection().srcField
			&& (*it).rcvField == m_selectedConnection->connection().rcvField)
		{
			kdDebug() << "KexiRelationView::removeSelectedConnection(): matching found!" << endl;
//			l.remove(it);
		}
		else
		{
			nl.append(*it);
		}
	}

	kdDebug() << "KexiRelationView::removeSelectedConnection(): d2" << endl;
	m_relation->updateRelationList(this, nl);
	kdDebug() << "KexiRelationView::removeSelectedConnection(): d3" << endl;
#endif
		delete m_selectedConnection;
		m_selectedConnection = 0;
//		invalidateActions();
	}
	else if (m_focusedTableView) {
		KexiRelationViewTableContainer *tmp = m_focusedTableView;
		m_focusedTableView = 0;
		hideTable(tmp);
	}
}

void
KexiRelationView::hideTable(KexiRelationViewTableContainer* tableView)
{
	KexiDB::TableSchema *ts = tableView->table();
	//for all connections: find and remove all connected with this table
	QPtrListIterator<KexiRelationViewConnection> it(m_connectionViews);
	for (;it.current();) {
		if (it.current()->masterTable() == tableView 
			|| it.current()->detailsTable() == tableView)
		{
			//remove this
			removeConnection(it.current());
		}
		else {
			++it;
		}
	}
	m_tables.take(tableView->table()->name());
	delete tableView;
	emit tableHidden( *ts );
}

void
KexiRelationView::removeConnection(KexiRelationViewConnection *conn)
{
	m_connectionViews.remove(conn);
	updateContents(conn->connectionRect());
	kdDebug() << "KexiRelationView::removeConnection()" << endl;
}

void KexiRelationView::slotTableViewGotFocus()
{
	if (m_focusedTableView == sender())
		return;
	kdDebug() << "GOT FOCUS!" <<endl;
	clearSelection();
//	if (m_focusedTableView)
//		m_focusedTableView->unsetFocus();
	m_focusedTableView = (KexiRelationViewTableContainer*)sender();
//	invalidateActions();
	emit tableViewGotFocus();
}

QSize KexiRelationView::sizeHint() const
{
	return QSize(QScrollView::sizeHint());//.width(), 600);
}


/*

void KexiRelationView::tableHeaderContextMenuRequest(const QPoint& pos)
{
	if (m_focusedTableView != sender())
		return;
	kdDebug() << "HEADER CTXT MENU!" <<endl;
	invalidateActions();
	m_tableQueryPopup->exec(pos);	
}

//! Invalidates all actions availability
void KexiRelationView::invalidateActions()
{
	setAvailable("edit_delete", m_selectedConnection || m_focusedTableView);
}

void KexiRelationView::executePopup( QPoint pos )
{
	if (pos==QPoint(-1,-1)) {
		pos = mapToGlobal( m_focusedTableView ? m_focusedTableView->pos() + m_focusedTableView->rect().center() : rect().center() );
	}
	if (m_focusedTableView)
		m_tableQueryPopup->exec(pos);
	else if (m_selectedConnection)
		m_connectionPopup->exec(pos);
}
*/

#include "kexirelationview.moc"
