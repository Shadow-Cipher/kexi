/* This file is part of the KDE project
   Copyright (C) 2003 Joseph Wenninger <jowenn@kde.org>

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

#ifndef _KEXI_TABLE_IMPORT_FORM_H_
#define _KEXI_TABLE_IMPORT_FORM_H_

#include "kexiimportwizardformbase.h"
#include <kexiDB/kexidbtable.h>

class KexiTableImportSourceIface;
class KexiFilterManager;
class KexiFilter;
class KexiImportWizard;

class KexiTableImportForm: public KexiImportWizardFormBase {
	Q_OBJECT
public:
	KexiTableImportForm(KexiFilterManager *filterManager,KexiImportWizard *wiz,const QString& 
		dialogfilter,QMap<QString,QString> mimePluginMapping);
	virtual ~KexiTableImportForm();
private:
	QWidget *m_previousPage;
        bool m_idColumn;
	KexiDBTable m_sourceTable;
	KexiDBTable m_destinationTable;
	KexiFilterManager *m_filterManager;
	QString m_dialogFilter;
	QMap<QString,QString> m_mimePluginMapping;
	KexiFilter *m_filter;
	bool m_recursiveOpen;
	KexiImportWizard *m_wiz;
	QWidget *m_openWidget;
	bool m_initializing;
	bool m_insertingPages;
	bool m_deleting;
	QPtrList<QWidget> m_sourceWidgets;

	void initDestChoice();
	void buildNewTablePage();
	void buildExistingTablePage();
	bool createTable();
	bool importValues(const QString& tableName, int mapLen, int mapping[]);
	void initFileDialog();
protected:
	virtual void accept();
protected slots:
	void pageSelected(const QString&);
	void changeDestinationType(bool);
	void loadPlugin();
public:
	void setMode(unsigned long);
public slots:
	void filterHasBeenLoaded(KexiFilter*,const KURL &);
};


#endif
