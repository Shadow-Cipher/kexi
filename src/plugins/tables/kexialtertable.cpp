/* This file is part of the KDE project
   Copyright (C) 2002   Peter Simonsson <psn@linux.se>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
 */

#include "kexialtertable.h"
#include "kexitableview.h"
#include "kexiDB/kexidbfield.h"
#include "formeditor/propertyeditor.h"
#include "formeditor/propertyeditoritem.h"
#include "kexiproject.h"

#include <qlayout.h>
#include <qlabel.h>
#include <qstringlist.h>

#include <klocale.h>
#include <kdebug.h>
#include <kdialog.h>
#include <klistview.h>

KexiAlterTable::KexiAlterTable(KexiView *view, QWidget *parent,
	const QString &table, bool create, const char *name)
	: KexiDialogBase(view,parent, name)
{
	m_table = table;
	m_create = create;
	initView();
	getFields();
	setCaption(i18n("%1 - Table Editor").arg(m_table));
	registerAs(DocumentWindow);
}

void
KexiAlterTable::initView()
{
	kdDebug() << "KexiAlterTable: Init gui..." << endl;
	QLabel* tableLbl = new QLabel(i18n("Table: %1").arg(m_table), this);
	QFrame* lineFrm = new QFrame(this);
	lineFrm->setFrameStyle(QFrame::HLine | QFrame::Sunken);

	m_fieldTable = new KexiTableView(this);
	m_fieldTable->m_editOnDubleClick = true;
	m_fieldTable->setRecordIndicator(true);
	m_fieldTable->addColumn(i18n("Field Name"), QVariant::String, true);
	QStringList strings;

	for(int i = 1; i < KexiDBField::SQLLastType; i++)
	{
		strings.append(KexiDBField::typeName(static_cast<KexiDBField::ColumnType>(i)));
	}

	m_fieldTable->addColumn(i18n("Datatype"), QVariant::StringList, true, QVariant(strings));
	m_fieldTable->addColumn(i18n("Primary Key"), QVariant::Bool, true);

	m_propList = new PropertyEditor(this, true);
	m_propList->setFullWidth(true);
	m_nameItem = new PropertyEditorItem(m_propList, i18n("Field Name"), QVariant::String, "");
	m_datatypeItem = new PropertyEditorItem(m_propList, i18n("Datatype"), QVariant::StringList, KexiDBField::SQLVarchar - 1, strings);
	m_lengthItem = new PropertyEditorItem(m_propList, i18n("Field Length"), QVariant::Int, 50);
	m_requiredItem = new PropertyEditorItem(m_propList, i18n("Required Field"), QVariant::Bool, QVariant(false, 1));
	m_defaultItem = new PropertyEditorItem(m_propList, i18n("Default Value"), QVariant::String, "");
	m_unsignedItem = new PropertyEditorItem(m_propList, i18n("Unsigned Value"), QVariant::Bool, QVariant(false, 1));
	m_precisionItem = new PropertyEditorItem(m_propList, i18n("Precision"), QVariant::Int, 0);
	m_autoIncItem = new PropertyEditorItem(m_propList, i18n("Auto Increment"), QVariant::Bool, QVariant(false, 1));
	m_primaryItem = new PropertyEditorItem(m_propList, i18n("Primary Key"), QVariant::Bool, QVariant(false, 1));

	QGridLayout* l = new QGridLayout(this);
	l->setSpacing(KDialog::spacingHint());
	l->setMargin(KDialog::marginHint());
	l->addWidget(tableLbl, 0, 0);
	l->addWidget(lineFrm, 1, 0);
	l->addWidget(m_fieldTable, 2, 0);
	l->addWidget(m_propList, 3, 0);

	connect(m_fieldTable, SIGNAL(itemSelected(KexiTableItem*)), SLOT(changeShownField(KexiTableItem*)));
	connect(m_fieldTable, SIGNAL(itemChanged(KexiTableItem *, int)), SLOT(tableItemChanged(KexiTableItem *, int)));
	connect(m_propList, SIGNAL(itemRenamed(QListViewItem*)), SLOT(propertyChanged()));
	kdDebug() << "Ready." << endl;
}

void
KexiAlterTable::getFields()
{
	int fc = 0;
	if(!m_create)
	{
		m_tableFields = kexiProject()->db()->getStructure(m_table);

		for(KexiDBField* field = m_tableFields.first(); field; field = m_tableFields.next())
		{
			KexiTableItem *it = new KexiTableItem(m_fieldTable);
			it->setValue(0, field->name());
			it->setValue(1, field->sqlType() - 1);
			it->setValue(2, field->primary_key());
			it->setHint(QVariant(fc++));
		}
	}
	else
	{
		m_tableFields.clear();
	}

	// Insert item
	KexiTableItem *insert = new KexiTableItem(m_fieldTable);
	insert->setValue(1, KexiDBField::SQLVarchar - 1);
	insert->setValue(2, false);
	insert->setHint(QVariant(fc));
	insert->setInsertItem(true);

	m_fieldTable->setCursor(0, 0);
	changeShownField(m_fieldTable->selectedItem());
}

void
KexiAlterTable::changeShownField(KexiTableItem* i)
{
	KexiDBField* field = m_tableFields.first();
	bool found = false;
	m_propList->reset(true);

	if(!i->isInsertItem())
	{
		while(field && !found)
		{
			if(field->name() == i->getText(0))
			{
				found = true;
			}
			else
			{
				field = m_tableFields.next();
			}
		}

		if(!found)
		{
			return;
		}

		m_nameItem->setValue(field->name());
		m_datatypeItem->setValue(field->sqlType() - 1);
		m_lengthItem->setValue(field->length());
		m_requiredItem->setValue(QVariant(field->not_null(), 1));
		m_defaultItem->setValue(field->defaultValue());
		m_unsignedItem->setValue(QVariant(field->unsignedType(), 1));
		m_precisionItem->setValue(field->precision());
		m_autoIncItem->setValue(QVariant(field->auto_increment(), 1));
		m_primaryItem->setValue(QVariant(field->primary_key(), 1));
	}
	else
	{
		m_nameItem->setValue("");
		m_datatypeItem->setValue(KexiDBField::SQLVarchar - 1);
		m_lengthItem->setValue(50);
		m_requiredItem->setValue(QVariant(false, 1));
		m_defaultItem->setValue("");
		m_unsignedItem->setValue(QVariant(false, 1));
		m_precisionItem->setValue(0);
		m_autoIncItem->setValue(QVariant(false, 1));
		m_primaryItem->setValue(QVariant(false, 1));
	}
}

void
KexiAlterTable::tableItemChanged(KexiTableItem *i, int col)
{
	if(col == 0)
	{
		m_nameItem->setValue(i->getValue(col));
	}
	else if(col == 1)
	{
		m_datatypeItem->setValue(i->getValue(col));
	}
	else if(col == 2)
	{
		m_primaryItem->setValue(QVariant(i->getValue(col).toBool(), 1));
	}

	changeTable();
}

void
KexiAlterTable::changeTable()
{
	KexiDBField* field = new KexiDBField(m_table);
	field->setName(m_nameItem->value().toString());
	field->setColumnType(static_cast<KexiDBField::ColumnType>(m_datatypeItem->value().toInt() + 1));
	field->setLength(m_lengthItem->value().toInt());
	field->setNotNull(m_requiredItem->value().toBool() || m_primaryItem->value().toBool());
	field->setDefaultValue(m_defaultItem->value());
	field->setUnsigned(m_unsignedItem->value().toBool());
	field->setPrecision(m_precisionItem->value().toInt());
	field->setAutoIncrement(m_autoIncItem->value().toBool());
	field->setPrimaryKey(m_primaryItem->value().toBool());
	KexiTableItem* i = m_fieldTable->selectedItem();
	bool ok = false;

	if(i->isInsertItem())
	{
		if(!i->getValue(0).toString().isEmpty() && i->getValue(1).toInt() != 0)
		{
			kdDebug() << "Create new field!" << endl;
			ok = kexiProject()->db()->createField(*field, m_tableFields, m_create);
			m_create = !ok;

			if(ok)
			{
				kdDebug() << "New field created!" << endl;
				i->setInsertItem(false);
				m_tableFields.append(field);
				// Insert item
				KexiTableItem *insert = new KexiTableItem(m_fieldTable);
				insert->setValue(1, KexiDBField::SQLVarchar - 1);
				insert->setHint(QVariant(i->getHint().toInt() + 1));
				insert->setInsertItem(true);
			}
		}
	}
	else
	{
		int index = i->getHint().toInt();
		kdDebug() << "KexiAlterTable::changeTable(" << index <<
			")" << endl;
		ok = kexiProject()->db()->alterField(*field, index, m_tableFields);

		if(ok)
		{
			kdDebug() << "Field changed!" << endl;
			m_tableFields.replace(index, field);
			changeShownField(i);
		}
		else
		{
			i->setValue(0, field->name());
			i->setValue(1, field->sqlType() - 1);
			i->setValue(2, QVariant(field->primary_key(), 1));
			changeShownField(i);
		}
	}

	if(!ok)
	{
		delete field;
	}
}

void
KexiAlterTable::propertyChanged()
{
	m_fieldTable->selectedItem()->setValue(0, m_nameItem->value());
	m_fieldTable->selectedItem()->setValue(1, m_datatypeItem->value());
	m_fieldTable->selectedItem()->setValue(2, m_primaryItem->value());
	m_fieldTable->triggerUpdate();

	changeTable();
}

#include "kexialtertable.moc"
