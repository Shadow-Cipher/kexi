/* This file is part of the KDE project
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

#include "KexiProjectSelector.h"

#include <kexidb/drivermanager.h>
#include <kexidb/connectiondata.h>
#include "core/kexi.h"

#include <kapplication.h>
#include <kiconloader.h>
#include <kmimetype.h>
#include <klocale.h>
#include <kdebug.h>

#include <qlabel.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qlistview.h>

#include <assert.h>

class KexiProjectSelectorWidgetPrivate
{
public:
	KexiProjectSelectorWidgetPrivate()
	{
		selectable = true;
	}
	QPixmap fileicon, dbicon;
	bool showProjectNameColumn : 1;
	bool showConnectionColumns : 1;
	bool selectable : 1;
};

/*================================================================*/

//! helper class
class ProjectDataLVItem : public QListViewItem
{
public:
	ProjectDataLVItem(KexiProjectData *d, 
		const KexiDB::Driver::Info& info, KexiProjectSelectorWidget *selector )
		: QListViewItem(selector->list)
		, data(d)
	{
		int colnum = 0;
		const KexiDB::ConnectionData *cdata = data->constConnectionData();
		if (selector->d->showProjectNameColumn)
			setText(colnum++, data->caption()+"  ");
		
		setText(colnum++, data->databaseName()+"  ");
		
		if (selector->d->showConnectionColumns) {
			QString drvname = info.caption.isEmpty() ? cdata->driverName : info.caption;
			if (info.fileBased) {
				setText(colnum++, i18n("File") + " ("+drvname+")  " );
			} else {
				setText(colnum++, drvname+"  " );
			}
		
			QString conn;
			if (!cdata->connName.isEmpty())
				conn = cdata->connName + ": ";
			conn += cdata->serverInfoString();
			setText(3, conn + "  ");
		}
	}
	~ProjectDataLVItem() {}
	
	KexiProjectData *data;
};

/*================================================================*/

/*!
 *  Constructs a KexiProjectSelector which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 */
KexiProjectSelectorWidget::KexiProjectSelectorWidget( 
	QWidget* parent, const char* name, 
	KexiProjectSet* prj_set, bool showProjectNameColumn,
	bool showConnectionColumns )
    : KexiProjectSelectorBase( parent, name )
	,m_prj_set(prj_set)
	,d(new KexiProjectSelectorWidgetPrivate())
{
	d->showProjectNameColumn = showProjectNameColumn;
	d->showConnectionColumns = showConnectionColumns;
	QString none, iconname = KMimeType::mimeType( KexiDB::Driver::defaultFileBasedDriverMimeType() )->icon(none,0);
	d->fileicon = KGlobal::iconLoader()->loadIcon( iconname, KIcon::Desktop );
	setIcon( d->fileicon );
	d->dbicon = SmallIcon("database");
//	list->setHScrollBarMode( QScrollView::AlwaysOn );

	if (!d->showConnectionColumns) {
		list->removeColumn(2);
		list->removeColumn(2);
	}
	if (!d->showProjectNameColumn) {
		list->removeColumn(0);
	}
	setFocusProxy(list);

	//show projects
	setProjectSet( m_prj_set );
	connect(list,SIGNAL(doubleClicked(QListViewItem*)),this,SLOT(itemExecuted(QListViewItem*)));
	connect(list,SIGNAL(returnPressed(QListViewItem*)),this,SLOT(itemExecuted(QListViewItem*)));
}

/*!  
 *  Destroys the object and frees any allocated resources
 */
KexiProjectSelectorWidget::~KexiProjectSelectorWidget()
{
	delete d;
}

KexiProjectData* KexiProjectSelectorWidget::selectedProjectData() const
{
	ProjectDataLVItem *item = static_cast<ProjectDataLVItem*>(list->selectedItem());
	if (item)
		return item->data;
	return 0;
}

void KexiProjectSelectorWidget::itemExecuted(QListViewItem *item)
{
	if (!d->selectable)
		return;
	ProjectDataLVItem *ditem = static_cast<ProjectDataLVItem*>(item);
	if (ditem)
		emit projectExecuted( ditem->data );
}

void KexiProjectSelectorWidget::setProjectSet( KexiProjectSet* prj_set )
{
	if (prj_set) {
		//old list
		list->clear();
	}
	m_prj_set = prj_set; 
	if (!m_prj_set)
		return;
//TODO: what with project set's ownership?
	if (m_prj_set->error()) {
		kdDebug() << "KexiProjectSelectorWidget::setProjectSet() : m_prj_set->error() !"<<endl;
		return;
	}
	KexiDB::DriverManager manager;
	KexiProjectData::List prjlist = m_prj_set->list();
	KexiProjectData *data = prjlist.first();
	while (data) {
		KexiDB::Driver::Info info = manager.driverInfo(data->constConnectionData()->driverName);
		if (!info.name.isEmpty()) {
			ProjectDataLVItem *item = new ProjectDataLVItem(data, info, this);
			if (!d->selectable)
				item->setSelectable(false);
			if (info.fileBased)
				item->setPixmap( 0, d->fileicon );
			else
				item->setPixmap( 0, d->dbicon );
		}
		else {
			kdWarning() << "KexiProjectSelector::KexiProjectSelector(): no driver found for '" << data->constConnectionData()->driverName << "'!" << endl;
		}
		data=prjlist.next();
	}
	if (list->firstChild()) {
		list->setSelected(list->firstChild(),true);
	}
}

void KexiProjectSelectorWidget::setSelectable(bool set)
{
	if (d->selectable == set)
		return;
	d->selectable = set;
	//update items' state
	QListViewItemIterator it( list );
	while ( it.current() ) {
		it.current()->setSelectable( d->selectable );
	}
}
	
bool KexiProjectSelectorWidget::isSelectable() const
{
	return d->selectable;
}

/*================================================================*/

KexiProjectSelectorDialog::KexiProjectSelectorDialog( QWidget *parent, const char *name,
	KexiProjectSet* prj_set, bool showProjectNameColumn, bool showConnectionColumns)
	: KDialogBase( Plain, i18n("Open Recent Project"), Help | Ok | Cancel, Ok, parent, name )
{
	init(prj_set, showProjectNameColumn, showConnectionColumns);
}

KexiProjectSelectorDialog::KexiProjectSelectorDialog( QWidget *parent, const char *name,
	KexiDB::ConnectionData* cdata, 
	bool showProjectNameColumn, bool showConnectionColumns)
	: KDialogBase( Plain, i18n("Open Project"), Help | Ok | Cancel, Ok, parent, name )
{
	assert(cdata);
	if (!cdata)
		return;
	KexiProjectSet *prj_set = new KexiProjectSet( *cdata );
	init(prj_set, showProjectNameColumn, showConnectionColumns);
	
	m_sel->label->setText( i18n("Select one of these existing projects on server <b>%1</b>:")
		.arg(cdata->serverInfoString(false)) );
}

KexiProjectSelectorDialog::~KexiProjectSelectorDialog()
{
}

void KexiProjectSelectorDialog::init(KexiProjectSet* prj_set, bool showProjectNameColumn, bool showConnectionColumns)
{
	setSizeGripEnabled(true);
	
	QVBoxLayout *lyr = new QVBoxLayout(plainPage(), 0, KDialogBase::spacingHint(), "lyr");
	m_sel = new KexiProjectSelectorWidget(plainPage(), "sel", prj_set, showProjectNameColumn, showConnectionColumns);
	lyr->addWidget(m_sel);
	setIcon(*m_sel->icon());
	m_sel->setFocus();
	
	connect(m_sel,SIGNAL(projectExecuted(KexiProjectData*)),this,SLOT(slotProjectExecuted(KexiProjectData*)));
	KDialog::centerOnScreen(this);
}

KexiProjectData* KexiProjectSelectorDialog::selectedProjectData() const
{
	return m_sel->selectedProjectData();
}

void KexiProjectSelectorDialog::slotProjectExecuted(KexiProjectData*)
{
	accept();
}
 

#include "KexiProjectSelector.moc"

