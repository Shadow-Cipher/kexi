/* This file is part of the KDE project
   Copyright (C) 2004 Cedric Pasteur <cedric.pasteur@free.fr>

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

#ifndef KFORMDESIGNEREXTRAWIDGET_H
#define KFORMDESIGNEREXTRAWIDGET_H

#include <qintdict.h>
#include <qtoolbutton.h>
#include <qcheckbox.h>

#include <ktextedit.h>
#include <kdialogbase.h>

class QListView;
class QListViewItem;
class KListViewItem;
class KListView;
class KListBox;
class KToolBar;
class KTextEdit;
class KFontCombo;
class KColorCombo;
class KTabWidget;
class KexiPropertyBuffer;
class KexiPropertyEditor;
class KexiProperty;

namespace KFormDesigner {

class ObjectTreeView;
class Form;

class KFORMEDITOR_EXPORT RichTextDialog : public KDialogBase
{
	Q_OBJECT

	public:
		RichTextDialog(QWidget *parent, const QString &text);
		~RichTextDialog(){;}

		QString  text();

		enum VerticalAlignment{AlignNormal = QTextEdit::AlignNormal, AlignSuperScript = QTextEdit::AlignSuperScript, AlignSubScript = QTextEdit::AlignSubScript};

	public slots:
		void  changeFont(const QString &);
		void  changeColor(const QColor&);
		void  buttonToggled(int);
		void  cursorPositionChanged(int, int);
		void  slotVerticalAlignmentChanged(VerticalAlignment align);

	private:
		enum { TBFont = 100, TBColor, TBBold, TBItalic, TBUnder, TBSuper, TBSub, TBLeft = 201, TBCenter, TBRight, TBJustify };
		KToolBar  *m_toolbar;
		KTextEdit  *m_edit;
		KFontCombo  *m_fcombo;
		KColorCombo  *m_colCombo;
};

class KFORMEDITOR_EXPORT EditListViewDialog : public KDialogBase
{
	Q_OBJECT

	public:
		EditListViewDialog(QListView *listview, QWidget *parent);
		~EditListViewDialog(){;}

	public slots:
		// Columns page
		void updateItemProperties(QListBoxItem*);
		void newItem();
		void removeItem();
		void MoveItemUp();
		void MoveItemDown();
		void changeProperty(KexiPropertyBuffer&, KexiProperty&);

		// Contents page
		void updateButtons(QListViewItem*);
		void newRow();
		void newChildRow();
		void removeRow();
		void MoveRowUp();
		void MoveRowDown();

	protected:
		void loadChildNodes(QListView *listview, QListViewItem *item, QListViewItem *parent);

	protected:
		enum { BNewRow = 10, BNewChild, BRemRow, BRowUp, BRowDown , BColAdd = 20, BColRem, BColUp, BColDown };
		KexiPropertyEditor  *m_editor;
		KexiPropertyBuffer  *m_buffer;
		QFrame   *m_contents, *m_column;
		KListBox  *m_listbox;
		KListView  *m_listview;
		QIntDict<QToolButton>  m_buttons;
};

class KFORMEDITOR_EXPORT TabStopDialog : public KDialogBase
{
	Q_OBJECT

	public:
		TabStopDialog(QWidget *parent);
		virtual ~TabStopDialog() {;}

	public slots:
		int exec(Form *form);
		void MoveItemUp();
		void MoveItemDown();
		void updateButtons(QListViewItem*);
		void slotRadioClicked(bool isOn);

	protected:
		enum {BUp = 10, BDown};
		ObjectTreeView   *m_treeview;
		QIntDict<QToolButton>  m_buttons;
		QCheckBox *m_check;
};

}

#endif

