/* This file is part of the KDE project
   Copyright (C) 2002   Peter Simonsson <psn@linux.se>

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

#ifndef _KEXIALTERTABLE_H_
#define _KEXIALTERTABLE_H_

#include "kexidialogbase.h"
#include <kexidb/connection.h>
#include <kexidb/table.h>

class KexiTableView;
class KexiTableItem;
class PropertyEditor;
class PropertyEditorItem;
class KexiProjectHandlerItem;

class KEXI_HAND_TBL_EXPORT KexiAlterTable : public KexiDialogBase
{
	Q_OBJECT
	public:
		KexiAlterTable(KexiView *view, KexiProjectHandlerItem *item, QWidget *parent = 0, bool create=false);


		virtual KXMLGUIClient *guiClient(){return new KXMLGUIClient();}

	public slots:
		void setFocus();

	protected:
		void init();
		void initView();
		void getFields();
		virtual void closeEvent(QCloseEvent *ev);

	protected slots:
		void changeShownField(KexiTableItem* i);
		void tableItemChanged(KexiTableItem *i, int col);
		void changeTable();
		void propertyChanged();
	
		void tmp_filedTableCurrentItemRemoveRequest(); //TMP

	private:
//		QString m_table;
		KexiTableView* m_fieldTable;
		PropertyEditor* m_propList;
		KexiDB::Table m_table;
		bool m_create;
		PropertyEditorItem* m_nameItem;
		PropertyEditorItem* m_datatypeItem;
		PropertyEditorItem* m_lengthItem;
		PropertyEditorItem* m_requiredItem;
		PropertyEditorItem* m_defaultItem;
		PropertyEditorItem* m_unsignedItem;
		PropertyEditorItem* m_precisionItem;
		PropertyEditorItem* m_autoIncItem;
		PropertyEditorItem* m_primaryItem;
};

#endif
