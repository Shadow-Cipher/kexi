/* This file is part of the KDE project
   Copyright (C) 2002   Lucijan Busch <lucijan@gmx.at>

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

#include "kexiformhandlerproxy.h"
#include "kexiformhandler.h"
#include "kexiformhandleritem.h"
#include "kexiprojecthandleritem.h"
#include "kexiformbase.h"
#include "kexidatasourcedlg.h"

#include <qpixmap.h>

#include <kdebug.h>
#include <klocale.h>
#include <klineeditdlg.h>
#include <kaction.h>


KexiFormHandlerProxy::KexiFormHandlerProxy(KexiFormHandler *handler, KexiView *view)
 : KexiProjectHandlerProxy(handler, view), KXMLGUIClient()
{
    (void) new KAction(i18n("Create &Form..."), 0,
                       this,SLOT(slotCreate()), actionCollection(), "formpart_create");

    setXMLFile("kexiformpartui.rc");

    view->insertChildClient(this);

	m_formHandler = handler;
}

KexiPartPopupMenu *
KexiFormHandlerProxy::groupContext()
{
	kdDebug() << "KexiFormHandlerProxy::groupContext()" << endl;
	KexiPartPopupMenu *m = new KexiPartPopupMenu(this);
	m->insertAction(i18n("Create Form..."), SLOT(slotCreate()));

	return m;
}

KexiPartPopupMenu *
KexiFormHandlerProxy::itemContext(const QString &identifier)
{
	KexiPartPopupMenu *m = new KexiPartPopupMenu(this);
	m->insertAction(i18n("Open Form"), SLOT(slotOpen(const QString &)));
	m->insertAction(i18n("Design Form"), SLOT(slotAlter(const QString &)));
	m->insertAction(i18n("Delete Form"), SLOT(slotDelete(const QString &)));
	m->insertSeparator();
	m->insertAction(i18n("Create Form..."), SLOT(slotCreate()));

	return m;
}

void
KexiFormHandlerProxy::slotCreate()
{
	bool ok = false;
//	QString name = KLineEditDlg::getText(i18n("New Form"), i18n("Form name:"), "", &ok, 0);
	KexiDataSourceDlg *d = new KexiDataSourceDlg(m_view->project(), kexiView());
	if(d->exec() == QDialog::Accepted)
	{
		kdDebug() << "KexiFormHandlerProxy::slotCreate(): source: " << d->source() << endl;
		QString name = d->name();
		KexiFormHandlerItem *i = new KexiFormHandlerItem(part(), name, name + ".ui");
		i->setSource(d->source());
		part()->items()->insert("kexi/form/" + name + ".ui", i);
        emit m_formHandler->itemListChanged(part());
		KexiFormBase *nform = new KexiFormBase(kexiView(), i, 0, false, d->source(), "nform", name);
		nform->show();
		kexiView()->project()->addFileReference(FileReference("Forms",name,"/form/" + name + ".ui"));
	}
}

void
KexiFormHandlerProxy::slotOpen(const QString &identifier)
{
	if(kexiView()->activateWindow(identifier))
		return;


	KexiFormHandlerItem *i = static_cast<KexiFormHandlerItem *>(part()->items()->find(identifier));
	kdDebug() << "KexiFormHandlerProxy::slotOpen() i: " << identifier << " " << i << endl;
	if(i)
	{
		KexiFormBase *nform = new KexiFormBase(kexiView(), i, 0, true, "aa", "nform", i->name());
		nform->show();
	}
}

void
KexiFormHandlerProxy::slotAlter(const QString &identifier)
{
}

void
KexiFormHandlerProxy::slotDelete(const QString &identifier)
{
}

void
KexiFormHandlerProxy::executeItem(const QString &identifier)
{
	slotOpen(identifier);
}

#include "kexiformhandlerproxy.moc"
