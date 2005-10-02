/* This file is part of the KDE project
   Copyright (C) 2004 Cedric Pasteur <cedric.pasteur@free.fr>
   Copyright (C) 2004-2005 Jaroslaw Staniek <js@iidea.pl>

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
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
*/

#ifndef KEXIDBFACTORY_H
#define KEXIDBFACTORY_H

#include <widgetfactory.h>

class KAction;

namespace KFormDesigner {
	class Form;
	class FormManager;
}

using KFormDesigner::Form;

//! A form embedded as a widget inside other form
class KexiSubForm : public QScrollView
{
	Q_OBJECT
	Q_PROPERTY(QString formName READ formName WRITE setFormName DESIGNABLE true)

	public:
		KexiSubForm(KFormDesigner::Form *parentForm, QWidget *parent, const char *name);
		~KexiSubForm() {}

		//! \return the name of the subform to display inside this widget
		QString formName() const { return m_formName; }

		//! Sets the name of the subform to display inside this widget
		void setFormName(const QString &name);

		//void  paintEvent(QPaintEvent *ev);

	private:
		Form *m_parentForm;
		Form *m_form;
		QWidget *m_widget;
		QString m_formName;
};


//! Kexi Factory (DB widgets + subform)
class KexiDBFactory : public KFormDesigner::WidgetFactory
{
	Q_OBJECT

	public:
		KexiDBFactory(QObject *parent, const char *name, const QStringList &args);
		virtual ~KexiDBFactory();

//		virtual QString	name();
		virtual QWidget *createWidget(const QCString &, QWidget *, const char *, 
			KFormDesigner::Container *, int options = DefaultOptions );

		virtual void createCustomActions(KActionCollection* col);
		virtual bool createMenuActions(const QCString &classname, QWidget *w, QPopupMenu *menu,
		   KFormDesigner::Container *container);
		virtual bool startEditing(const QCString &classname, QWidget *w, KFormDesigner::Container *container);
		virtual bool previewWidget(const QCString &, QWidget *, KFormDesigner::Container *);
		virtual bool clearWidgetContent(const QCString &classname, QWidget *w);

		//virtual void		saveSpecialProperty(const QString &classname, const QString &name, const QVariant &value, QWidget *w,
		         //QDomElement &parentNode, QDomDocument &parent) {}
		//virtual void            readSpecialProperty(const QCString &classname, QDomElement &node, QWidget *w, KFormDesigner::ObjectTreeItem *item) {}
		virtual QValueList<QCString> autoSaveProperties(const QCString &classname);

	protected:
		virtual bool changeText(const QString &newText);
		virtual void resizeEditor(QWidget *editor, QWidget *widget, const QCString &classname);

		virtual bool isPropertyVisibleInternal(const QCString &, QWidget *, const QCString &, bool isTopLevel);
		QWidget *m_widget;
		KFormDesigner::Container *m_container;

		KAction* m_assignAction;
};

#endif
