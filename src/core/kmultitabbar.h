
/***************************************************************************
                          kmultitabbar.h -  description
                             -------------------
    begin                :  2001
    copyright            : (C) 2001,2002 by Joseph Wenninger <jowenn@kde.org>
 ***************************************************************************/

/***************************************************************************
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
 ***************************************************************************/

#ifndef _KMultitabbar_h_
#define _KMultitabbar_h_

#include <qscrollview.h>
#include <qvbox.h>
#include <qhbox.h>
#include <qlayout.h>
#include <qstring.h>
#include <qptrlist.h>
#include <qpushbutton.h>

class QPixmap;
class QPainter;
class QFrame;


class KEXICORE_EXPORT KMultiTabBar: public QWidget
{
	Q_OBJECT
public:
	enum KMultiTabBarBasicMode{Horizontal, Vertical};
	KMultiTabBar(QWidget *parent,KMultiTabBarBasicMode bm);
	~KMultiTabBar(){;}

	enum KMultiTabBarPosition{Left, Right, Top, Bottom};
	enum KMultiTabBarStyle{VSNET=0, KDEV3=1, KONQSBC=2};
 	//int insertButton(QPixmap,int=-1,const QString& =QString::null);
 	int appendButton(const QPixmap &,int=-1,QPopupMenu* =0,const QString& =QString::null);
	void removeButton(int);
	int appendTab(const QPixmap &,int=-1,const QString& =QString::null);
	void removeTab(int);
	void setTab(int,bool);
	bool isTabRaised(int);
	class KMultiTabBarButton *getButton(int);
	class KMultiTabBarTab *getTab(int);
	QPtrList<class KMultiTabBarButton> buttons;
	void setPosition(KMultiTabBarPosition pos);
	void setStyle(KMultiTabBarStyle style);
        QPtrList<KMultiTabBarTab>* tabs();
	void showActiveTabTexts(bool show=true);

private:
	class KMultiTabBarInternal *internal;
	QBoxLayout *l;
	QFrame *btnTabSep;	
	KMultiTabBarPosition position;
};

class KEXICORE_EXPORT KMultiTabBarButton: public QPushButton
{
	Q_OBJECT
public:
	KMultiTabBarButton(const QPixmap& pic,const QString&, QPopupMenu *popup,
		int id,QWidget *parent, KMultiTabBar::KMultiTabBarPosition pos, KMultiTabBar::KMultiTabBarStyle style);
	~KMultiTabBarButton(){;}
	int id(){return m_id;}

public slots:
	void setPosition(KMultiTabBar::KMultiTabBarPosition);
	void setStyle(KMultiTabBar::KMultiTabBarStyle);
	void setText(const QString &);
	
protected:
	KMultiTabBar::KMultiTabBarPosition position;
	KMultiTabBar::KMultiTabBarStyle m_style;
	QString m_text;

private:
	int m_id;
signals:
	void clicked(int);
protected slots:
	virtual void slotClicked();
};


class KEXICORE_EXPORT KMultiTabBarTab: public KMultiTabBarButton
{
	Q_OBJECT
public:
	KMultiTabBarTab(const QPixmap& pic,const QString&,int id,QWidget *parent,
		KMultiTabBar::KMultiTabBarPosition pos,KMultiTabBar::KMultiTabBarStyle style);
	~KMultiTabBarTab(){;}
	void setState(bool);
	void showActiveTabText(bool show);
	void setSize(int);
	int neededSize();
private:
	bool m_showActiveTabText;
	int m_expandedSize;
protected:
	void updateState();
	virtual void drawButton(QPainter *);
	void drawButtonStyled(QPainter *);
	void drawButtonClassic(QPainter *);
protected slots:
	virtual void slotClicked();
public slots:
	virtual void setIcon(const QString&);
};

class KEXICORE_EXPORT KMultiTabBarInternal: public QScrollView
{
	Q_OBJECT
public:
	KMultiTabBarInternal(QWidget *parent,KMultiTabBar::KMultiTabBarBasicMode bm);
	int appendTab(const QPixmap &,int=-1,const QString& =QString::null);
	KMultiTabBarTab *getTab(int);
	void removeTab(int);
	void setPosition(enum KMultiTabBar::KMultiTabBarPosition pos);
	void setStyle(enum KMultiTabBar::KMultiTabBarStyle style);
	void showActiveTabTexts(bool show);
	QPtrList<KMultiTabBarTab>* tabs(){return &m_tabs;}
private:
	friend class KMultiTabBar;
	QHBox *box;
	QPtrList<KMultiTabBarTab> m_tabs;
	enum KMultiTabBar::KMultiTabBarPosition position;
	bool m_showActiveTabTexts;
	enum  KMultiTabBar::KMultiTabBarStyle m_style;
protected:
	virtual void drawContents ( QPainter *, int, int, int, int);
	
	/**
	 * [contentsM|m]ousePressEvent are reimplemented from QScrollView 
	 * in order to ignore all mouseEvents on the viewport, so that the
	 * parent can handle them.
	 */
	virtual void contentsMousePressEvent(QMouseEvent *);
	virtual void mousePressEvent(QMouseEvent *);
};

#endif
