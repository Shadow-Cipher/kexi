/***************************************************************************
 *   Copyright (C) 2003 by Lucijan Busch          lucijan@kde.org          *
 *   Copyright (C) 2004 Cedric Pasteur <cedric.pasteur@free.fr>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 ***************************************************************************/

#include <qwidgetstack.h>
#include <qframe.h>
#include <qbuttongroup.h>
#include <qwidget.h>
#include <qstring.h>
#include <qpopupmenu.h>
#include <qdom.h>
#include <qevent.h>
#include <qobjectlist.h>

#include <kiconloader.h>
#include <kgenericfactory.h>
#include <klocale.h>
#include <kdebug.h>
#include <kdeversion.h>

#include "containerfactory.h"
#include "container.h"
#include "form.h"
#include "formIO.h"
#include "objecttree.h"
#include "commands.h"
#include "formmanager.h"
#include "widgetlibrary.h"

#if !KDE_IS_VERSION(3,1,9) //TMP
# define KTabWidget QTabWidget
# define KInputDialog QInputDialog
# include <qtabwidget.h>
# include <qinputdialog.h>
# include <qlineedit.h>
#else
# include <ktabwidget.h>
# include <kinputdialog.h>
#endif


class KFORMEDITOR_EXPORT MyTabWidget : public KTabWidget
{
	public:
		MyTabWidget(QWidget *parent, const char *name, QObject *container)
		 : KTabWidget(parent, name)
		{
			m_container = container;

			QObjectList *list = new QObjectList(*children());
			for(QObject *obj = list->first(); obj; obj = list->next())
			{
				if(obj->isA("KTabBar"))
					obj->installEventFilter(this);
			}
			delete list;
		}
		~MyTabWidget() {;}

		void setContainer(QObject *container)
		{
			m_container = container;
		}
		virtual bool eventFilter(QObject *o, QEvent *ev)
		{
			if((!m_container) || (ev->type() != QEvent::MouseButtonRelease))
				return KTabWidget::eventFilter(o, ev);

			QMouseEvent *mev = static_cast<QMouseEvent*>(ev);
			if(mev->button() != RightButton)
				return KTabWidget::eventFilter(o, ev);

			bool ok = m_container->eventFilter(this, ev);
			if(!ok)
				return KTabWidget::eventFilter(o, ev);
			return true;
		}

	private:
		QGuardedPtr<QObject>   m_container;
};

///////  Tab related KCommand (to allow tab creation/deletion undoing)

InsertPageCommand::InsertPageCommand(KFormDesigner::Container *container, QWidget *parent)
  : KCommand()
{
	m_containername = container->widget()->name();
	m_form = container->form();
	m_parentname = parent->name();
	m_pageid = -1;
}

void
InsertPageCommand::execute()
{
	KFormDesigner::Container *m_container = m_form->objectTree()->lookup(m_containername)->container();
	QWidget *parent = m_form->objectTree()->lookup(m_parentname)->widget();
	if(m_name.isEmpty())
		m_name = m_container->form()->objectTree()->genName(m_container->form()->manager()->lib()->displayName("QWidget"));

	QWidget *page = new QWidget(parent, m_name.latin1());
	new KFormDesigner::Container(m_container, page, parent);

	QString classname = parent->className();
	if(classname == "KTabWidget")
	{
		KTabWidget *tab = (KTabWidget *)parent;
		QString n = QString(i18n("Page %1").arg(tab->count() + 1));
		tab->addTab(page, n);
		tab->showPage(page);

		KFormDesigner::ObjectTreeItem *item = m_container->form()->objectTree()->lookup(m_name);
		item->addModProperty("title", n);
	}
	else if(classname == "QWidgetStack")
	{
		QWidgetStack *stack = (QWidgetStack*)parent;
		stack->addWidget(page, m_pageid);
		stack->raiseWidget(page);
		m_pageid = stack->id(page);

		KFormDesigner::ObjectTreeItem *item = m_container->form()->objectTree()->lookup(m_name);
		item->addModProperty("id", stack->id(page));
	}
}

void
InsertPageCommand::unexecute()
{
	QWidget *page = m_form->objectTree()->lookup(m_name)->widget();
	QWidget *parent = m_form->objectTree()->lookup(m_parentname)->widget();

	WidgetList list;
	list.append(page);
	KCommand *com = new KFormDesigner::DeleteWidgetCommand(list, m_form);

	QString classname = parent->className();
	if(classname == "KTabWidget")
	{
		KTabWidget *tab = (KTabWidget *)parent;
		tab->removePage(page);
	}
	else if(classname == "QWidgetStack")
	{
		QWidgetStack *stack = (QWidgetStack*)parent;
		int id = stack->id(page) - 1;
		while(!stack->widget(id))
			id--;

		stack->raiseWidget(id);
		stack->removeWidget(page);
	}

	com->execute();
	delete com;
}

QString
InsertPageCommand::name() const
{
	return i18n("Add Page");
}

/////   The factory /////////////////////////

ContainerFactory::ContainerFactory(QObject *parent, const char *name, const QStringList &)
 : KFormDesigner::WidgetFactory(parent, name)
{
	m_classes.setAutoDelete(true);

	KFormDesigner::Widget *wBtnGroup = new KFormDesigner::Widget(this);
	wBtnGroup->setPixmap("frame");
	wBtnGroup->setClassName("QButtonGroup");
	wBtnGroup->setName(i18n("Button Group"));
	wBtnGroup->setDescription(i18n("A simple container to group buttons"));
	m_classes.append(wBtnGroup);

	KFormDesigner::Widget *wTabWidget = new KFormDesigner::Widget(this);
	wTabWidget->setPixmap("tabwidget");
	wTabWidget->setClassName("KTabWidget");
	wTabWidget->setName(i18n("Tab Widget"));
	wTabWidget->setDescription(i18n("A widget to display multiple pages using tabs"));
	m_classes.append(wTabWidget);

	KFormDesigner::Widget *wWidget = new KFormDesigner::Widget(this);
	wWidget->setPixmap("widget");
	wWidget->setClassName("QWidget");
	wWidget->setName(i18n("Basic container"));
	wWidget->setDescription(i18n("An empty container with no frame"));
	m_classes.append(wWidget);

	KFormDesigner::Widget *wGroupBox = new KFormDesigner::Widget(this);
	wGroupBox->setPixmap("frame");
	wGroupBox->setClassName("QGroupBox");
	wGroupBox->setName(i18n("Group Box"));
	wGroupBox->setDescription(i18n("A container to group some widgets"));
	m_classes.append(wGroupBox);

	KFormDesigner::Widget *wFrame = new KFormDesigner::Widget(this);
	wFrame->setPixmap("frame");
	wFrame->setClassName("QFrame");
	wFrame->setName(i18n("Frame"));
	wFrame->setDescription(i18n("A very simple container"));
	m_classes.append(wFrame);

	KFormDesigner::Widget *wWidgetStack = new KFormDesigner::Widget(this);
	wWidgetStack->setPixmap("frame");
	wWidgetStack->setClassName("QWidgetStack");
	wWidgetStack->setName(i18n("Widget Stack"));
	wWidgetStack->setDescription(i18n("A container with multiple pages"));
	m_classes.append(wWidgetStack);
}

QString
ContainerFactory::name()
{
	return("containers");
}

KFormDesigner::WidgetList
ContainerFactory::classes()
{
	return m_classes;
}

QWidget*
ContainerFactory::create(const QString &c, QWidget *p, const char *n, KFormDesigner::Container *container)
{
	kdDebug() << "ContainerFactory::create() " << this << endl;

	if(c == "QButtonGroup")
	{
		QButtonGroup *w = new QButtonGroup(i18n("Button Broup"), p, n);
		new KFormDesigner::Container(container, w, container);
		return w;
	}
	else if(c == "KTabWidget")
	{
		MyTabWidget *tab = new MyTabWidget(p, n, container);
#if KDE_IS_VERSION(3,1,9)
		tab->setTabReorderingEnabled(true);
#endif
		connect(tab, SIGNAL(movedTab(int,int)), this, SLOT(reorderTabs(int,int)));
		container->form()->objectTree()->addChild(container->tree(), new KFormDesigner::ObjectTreeItem(
		        container->form()->manager()->lib()->displayName(c), n, tab));
		tab->installEventFilter(container);
		tab->setContainer(container);
		m_manager = container->form()->manager();

		if(container->form()->interactiveMode())
		{
			m_widget=tab;
			m_container=container;
			AddTabPage();
		}

		return tab;
	}
	else if(c == "QWidget")
	{
		QWidget *w = new QWidget(p, n);
		new KFormDesigner::Container(container, w, p);
		return w;
	}
	else if(c == "QGroupBox")
	{
		QGroupBox *w = new QGroupBox(i18n("Group Box"), p, n);
		new KFormDesigner::Container(container, w, container);
		return w;
	}
	else if(c == "QFrame")
	{
		QFrame *w = new QFrame(p, n);
		w->setLineWidth(2);
		w->setFrameStyle(QFrame::StyledPanel|QFrame::Raised);
		new KFormDesigner::Container(container, w, container);
		return w;
	}
	else if(c == "QWidgetStack")
	{
		QWidgetStack *stack = new QWidgetStack(p, n);
		stack->setLineWidth(2);
		stack->setFrameStyle(QFrame::StyledPanel|QFrame::Raised);
		container->form()->objectTree()->addChild(container->tree(), new KFormDesigner::ObjectTreeItem(
		     container->form()->manager()->lib()->displayName(c), n, stack));
		stack->installEventFilter(container);

		if(container->form()->interactiveMode())
		{
			m_widget = stack;
			m_container = container;
			AddStackPage();
		}
		return stack;
	}

	return 0;
}

bool
ContainerFactory::createMenuActions(const QString &classname, QWidget *w, QPopupMenu *menu, KFormDesigner::Container *container)
{
	m_widget = w;
	m_container = container;

	if(classname == "KTabWidget")
	{
		menu->insertItem(SmallIconSet("tab_new"), i18n("Add Page"), this, SLOT(AddTabPage()) );
		menu->insertItem(SmallIconSet("edit"), i18n("Rename Page"), this, SLOT(renameTabPage()));
		int id = menu->insertItem(SmallIconSet("tab_remove"), i18n("Remove Page"), this, SLOT(removeTabPage()));
		if( ((KTabWidget*)w)->count() == 1)
			menu->setItemEnabled(id, false);
		return true;
	}
	else if(w->parentWidget()->parentWidget()->inherits("QTabWidget"))
	{
		m_widget = w->parentWidget()->parentWidget();
		m_container = m_container->toplevel();
		menu->insertItem(SmallIconSet("tab_new"), i18n("Add Page"), this, SLOT(AddTabPage()) );
		menu->insertItem(SmallIconSet("edit"), i18n("Rename Page"), this, SLOT(renameTabPage()));
		int id = menu->insertItem(SmallIconSet("tab_remove"), i18n("Remove Page"), this, SLOT(removeTabPage()));
		if( ((KTabWidget*)m_widget)->count() == 1)
			menu->setItemEnabled(id, false);
		return true;
	}
	else if(w->parentWidget()->isA("QWidgetStack") && !w->parentWidget()->parentWidget()->inherits("QTabWidget"))
	{
		m_widget = w->parentWidget();
		QWidgetStack *stack = (QWidgetStack*)m_widget;
		m_container = container->form()->objectTree()->lookup(m_widget->name())->parent()->container();
		menu->insertItem(SmallIconSet("tab_new"), i18n("Add Page"), this, SLOT(AddStackPage()) );

		int id = menu->insertItem(SmallIconSet("tab_remove"), i18n("Remove Page"), this, SLOT(removeStackPage()) );
		if( ((QWidgetStack*)m_widget)->children()->count() == 4) // == the stack has only one page
			menu->setItemEnabled(id, false);

		id = menu->insertItem(SmallIconSet("next"), i18n("Jump to next page"), this, SLOT(nextStackPage()));
		if(!stack->widget(stack->id(stack->visibleWidget())+1))
			menu->setItemEnabled(id, false);

		id = menu->insertItem(SmallIconSet("previous"), i18n("Jump to previous page"), this, SLOT(prevStackPage()));
		if(!stack->widget(stack->id(stack->visibleWidget()) -1) )
			menu->setItemEnabled(id, false);
		return true;
	}
	return false;
}

void
ContainerFactory::startEditing(const QString &classname, QWidget *w, KFormDesigner::Container *container)
{
	m_container = container;
	if(classname == "QButtonGroup")
	{
		QButtonGroup *group = static_cast<QButtonGroup*>(w);
		QRect r = QRect(group->x()+2, group->y()-5, group->width()-10, 20);
		createEditor(group->title(), group, r, Qt::AlignAuto);
		return;
	}
	if(classname == "QGroupBox")
	{
		QGroupBox *group = static_cast<QGroupBox*>(w);
		QRect r = QRect(group->x()+2, group->y()-5, group->width()-10, 20);
		createEditor(group->title(), group, r, Qt::AlignAuto);
		return;
	}
	return;
}

void
ContainerFactory::saveSpecialProperty(const QString &classname, const QString &name, const QVariant &value, QWidget *w, QDomElement &parentNode, QDomDocument &parent)
{
	if((name == "title") && (w->parentWidget()->parentWidget()->inherits("QTabWidget")))
	{
		QTabWidget *tab = (QTabWidget*)w->parentWidget()->parentWidget();
		KFormDesigner::FormIO::saveProperty(parentNode, parent, "attribute", "title", tab->tabLabel(w));
	}
	else if((name == "id") && (w->parentWidget()->isA("QWidgetStack")))
	{
		QWidgetStack *stack = (QWidgetStack*)w->parentWidget();
		KFormDesigner::FormIO::saveProperty(parentNode, parent, "attribute", "id", stack->id(w));
	}
}

void
ContainerFactory::readSpecialProperty(const QString &classname, QDomElement &node, QWidget *w, KFormDesigner::ObjectTreeItem *item)
{
	QString name = node.attribute("name");
	if((name == "title") && (item->parent()->widget()->inherits("QTabWidget")))
	{
		QTabWidget *tab = (QTabWidget*)w->parentWidget();
		tab->addTab(w, node.firstChild().toElement().text());
		item->addModProperty("title", node.firstChild().toElement().text());
	}
	else if((name == "id") && (w->parentWidget()->isA("QWidgetStack")))
	{
		QWidgetStack *stack = (QWidgetStack*)w->parentWidget();
		int id = KFormDesigner::FormIO::readProp(node.firstChild(), w, name).toInt();
		stack->addWidget(w, id);
		stack->raiseWidget(w);
		item->addModProperty("id", id);
	}
}

QStringList
ContainerFactory::autoSaveProperties(const QString &classname)
{
	return QStringList();
}

void
ContainerFactory::changeText(const QString &text)
{
	changeProperty("title", text, m_container);
}

void ContainerFactory::AddTabPage()
{
	if (!m_widget->inherits("QTabWidget")){ return ;}
	KCommand *com = new InsertPageCommand(m_container, m_widget);
	if(((KTabWidget*)m_widget)->count() == 0)
	{
		com->execute();
		delete com;
	}
	else
		m_container->form()->addCommand(com, true);
}

void ContainerFactory::removeTabPage()
{
	if (!m_widget->inherits("QTabWidget")){ return ;}
	QTabWidget *tab = (QTabWidget *)m_widget;
	QWidget *w = tab->currentPage();

	WidgetList list;
	list.append(w);
	KCommand *com = new KFormDesigner::DeleteWidgetCommand(list, m_container->form());
	tab->removePage(w);
	m_container->form()->addCommand(com, true);
}

void ContainerFactory::renameTabPage()
{
	if (!m_widget->inherits("QTabWidget")){ return ;}
	QTabWidget *tab = (QTabWidget *)m_widget;
	QWidget *w = tab->currentPage();
	bool ok;

	QString name = KInputDialog::getText(i18n("New Page Title"), i18n("Enter a new title for the current page"),
#if !KDE_IS_VERSION(3,1,9) //TMP
	       QLineEdit::Normal,
#endif
	       tab->tabLabel(w), &ok, w->topLevelWidget());
	if(ok)
		tab->changeTab(w, name);
}

void ContainerFactory::reorderTabs(int oldpos, int newpos)
{
	KFormDesigner::ObjectTreeItem *tab = m_manager->activeForm()->objectTree()->lookup(sender()->name());
	if(!tab)
		return;

	kdDebug() << "ContainerFactory reordering tabs for " << tab->name() << endl;
	KFormDesigner::ObjectTreeItem *item = tab->children()->take(oldpos);
	tab->children()->insert(newpos, item);
}

void ContainerFactory::AddStackPage()
{
	if (!m_widget->isA("QWidgetStack")){ return ;}
	KCommand *com = new InsertPageCommand(m_container, m_widget);
	if(!((QWidgetStack*)m_widget)->visibleWidget())
	{
		com->execute();
		delete com;
	}
	else
		m_container->form()->addCommand(com, true);
}

void ContainerFactory::removeStackPage()
{
	if (!m_widget->isA("QWidgetStack")){ return ;}
	QWidgetStack *stack = (QWidgetStack*)m_widget;
	QWidget *page = stack->visibleWidget();

	WidgetList list;
	list.append(page);
	KCommand *com = new KFormDesigner::DeleteWidgetCommand(list, m_container->form());

	int id = stack->id(page) - 1;
	while(!stack->widget(id))
		id--;
	stack->raiseWidget(id);

	stack->removeWidget(page);
	m_container->form()->addCommand(com, true);
}

void ContainerFactory::prevStackPage()
{
	QWidgetStack *stack = (QWidgetStack*)m_widget;
	int id = stack->id(stack->visibleWidget()) - 1;
	if(stack->widget(id))
		stack->raiseWidget(id);
}

void ContainerFactory::nextStackPage()
{
	QWidgetStack *stack = (QWidgetStack*)m_widget;
	int id = stack->id(stack->visibleWidget()) + 1;
	if(stack->widget(id))
		stack->raiseWidget(id);
}

ContainerFactory::~ContainerFactory()
{
}

K_EXPORT_COMPONENT_FACTORY(containers, KGenericFactory<ContainerFactory>)

#include "containerfactory.moc"
