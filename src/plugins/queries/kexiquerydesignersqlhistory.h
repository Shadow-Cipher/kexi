/* This file is part of the KDE project
   Copyright (C) 2003   Lucijan Busch <lucijan@gmx.at>

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

#ifndef KEXIQUERYDESIGNERSQLHISTORY_H
#define KEXIQUERYDESIGNERSQLHISTORY_H

#include <qscrollview.h>
#include <qdatetime.h>
#include <qptrlist.h>
#include <qmap.h>
#include <qsimplerichtext.h>

class QSimpleRichText;

class KEXI_HAND_QUERY_EXPORT HistoryEntry
{
	public:
		HistoryEntry(bool success, const QTime &time, const QString &statement, int y);
		~HistoryEntry();

		QRect	geometry(int y, int width, QFontMetrics f);
		void	drawItem(QPainter *p, int width, const QColorGroup &cg);

		void	setSelected(bool selected) { m_selected = selected; }
		bool	isSelected() { return m_selected; }
		void	highlight();

		QString	statement() { return m_statement; }
		void updateTime(const QTime &execTime);

	private:
		bool	m_succeed;
		QTime	m_execTime;
		QString	m_statement;
		QSimpleRichText	*m_formated;

		int	m_y;
		bool	m_selected;
};

typedef QPtrList<HistoryEntry> History;

class KEXI_HAND_QUERY_EXPORT KexiQueryDesignerSQLHistory : public QScrollView
{
	Q_OBJECT

	public:
		KexiQueryDesignerSQLHistory(QWidget *parent, const char *name=0);
		~KexiQueryDesignerSQLHistory();

		void		addEntry(HistoryEntry *e);
		void		contextMenu(const QPoint &pos, HistoryEntry *e);

	public slots:
		void		addEvent(QString q, bool s);

		void		slotToClipboard();

//		HistoryItem	itemAt(int y);

	protected:
		void	drawContents(QPainter *p, int cx, int cy, int cw, int ch);
		void	contentsMousePressEvent(QMouseEvent * e);

	private:
		History		m_history;
		HistoryEntry	*m_selected;
};

#endif
