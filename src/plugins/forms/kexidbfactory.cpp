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
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#include <qpopupmenu.h>
#include <qscrollview.h>
#include <qcursor.h>
#include <qpainter.h>
#include <qstyle.h>

#include <kgenericfactory.h>
#include <klocale.h>
#include <kdebug.h>
#include <kiconloader.h>

#include <container.h>
#include <form.h>
#include <formIO.h>
#include <formmanager.h>
#include <objecttree.h>
#include <formeditor/utils.h>
#include <kexidb/utils.h>
#include <kexidb/connection.h>
#include <kexipart.h>
#include <widgetlibrary.h>
#include <kexigradientwidget.h>
#include <keximainwindow.h>
#include <kexiutils/utils.h>

#include "kexidbform.h"
#include "kexiformview.h"
#include "kexilabel.h"
#include "kexidbwidgets.h"
#include "kexidbfieldedit.h"
#include "kexidataawarewidgetinfo.h"

#include "kexidbfactory.h"
#include <core/kexi.h>

KexiSubForm::KexiSubForm(Form *parentForm, QWidget *parent, const char *name)
: QScrollView(parent, name), m_parentForm(parentForm), m_form(0), m_widget(0)
{
	setFrameStyle(QFrame::WinPanel | QFrame::Sunken);
	viewport()->setPaletteBackgroundColor(colorGroup().mid());
}
/*
void
KexiSubForm::paintEvent(QPaintEvent *ev)
{
	QScrollView::paintEvent(ev);
	QPainter p;

	setWFlags(WPaintUnclipped);

	QString txt("Subform");
	QFont f = font();
	f.setPointSize(f.pointSize() * 3);
	QFontMetrics fm(f);
	const int txtw = fm.width(txt), txth = fm.height();

	p.begin(this, true);
	p.setPen(black);
	p.setFont(f);
	p.drawText(width()/2, height()/2, txt, Qt::AlignCenter|Qt::AlignVCenter);
	p.end();

	clearWFlags( WPaintUnclipped );
}
*/
void
KexiSubForm::setFormName(const QString &name)
{
	if(m_formName==name)
		return;

	m_formName = name; //assign, even if the name points to nowhere

	if(name.isEmpty()) {
		delete m_widget;
		m_widget = 0;
		updateScrollBars();
		return;
	}

	QWidget *pw = parentWidget();
	KexiFormView *view = 0;
	QStringList list;
	while(pw) {
		if(pw->isA("KexiSubForm")) {
			if(list.contains(pw->name())) {
//! @todo error message
				return; // Be sure to don't run into a endless-loop cause of recursive subforms.
			}
			list.append(pw->name());
		}
		else if(! view && pw->isA("KexiFormView"))
			view = static_cast<KexiFormView*>(pw); // we need a KexiFormView*
		pw = pw->parentWidget();
	}

	if (!view || !view->parentDialog() || !view->parentDialog()->mainWin()
		|| !view->parentDialog()->mainWin()->project()->dbConnection())
		return;

	KexiDB::Connection *conn = view->parentDialog()->mainWin()->project()->dbConnection();

	// we check if there is a form with this name
	int id = KexiDB::idForObjectName(*conn, name, KexiPart::FormObjectType);
	if((id == 0) || (id == view->parentDialog()->id())) // == our form
		return; // because of recursion when loading

	// we create the container widget
	delete m_widget;
	m_widget = new KexiDBFormBase(viewport(), "kexisubform_widget");
	m_widget->show();
	addChild(m_widget);
	m_form = new Form(m_parentForm->manager(), this->name());
	m_form->createToplevel(m_widget);

	// and load the sub form
	QString data;
	bool ok = conn->loadDataBlock(id, data, QString::null);
	if (ok)
		ok = KFormDesigner::FormIO::loadFormFromString(m_form, m_widget, data);
	if(!ok) {
		delete m_widget;
		m_widget = 0;
		updateScrollBars();
		m_formName = QString::null;
		return;
	}
	m_form->setDesignMode(false);

	// Install event filters on the whole newly created form
	KFormDesigner::ObjectTreeItem *tree = m_parentForm->objectTree()->lookup(QObject::name());
	KFormDesigner::installRecursiveEventFilter(this, tree->eventEater());
}

//////////////////////////////////////////

KexiDBFactory::KexiDBFactory(QObject *parent, const char *name, const QStringList &)
 : KFormDesigner::WidgetFactory(parent, name)
{
//	KFormDesigner::WidgetInfo *wView = new KFormDesigner::WidgetInfo(this);
	KexiDataAwareWidgetInfo *wView = new KexiDataAwareWidgetInfo(this);
	wView->setPixmap("form");
	wView->setClassName("KexiDBForm");
	wView->setName(i18n("Form"));
	wView->setNamePrefix(
		i18n("Widget name. This string will be used to name widgets of this class. It must _not_ contain white spaces and non latin1 characters.", "form"));
	wView->setDescription(i18n("A data-aware form widget"));
	addClass(wView);

#ifndef KEXI_NO_SUBFORM
	KexiDataAwareWidgetInfo *wSubForm = new KexiDataAwareWidgetInfo(this);
	wSubForm->setPixmap("subform");
	wSubForm->setClassName("KexiSubForm");
	wSubForm->setName(i18n("Sub Form"));
	wSubForm->setNamePrefix(
		i18n("Widget name. This string will be used to name widgets of this class. It must _not_ contain white spaces and non latin1 characters.", "subForm"));
	wSubForm->setDescription(i18n("A form widget included in another Form"));
	wSubForm->setAutoSyncForProperty( "formName", false );
	addClass(wSubForm);
#endif
	KFormDesigner::WidgetInfo *wi;

	// inherited
	wi = new KexiDataAwareWidgetInfo(this, "stdwidgets", "KLineEdit");
	wi->setPixmap("lineedit");
	wi->setClassName("KexiDBLineEdit");
	wi->addAlternateClassName("QLineEdit", true/*override*/);
	wi->addAlternateClassName("KLineEdit", true/*override*/);
	wi->setIncludeFileName("klineedit.h");
	wi->setName(i18n("Text Box"));
	wi->setNamePrefix(
		i18n("Widget name. This string will be used to name widgets of this class. It must _not_ contain white spaces and non latin1 characters.", "textBox"));
	wi->setDescription(i18n("A widget for entering and displaying text"));
	addClass(wi);

	// inherited
	wi = new KexiDataAwareWidgetInfo(this, "stdwidgets", "KTextEdit");
	wi->setPixmap("textedit");
	wi->setClassName("KexiDBTextEdit");
	wi->addAlternateClassName("QTextEdit", true/*override*/);
	wi->addAlternateClassName("KTextEdit", true/*override*/);
	wi->setIncludeFileName("ktextedit.h");
	wi->setName(i18n("Text Editor"));
	wi->setNamePrefix(
		i18n("Widget name. This string will be used to name widgets of this class. It must _not_ contain white spaces and non latin1 characters.", "textEditor"));
	wi->setDescription(i18n("A multiline text editor"));
	addClass(wi);

	wi = new KexiDataAwareWidgetInfo(
		this, "stdwidgets", "QLabel" /*we're inheriting to get i18n'd strings already translated there*/);
	wi->setPixmap("label");
	wi->setClassName("KexiLabel");
	wi->addAlternateClassName("QLabel", true/*override*/);
	wi->setName(i18n("Text Label", "Label"));
	wi->setNamePrefix(
		i18n("Widget name. This string will be used to name widgets of this class. It must _not_ contain white spaces and non latin1 characters.", "label"));
	wi->setDescription(i18n("A widget for displaying text"));
	addClass(wi);

	wi = new KexiDataAwareWidgetInfo(this, "stdwidgets", "QCheckBox");
	wi->setPixmap("check");
	wi->setClassName("KexiDBCheckBox");
	wi->addAlternateClassName("QCheckBox", true/*override*/);
	wi->setName(i18n("Check Box"));
	wi->setNamePrefix(
		i18n("Widget name. This string will be used to name widgets of this class. It must _not_ contain white spaces and non latin1 characters.", "checkBox"));
	wi->setDescription(i18n("A check box with text label"));
	addClass(wi);

	KexiDataAwareWidgetInfo *wFieldEdit = new KexiDataAwareWidgetInfo(this);
	wFieldEdit->setPixmap("edit");
	wFieldEdit->setClassName("KexiDBFieldEdit");
	wFieldEdit->setName(i18n("Auto Field"));
	wFieldEdit->setNamePrefix(
		i18n("Widget name. This string will be used to name widgets of this class. It must _not_ contain white spaces and non latin1 characters", "autoField"));
	wFieldEdit->setDescription(i18n("A widget containing an automatically chosen editor and a label to edit value database field of any type."));
	addClass(wFieldEdit);

#if KDE_VERSION >= KDE_MAKE_VERSION(3,1,9)
	KexiDataAwareWidgetInfo *wDate = new KexiDataAwareWidgetInfo(this, "stdwidgets", "KDateWidget");
#else
	KexiDataAwareWidgetInfo *wDate = new KexiDataAwareWidgetInfo(this, "stdwidgets", "QDateEdit");
#endif
	wDate->setPixmap("dateedit");
	wDate->setClassName("KexiDBDateEdit");
	wDate->addAlternateClassName("QDateEdit", true/*override*/);
	wDate->addAlternateClassName("KDateWidget", true/*override*/);
	wDate->setName(i18n("Date Widget"));
	wDate->setNamePrefix(
		i18n("Widget name. This string will be used to name widgets of this class. It must _not_ contain white spaces and non latin1 characters.", "dateWidget"));
	wDate->setDescription(i18n("A widget to input and display a date"));
	addClass(wDate);

#if KDE_VERSION >= KDE_MAKE_VERSION(3,1,9)
	KexiDataAwareWidgetInfo *wTime = new KexiDataAwareWidgetInfo(this, "stdwidgets", "KTimeWidget");
#else
	KexiDataAwareWidgetInfo *wTime = new KexiDataAwareWidgetInfo(this, "stdwidgets", "QTimeEdit");
#endif
	wTime->setPixmap("timeedit");
	wTime->setClassName("KexiDBTimeEdit");
	wTime->addAlternateClassName("QTimeEdit", true/*override*/);
	wTime->addAlternateClassName("KTimeWidget", true/*override*/);
	wTime->setName(i18n("Time Widget"));
	wTime->setNamePrefix(
		i18n("Widget name. This string will be used to name widgets of this class. It must _not_ contain white spaces and non latin1 characters.", "timeWidget"));
	wTime->setDescription(i18n("A widget to input and display a time"));
	addClass(wTime);

#if KDE_VERSION >= KDE_MAKE_VERSION(3,1,9)
	KexiDataAwareWidgetInfo *wDateTime = new KexiDataAwareWidgetInfo(this, "stdwidgets", "KDateTimeWidget");
#else
	KexiDataAwareWidgetInfo *wDateTime = new KexiDataAwareWidgetInfo(this, "stdwidgets", "KDateTimeWidget");
#endif
	wDateTime->setPixmap("datetimeedit");
	wDateTime->setClassName("KexiDBDateTimeEdit");
	wDateTime->addAlternateClassName("QDateTimeEdit", true/*override*/);
	wDateTime->addAlternateClassName("KDateTimeWidget", true/*override*/);
	wDateTime->setName(i18n("Date/Time Widget"));
	wDateTime->setNamePrefix(
		i18n("Widget name. This string will be used to name widgets of this class. It must _not_ contain white spaces and non latin1 characters.", "dateTimeWidget"));
	wDateTime->setDescription(i18n("A widget to input and display a time and a date"));
	addClass(wDateTime);

	KexiDataAwareWidgetInfo *wIntSpinBox = new KexiDataAwareWidgetInfo(this, "stdwidgets", "KIntSpinBox");
	wIntSpinBox->setPixmap("spin");
	wIntSpinBox->setClassName("KexiDBIntSpinBox");
	wIntSpinBox->addAlternateClassName("QSpinBox", true);
	wIntSpinBox->addAlternateClassName("KIntSpinBox", true);
	wIntSpinBox->setName(i18n("Integer Spin Box"));
	wIntSpinBox->setNamePrefix(
		i18n("Widget name. This string will be used to name widgets of this class. It must _not_ contain white spaces and non latin1 characters.", "intSpinBox"));
	wIntSpinBox->setDescription(i18n("A spin box widget to enter integers"));
	addClass(wIntSpinBox);

	KexiDataAwareWidgetInfo *wDoubleSpinBox = new KexiDataAwareWidgetInfo(this, "stdwidgets");
	wDoubleSpinBox->setPixmap("spin");
	wDoubleSpinBox->setClassName("KexiDBDoubleSpinBox");
	wDoubleSpinBox->addAlternateClassName("KDoubleSpinBox", true);
	wDoubleSpinBox->setName(i18n("Decimal Spin Box"));
	wDoubleSpinBox->setNamePrefix(
		i18n("Widget name. This string will be used to name widgets of this class. It must _not_ contain white spaces and non latin1 characters.", "dblSpinBox"));
	wDoubleSpinBox->setDescription(i18n("A spin box widget to enter decimals"));
	addClass(wDoubleSpinBox);

	// inherited
	wi = new KFormDesigner::WidgetInfo(
		this, "stdwidgets", "KPushButton");
	wi->addAlternateClassName("KexiPushButton");
	wi->setName(i18n("Command Button"));
	wi->setNamePrefix(
		i18n("Widget name. This string will be used to name widgets of this class. It must _not_ contain white spaces and non latin1 characters.", "button"));
	wi->setDescription(i18n("A command button to execute actions"));
	addClass(wi);

	m_propDesc["dataSource"] = i18n("Data Source");
	m_propDesc["formName"] = i18n("Form Name");
	m_propDesc["onClickAction"] = i18n("On Click");
	m_propDesc["autoTabStops"] = i18n("Auto Tab Stops");
	m_propDesc["shadowEnabled"] = i18n("Shadow Enabled");

	m_propDesc["widgetType"] = i18n("Editor Type");
	//for autofield's type: inherit i18n from KexiDB
	m_propValDesc["Auto"] = i18n("AutoField editor's type", "Auto"); 
	m_propValDesc["Text"] = KexiDB::Field::typeName(KexiDB::Field::Text);
	m_propValDesc["Integer"] = KexiDB::Field::typeName(KexiDB::Field::Integer);
	m_propValDesc["Double"] = KexiDB::Field::typeName(KexiDB::Field::Double);
	m_propValDesc["Boolean"] = KexiDB::Field::typeName(KexiDB::Field::Boolean);
	m_propValDesc["Date"] = KexiDB::Field::typeName(KexiDB::Field::Date);
	m_propValDesc["Time"] = KexiDB::Field::typeName(KexiDB::Field::Time);
	m_propValDesc["DateTime"] = KexiDB::Field::typeName(KexiDB::Field::DateTime);
	m_propValDesc["MultiLineText"] = i18n("AutoField editor's type", "Multiline Text");
	m_propValDesc["Enum"] = i18n("AutoField editor's type", "List of Values"); 

//	m_propDesc["labelCaption"] = i18n("Label Text");
	m_propDesc["autoCaption"] = i18n("Auto Label");

	m_propDesc["labelPosition"] = i18n("Label Position");
	m_propValDesc["Left"] = i18n("Label Position", "Left");
	m_propValDesc["Top"] = i18n("Label Position", "Top");
	m_propValDesc["NoLabel"] = i18n("Label Position", "No Label");

#ifdef KEXI_NO_UNFINISHED
	//we don't want not-fully implemented/usable classes:
	hideClass("KexiPictureLabel");
	//hideClass("KIntSpinBox");
	hideClass("KComboBox");
#endif
}

KexiDBFactory::~KexiDBFactory()
{
}

QWidget*
KexiDBFactory::create(const QCString &c, QWidget *p, const char *n, KFormDesigner::Container *container,
	WidgetFactory::OrientationHint)
{
	kexipluginsdbg << "KexiDBFactory::create() " << this << endl;

	QWidget *w=0;
	QString text = container->form()->manager()->lib()->textForWidgetName(n, c);

	if(c == "KexiSubForm")
		w = new KexiSubForm(container->form(), p, n);
	else if(c == "KexiDBLineEdit")
	{
		w = new KexiDBLineEdit(p, n);
		w->setCursor(QCursor(Qt::ArrowCursor));
	}
	else if(c == "KexiDBTextEdit")
	{
		w = new KexiDBTextEdit(p, n);
		w->setCursor(QCursor(Qt::ArrowCursor));
	}
	else if(c == "KexiLabel")
		w = new KexiLabel(text, p, n);
	else if(c == "KexiDBFieldEdit")
		w = new KexiDBFieldEdit(p, n);
	else if(c == "KexiDBCheckBox")
		w = new KexiDBCheckBox(text, p, n);
	else if(c == "KexiDBTimeEdit")
		w = new KexiDBTimeEdit(QTime::currentTime(), p, n);
	else if(c == "KexiDBDateEdit")
		w = new KexiDBDateEdit(QDate::currentDate(), p, n);
	else if(c == "KexiDBDateTimeEdit")
		w = new KexiDBDateTimeEdit(QDateTime::currentDateTime(), p, n);
	else if(c == "KexiDBIntSpinBox")
		w = new KexiDBIntSpinBox(p, n);
	else if(c == "KexiDBDoubleSpinBox")
		w = new KexiDBDoubleSpinBox(p, n);
	else if(c == "KPushButton" || c == "KexiPushButton")
		w = new KexiPushButton(text, p, n);

	return w;
}

bool
KexiDBFactory::createMenuActions(const QCString &classname, QWidget *w, QPopupMenu *menu,
		   KFormDesigner::Container *)
{
	if(classname == "QPushButton" || classname == "KPushButton" || classname == "KexiPushButton")
	{
/*! @todo also call createMenuActions() for inherited factory! */
		m_assignAction->plug( menu );
		return true;
	}
	return false;
}

void
KexiDBFactory::createCustomActions(KActionCollection* col)
{
	//this will create shared instance action for design mode (special collection is provided)
	m_assignAction = new KAction( i18n("Assign Action..."), SmallIconSet("form_action"),
		0, 0, 0, col, "widget_assign_action");
}

/*KexiDBFactory::assignAction()
{
	emit executeCustomAction("assignAction", m_widget);
}*/

bool
KexiDBFactory::startEditing(const QCString &classname, QWidget *w, KFormDesigner::Container *container)
{
	m_container = container;
#ifndef Q_WS_WIN
	#warning Is there any reason to edit a lineedit in design-mode?
#endif
	if(classname == "KexiDBLineEdit")
	{
//! @todo this code should not be copied here but
//! just inherited StdWidgetFactory::clearWidgetContent() should be called
		KLineEdit *lineedit = static_cast<KLineEdit*>(w);
		createEditor(classname, lineedit->text(), lineedit, container,
			lineedit->geometry(), lineedit->alignment(), true);
		return true;
	}
	if(classname == "KexiDBTextEdit")
	{
//! @todo this code should not be copied here but
//! just inherited StdWidgetFactory::clearWidgetContent() should be called
		KTextEdit *textedit = static_cast<KTextEdit*>(w);
		createEditor(classname, textedit->text(), textedit, container,
			textedit->geometry(), textedit->alignment(), true, true);
		//copy a few properties
		KTextEdit *ed = dynamic_cast<KTextEdit *>( editor(w) );
		ed->setWrapPolicy(textedit->wrapPolicy());
		ed->setWordWrap(textedit->wordWrap());
		ed->setTabStopWidth(textedit->tabStopWidth());
		ed->setWrapColumnOrWidth(textedit->wrapColumnOrWidth());
		ed->setLinkUnderline(textedit->linkUnderline());
		ed->setTextFormat(textedit->textFormat());
		ed->setHScrollBarMode(textedit->hScrollBarMode());
		ed->setVScrollBarMode(textedit->vScrollBarMode());
		return true;
	}
	else if ( classname == "KexiLabel" ) {
		KexiLabel *label = static_cast<KexiLabel*>(w);
		m_widget = w;
		if(label->textFormat() == RichText)
		{
			QString text = label->text();
			if ( editRichText( label, text ) )
			{
				changeProperty( "textFormat", "RichText", container );
				changeProperty( "text", text, container );
			}

			if ( classname == "KexiLabel" )
				w->resize(w->sizeHint());
		}
		else
		{
			createEditor(classname, label->text(), label, container,
				label->geometry(), label->alignment());
		}
		return true;
	}
	else if (classname == "KexiSubForm") {
		// open the form in design mode
		KexiMainWindow *mainWin = KexiUtils::findParent<KexiMainWindow>(w, "KexiMainWindow");
		KexiSubForm *subform = static_cast<KexiSubForm*>(w);
		if(mainWin)
			mainWin->openObject("kexi/form", subform->formName(), Kexi::DesignViewMode);
	}
	else if( (classname == "KexiDBDateEdit") || (classname == "KexiDBDateTimeEdit") || (classname == "KexiDBTimeEdit")
			|| (classname == "KexiDBIntSpinBox") || (classname == "KexiDBDoubleSpinBox") ) {
		disableFilter(w, container);
		return true;
	}
	else if(classname == "KexiDBFieldEdit") {
		if(static_cast<KexiDBFieldEdit*>(w)->hasAutoCaption())
			return false; // caption is auto, abort editing
		QLabel *label = static_cast<KexiDBFieldEdit*>(w)->label();
		createEditor(classname, label->text(), label, container, label->geometry(), label->alignment());
		return true;
	}
	else if (classname == "KexiDBCheckBox") {
		KexiDBCheckBox *cb = static_cast<KexiDBCheckBox*>(w);
		QRect r( cb->geometry() );
		r.setLeft( r.left() + 2 + cb->style().subRect( QStyle::SR_CheckBoxIndicator, cb ).width() );
		createEditor(classname, cb->text(), cb, container, r, Qt::AlignAuto);
	}
	return false;
}

bool
KexiDBFactory::previewWidget(const QCString &, QWidget *, KFormDesigner::Container *)
{
	return false;
}

bool
KexiDBFactory::clearWidgetContent(const QCString &classname, QWidget *w)
{
//! @todo this code should not be copied here but
//! just inherited StdWidgetFactory::clearWidgetContent() should be called
	KexiFormDataItemInterface *iface = dynamic_cast<KexiFormDataItemInterface*>(w);
	if(iface)
		iface->clear();
	return true;
}

QValueList<QCString>
KexiDBFactory::autoSaveProperties(const QCString &classname)
{
	QValueList<QCString> lst;
//	if(classname == "KexiSubForm")
		//lst << "formName";
//	if(classname == "KexiDBLineEdit")
//	lst += "dataSource";
//	if(classname == "KexiDBFieldEdit")
//		lst << "labelCaption";
	return lst;
}

bool
KexiDBFactory::isPropertyVisibleInternal(const QCString& classname, QWidget *w,
	const QCString& property, bool isTopLevel)
{
	//general
	if (property=="dataSource" || property=="dataSourceMimeType")
		return false;

	if(classname == "KexiPushButton") {
		return property!="isDragEnabled"
#ifdef KEXI_NO_UNFINISHED
			&& property!="onClickAction" /*! @todo reenable */
			&& property!="iconSet" /*! @todo reenable */
			&& property!="stdItem" /*! @todo reenable stdItem */
#endif
			;
	}
	else if(classname == "KexiDBLineEdit")
		return property!="urlDropsEnabled"
			&& property != "vAlign"
#ifdef KEXI_NO_UNFINISHED
			&& property!="inputMask"
			&& property!="maxLength" //!< we may want to integrate this with db schema
#endif
		;
	else if(classname == "KexiDBTextEdit")
		return property!="undoDepth"
			&& property!="undoRedoEnabled" //always true!
			&& property!="dragAutoScroll" //always true!
			&& property!="overwriteMode" //always false!
			&& property!="resizePolicy"
			&& property!="autoFormatting" //too complex
#ifdef KEXI_NO_UNFINISHED
			&& property!="paper"
#endif
			;
	else if(classname == "KexiSubForm")
		return property!="dragAutoScroll"
			&& property!="resizePolicy"
			&& property!="focusPolicy";
	else if(classname == "KexiDBForm")
		return property!="iconText";
	else if(classname == "KexiLabel")
		return property!="focusPolicy";
	else if(classname == "KexiDBFieldEdit") {
		if (!isTopLevel && property=="caption")
			return true;
	}

	return WidgetFactory::isPropertyVisibleInternal(classname, w, property, isTopLevel);
}

bool
KexiDBFactory::changeText(const QString &text)
{
	QCString n = WidgetFactory::widget()->className();
//	QWidget *w = WidgetFactory::widget();
	if(n == "KexiDBFieldEdit") {
		changeProperty("caption", text, m_container);
		return true;
	}
	//! \todo check field's geometry
	return false;
}

void
KexiDBFactory::resizeEditor(QWidget *editor, QWidget *w, const QCString &classname)
{
	//QSize s = widget->size();
	//QPoint p = widget->pos();

	if(classname == "KexiDBFieldEdit")
		editor->setGeometry( static_cast<KexiDBFieldEdit*>(w)->label()->geometry() );
}

K_EXPORT_COMPONENT_FACTORY(kexidbwidgets, KGenericFactory<KexiDBFactory>("kexidbwidgets"))

#include "kexidbfactory.moc"
