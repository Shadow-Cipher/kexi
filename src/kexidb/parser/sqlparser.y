/* This file is part of the KDE project
   Copyright (C) 2004 Lucijan Busch <lucijan@kde.org>
   Copyright (C) 2004 Jaroslaw Staniek <js@iidea.pl>

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

%token UMINUS

%token SQL_TYPE
%token SQL_ABS
%token ACOS
%token AMPERSAND
%token SQL_ABSOLUTE
%token ACOS
%token ADA
%token ADD
%token ADD_DAYS
%token ADD_HOURS
%token ADD_MINUTES
%token ADD_MONTHS
%token ADD_SECONDS
%token ADD_YEARS
%token ALL
%token ALLOCATE
%token ALTER
%token AND
%token ANY
%token ARE
%token AS
%token ASIN
%token ASC
%token ASCII
%token ASSERTION
%token ATAN
%token ATAN2
%token AUTHORIZATION
%token AUTO_INCREMENT
%token AVG
%token BEFORE
%token SQL_BEGIN
%token BETWEEN
%token BIGINT
%token BINARY
%token BIT
%token BIT_LENGTH
%token BITWISE_SHIFT_LEFT
%token BITWISE_SHIFT_RIGHT
%token BREAK
%token BY
%token CASCADE
%token CASCADED
%token CASE
%token CAST
%token CATALOG
%token CEILING
%token CENTER
%token SQL_CHAR
%token CHAR_LENGTH
%token CHARACTER_STRING_LITERAL
%token CHECK
%token CLOSE
%token COALESCE
%token COBOL
%token COLLATE
%token COLLATION
%token COLUMN
%token COMMIT
%token COMPUTE
%token CONCAT
%token CONCATENATION /* || */
%token CONNECT
%token CONNECTION
%token CONSTRAINT
%token CONSTRAINTS
%token CONTINUE
%token CONVERT
%token CORRESPONDING
%token COS
%token COT
%token COUNT
%token CREATE
%token CURDATE
%token CURRENT
%token CURRENT_DATE
%token CURRENT_TIME
%token CURRENT_TIMESTAMP
%token CURTIME
%token CURSOR
%token DATABASE
%token SQL_DATE
%token DATE_FORMAT
%token DATE_REMAINDER
%token DATE_VALUE
%token DAY
%token DAYOFMONTH
%token DAYOFWEEK
%token DAYOFYEAR
%token DAYS_BETWEEN
%token DEALLOCATE
%token DEC
%token DECLARE
%token DEFAULT
%token DEFERRABLE
%token DEFERRED
%token SQL_DELETE
%token DESC
%token DESCRIBE
%token DESCRIPTOR
%token DIAGNOSTICS
%token DICTIONARY
%token DIRECTORY
%token DISCONNECT
%token DISPLACEMENT
%token DISTINCT
%token DOMAIN_TOKEN
%token SQL_DOUBLE
%token DOUBLE_QUOTED_STRING
%token DROP
%token ELSE
%token END
%token END_EXEC
%token EQUAL
%token ESCAPE
%token EXCEPT
%token SQL_EXCEPTION
%token EXEC
%token EXECUTE
%token EXISTS
%token EXP
%token EXPONENT
%token EXTERNAL
%token EXTRACT
%token SQL_FALSE
%token FETCH
%token FIRST
%token SQL_FLOAT
%token FLOOR
%token FN
%token FOR
%token FOREIGN
%token FORTRAN
%token FOUND
%token FOUR_DIGITS
%token FROM
%token FULL
%token GET
%token GLOBAL
%token GO
%token GOTO
%token GRANT
%token GREATER_OR_EQUAL
//%token GREATER_THAN
//conflict %token GROUP
%token HAVING
%token HOUR
%token HOURS_BETWEEN
%token IDENTITY
%token IFNULL
%token SQL_IGNORE
%token IMMEDIATE
%token SQL_IN
%token INCLUDE
%token INDEX
%token INDICATOR
%token INITIALLY
%token INNER
%token INPUT
%token INSENSITIVE
%token INSERT
%token INTEGER
%token INTERSECT
%token INTERVAL
%token INTO
%token IS
%token ISOLATION
%token JOIN
%token JUSTIFY
%token KEY
%token LANGUAGE
%token LAST
%token LCASE
%token LEFT
%token LENGTH
%token LESS_OR_EQUAL
//%token LESS_THAN
%token LEVEL
%token LIKE
%token LINE_WIDTH
%token LOCAL
%token LOCATE
%token LOG
%token SQL_LONG
%token LOWER
%token LTRIM
%token LTRIP
%token MATCH
%token SQL_MAX
%token MICROSOFT
%token SQL_MIN
%token MINUS
%token MINUTE
%token MINUTES_BETWEEN
%token MOD
%token MODIFY
%token MODULE
%token MONTH
%token MONTHS_BETWEEN
%token MUMPS
%token NAMES
%token NATIONAL
%token NCHAR
%token NEXT
%token NODUP
%token NONE
%token NOT
%token NOT_EQUAL //<>
%token NOT_EQUAL2 //!=
%token NOW
%token SQL_NULL
%token SQL_IS
%token SQL_IS_NULL /*helper */
%token SQL_IS_NOT_NULL /*helper */
%token NULLIF
%token NUMERIC
%token OCTET_LENGTH
%token ODBC
%token OF
%token SQL_OFF
%token SQL_ON
%token ONLY
%token OPEN
%token OPTION
%token OR
%token ORDER
%token OUTER
%token OUTPUT
%token OVERLAPS
%token PAGE
%token PARTIAL
%token SQL_PASCAL
%token PERSISTENT
%token CQL_PI
%token PLI
%token POSITION
%token PRECISION
%token PREPARE
%token PRESERVE
%token PRIMARY
%token PRIOR
%token PRIVILEGES
%token PROCEDURE
%token PRODUCT
%token PUBLIC
%token QUARTER
%token QUIT
%token RAND
%token READ_ONLY
%token REAL
%token REFERENCES
%token REPEAT
%token REPLACE
%token RESTRICT
%token REVOKE
%token RIGHT
%token ROLLBACK
%token ROWS
%token RPAD
%token RTRIM
%token SCHEMA
%token SCREEN_WIDTH
%token SCROLL
%token SECOND
%token SECONDS_BETWEEN
%token SELECT
%token SEQUENCE
%token SETOPT
%token SET
%token SHOWOPT
%token SIGN
//%token SIMILAR
%token SIMILAR_TO /* helper */
%token NOT_SIMILAR_TO /* helper */
%token INTEGER_CONST
%token REAL_CONST
%token DATE_CONST
%token DATETIME_CONST
%token TIME_CONST
%token SIN
%token SQL_SIZE
%token SMALLINT
%token SOME
%token SPACE
%token SQL
%token SQL_TRUE
%token SQLCA
%token SQLCODE
%token SQLERROR
%token SQLSTATE
%token SQLWARNING
%token SQRT
%token STDEV
%token SUBSTRING
%token SUM
%token SYSDATE
%token SYSDATE_FORMAT
%token SYSTEM
%token TABLE
%token TAN
%token TEMPORARY
%token THEN
%token THREE_DIGITS
%token TIME
%token TIMESTAMP
%token TIMEZONE_HOUR
%token TIMEZONE_MINUTE
%token TINYINT
%token TO
%token TO_CHAR
%token TO_DATE
%token TRANSACTION
%token TRANSLATE
%token TRANSLATION
%token TRUNCATE
%token GENERAL_TITLE
%token TWO_DIGITS
%token UCASE
%token UNION
%token UNIQUE
%token SQL_UNKNOWN
//%token UNSIGNED_INTEGER
%token UPDATE
%token UPPER
%token USAGE
%token USER
%token IDENTIFIER
%token IDENTIFIER_DOT_ASTERISK
//%token ERROR_DIGIT_BEFORE_IDENTIFIER
%token USING
%token VALUE
%token VALUES
%token VARBINARY
%token VARCHAR
%token VARYING
%token VENDOR
%token VIEW
%token WEEK
%token WHEN
%token WHENEVER
%token WHERE
%token WHERE_CURRENT_OF
%token WITH
%token WORD_WRAPPED
%token WORK
%token WRAPPED
%token XOR
%token YEAR
%token YEARS_BETWEEN

%token SCAN_ERROR
%token __LAST_TOKEN /* sentinel */

%token '-' '+'
%token '*'
%token '%'
%token '@'
%token ';'
%token ','
%token '.'
%token '$'
//%token '<'
//%token '>'
%token '(' ')'
%token '?'
%token '\''
%token '/'

%type <stringValue> IDENTIFIER
%type <stringValue> IDENTIFIER_DOT_ASTERISK
%type <stringValue> CHARACTER_STRING_LITERAL
%type <stringValue> DOUBLE_QUOTED_STRING

/*
%type <field> ColExpression
%type <field> ColView
*/
%type <expr> ColExpression
%type <expr> ColWildCard
//%type <expr> ColView
%type <expr> ColItem
%type <exprList> ColViews
%type <expr> aExpr
%type <expr> aExpr2
%type <expr> aExpr3
%type <expr> aExpr4
%type <expr> aExpr5
%type <expr> aExpr6
%type <expr> aExpr7
%type <expr> aExpr8
%type <expr> aExpr9
%type <expr> aExpr10
%type <exprList> aExprList
%type <exprList> aExprList2
%type <expr> WhereClause
%type <expr> FlatTable
%type <exprList> Tables
%type <exprList> FlatTableList
%type <querySchema> SelectStatement
%type <querySchema> Select
/*todo : list*/
%type <querySchema> StatementList
/*todo: not onlu select*/
%type <querySchema> Statement

%type <colType> SQL_TYPE
%type <integerValue> INTEGER_CONST
%type <realValue> REAL_CONST
/*%type <integerValue> SIGNED_INTEGER */

%{
#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>
#include <assert.h>
#include <limits.h>
//TODO OK?
#ifdef Q_WS_WIN
//workaround for bug on msvc
# undef LLONG_MIN
#endif
#ifndef LLONG_MAX
# define LLONG_MAX     0x7fffffffffffffffLL
#endif
#ifndef LLONG_MIN
# define LLONG_MIN     0x8000000000000000LL
#endif
#ifndef LLONG_MAX
# define ULLONG_MAX    0xffffffffffffffffLL
#endif

#ifdef _WIN32
# include <malloc.h>
#endif

#include <qobject.h>
#include <kdebug.h>
#include <klocale.h>
#include <qptrlist.h>
#include <qcstring.h>

#include <connection.h>
#include <queryschema.h>
#include <field.h>
#include <tableschema.h>

#include "parser.h"
#include "parser_p.h"
#include "sqltypes.h"

int yylex();

//	using namespace std;
using namespace KexiDB;

#define YY_NO_UNPUT
#define YYSTACK_USE_ALLOCA 1
#define YYMAXDEPTH 255

	extern "C"
	{
		int yywrap()
		{
			return 1;
		}
	}

#if 0
	struct yyval
	{
		QString parserUserName;
		int integerValue;
		KexiDBField::ColumnType coltype;
	}
#endif
%}

%union {
	char stringValue[255];
	Q_LLONG integerValue;
	struct realType realValue;
	KexiDB::Field::Type colType;
	KexiDB::Field *field;
	KexiDB::BaseExpr *expr;
	KexiDB::NArgExpr *exprList;
	KexiDB::ConstExpr *constExpr;
	KexiDB::QuerySchema *querySchema;
}

//%left '=' NOT_EQUAL '>' GREATER_OR_EQUAL '<' LESS_OR_EQUAL LIKE '%' NOT
//%left '+' '-'
//%left ASTERISK SLASH

/* precedence: lowest to highest */
%left		UNION EXCEPT
%left		INTERSECT
%left		OR
%left		AND XOR
%right	NOT
//%right		'='
//%nonassoc	'<' '>'
//%nonassoc '=' '<' '>' "<=" ">=" "<>" ":=" LIKE ILIKE SIMILAR
//%nonassoc '=' LESS_THAN GREATER_THAN LESS_OR_EQUAL GREATER_OR_EQUAL NOT_EQUAL
%nonassoc '=' '<' '>'
//LESS_THAN GREATER_THAN 
%nonassoc LESS_OR_EQUAL GREATER_OR_EQUAL 
%nonassoc NOT_EQUAL NOT_EQUAL2
%nonassoc SQL_IN LIKE ILIKE SIMILAR_TO NOT_SIMILAR_TO
//%nonassoc	LIKE ILIKE SIMILAR
//%nonassoc	ESCAPE
//%nonassoc	OVERLAPS
%nonassoc	BETWEEN
//%nonassoc	IN_P
//%left		POSTFIXOP		// dummy for postfix Op rules 
//%left		Op OPERATOR		// multi-character ops and user-defined operators 
//%nonassoc	NOTNULL
//%nonassoc	ISNULL
//%nonassoc	IS NULL_P TRUE_P FALSE_P UNKNOWN // sets precedence for IS NULL, etc 
%left		'+' '-'
%left		'*' '/' '%'
%left		'^'
%left UMINUS
// Unary Operators 
//%left		AT ZONE			// sets precedence for AT TIME ZONE
//%right		UMINUS
%left		'[' ']'
%left		'(' ')'
//%left		TYPECAST
%left		'.'

/*
 * These might seem to be low-precedence, but actually they are not part
 * of the arithmetic hierarchy at all in their use as JOIN operators.
 * We make them high-precedence to support their use as function names.
 * They wouldn't be given a precedence at all, were it not that we need
 * left-associativity among the JOIN rules themselves.
 */
/*%left		JOIN UNIONJOIN CROSS LEFT FULL RIGHT INNER_P NATURAL
*/
%%

TopLevelStatement :
StatementList
{
//todo: multiple statements
//todo: not only "select" statements
	parser->setOperation(Parser::OP_Select);
	parser->setQuerySchema($1);
}
;

StatementList:
Statement ';' StatementList
{
//todo: multiple statements
}
| Statement
| Statement ';'
{
	$$ = $1;
}
;

/*		Statement CreateTableStatement 		{ YYACCEPT; }
	| Statement SelectStatement 		{  }
*/
Statement :
CreateTableStatement
{
YYACCEPT;
}
| SelectStatement
{
	$$ = $1;
}
;

CreateTableStatement :
CREATE TABLE IDENTIFIER
{
	parser->setOperation(Parser::OP_CreateTable);
	parser->createTable($3);
}
'(' ColDefs ')'
;

ColDefs:
ColDefs ',' ColDef|ColDef
{
}
;

ColDef:
IDENTIFIER ColType
{
	kdDebug() << "adding field " << $1 << endl;
	field->setName($1);
	parser->table()->addField(field);

//	delete field;
	field = 0;
}
| IDENTIFIER ColType ColKeys
{
	kdDebug() << "adding field " << $1 << endl;
	field->setName($1);
	parser->table()->addField(field);

//	if(field->isPrimaryKey())
//		parser->table()->addPrimaryKey(field->name());

//	delete field;
//	field = 0;
}
;

ColKeys:
ColKeys ColKey|ColKey
{
}
;

ColKey:
PRIMARY KEY
{
	field->setPrimaryKey(true);
	kdDebug() << "primary" << endl;
}
| NOT SQL_NULL
{
	field->setNotNull(true);
	kdDebug() << "not_null" << endl;
}
| AUTO_INCREMENT
{
	field->setAutoIncrement(true);
	kdDebug() << "ainc" << endl;
}
;

ColType:
SQL_TYPE
{
	field = new Field();
	field->setType($1);
}
| SQL_TYPE '(' INTEGER_CONST ')'
{
	kdDebug() << "sql + length" << endl;
	field = new Field();
	field->setPrecision($3);
	field->setType($1);
}
| VARCHAR '(' INTEGER_CONST ')'
{
	field = new Field();
	field->setPrecision($3);
	field->setType(Field::Text);
}
|
{
	// SQLITE compatibillity
	field = new Field();
	field->setType(Field::InvalidType);
}
;

SelectStatement:
Select ColViews
{
	kdDebug() << "Select ColViews=" << $2->debugString() << endl;

	if (!($$ = parseSelect( $1, $2 )))
		return 0;
}
| Select ColViews Tables
{
	if (!($$ = parseSelect( $1, $2, $3 )))
		return 0;
}
| Select Tables
{
	kdDebug() << "Select ColViews Tables" << endl;
	if (!($$ = parseSelect( $1, 0, $2 )))
		return 0;
}
| Select ColViews WhereClause
{
	kdDebug() << "Select ColViews Conditions" << endl;
	if (!($$ = parseSelect( $1, $2, 0, $3 )))
		return 0;
}
| Select ColViews Tables WhereClause
{
	kdDebug() << "Select ColViews Tables Conditions" << endl;
	if (!($$ = parseSelect( $1, $2, $3, $4 )))
		return 0;
}
| Select Tables WhereClause
{
	kdDebug() << "Select Tables Conditions" << endl;
	if (!($$ = parseSelect( $1, 0, $2, $3 )))
		return 0;
}
;

Select:
SELECT
{
	kdDebug() << "SELECT" << endl;
//	parser->createSelect();
//	parser->setOperation(Parser::OP_Select);
	$$ = new QuerySchema();
}
;

WhereClause:
WHERE aExpr
{
	$$ = $2;
}
;

aExpr:
aExpr2
;

/* --- binary logical --- */
aExpr2:
aExpr3 AND aExpr2
{
//	kdDebug() << "AND " << $3.debugString() << endl;
	$$ = new BinaryExpr( KexiDBExpr_Logical, $1, AND, $3 );
}
| aExpr3 OR aExpr2
{
	$$ = new BinaryExpr( KexiDBExpr_Logical, $1, OR, $3 );
}
| aExpr3 XOR aExpr2
{
	$$ = new BinaryExpr( KexiDBExpr_Arithm, $1, XOR, $3 );
}
|
aExpr3
;

/* relational op precedence */
aExpr3:
aExpr4 '>' %prec GREATER_OR_EQUAL aExpr3
{
	$$ = new BinaryExpr(KexiDBExpr_Relational, $1, '>', $3);
}
| aExpr4 GREATER_OR_EQUAL aExpr3
{
	$$ = new BinaryExpr(KexiDBExpr_Relational, $1, GREATER_OR_EQUAL, $3);
}
| aExpr4 '<' %prec LESS_OR_EQUAL aExpr3
{
	$$ = new BinaryExpr(KexiDBExpr_Relational, $1, '<', $3);
}
| aExpr4 LESS_OR_EQUAL aExpr3
{
	$$ = new BinaryExpr(KexiDBExpr_Relational, $1, LESS_OR_EQUAL, $3);
}
| aExpr4 '=' aExpr3
{
	$$ = new BinaryExpr(KexiDBExpr_Relational, $1, '=', $3);
}
|
aExpr4
;

/* relational (equality) op precedence */
aExpr4:
aExpr5 NOT_EQUAL aExpr4
{
	$$ = new BinaryExpr(KexiDBExpr_Relational, $1, NOT_EQUAL, $3);
}
|
aExpr5 NOT_EQUAL2 aExpr4
{
	$$ = new BinaryExpr(KexiDBExpr_Relational, $1, NOT_EQUAL2, $3);
}
| aExpr5 LIKE aExpr4
{
	$$ = new BinaryExpr(KexiDBExpr_Relational, $1, LIKE, $3);
}
| aExpr5 SQL_IN aExpr4
{
	$$ = new BinaryExpr(KexiDBExpr_Relational, $1, SQL_IN, $3);
}
| aExpr5 SIMILAR_TO aExpr4
{
	$$ = new BinaryExpr(KexiDBExpr_Relational, $1, SIMILAR_TO, $3);
}
| aExpr5 NOT_SIMILAR_TO aExpr4
{
	$$ = new BinaryExpr(KexiDBExpr_Relational, $1, NOT_SIMILAR_TO, $3);
}
|
aExpr5
;

/* --- unary logical right --- */
aExpr5:
aExpr5 SQL_IS_NULL
{
	$$ = new UnaryExpr( SQL_IS_NULL, $1 );
}
| aExpr5 SQL_IS_NOT_NULL
{
	$$ = new UnaryExpr( SQL_IS_NOT_NULL, $1 );
}
|
aExpr6
;

/* arithm. lowest precedence */
aExpr6:
aExpr7 BITWISE_SHIFT_LEFT aExpr6
{
	$$ = new BinaryExpr(KexiDBExpr_Arithm, $1, BITWISE_SHIFT_LEFT, $3);
}
| aExpr7 BITWISE_SHIFT_RIGHT aExpr6
{
	$$ = new BinaryExpr(KexiDBExpr_Arithm, $1, BITWISE_SHIFT_RIGHT, $3);
}
|
aExpr7
;

/* arithm. lower precedence */
aExpr7:
aExpr8 '+' aExpr7
{
	$$ = new BinaryExpr(KexiDBExpr_Arithm, $1, '+', $3);
	$$->debug();
}
| aExpr8 '-' %prec UMINUS aExpr7
{
	$$ = new BinaryExpr(KexiDBExpr_Arithm, $1, '-', $3);
}
| aExpr8 '&' aExpr7
{
	$$ = new BinaryExpr(KexiDBExpr_Arithm, $1, '&', $3);
}
| aExpr8 '|' aExpr7
{
	$$ = new BinaryExpr(KexiDBExpr_Arithm, $1, '|', $3);
}
|
aExpr8
;

/* arithm. higher precedence */
aExpr8:
aExpr9 '/' aExpr8
{
	$$ = new BinaryExpr(KexiDBExpr_Arithm, $1, '/', $3);
}
| aExpr9 '*' aExpr8
{
	$$ = new BinaryExpr(KexiDBExpr_Arithm, $1, '*', $3);
}
| aExpr9 '%' aExpr8
{
	$$ = new BinaryExpr(KexiDBExpr_Arithm, $1, '%', $3);
}
|
aExpr9
;

/* parenthesis, unary operators, and terminals precedence */
aExpr9:
/* --- unary logical left --- */
'-' aExpr9
{
	$$ = new UnaryExpr( '-', $2 );
}
| '+' aExpr9
{
	$$ = new UnaryExpr( '+', $2 );
}
| '~' aExpr9
{
	$$ = new UnaryExpr( '~', $2 );
}
| NOT aExpr9
{
	$$ = new UnaryExpr( NOT, $2 );
}
| IDENTIFIER
{
	$$ = new VariableExpr( QString::fromLatin1($1) );
	
//TODO: simplify this later if that's 'only one field name' expression
	kdDebug() << "  + identifier: " << $1 << endl;
//	$$ = new Field();
//	$$->setName($1);
//	$$->setTable(dummy);

//	parser->select()->addField(field);
//	requiresTable = true;
}
| IDENTIFIER aExprList
{
	kdDebug() << "  + function: " << $1 << "(" << $2->debugString() << ")" << endl;
	$$ = new FunctionExpr($1, $2);
}
/*TODO: shall we also support db name? */
| IDENTIFIER '.' IDENTIFIER
{
	$$ = new VariableExpr( QString::fromLatin1($1) + "." + QString::fromLatin1($3) );
	kdDebug() << "  + identifier.identifier: " << $3 << "." << $1 << endl;
//	$$ = new Field();
//	s->setTable($1);
//	$$->setName($3);
	//$$->setTable(parser->db()->tableSchema($1));
//	parser->select()->addField(field);
//??	requiresTable = true;
}
| SQL_NULL
{
	$$ = new ConstExpr( SQL_NULL, QVariant() );
	kdDebug() << "  + NULL" << endl;
//	$$ = new Field();
	//$$->setName(QString::null);
}
| CHARACTER_STRING_LITERAL
{
	QCString s($1);
	$$ = new ConstExpr( CHARACTER_STRING_LITERAL, s.mid(1,s.length()-2) );
//	$$ = new Field();
//	$$->setName($1);
//	parser->select()->addField(field);
	kdDebug() << "  + constant << s << endl;
}
| INTEGER_CONST
{
	QVariant val;
	if ($1 <= INT_MAX && $1 >= INT_MIN)
		val = (int)$1;
	else if ($1 <= UINT_MAX && $1 >= 0)
		val = (uint)$1;
	else if ($1 <= LLONG_MAX && $1 >= LLONG_MIN)
		val = (Q_LLONG)$1;

//	if ($1 < ULLONG_MAX)
//		val = (Q_ULLONG)$1;
//TODO ok?

	$$ = new ConstExpr( INTEGER_CONST, val );
	kdDebug() << "  + int constant: " << val.toString() << endl;
}
| REAL_CONST
{
	$$ = new ConstExpr( REAL_CONST, QPoint( $1.integer, $1.fractional ) );
	kdDebug() << "  + real constant: " << $1.integer << "." << $1.fractional << endl;
}
|
aExpr10
;


aExpr10:
'(' aExpr ')'
{
	kdDebug() << "(expr)" << endl;
	$$ = new UnaryExpr('(', $2);
}
;

aExprList:
'(' aExprList2 ')'
{
//	$$ = new NArgExpr(0, 0);
//	$$->add( $1 );
//	$$->add( $3 );
	$$ = $2;
}
;

aExprList2:
aExpr ',' aExprList2
{
	$$ = $3;
	$$->prepend( $1 );
}
| aExpr ',' aExpr
{
	$$ = new NArgExpr(0, 0);
	$$->add( $1 );
	$$->add( $3 );
}
;

Tables:
FROM FlatTableList
{
	$$ = $2;
}
/*
| Tables LEFT JOIN IDENTIFIER SQL_ON ColExpression
{
	kdDebug() << "LEFT JOIN: '" << $4 << "' ON " << $6 << endl;
	addTable($4);
}
| Tables LEFT OUTER JOIN IDENTIFIER SQL_ON ColExpression
{
	kdDebug() << "LEFT OUTER JOIN: '" << $5 << "' ON " << $7 << endl;
	addTable($5);
}
| Tables INNER JOIN IDENTIFIER SQL_ON ColExpression
{
	kdDebug() << "INNER JOIN: '" << $4 << "' ON " << $6 << endl;
	addTable($4);
}
| Tables RIGHT JOIN IDENTIFIER SQL_ON ColExpression
{
	kdDebug() << "RIGHT JOIN: '" << $4 << "' ON " << $6 << endl;
	addTable($4);
}
| Tables RIGHT OUTER JOIN IDENTIFIER SQL_ON ColExpression
{
	kdDebug() << "RIGHT OUTER JOIN: '" << $5 << "' ON " << $7 << endl;
	addTable($5);
}*/
;

/*
FlatTableList:
aFlatTableList
{
	$$
}
;*/

FlatTableList:
FlatTableList ',' FlatTable
{
	$$ = $1;
	$$->add($3);
}
|FlatTable
{
	$$ = new NArgExpr(KexiDBExpr_TableList, IDENTIFIER); //ok?
	$$->add($1);
}
;

FlatTable:
IDENTIFIER
{
	kdDebug() << "FROM: '" << $1 << "'" << endl;

//	TableSchema *schema = parser->db()->tableSchema($1);
//	parser->select()->setParentTable(schema);
//	parser->select()->addTable(schema);
//	requiresTable = false;
	
//addTable($1);

	$$ = new VariableExpr(QString::fromLatin1($1));

	/*
//TODO: this isn't ok for more tables:
	Field::ListIterator it = parser->select()->fieldsIterator();
	for(Field *item; (item = it.current()); ++it)
	{
		if(item->table() == dummy)
		{
			item->setTable(schema);
		}

		if(item->table() && !item->isQueryAsterisk())
		{
			Field *f = item->table()->field(item->name());
			if(!f)
			{
				ParserError err(i18n("Field List Error"), i18n("Unknown column '%1' in table '%2'").arg(item->name()).arg(schema->name()), ctoken, current);
				parser->setError(err);
				yyerror("fieldlisterror");
			}	
		}
	}*/
}
| IDENTIFIER IDENTIFIER
{
	//table + alias
	$$ = new BinaryExpr(
		KexiDBExpr_SpecialBinary, 
		new VariableExpr(QString::fromLatin1($1)), 0,
		new VariableExpr(QString::fromLatin1($2))
	);
}
| IDENTIFIER AS IDENTIFIER
{
	//table + alias
	$$ = new BinaryExpr(
		KexiDBExpr_SpecialBinary,
		new VariableExpr(QString::fromLatin1($1)), AS,
		new VariableExpr(QString::fromLatin1($3))
	);
}
;



ColViews:
ColViews ',' ColItem
{
	$$ = $1;
	$$->add( $3 );
	kdDebug() << "ColViews: ColViews , ColItem" << endl;
}
|ColItem
{
	$$ = new NArgExpr(0,0);
	$$->add( $1 );
	kdDebug() << "ColViews: ColItem" << endl;
}
;

ColItem:
ColExpression
{
//	$$ = new Field();
//	dummy->addField($$);
//	$$->setExpression( $1 );
//	parser->select()->addField($$);
	$$ = $1;
	kdDebug() << " added column expr: '" << $1->debugString() << "'" << endl;
}
| ColWildCard
{
	$$ = $1;
	kdDebug() << " added column wildcard: '" << $1->debugString() << "'" << endl;
}
| ColExpression AS IDENTIFIER
{
//	$$ = new Field();
//	$$->setExpression( $1 );
//	parser->select()->addField($$);
	$$ = new BinaryExpr(
		KexiDBExpr_SpecialBinary, $1, AS,
		new VariableExpr(QString::fromLatin1($3))
//		new ConstExpr(IDENTIFIER, QString::fromLocal8Bit($3))
	);
	kdDebug() << " added column expr: " << $$->debugString() << endl;
}
| ColExpression IDENTIFIER
{
//	$$ = new Field();
//	$$->setExpression( $1 );
//	parser->select()->addField($$);
	$$ = new BinaryExpr(
		KexiDBExpr_SpecialBinary, $1, 0, 
		new VariableExpr(QString::fromLatin1($2))
//		new ConstExpr(IDENTIFIER, QString::fromLocal8Bit($2))
	);
	kdDebug() << " added column expr: " << $$->debugString() << endl;
}
;

ColExpression:
aExpr
{
	$$ = $1;
}
/* HANDLED BY 'IDENTIFIER aExprList'
| IDENTIFIER '(' ColViews ')'
{
	$$ = new FunctionExpr( $1, $3 );
}*/
/*
| SUM '(' ColExpression ')'
{
	FunctionExpr(
//	$$ = new AggregationExpr( SUM,  );
//TODO
//	$$->setName("SUM(" + $3->name() + ")");
//wait	$$->containsGroupingAggregate(true);
//wait	parser->select()->grouped(true);
}
| SQL_MIN '(' ColExpression ')'
{
	$$ = $3;
//TODO
//	$$->setName("MIN(" + $3->name() + ")");
//wait	$$->containsGroupingAggregate(true);
//wait	parser->select()->grouped(true);
}
| SQL_MAX '(' ColExpression ')'
{
	$$ = $3;
//TODO
//	$$->setName("MAX(" + $3->name() + ")");
//wait	$$->containsGroupingAggregate(true);
//wait	parser->select()->grouped(true);
}
| AVG '(' ColExpression ')'
{
	$$ = $3;
//TODO
//	$$->setName("AVG(" + $3->name() + ")");
//wait	$$->containsGroupingAggregate(true);
//wait	parser->select()->grouped(true);
}*/
//?
| DISTINCT '(' ColExpression ')' 
{
	$$ = $3;
//TODO
//	$$->setName("DISTINCT(" + $3->name() + ")");
}
;

ColWildCard:
'*'
{
	$$ = new VariableExpr("*");
	kdDebug() << "all columns" << endl;

//	QueryAsterisk *ast = new QueryAsterisk(parser->select(), dummy);
//	parser->select()->addAsterisk(ast);
//	requiresTable = true;
}
| IDENTIFIER '.' '*'
{
	QString s = QString::fromLatin1($1);
	s+=".*";
	$$ = new VariableExpr(s);
	kdDebug() << "  + all columns from " << s << endl;
}
/*| ERROR_DIGIT_BEFORE_IDENTIFIER
{
	$$ = new VariableExpr($1);
	kdDebug() << "  Invalid identifier! " << $1 << endl;
	setError(i18n("Invalid identifier \"%1\"").arg($1));
}*/
;

%%

