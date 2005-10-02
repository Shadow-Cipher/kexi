/* This file is part of the KDE project
   Copyright (C) 2004 Martin Ellis <m.a.ellis@ncl.ac.uk>
 
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

#include "mysqlmigrate.h"

#include <qstring.h>
#include <qregexp.h>
#include <qfile.h>
#include <qvariant.h>
#include <qvaluelist.h>
#include <kdebug.h>

#include <mysql.h>
#include "migration/keximigratedata.h"
#include "kexidb/cursor.h"
#include "kexidb/field.h"
#include "kexidb/drivers/mySQL/mysqlconnection_p.cpp"

using namespace KexiMigration;

/* This is the implementation for the MySQL specific import routines. */

KEXIMIGRATE_DRIVER_INFO( MySQLMigrate, mysql );

/* ************************************************************************** */
//! Constructor
MySQLMigrate::MySQLMigrate() :
	d(new MySqlConnectionInternal())
{
}

//! Constructor (needed for trading interface)
MySQLMigrate::MySQLMigrate(QObject *parent, const char *name,
                           const QStringList &args) :
	KexiMigrate(parent, name, args),
	d(new MySqlConnectionInternal()) {
}

/* ************************************************************************** */
//! Destructor
MySQLMigrate::~MySQLMigrate() {
}


/* ************************************************************************** */
/*! Connect to the db backend */
bool MySQLMigrate::drv_connect() {
  if(d->db_connect(*m_migrateData->source)) {
		return d->useDatabase(m_migrateData->sourceName);
	} else {
	  return false;
	}
}


/*! Disconnect from the db backend */
bool MySQLMigrate::drv_disconnect()
{
	return d->db_disconnect();
}


/* ************************************************************************** */
/*! Get the types and properties for each column. */
bool MySQLMigrate::drv_readTableSchema(const QString table)
{
	m_table = new KexiDB::TableSchema(table);

	//TODO IDEA: ask for user input for captions
	m_table->setCaption(table + " table");

	//Perform a query on the table to get some data
	QString query = QString("SELECT * FROM `" + d->escapeIdentifier(table) + 
	                        "` LIMIT 0");
	if(d->executeSQL(query)) {
		MYSQL_RES *res = mysql_store_result(d->mysql);
		if (res != NULL) {
		
			unsigned int numFlds = mysql_num_fields(res);
			MYSQL_FIELD *fields = mysql_fetch_fields(res); 
			
			for(unsigned int i = 0; i < numFlds; i++) {
				QString fldName = QString(fields[i].name);
				KexiDB::Field *fld = 
				  new KexiDB::Field(fldName, type(table, &fields[i]));
				
				if(fld->type() == KexiDB::Field::Enum) {
					QStringList values = examineEnumField(table, &fields[i]);
				}
				
				fld->setCaption(fldName);
				getConstraints(fields[i].flags, fld);
				getOptions(fields[i].flags, fld);
				m_table->addField(fld);
			}
			mysql_free_result(res);
		} else {
			kdDebug() << "MySQLMigrate::drv_tableNames: null result" << endl;
		}
		return true;
	} else {
	  return false;
	}
}


/*! Get a list of tables and put into the supplied string list */
bool MySQLMigrate::drv_tableNames(QStringList& tableNames)
{
	if(d->executeSQL("SHOW TABLES")) {
		MYSQL_RES *res = mysql_store_result(d->mysql);
		if (res != NULL) {
			MYSQL_ROW row;
			while ((row = mysql_fetch_row(res)) != NULL) {
			  tableNames << QString(row[0]);
			}
			mysql_free_result(res);
		} else {
			kdDebug() << "MySQLMigrate::drv_tableNames: null result" << endl;
		}
		return true;
	} else {
	  return false;
	}
}


/*! Copy MySQL table to KexiDB database */
bool MySQLMigrate::drv_copyTable(const QString& srcTable,
                                 KexiDB::TableSchema* dstTable) {
	if(d->executeSQL("SELECT * FROM " + d->escapeIdentifier(srcTable))) {
		MYSQL_RES *res = mysql_use_result(d->mysql);
		if (res != NULL) {
			MYSQL_ROW row;
			while ((row = mysql_fetch_row(res)) != NULL) {
				int numFields = mysql_num_fields(res);
				QValueList<QVariant> vals = QValueList<QVariant>();
				for(int i = 0; i < numFields; i++) {
					QVariant var = QVariant(row[i]);
					vals << var;
				}
				m_migrateData->dest->insertRecord(*dstTable, vals);
				progressDoneRow();
			}
			/*! @todo Check that wasn't an error, rather than end of result set */
			mysql_free_result(res);
		} else {
			kdDebug() << "MySQLMigrate::drv_copyTable: null result" << endl;
		}
		return true;
	} else {
		return false;
	}
}


bool MySQLMigrate::drv_getTableSize(const QString& table, Q_ULLONG& size) {
	if(d->executeSQL("SELECT COUNT(*) FROM " + d->escapeIdentifier(table))) {
		MYSQL_RES *res = mysql_store_result(d->mysql);
		if (res != NULL) {
			MYSQL_ROW row;
			while ((row = mysql_fetch_row(res)) != NULL) {
				//! @todo check result valid
				size = QString(row[0]).toULongLong();
			}
			mysql_free_result(res);
		} else {
			kdDebug() << "MySQLMigrate::drv_getTableSize: null result" << endl;
		}
		return true;
	} else {
	  return false;
	}
}

//! Convert a MySQL type to a KexiDB type, prompting user if necessary.
KexiDB::Field::Type MySQLMigrate::type(const QString& table,
                                       const MYSQL_FIELD *fld)
{
  // Field type
	KexiDB::Field::Type kexiType = KexiDB::Field::InvalidType;

	switch(fld->type)
	{
  // These are in the same order as mysql_com.h.
	// MySQL names given on the right
		case FIELD_TYPE_DECIMAL:    // DECIMAL or NUMERIC
			break;
		case FIELD_TYPE_TINY:       // TINYINT (-2^7..2^7-1 or 2^8)
			kexiType = KexiDB::Field::Byte;
			break;
		case FIELD_TYPE_SHORT:      // SMALLINT (-2^15..2^15-1 or 2^16)
			kexiType = KexiDB::Field::ShortInteger;
			break;
		case FIELD_TYPE_LONG:       // INTEGER (-2^31..2^31-1 or 2^32)
			kexiType = KexiDB::Field::Integer;
			break;
		case FIELD_TYPE_FLOAT:      // FLOAT
			kexiType = KexiDB::Field::Float;
			break;
		case FIELD_TYPE_DOUBLE:     // DOUBLE or REAL (8 byte)
			kexiType = KexiDB::Field::Double;
			break;
		case FIELD_TYPE_NULL:       // WTF?
			break;
		case FIELD_TYPE_TIMESTAMP:  // TIMESTAMP (promote?)
			kexiType = KexiDB::Field::DateTime;
			break;
		case FIELD_TYPE_LONGLONG:   // BIGINT (-2^63..2^63-1 or 2^64)
		case FIELD_TYPE_INT24:      // MEDIUMINT (-2^23..2^23-1 or 2^24) (promote)
			kexiType = KexiDB::Field::BigInteger;
			break;
		case FIELD_TYPE_DATE:       // DATE
			kexiType = KexiDB::Field::Date;
			break;
		case FIELD_TYPE_TIME:       // TIME
			kexiType = KexiDB::Field::Time;
			break;
		case FIELD_TYPE_DATETIME:   // DATETIME
			kexiType = KexiDB::Field::DateTime;
			break;
		case FIELD_TYPE_YEAR:       // YEAR (promote)
			kexiType = KexiDB::Field::ShortInteger;
			break;
		case FIELD_TYPE_NEWDATE:    // WTF?
		case FIELD_TYPE_ENUM:       // ENUM
			// If MySQL did what it's documentation said it did, we would come here
			// for enum fields ...
			kexiType = KexiDB::Field::Enum;
			break;
		case FIELD_TYPE_SET:        // SET
			//! @todo: Support set column type
			break;
		case FIELD_TYPE_TINY_BLOB:
		case FIELD_TYPE_MEDIUM_BLOB:
		case FIELD_TYPE_LONG_BLOB:
		case FIELD_TYPE_BLOB:       // BLOB or TEXT
		case FIELD_TYPE_VAR_STRING: // VARCHAR
		case FIELD_TYPE_STRING:     // CHAR
			
			if (fld->flags & ENUM_FLAG) {
				// ... instead we come here, using the ENUM_FLAG which is supposed to
				// be deprecated! Duh.
				kexiType = KexiDB::Field::Enum;
				break;
			}
			kexiType = examineBlobField(table, fld);
			break;	
		default:
		  kexiType = KexiDB::Field::InvalidType;
	}

	if (kexiType == KexiDB::Field::InvalidType) {
		return userType(table);
	}
	return kexiType;
}


//! Distinguish between a BLOB and a TEXT field
/*! MySQL uses the same field type to identify BLOB and TEXT fields.
    This method queries the server to find out if a field is a binary
    field or a text field.  It also considers the length of CHAR and VARCHAR
    fields to see whether Text or LongText is the appropriate Kexi field type.
    Assumes fld is a CHAR, VARCHAR, one of the BLOBs or TEXTs.
    \return KexiDB::Field::Text, KexiDB::Field::LongText or KexiDB::Field::BLOB
*/
KexiDB::Field::Type MySQLMigrate::examineBlobField(const QString& table,
    const MYSQL_FIELD* fld) {
	QString mysqlType;
	KexiDB::Field::Type kexiType;
	QString query = "SHOW COLUMNS FROM `" + d->escapeIdentifier(table) + 
	                "` LIKE '" + QString::fromLatin1(fld->name) + "'";

	if(d->executeSQL(query)) {
		MYSQL_RES *res = mysql_store_result(d->mysql);

		if (res != NULL) {
			MYSQL_ROW row;
			while ((row = mysql_fetch_row(res)) != NULL) {
				mysqlType = QString(row[1]);
			}
			mysql_free_result(res);
		} else {
			kdDebug() << "MySQLMigrate::examineBlobField: null result" << endl;
		}
	} else {
		// Huh? MySQL wont tell us what kind of field it is! Lets guess.
	  return KexiDB::Field::LongText;
	}

	kdDebug() << "MySQLMigrate::examineBlobField: considering "
	          << mysqlType << endl;
	if(mysqlType.contains("blob", false) != 0) {
		// Doesn't matter how big it is, it's binary
		kexiType = KexiDB::Field::BLOB;
	} else if(mysqlType.contains("text", false) != 0) {
		// All the TEXT types are too big for Kexi text.
		kexiType = KexiDB::Field::BLOB;
	} else if(fld->length < 200) {
		kexiType = KexiDB::Field::Text;
	} else {
		kexiType = KexiDB::Field::LongText;
	}
	return kexiType;
}


//! Get the strings that identify values in an enum field
/*! Parse the type of a MySQL enum field as returned by the server in a 
    'DESCRIBE table' or 'SHOW COLUMNS FROM table' statement.  The string
    returned by the server is in the form 'enum('option1','option2').
    In this example, the result should be a string list containing two
    strings, "option1", "option2".
    \return list of possible values the field can take
 */
QStringList MySQLMigrate::examineEnumField(const QString& table,
		const MYSQL_FIELD* fld) {
	QString vals;
	QString query = "SHOW COLUMNS FROM `" + d->escapeIdentifier(table) + 
			"` LIKE '" + QString::fromLatin1(fld->name) + "'";

	if(d->executeSQL(query)) {
		MYSQL_RES *res = mysql_store_result(d->mysql);

		if (res != NULL) {
			MYSQL_ROW row;
			while ((row = mysql_fetch_row(res)) != NULL) {
				vals = QString(row[1]);
			}
			mysql_free_result(res);
		} else {
			kdDebug() << "MySQLMigrate::examineEnumField: null result" << endl;
		}
	} else {
		// Huh? MySQL wont tell us what values it can take.
		return QStringList();
	}

	kdDebug() << "MySQLMigrate::examineEnumField: considering " 
						<< vals << endl;
	
	// Crash and burn if we get confused...
	if(!vals.startsWith("enum(")) {
		// Huh? We're supposed to be parsing an enum!
		kdDebug() << "MySQLMigrate::examineEnumField:1 not an enum!" << endl;
		return QStringList();
	}
	if(!vals.endsWith(")")) {
		kdDebug() << "MySQLMigrate::examineEnumField:2 not an enum!" << endl;
		return QStringList();
	}
	
	// It'd be nice to use QString.section or QStringList.split, but we need
	// to be careful as enum values can have commas and quote marks in them
	// e.g. CREATE TABLE t(f enum('option,''') gives one option: "option,'"
	vals = vals.remove(0,5);
	QRegExp rx = QRegExp("^'((?:[^,']|,|'')*)'");
	QStringList values = QStringList();
	int index = 0;

	while ((index = rx.search(vals, index, QRegExp::CaretAtOffset)) != -1) {
		int len = rx.matchedLength();
		if (len != -1) {
			kdDebug() << "MySQLMigrate::examineEnumField:3 " << rx.cap(1) << endl;
			values << rx.cap(1);
		} else {
			kdDebug() << "MySQLMigrate::examineEnumField:4 lost" << endl;
		}
		
		QChar next = vals[index + len];
		if (next != QChar(',') && next != QChar(')')) {
			kdDebug() << "MySQLMigrate::examineEnumField:5 " << (char)next << endl;
		}
		index += len + 1;
	}

	return values;
}


void MySQLMigrate::getConstraints(int flags, KexiDB::Field* fld) {
	fld->setPrimaryKey(flags & PRI_KEY_FLAG);
	fld->setAutoIncrement(flags & AUTO_INCREMENT_FLAG);
	fld->setNotNull(flags & NOT_NULL_FLAG);
	fld->setUniqueKey(flags & UNIQUE_KEY_FLAG);
	//! @todo: Keys and uniqueness
}


void MySQLMigrate::getOptions(int flags, KexiDB::Field* fld) {
	fld->setUnsigned(flags & UNSIGNED_FLAG);
}


#include "mysqlmigrate.moc"
