/* This file is part of the KDE project
   Copyright (C) 2002   Lucijan Busch <lucijan@gmx.at>
   Copyright (C) 2002   Joseph Wenninger <jowenn@kde.org>

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

#ifndef KEXIQUERYPARTPROXY_H
#define KEXIQUERYPARTPROXY_H

class QPixmap;

#include "kexiquerypart.h"
#include "kexiprojecthandlerproxy.h"

class KexiView;
class KexiPartPopupMenu;

class KEXI_HAND_QUERY_EXPORT KexiQueryPartProxy : public KexiProjectHandlerProxy, public KXMLGUIClient
{
	Q_OBJECT

	public:
		KexiQueryPartProxy(KexiQueryPart *part, KexiView*view);

//                virtual KexiPartPopupMenu       *groupContext();
//                virtual KexiPartPopupMenu       *itemContext(const QString& identifier);



	protected slots:
		virtual bool executeItem(const QString& identifier);
		void				slotCreateQuery();
		bool				slotOpen(const QString& identifier);
		void				slotEdit(const QString &identifier);
		void				slotDelete(const QString& identifier);

	private:
		KexiQueryPart *m_queryPart;
};

#endif
