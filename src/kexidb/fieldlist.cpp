/* This file is part of the KDE project
   Copyright (C) 2003 Jaroslaw Staniek <js@iidea.pl>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#include <kexidb/fieldlist.h>
#include <kexidb/object.h>

#include <kdebug.h>

#include <assert.h>

using namespace KexiDB;

FieldList::FieldList(bool owner)
{
	m_fields.setAutoDelete( owner );
	m_fields_by_name.setAutoDelete( false );
	//reasonable sizes: TODO
	m_fields_by_name.resize(101);
}

FieldList::~FieldList()
{
}

void FieldList::clear()
{
//	m_name = QString::null;
	m_fields.clear();
	m_fields_by_name.clear();
	m_sqlFields = QString::null;
}

FieldList& FieldList::addField(KexiDB::Field *field)
{
//	field.setTable(m_name);
	assert(field);
	if (!field)
		return *this;
	m_fields.append(field);
	m_fields_by_name.insert(field->name(),field);
	m_sqlFields = QString::null;
	return *this;
}

bool FieldList::isOwner() const
{
	return m_fields.autoDelete();
}

KexiDB::Field* FieldList::field(unsigned int id)
{
	if (id < m_fields.count())
		return m_fields.at(id);
	return 0;
}

Field* FieldList::field(const QString& name) const
{
	return m_fields_by_name[name];
}

unsigned int FieldList::fieldCount() const
{
	return m_fields.count();
}

void FieldList::debug() const
{
	QString dbg;
	Field::ListIterator it( m_fields );
	Field *field;
	bool start = true;
	for (; (field = it.current())!=0; ++it) {
		if (!start)
			dbg += ",\n";
		else
			start = false;
		dbg += "  ";
		dbg += field->debugString();
	}
	KexiDBDbg << dbg << endl;
}


#define _ADD_FIELD(fname) \
{ \
	if (fname.isEmpty()) return fl; \
	f = m_fields_by_name[fname]; \
	if (!f) { delete fl; return 0; } \
	fl->addField(f); \
}

FieldList* FieldList::subList(const QString& n1, const QString& n2, 
	const QString& n3, const QString& n4,
	const QString& n5, const QString& n6,
	const QString& n7, const QString& n8,
	const QString& n9, const QString& n10,
	const QString& n11, const QString& n12,
	const QString& n13, const QString& n14,
	const QString& n15, const QString& n16,
	const QString& n17, const QString& n18)
{
	if (n1.isEmpty())
		return 0;
	Field *f;
	FieldList *fl = new FieldList(false);
	_ADD_FIELD(n1);
	_ADD_FIELD(n2);
	_ADD_FIELD(n3);
	_ADD_FIELD(n4);
	_ADD_FIELD(n5);
	_ADD_FIELD(n6);
	_ADD_FIELD(n7);
	_ADD_FIELD(n8);
	_ADD_FIELD(n9);
	_ADD_FIELD(n10);
	_ADD_FIELD(n11);
	_ADD_FIELD(n12);
	_ADD_FIELD(n13);
	_ADD_FIELD(n14);
	_ADD_FIELD(n15);
	_ADD_FIELD(n16);
	_ADD_FIELD(n17);
	_ADD_FIELD(n18);
	return fl;
}

QStringList FieldList::names() const
{
	QStringList r;
	for (QDictIterator<Field> it(m_fields_by_name);it.current();++it) {
		r += it.currentKey();
	}
	return r;
}

QString FieldList::sqlFieldsList()
{
	if (!m_sqlFields.isEmpty())
		return m_sqlFields;

	Field::ListIterator it( m_fields );
	bool start = true;
	for (; it.current(); ++it) {
		if (!start)
			m_sqlFields += ",";
		else
			start = false;
		m_sqlFields += it.current()->name();
	}
	return m_sqlFields;
}

