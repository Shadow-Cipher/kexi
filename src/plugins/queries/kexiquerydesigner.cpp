/* This file is part of the KDE project
   Copyright (C) 2003   Lucijan Busch <lucijan@gmx.at>
             (C) 2003 by Joseph Wenninger <jowenn@kde.org>

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

#include <qtabwidget.h>
#include <qlayout.h>
#include <qstatusbar.h>
#include <qregexp.h>

#include <klocale.h>
#include <kaction.h>
#include <kiconloader.h>
#include <kdebug.h>
#include <kxmlguiclient.h>
#include <klineeditdlg.h>
#include <klistview.h>

#include <koStore.h>

#ifndef KEXI_NO_PRINT
# include <kprinter.h>
#endif

#include <kexiDB/kexidberror.h>

#include "kexiquerypartitem.h"
#include "kexidatatable.h"
#include "kexiquerydesignerguieditor.h"
#include "kexiquerydesignersql.h"
#include "kexiquerydesigner.h"
#include "kexiparameterlisteditor.h"
#include "kexidataprovider.h"

KexiQueryDesigner::KexiQueryDesigner(KexiView *view, KexiQueryPartItem *item, 
	QWidget *parent, bool modeview)
	: KexiDialogBase(view, item, parent)
	, m_queryPartItem(item)
{
//	QVBoxLayout *l = new QVBoxLayout(this);
//	setCaption(i18n("%1 - Query").arg(item->title()));

	m_tab = new QTabWidget(this);
	m_tab->setTabPosition(QTabWidget::Bottom);
	gridLayout()->addWidget(m_tab, 0, 0);

	m_editor = new KexiQueryDesignerGuiEditor(view, this, this, item, "design");
	connect(m_editor, SIGNAL(contextHelp(const QString &, const QString &)), this,
	 SLOT(slotContextHelp(const QString &, const QString &)));
	m_sql = new KexiQueryDesignerSQL(this);
	m_queryView = new KexiDataTable(m_view, "Query View", "query_view", this, true);
//	m_view = new KexiDataTable(view, this, "sql", 0, true);

	m_tab->insertTab(m_editor, SmallIcon("state_edit"), i18n("Composer"));
	m_tab->insertTab(m_sql, SmallIcon("state_sql"), i18n("SQL"));
	m_tab->insertTab(m_queryView, SmallIcon("table"), i18n("View"));

	m_currentView = 0;

	QStatusBar *status = new QStatusBar(this);
	gridLayout()->addWidget(status, 1, 0);

//	registerAs(DocumentWindow, item->fullIdentifier());
	setContextHelp(i18n("Queries"), i18n("After having set up relations you can drag fields from different tables into the \"query table\"."));

	item->setClient(this);
	m_parameters = item->parameters();
	m_editor->setPrameters(m_parameters);

	if(modeview)
	{
		m_statement = item->sql();
		m_tab->setCurrentPage(2);

		query();
	}

	connect(m_tab, SIGNAL(currentChanged(QWidget *)), this, SLOT(viewChanged(QWidget *)));
	connect(this, SIGNAL(closing(KexiDialogBase *)), this, SLOT(slotClosing(KexiDialogBase *)));
}

void
KexiQueryDesigner::query()
{
	m_queryPartItem->setSQL(m_statement);
	m_queryPartItem->setParameters(m_parameters);
	KexiDBRecordSet *rec = m_queryPartItem->records(this);
	if (rec) {
		m_queryView->setDataSet(rec);
		emit queryExecuted(m_statement,true);
	}
	else
		emit queryExecuted(m_statement,false);
}

void
KexiQueryDesigner::viewChanged(QWidget *w)
{
	int view = m_tab->indexOf(w);

	if(view == 0)
	{
		m_currentView = 0;
	}
	else if(view == 1)
	{
		m_currentView = 1;
	}
	else
	{
		if(m_currentView == 0)
		{
			m_statement = m_editor->getQuery();
			m_editor->getParameters(m_parameters);
			query();
		}
		else
		{
			m_statement = m_sql->getQuery();
			query();
		}
	}
}

void
KexiQueryDesigner::slotContextHelp(const QString &t, const QString &m)
{
	setContextHelp(t, m);
}

#ifndef KEXI_NO_PRINT
void
KexiQueryDesigner::print(KPrinter &p)
{
	m_queryView->print(p);
}
#endif

void
KexiQueryDesigner::saveBack()
{
	kdDebug() << "KexiQueryDesigner::saveBack() e=" << m_editor << endl;

	if(m_currentView == 0)
	{
		m_queryPartItem->setSQL(m_editor->getQuery());
		m_editor->getParameters(m_parameters);
		m_queryPartItem->setParameters(m_parameters);
	}
	else if(m_currentView == 1)
	{
		m_queryPartItem->setSQL(m_sql->getQuery());
	}
}

void
KexiQueryDesigner::slotClosing(KexiDialogBase *)
{
	kdDebug() << "KexiQueryDesigner::slotClosing()" << endl;
	saveBack();
	m_queryPartItem->setClient(0);
}

KexiQueryDesigner::~KexiQueryDesigner()
{
}

#include "kexiquerydesigner.moc"
