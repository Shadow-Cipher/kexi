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

#include <qfileinfo.h>

#include <kdebug.h>
#include <kinstance.h>
#include <kapplication.h>
#include <kiconloader.h>

#include <kexidb/drivermanager.h>
#include <kexidb/driver.h>
#include <kexidb/connection.h>
#include <kexidb/cursor.h>
#include <kexidb/field.h>
#include <kexidb/tableschema.h>
#include <kexidb/queryschema.h>
#include <kexidb/indexschema.h>

QCString prgname;
QCString db_name;
QCString drv_name;
QCString test_name;

KexiDB::ConnectionData conn_data;
KexiDB::Connection *conn = 0;
KInstance *instance;
KApplication *app;

void usage()
{
	kdDebug() << "usage: " << endl;
	kdDebug() << prgname << " <driver_name> cursors <db_name>" << endl;
	kdDebug() << "  - test for cursors behaviour" << endl;
	kdDebug() << prgname << " <driver_name> schema <db_name>" << endl;
	kdDebug() << "  - test for db schema retrieving" << endl;
	kdDebug() << prgname << " <driver_name> dbcreation <new_db_name>" << endl;
	kdDebug() << "  - test for new db creation" << endl;
	kdDebug() << prgname << " <driver_name> tables <new_db_name>" << endl;
	kdDebug() << "  - test for tables creation and data inserting" << endl;
	kdDebug() << prgname << " <driver_name> tableview <db_name>" << endl;
	kdDebug() << "  - test for KexiDataTableView data-aware widget" << endl;
	kdDebug() << " Notes:\n 'tables' test, automatically runs 'dbcreation' test.\n" 
		<<" <new_db_name> is removed if already exists.\n"
		<<" <db_name> must be a valid kexi database e.g. created with 'tables' test." 
		<< endl;
}

#include "dbcreation_test.h"
#include "cursors_test.h"
#include "schema_test.h"
#include "tables_test.h"
#include "tableview_test.h"


int main(int argc, char** argv)
{
	if (argc<=2) {
		usage();
		return 0;
	}
	QFileInfo info=QFileInfo(argv[0]);
	prgname = info.baseName().latin1();
	drv_name = argv[1];
	test_name = QString(argv[2]).lower().latin1();

	if (test_name=="tableview") {
		//GUI test
		app = new KApplication(argc, argv, prgname);
		KGlobal::iconLoader()->addAppDir("kexi");
    		instance = app;
	}
	else {
		//CLI test
		instance = new KInstance( prgname );
	}

	KexiDB::DriverManager *manager = new KexiDB::DriverManager;
	QStringList names = manager->driverNames();
	kdDebug() << "DRIVERS: " << endl;
	for (QStringList::iterator it = names.begin(); it != names.end() ; ++it)
		kdDebug() << *it << endl;
	if (manager->error()) {
		manager->debugError();
		return 1;
	}

	//get driver
	KexiDB::Driver *driver = manager->driver(drv_name);
	if (manager->error()) {
		manager->debugError();
		return 1;
	}
	kdDebug() << "MIME type for '" << driver->name() << "': " << driver->fileDBDriverMime() << endl;

//connection data
//	conn_data.host = "myhost";
//	conn_data.password = "mypwd";

//open connection
/*	if (test_name == "cursors")
		db_name = "mydb";
	else if (test_name == "schema")
		db_name = "db";
	else {*/
		if (argc<=3) {
			kdDebug() << prgname << ": name for new db?" << endl;
			usage();
			return 0;
		}
		db_name = QCString(argv[3]);
//	}
	conn_data.setFileName( db_name );

	conn = driver->createConnection(conn_data);

	if (driver->error()) {
		driver->debugError();
		return 1;
	}
	if (!conn->connect()) {
		conn->debugError();
		return 1;
	}

//start test:
	int r=0;
	if (test_name == "cursors")
		r=cursorsTest();
	else if (test_name == "schema")
		r=schemaTest();
	else if (test_name == "dbcreation")
		r=dbCreationTest();
	else if (test_name == "tables")
		r=tablesTest();
	else if (test_name == "tableview")
		r=tableViewTest();
	else {
		kdDebug() << "No such test: " << test_name << endl;
		usage();
		return 1;
	}
	kdDebug()<< test_name << " TEST: " << (r==0?"PASSED":"ERROR") << endl;

	if (app && r==0)
		app->exec();

	r = conn ? (conn->disconnect() ? 0 : 1) : 0;
	
	kdDebug() << "!!! KexiDB::Transaction::globalcount == " << KexiDB::Transaction::globalCount() << endl;
	kdDebug() << "!!! KexiDB::TransactionData::globalcount == " << KexiDB::TransactionData::globalCount() << endl;

	delete manager;
	delete instance;

	return r;
}

