/* This file is part of the KDE project
   Copyright (C) 2001 Laurent Montel <lmontel@mandrakesoft.com>

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

#ifndef KEXIVIEWIFACE_H
#define KEXIVIEWIFACE_H

#include <KoViewIface.h>

#include <qstring.h>

class KexiView;

#ifdef Q_WS_WIN //(js) DIRTY? - it is temporary, to fix dcopidl problems
# define class class KEXICORE_EXPORT
#endif
class KexiViewIface : public KoViewIface
#ifdef Q_WS_WIN //(js) temporary
# undef class
#endif
{
    K_DCOP
public:
    KexiViewIface( KexiView *view_ );
/*
k_dcop:
    void slotShowRelations();
*/
private:
    KexiView *view;

};

#endif
