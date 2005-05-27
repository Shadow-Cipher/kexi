/* This file is part of the KDE project
   Copyright (C) 2005 Jaroslaw Staniek <js@iidea.pl>

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
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.

   Original Author:  Till Busch <till@bux.at>
   Original Project: buX (www.bux.at)
*/

#include "kexidisplayutils.h"

#include <qpixmap.h>
#include <qpainter.h>
#include <qimage.h>
#include <qwidget.h>

#include <klocale.h>
#include <kstaticdeleter.h>

static KStaticDeleter<QPixmap> KexiDisplayUtils_autonum_deleter;
QPixmap* KexiDisplayUtils_autonum = 0;

static const unsigned int  autonumber_png_len = 245;
static const unsigned char autonumber_png_data[] = {
    0x89,0x50,0x4e,0x47,0x0d,0x0a,0x1a,0x0a,0x00,0x00,0x00,0x0d,0x49,0x48,
    0x44,0x52,0x00,0x00,0x00,0x0b,0x00,0x00,0x00,0x0d,0x08,0x06,0x00,0x00,
    0x00,0x7f,0xf5,0x94,0x3b,0x00,0x00,0x00,0x06,0x62,0x4b,0x47,0x44,0x00,
    0xff,0x00,0xff,0x00,0xff,0xa0,0xbd,0xa7,0x93,0x00,0x00,0x00,0x09,0x70,
    0x48,0x59,0x73,0x00,0x00,0x0b,0x11,0x00,0x00,0x0b,0x11,0x01,0x7f,0x64,
    0x5f,0x91,0x00,0x00,0x00,0x07,0x74,0x49,0x4d,0x45,0x07,0xd4,0x08,0x14,
    0x0c,0x09,0x11,0x18,0x18,0x1d,0x4f,0x00,0x00,0x00,0x82,0x49,0x44,0x41,
    0x54,0x78,0x9c,0x8d,0x91,0x41,0x0e,0x03,0x31,0x08,0x03,0x87,0xbe,0x2e,
    0x1c,0xb3,0xff,0xbf,0xf6,0x1d,0xee,0x81,0xa0,0x05,0xaa,0x55,0x6b,0x29,
    0x92,0x03,0x06,0x59,0x06,0x49,0x48,0x02,0xa4,0xe4,0xf1,0x5f,0x1b,0xa4,
    0x78,0x6b,0xc3,0xc2,0x24,0x61,0x86,0x00,0x24,0x8c,0x83,0x53,0x33,0xe9,
    0xe6,0xaf,0x29,0x4a,0x48,0x29,0xf4,0x0d,0xbc,0xc1,0xe1,0xc9,0x46,0xb5,
    0x72,0xfa,0xcf,0xe2,0x2a,0x4c,0x71,0xf3,0x5c,0x2d,0xd5,0x5a,0xc0,0xcd,
    0x62,0xea,0x6f,0xf4,0x88,0x86,0x95,0xf0,0x4a,0xf2,0xee,0x6b,0xf8,0x1e,
    0x03,0x55,0xf8,0x73,0xf3,0x28,0x7e,0x6d,0x6e,0x69,0xc4,0xc6,0xfb,0x52,
    0x23,0x8d,0x3c,0x56,0x5e,0xd0,0x2f,0x40,0xd1,0xf4,0x6b,0xc4,0xd5,0xf8,
    0x07,0x69,0x14,0xc6,0x69,0x9a,0x12,0x79,0x9a,0x00,0x00,0x00,0x00,0x49,
    0x45,0x4e,0x44,0xae,0x42,0x60,0x82
};

/* Generated by qembed */
#include <qcstring.h>
#include <qdict.h>
static struct Embed {
    unsigned int size;
    const unsigned char *data;
    const char *name;
} embed_vec[] = {
    { 245, autonumber_png_data, "autonumber.png" },
    { 0, 0, 0 }
};

QPixmap* getPix(int id)
{
//	QByteArray ba;
//	ba.setRawData( (char*)embed_vec[id].data, embed_vec[id].size );
	QPixmap *pix = new QPixmap();
	pix->loadFromData( embed_vec[id].data, embed_vec[id].size );
	return pix;
}

static void initImages()
{
	if (!KexiDisplayUtils_autonum) {
/*! @warning not reentrant! */
		KexiDisplayUtils_autonum_deleter.setObject( KexiDisplayUtils_autonum, getPix(0) );
	}
}

//-----------------

void KexiDisplayUtils::initDisplayForAutonumberSign(DisplayParameters& par, QWidget *widget)
{
	initImages();

	par.textColor = Qt::blue;
	par.font = widget->font();
	par.font.setItalic(true);
	QFontMetrics fm(par.font);
	par.textWidth = fm.width(i18n("(autonumber)"));
	par.textHeight = fm.height();
}

void KexiDisplayUtils::drawAutonumberSign(const DisplayParameters& par, QPainter* painter, 
	int x, int y, int width, int height, int align, bool overrideColor)
{
	painter->save();
	
	painter->setFont(par.font);
	if (!overrideColor)
		painter->setPen(par.textColor);

//	int text_x = x;
	if (!(align & Qt::AlignVertical_Mask))
		align |= Qt::AlignVCenter;
	if (!(align & Qt::AlignHorizontal_Mask))
		align |= Qt::AlignLeft;

	int y_pixmap_pos;
	if (align & Qt::AlignVCenter) {
		y_pixmap_pos = QMAX(0, y+1 + (height - KexiDisplayUtils_autonum->height())/2);
	}
	else if (align & Qt::AlignTop) {
		y_pixmap_pos = y + QMAX(0, (par.textHeight - KexiDisplayUtils_autonum->height())/2);
	}
	else if (align & Qt::AlignBottom) {
		y_pixmap_pos = y+1 + height - KexiDisplayUtils_autonum->height() 
			- QMAX(0, (par.textHeight - KexiDisplayUtils_autonum->height())/2);
	}

	if (align & (Qt::AlignLeft | Qt::AlignJustify)) {
//		text_x = x + KexiDisplayUtils_autonum->width() + 2;
		if (!overrideColor) {
			painter->drawPixmap( x, y_pixmap_pos, *KexiDisplayUtils_autonum );
			x += (KexiDisplayUtils_autonum->width() + 4);
		}
	}
	else if (align & Qt::AlignRight) {
		if (!overrideColor) {
			painter->drawPixmap( x + width - par.textWidth - KexiDisplayUtils_autonum->width() - 4,
				y_pixmap_pos, *KexiDisplayUtils_autonum );
		}
	}
	else if (align & Qt::AlignCenter) {
		//! @todo
		if (!overrideColor)
			painter->drawPixmap( x + (width - par.textWidth)/2 - KexiDisplayUtils_autonum->width() - 4,
				y_pixmap_pos, *KexiDisplayUtils_autonum );
	}
	else if (align & Qt::AlignJustify) {
		//! @todo
	}

	painter->drawText(x, y, width, height, align, i18n("(autonumber)"));

	painter->restore();
}

