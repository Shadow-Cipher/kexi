/***************************************************************************
 *   Copyright (C) 2003 by Lucijan Busch <lucijan@kde.org>                 *
 *   Copyright (C) 2004 Cedric Pasteur <cedric.pasteur@free.fr>            *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 2 of the    *
 *   License, or (at your option) any later version.                       *
 ***************************************************************************/

#include <qlabel.h>
#include <qpopupmenu.h>
#include <qcursor.h>
#include <qradiobutton.h>
#include <qcheckbox.h>
#include <qslider.h>
#include <qobjectlist.h>
#include <qstring.h>
#include <qvariant.h>
#include <qheader.h>
#include <qdom.h>
#include <qstyle.h>
#include <qvaluevector.h>

#include <klineedit.h>
#include <kpushbutton.h>
#include <knuminput.h>
#include <kcombobox.h>
#include <klistbox.h>
#include <ktextedit.h>
#include <klistview.h>
#include <kprogress.h>
#include <kiconloader.h>
#include <kgenericfactory.h>
#include <klocale.h>
#include <kdebug.h>
#include <kdeversion.h>

#if !KDE_IS_VERSION(3,1,9) //TMP
# include <qdatetimeedit.h>
# define KTimeWidget QTimeEdit
# define KDateWidget QDateEdit
# define KDateTimeWidget QDateTimeEdit
#else
# include <ktimewidget.h>
# include <kdatewidget.h>
# include <kdatetimewidget.h>
#endif

#include "spring.h"
#include "formIO.h"
#include "form.h"
#include "formmanager.h"
#include "widgetlibrary.h"
#include "stdwidgetfactory.h"

// Some widgets subclass to allow event filtering and some other things
MyPicLabel::MyPicLabel(const QPixmap &pix, QWidget *parent, const char *name)
 : QLabel(parent, name)
{
	setPixmap(pix);
	setScaledContents(false);
}

bool
MyPicLabel::setProperty(const char *name, const QVariant &value)
{
	if(QString(name) == "pixmap")
		resize(value.toPixmap().height(), value.toPixmap().width());
	return QLabel::setProperty(name, value);
}

Line::Line(Qt::Orientation orient, QWidget *parent, const char *name)
 : QFrame(parent, name)
{
	setFrameShadow(Sunken);
	if(orient == Horizontal)
		setFrameShape(HLine);
	else
		setFrameShape(VLine);
}

void
Line::setOrientation(Qt::Orientation orient)
{
	if(orient == Horizontal)
		setFrameShape(HLine);
	else
		setFrameShape(VLine);
}

Qt::Orientation
Line::orientation() const
{
	if(frameShape() == HLine)
		return Horizontal;
	else
		return Vertical;
}

// The factory itself

StdWidgetFactory::StdWidgetFactory(QObject *parent, const char *name, const QStringList &)
 : KFormDesigner::WidgetFactory(parent, name)
{
	m_classes.setAutoDelete(true);

	KFormDesigner::Widget *wLabel = new KFormDesigner::Widget(this);
	wLabel->setPixmap("label");
	wLabel->setClassName("QLabel");
	wLabel->setName(i18n("Text Label"));
	wLabel->setNamePrefix(i18n("This string will be used to name widgets of this class. It must _not_ contain white "
	"spaces and non latin1 characters", "TextLabel"));
	wLabel->setDescription(i18n("A widget to display text"));
	m_classes.append(wLabel);

	KFormDesigner::Widget *wPixLabel = new KFormDesigner::Widget(this);
	wPixLabel->setPixmap("pixmaplabel");
	wPixLabel->setClassName("MyPicLabel");
	wPixLabel->setName(i18n("Picture Label"));
	wPixLabel->setNamePrefix(i18n("PictureLabel"));
	wPixLabel->setDescription(i18n("A widget to display pixmaps"));
	m_classes.append(wPixLabel);

	KFormDesigner::Widget *wLineEdit = new KFormDesigner::Widget(this);
	wLineEdit->setPixmap("lineedit");
	wLineEdit->setClassName("KLineEdit");
	wLineEdit->setAlternateClassName("QLineEdit");
	wLineEdit->setInclude("klineedit.h");
	wLineEdit->setName(i18n("Line Edit"));
	wLineEdit->setNamePrefix(i18n("LineEdit"));
	wLineEdit->setDescription(i18n("A widget to input text"));
	m_classes.append(wLineEdit);

	KFormDesigner::Widget *wSpring = new KFormDesigner::Widget(this);
	wSpring->setPixmap("spring");
	wSpring->setClassName("Spring");
	wSpring->setName(i18n("Spring"));
	wSpring->setNamePrefix(i18n("Spring"));
	wSpring->setDescription(i18n("A spring to place between widgets"));
	m_classes.append(wSpring);

	KFormDesigner::Widget *wPushButton = new KFormDesigner::Widget(this);
	wPushButton->setPixmap("button");
	wPushButton->setClassName("KPushButton");
	wPushButton->setAlternateClassName("QPushButton");
	wPushButton->setInclude("kpushbutton.h");
	wPushButton->setName(i18n("Push Button"));
	wPushButton->setNamePrefix(i18n("PushButton"));
	wPushButton->setDescription(i18n("A simple push button to execute actions"));
	m_classes.append(wPushButton);

	KFormDesigner::Widget *wRadioButton = new KFormDesigner::Widget(this);
	wRadioButton->setPixmap("radio");
	wRadioButton->setClassName("QRadioButton");
	wRadioButton->setName(i18n("Radio"));
	wRadioButton->setNamePrefix(i18n("Radio"));
	wRadioButton->setDescription(i18n("A radio button with text or pixmap label"));
	m_classes.append(wRadioButton);

	KFormDesigner::Widget *wCheckBox = new KFormDesigner::Widget(this);
	wCheckBox->setPixmap("check");
	wCheckBox->setClassName("QCheckBox");
	wCheckBox->setName(i18n("Check Box"));
	wCheckBox->setNamePrefix(i18n("CheckBox"));
	wCheckBox->setDescription(i18n("A check box with text or pixmap label"));
	m_classes.append(wCheckBox);

	KFormDesigner::Widget *wSpinBox = new KFormDesigner::Widget(this);
	wSpinBox->setPixmap("spin");
	wSpinBox->setClassName("KIntSpinBox");
	wSpinBox->setAlternateClassName("QSpinBox");
	wSpinBox->setInclude("knuminput.h");
	wSpinBox->setName(i18n("Spin Box"));
	wSpinBox->setNamePrefix(i18n("SpinBox"));
	wSpinBox->setDescription(i18n("A spin box widget"));
	m_classes.append(wSpinBox);

	KFormDesigner::Widget *wComboBox = new KFormDesigner::Widget(this);
	wComboBox->setPixmap("combo");
	wComboBox->setClassName("KComboBox");
	wComboBox->setAlternateClassName("QComboBox");
	wComboBox->setInclude("kcombobox.h");
	wComboBox->setName(i18n("Combo Box"));
	wComboBox->setNamePrefix(i18n("ComboBox"));
	wComboBox->setDescription(i18n("A combo box widget"));
	m_classes.append(wComboBox);

	KFormDesigner::Widget *wListBox = new KFormDesigner::Widget(this);
	wListBox->setPixmap("listbox");
	wListBox->setClassName("KListBox");
	wListBox->setAlternateClassName("QListBox");
	wListBox->setInclude("klistbox.h");
	wListBox->setName(i18n("List Box"));
	wListBox->setNamePrefix(i18n("ListBox"));
	wListBox->setDescription(i18n("A simple list widget"));
	m_classes.append(wListBox);

	KFormDesigner::Widget *wTextEdit = new KFormDesigner::Widget(this);
	wTextEdit->setPixmap("textedit");
	wTextEdit->setClassName("KTextEdit");
	wTextEdit->setAlternateClassName("QTextEdit");
	wTextEdit->setInclude("ktextedit.h");
	wTextEdit->setName(i18n("Text Editor"));
	wTextEdit->setNamePrefix(i18n("TextEditor"));
	wTextEdit->setDescription(i18n("A simple single-page rich text editor"));
	m_classes.append(wTextEdit);

	KFormDesigner::Widget *wListView = new KFormDesigner::Widget(this);
	wListView->setPixmap("listview");
	wListView->setClassName("KListView");
	wListView->setAlternateClassName("QListView");
	wListView->setInclude("klistview.h");
	wListView->setName(i18n("List View"));
	wListView->setNamePrefix(i18n("ListView"));
	wListView->setDescription(i18n("A list (or tree) widget"));
	m_classes.append(wListView);

	KFormDesigner::Widget *wSlider = new KFormDesigner::Widget(this);
	wSlider->setPixmap("slider");
	wSlider->setClassName("QSlider");
	wSlider->setName(i18n("Slider"));
	wSlider->setNamePrefix(i18n("Slider"));
	wSlider->setDescription(i18n("An horizontal slider"));
	m_classes.append(wSlider);

	KFormDesigner::Widget *wProgressBar = new KFormDesigner::Widget(this);
	wProgressBar->setPixmap("progress");
	wProgressBar->setClassName("KProgress");
	wProgressBar->setAlternateClassName("QProgressBar");
	wProgressBar->setInclude("kprogress.h");
	wProgressBar->setName(i18n("Progress Bar"));
	wProgressBar->setNamePrefix(i18n("ProgressBar"));
	wProgressBar->setDescription(i18n("A progress indicator widget"));
	m_classes.append(wProgressBar);

	KFormDesigner::Widget *wLine = new KFormDesigner::Widget(this);
	wLine->setPixmap("line");
	wLine->setClassName("Line");
	wLine->setName(i18n("Line"));
	wLine->setNamePrefix(i18n("Line"));
	wLine->setDescription(i18n("A line to be used as a separator"));
	m_classes.append(wLine);

	KFormDesigner::Widget *wDate = new KFormDesigner::Widget(this);
	wDate->setPixmap("dateedit");
	wDate->setClassName("KDateWidget");
	#if KDE_IS_VERSION(3,1,9) //TMP
	wDate->setAlternateClassName("QDateEdit");
	wDate->setInclude("kdatewidget.h");
	#endif
	wDate->setName(i18n("Date Widget"));
	wDate->setNamePrefix(i18n("DateWidget"));
	wDate->setDescription(i18n("A widget to input or display a date"));
	m_classes.append(wDate);

	KFormDesigner::Widget *wTime = new KFormDesigner::Widget(this);
	wTime->setPixmap("timeedit");
	wTime->setClassName("KTimeWidget");
	#if KDE_IS_VERSION(3,1,9) //TMP
	wTime->setAlternateClassName("QTimeEdit");
	wTime->setInclude("ktimewidget.h");
	#endif
	wTime->setName(i18n("Time Widget"));
	wTime->setNamePrefix(i18n("TimeWidget"));
	wTime->setDescription(i18n("A widget to input or display a time"));
	m_classes.append(wTime);

	KFormDesigner::Widget *wDateTime = new KFormDesigner::Widget(this);
	wDateTime->setPixmap("datetimeedit");
	wDateTime->setClassName("KDateTimeWidget");
	#if KDE_IS_VERSION(3,1,9) //TMP
	wDateTime->setAlternateClassName("QDateTimeEdit");
	wDateTime->setInclude("kdatetimewidget.h");
	#endif
	wDateTime->setName(i18n("Date/Time Widget"));
	wDateTime->setNamePrefix(i18n("DateTimeWidget"));
	wDateTime->setDescription(i18n("A widget to input or display a time and a date"));
	m_classes.append(wDateTime);
}

QString
StdWidgetFactory::name()
{
	return("stdwidgets");
}

KFormDesigner::WidgetList
StdWidgetFactory::classes()
{
	return m_classes;
}

QWidget*
StdWidgetFactory::create(const QString &c, QWidget *p, const char *n, KFormDesigner::Container *container)
{
	QWidget *w=0;
	QString text = container->form()->manager()->lib()->textForWidgetName(n, c);

	if(c == "QLabel")
		w = new QLabel(/*i18n("Label")*/text, p, n);
	if(c == "MyPicLabel")
		w = new MyPicLabel(DesktopIcon("image"), p, n);

	else if(c == "KLineEdit")
	{
		w = new KLineEdit(p, n);
		w->setCursor(QCursor(Qt::ArrowCursor));
	}
	else if(c == "KPushButton")
		w = new KPushButton(/*i18n("Button")*/text, p, n);

	else if(c == "QRadioButton")
		w = new QRadioButton(/*i18n("Radio Button")*/text, p, n);

	else if(c == "QCheckBox")
		w = new QCheckBox(/*i18n("Check Box")*/text, p, n);

	else if(c == "KIntSpinBox")
		w = new KIntSpinBox(p, n);

	else if(c == "KComboBox")
		w = new KComboBox(p, n);

	else if(c == "KListBox")
		w = new KListBox(p, n);

	else if(c == "KTextEdit")
		w = new KTextEdit(/*i18n("Enter your text here")*/text, QString::null, p, n);

	else if(c == "KListView")
	{
		w = new KListView(p, n);
		if(container->form()->interactiveMode())
			((KListView*)w)->addColumn(i18n("Column 1"));
		((KListView*)w)->setRootIsDecorated(true);
	}
	else if(c == "QSlider")
		w = new QSlider(Qt::Horizontal, p, n);

	else if(c == "KProgress")
		w = new KProgress(p, n);

	else if(c == "KDateWidget")
		w = new KDateWidget(QDate::currentDate(), p, n);

	else if(c == "KTimeWidget")
		w = new KTimeWidget(QTime::currentTime(), p, n);

	else if(c == "KDateTimeWidget")
		w = new KDateTimeWidget(QDateTime::currentDateTime(), p, n);

	else if(c == "Line")
		w= new Line(Line::Horizontal, p, n);

	else if(c == "Spring")
		w = new Spring(p, n);

	if(w)
		return w;
	else
	{
		kdDebug() << "WARNING :: w == 0 "  << endl;
		return 0;
	}
}

void
StdWidgetFactory::previewWidget(const QString &classname, QWidget *widget, KFormDesigner::Container *)
{
	if(classname == "Spring")
		((Spring*)widget)->setPreviewMode();
}

bool
StdWidgetFactory::createMenuActions(const QString &classname, QWidget *w, QPopupMenu *menu, KFormDesigner::Container *container, QValueVector<int> *menuIds)
{
	m_widget = w;
	m_container = container;

	if((classname == "QLabel") || (classname == "KTextEdit"))
	{
		int id = menu->insertItem(SmallIconSet("edit"), i18n("Edit Rich Text"), this, SLOT(editText()));
		menuIds->append(id);
		return true;
	}
	else if(classname == "KListView")
	{
		int id = menu->insertItem(SmallIconSet("edit"), i18n("Edit Listview Contents"), this, SLOT(editListContents()));
		menuIds->append(id);
		return true;
	}

	return false;
}

void
StdWidgetFactory::startEditing(const QString &classname, QWidget *w, KFormDesigner::Container *container)
{
	m_container = container;
	if(classname == "KLineEdit")
	{
		KLineEdit *lineedit = static_cast<KLineEdit*>(w);
		createEditor(lineedit->text(), lineedit, container, lineedit->geometry(), lineedit->alignment(), true);
		return;
	}
	else if(classname == "QLabel")
	{
		QLabel *label = static_cast<QLabel*>(w);
		if(label->textFormat() == RichText)
		{
			m_widget = w;
			editText();
		}
		else
			createEditor(label->text(), label, container, label->geometry(), label->alignment());
		return;
	}
	else if(classname == "KPushButton")
	{
		KPushButton *push = static_cast<KPushButton*>(w);
		QRect r = w->style().subRect(QStyle::SR_PushButtonContents, w);
		QRect editorRect = QRect(push->x() + r.x(), push->y() + r.y(), r.width(), r.height());
		//r.setX(r.x() + 5);
		//r.setY(r.y() + 5);
		//r.setWidth(r.width()-10);
		//r.setHeight(r.height() - 10);
		createEditor(push->text(), push, container, editorRect, Qt::AlignCenter, false, Qt::PaletteButton);
	}
	else if(classname == "QRadioButton")
	{
		QRadioButton *radio = static_cast<QRadioButton*>(w);
		QRect r = w->style().subRect(QStyle::SR_RadioButtonContents, w);
		QRect editorRect = QRect(radio->x() + r.x(), radio->y() + r.y(), r.width(), r.height());
		createEditor(radio->text(), radio, container, editorRect, Qt::AlignAuto);
		return;
	}
	else if(classname == "QCheckBox")
	{
		QCheckBox *check = static_cast<QCheckBox*>(w);
		//QRect r(check->geometry());
		//r.setX(r.x() + 20);
		QRect r = w->style().subRect(QStyle::SR_CheckBoxContents, w);
		QRect editorRect = QRect(check->x() + r.x(), check->y() + r.y(), r.width(), r.height());
		createEditor(check->text(), check, container, editorRect, Qt::AlignAuto);
		return;
	}
	else if((classname == "KComboBox") || (classname == "KListBox"))
	{
		QStringList list;
		if(classname == "KListBox")
		{
			KListBox *listbox = (KListBox*)w;
			for(uint i=0; i < listbox->count(); i++)
				list.append(listbox->text(i));
		}
		else if(classname == "KComboBox")
		{
			KComboBox *combo = (KComboBox*)w;
			for(int i=0; i < combo->count(); i++)
				list.append(combo->text(i));
		}

		if(editList(w, list))
		{
			if(classname == "KListBox")
			{
				((KListBox*)w)->clear();
				((KListBox*)w)->insertStringList(list);
			}
			else if(classname == "KComboBox")
			{
				((KComboBox*)w)->clear();
				((KComboBox*)w)->insertStringList(list);
			}
		}
	}
	else if((classname == "KTextEdit") || (classname == "KDateTimeWidget") || (classname == "KTimeWidget") ||
		(classname == "KDateWidget") || (classname == "KIntSpinBox"))
		disableFilter(w, container);

	return;
}

void
StdWidgetFactory::changeText(const QString &text)
{
	QString n = WidgetFactory::m_widget->className();
	QWidget *w = WidgetFactory::m_widget;
	if(n == "KIntSpinBox")
		((KIntSpinBox*)w)->setValue(text.toInt());
	else
		changeProperty("text", text, m_container);

	/* By-hand method not needed as sizeHint() can do that for us
	QFontMetrics fm = w->fontMetrics();
	QSize s(fm.width( text ), fm.height());
	int width;
	if(n == "QLabel") // labels are resized to fit the text
	{
		w->resize(w->sizeHint());
		WidgetFactory::m_editor->resize(w->size());
		return;
	}
	// and other widgets are just enlarged if needed
	else if(n == "KPushButton")
		width = w->style().sizeFromContents( QStyle::CT_PushButton, w, s).width();
	else if(n == "QCheckBox")
		width = w->style().sizeFromContents( QStyle::CT_CheckBox, w, s).width();
	else if(n == "QRadioButton")
		width = w->style().sizeFromContents( QStyle::CT_RadioButton, w, s).width();
	else
		return;*/
	int width = w->sizeHint().width();

	if(w->width() < width)
	{
		w->resize(width, w->height() );
		//WidgetFactory::m_editor->resize(w->size());
	}
}

void
StdWidgetFactory::resizeEditor(QWidget *widget, const QString classname)
{
	QSize s = widget->size();
	QPoint p = widget->pos();
	QRect r;
	//if((classname == "QLabel") || (classname == "KPushButton") || (classname == "KLineEdit") || (classname == "QLabel"))

	if(classname == "QRadioButton")
	{
		r = widget->style().subRect(QStyle::SR_RadioButtonContents, widget);
		p += r.topLeft();
		s.setWidth(r.width());
	}
	else if(classname == "QCheckBox")
	{
		r = widget->style().subRect(QStyle::SR_CheckBoxContents, widget);
		p += r.topLeft();
		s.setWidth(r.width());
	}
	else if(classname == "KPushButton")
	{
		r = widget->style().subRect(QStyle::SR_PushButtonContents, widget);
		p += r.topLeft();
		s = r.size();
	}

	m_editor->resize(s);
	m_editor->move(p);
}

void
StdWidgetFactory::saveSpecialProperty(const QString &classname, const QString &name, const QVariant &, QWidget *w, QDomElement &parentNode, QDomDocument &domDoc)
{
	if(name == "list_items" && classname == "KComboBox")
	{
		KComboBox *combo = (KComboBox*)w;
		for(int i=0; i < combo->count(); i++)
		{
			QDomElement item = domDoc.createElement("item");
			KFormDesigner::FormIO::saveProperty(item, domDoc, "property", "text", combo->text(i));
			parentNode.appendChild(item);
		}
	}

	else if(name == "list_items" && classname == "KListBox")
	{
		KListBox *listbox = (KListBox*)w;
		for(uint i=0; i < listbox->count(); i++)
		{
			QDomElement item = domDoc.createElement("item");
			KFormDesigner::FormIO::saveProperty(item, domDoc, "property", "text", listbox->text(i));
			parentNode.appendChild(item);
		}
	}

	else if(name == "list_contents" && classname == "KListView")
	{
		KListView *listview = (KListView*)w;
		// First we save the columns
		for(int i = 0; i < listview->columns(); i++)
		{
			QDomElement item = domDoc.createElement("column");
			KFormDesigner::FormIO::saveProperty(item, domDoc, "property", "text", listview->columnText(i));
			KFormDesigner::FormIO::saveProperty(item, domDoc, "property", "width", listview->columnWidth(i));
			KFormDesigner::FormIO::saveProperty(item, domDoc, "property", "resizable", listview->header()->isResizeEnabled(i));
			KFormDesigner::FormIO::saveProperty(item, domDoc, "property", "clickable", listview->header()->isClickEnabled(i));
			KFormDesigner::FormIO::saveProperty(item, domDoc, "property", "fullwidth", listview->header()->isStretchEnabled(i));
			parentNode.appendChild(item);
		}

		// Then we save the list view items
		QListViewItem *item = listview->firstChild();
		while(item)
		{
			saveListItem(item, parentNode, domDoc);
			item = item->nextSibling();
		}
	}

	return;
}

void
StdWidgetFactory::saveListItem(QListViewItem *item, QDomNode &parentNode, QDomDocument &domDoc)
{
	QDomElement element = domDoc.createElement("item");
	parentNode.appendChild(element);

	// We save the text of each column
	for(int i = 0; i < item->listView()->columns(); i++)
		KFormDesigner::FormIO::saveProperty(element, domDoc, "property", "text", item->text(i));

	// Then we save every sub items
	QListViewItem *child = item->firstChild();
	while(child)
	{
		saveListItem(child, element, domDoc);
		child = child->nextSibling();
	}
}

void
StdWidgetFactory::readSpecialProperty(const QString &classname, QDomElement &node, QWidget *w, KFormDesigner::ObjectTreeItem *)
{
	QString tag = node.tagName();
	QString name = node.attribute("name");

	if((tag == "item") && (classname == "KComboBox"))
	{
		KComboBox *combo = (KComboBox*)w;
		QVariant val = KFormDesigner::FormIO::readProp(node.firstChild().firstChild(), w, name);
		if(val.canCast(QVariant::Pixmap))
			combo->insertItem(val.toPixmap());
		else
			combo->insertItem(val.toString());
	}

	else if((tag == "item") && (classname == "KListBox"))
	{
		KListBox *listbox = (KListBox*)w;
		QVariant val = KFormDesigner::FormIO::readProp(node.firstChild().firstChild(), w, name);
		if(val.canCast(QVariant::Pixmap))
			listbox->insertItem(val.toPixmap());
		else
			listbox->insertItem(val.toString());
	}

	else if((tag == "column") && (classname == "KListView"))
	{
		KListView *listview = (KListView*)w;
		int id=0;
		for(QDomNode n = node.firstChild(); !n.isNull(); n = n.nextSibling())
		{
			QString prop = n.toElement().attribute("name");
			QVariant val = KFormDesigner::FormIO::readProp(n.firstChild(), w, name);
			if(prop == "text")
				id = listview->addColumn(val.toString());
			else if(prop == "width")
				listview->setColumnWidth(id, val.toInt());
			else if(prop == "resizable")
				listview->header()->setResizeEnabled(val.toBool(), id);
			else if(prop == "clickable")
				listview->header()->setClickEnabled(val.toBool(), id);
			else if(prop == "fullwidth")
				listview->header()->setStretchEnabled(val.toBool(), id);
		}
	}
	else if((tag == "item") && (classname == "KListView"))
	{
		KListView *listview = (KListView*)w;
		readListItem(node, 0, listview);
	}
}

void
StdWidgetFactory::readListItem(QDomElement &node, QListViewItem *parent, KListView *listview)
{
	QListViewItem *item;
	if(parent)
		item = new KListViewItem(parent);
	else
		item = new KListViewItem(listview);

	// We need to move the item at the end of the list
	QListViewItem *last;
	if(parent)
		last = parent->firstChild();
	else
		last = listview->firstChild();

	while(last->nextSibling())
		last = last->nextSibling();
	item->moveItem(last);

	int i = 0;
	for(QDomNode n = node.firstChild(); !n.isNull(); n = n.nextSibling())
	{
		QDomElement childEl = n.toElement();
		QString prop = childEl.attribute("name");
		QString tag = childEl.tagName();

		// We read sub items
		if(tag == "item")
		{
			item->setOpen(true);
			readListItem(childEl, item, listview);
		}
		// and column texts
		else if((tag == "property") && (prop == "text"))
		{
			QVariant val = KFormDesigner::FormIO::readProp(n.firstChild(), listview, "item");
			item->setText(i, val.toString());
			i++;
		}
	}
}

bool
StdWidgetFactory::showProperty(const QString &classname, QWidget *, const QString &property, bool multiple)
{
	if(classname == "Spring")
	{
		return Spring::showProperty(property);
	}
	else if(classname == "MyPicLabel")
	{
		if((property == "text") || (property == "indent") || (property == "textFormat") || (property == "font") || (property == "alignment"))
			return false;
	}
	else if(classname == "QLabel")
	{
		if(property == "pixmap")
			return false;
	}
	else if(classname == "Line")
	{
		if((property == "frameShape") || (property == "font") || (property == "margin"))
			return false;
	}
	return !multiple;
}

QStringList
StdWidgetFactory::autoSaveProperties(const QString &classname)
{
	QStringList l;

	if(classname == "QLabel")
		l << "text";
	if(classname == "KPushButton")
		l << "text";
	else if(classname == "MyPicLabel")
		l << "pixmap";
	else if(classname == "KComboBox")
		l << "list_items";
	else if(classname == "KListBox")
		l << "list_items";
	else if(classname == "KListView")
		l << "list_contents";
	else if(classname == "Line")
		l << "orientation";
	else if(classname == "KTimeWidget")
		l << "time";
	else if(classname == "KDateWidget")
		l << "date";
	else if(classname == "KDateTimeWidget")
		l << "dateTime";
	else if(classname == "Spring")
		l << "sizeType" << "orientation";
	else if(classname == "KTextEdit")
		l << "textFormat" << "text";

	return l;
}

void
StdWidgetFactory::editText()
{
	QString classname = m_widget->className();
	QString text;
	if(classname == "KTextEdit")
		text = ((KTextEdit*)m_widget)->text();
	else if(classname == "QLabel")
		text = ((QLabel*)m_widget)->text();

	if(editRichText(m_widget, text))
	{
		changeProperty("textFormat", "RichText", m_container);
		changeProperty("text", text, m_container);
	}

	if(classname == "QLabel")
		m_widget->resize(m_widget->sizeHint());
}

void
StdWidgetFactory::editListContents()
{
	if(m_widget->inherits("QListView"))
		editListView((QListView*)m_widget);
}

StdWidgetFactory::~StdWidgetFactory()
{
}

K_EXPORT_COMPONENT_FACTORY(stdwidgets, KGenericFactory<StdWidgetFactory>)

#include "stdwidgetfactory.moc"

