/* This file is part of the KDE project
   Copyright (C) 2004,2006 Jaroslaw Staniek <js@iidea.pl>

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
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "kexitimetableedit.h"

#include <qapplication.h>
#include <qpainter.h>
#include <qvariant.h>
#include <qrect.h>
#include <qpalette.h>
#include <qcolor.h>
#include <qfontmetrics.h>
#include <qdatetime.h>
#include <qcursor.h>
#include <qpoint.h>
#include <qlayout.h>
#include <qtoolbutton.h>
#include <q3datetimeedit.h>

#include <kdebug.h>
#include <klocale.h>
#include <kglobal.h>
#include <kdatepicker.h>
#include <kdatetable.h>
#include <klineedit.h>
#include <kmenu.h>
#include <kdatewidget.h>

#include <kexiutils/utils.h>

KexiTimeTableEdit::KexiTimeTableEdit(KexiTableViewColumn &column, Q3ScrollView *parent)
 : KexiInputTableEdit(column, parent)
{
	setName("KexiTimeTableEdit");

//! @todo add QValidator so time like "99:88:77" cannot be even entered

	m_lineedit->setInputMask( m_formatter.inputMask() );
}

KexiTimeTableEdit::~KexiTimeTableEdit()
{
}

void KexiTimeTableEdit::setValueInternal(const QVariant& add_, bool removeOld)
{
	if (removeOld) {
		//new time entering... just fill the line edit
//! @todo cut string if too long..
		QString add(add_.toString());
		m_lineedit->setText(add);
		m_lineedit->setCursorPosition(add.length());
		return;
	}
	m_lineedit->setText( 
		m_formatter.timeToString( 
			//hack to avoid converting null variant to valid QTime(0,0,0)
			m_origValue.isValid() ? m_origValue.toTime() : QTime(99,0,0) 
		)
	);
	m_lineedit->setCursorPosition(0); //ok?
}

void KexiTimeTableEdit::setupContents( QPainter *p, bool focused, const QVariant& val,  
	QString &txt, int &align, int &x, int &y_offset, int &w, int &h )
{
	Q_UNUSED(p);
	Q_UNUSED(focused);
	Q_UNUSED(x);
	Q_UNUSED(w);
	Q_UNUSED(h);
#ifdef Q_WS_WIN
	y_offset = -1;
#else
	y_offset = 0;
#endif
	if (!val.isNull() && val.canCast(QVariant::Time))
		txt = m_formatter.timeToString(val.toTime());
	align |= Qt::AlignLeft;
}

bool KexiTimeTableEdit::valueIsNull()
{
	if (m_formatter.isEmpty( m_lineedit->text() )) //empty time is null
		return true;
	return !timeValue().isValid();
}

bool KexiTimeTableEdit::valueIsEmpty()
{
	return valueIsNull();// OK? TODO (nonsense?)
}

QTime KexiTimeTableEdit::timeValue()
{
	return m_formatter.stringToTime( m_lineedit->text() );
}

QVariant KexiTimeTableEdit::value()
{
	return m_formatter.stringToVariant( m_lineedit->text() );
}

bool KexiTimeTableEdit::valueIsValid()
{
	if (m_formatter.isEmpty( m_lineedit->text() )) //empty time is valid
		return true;
	return m_formatter.stringToTime( m_lineedit->text() ).isValid();
}

KEXI_CELLEDITOR_FACTORY_ITEM_IMPL(KexiTimeEditorFactoryItem, KexiTimeTableEdit)

#include "kexitimetableedit.moc"
