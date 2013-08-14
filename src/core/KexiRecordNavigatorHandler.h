/* This file is part of the KDE project
 * Copyright (C) 2004 Lucijan Busch <lucijan@kde.org>
 * Copyright (C) 2003-2007 Jarosław Staniek <staniek@kde.org>
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 * 
 * You should have received a copy of the GNU Library General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef KEXIRECORDNAVIGATORHANDLER_H
#define KEXIRECORDNAVIGATORHANDLER_H

#include <kexi_export.h>

//! @short An interface handling requests generated by KexiRecordNavigator
class KEXICORE_EXPORT KexiRecordNavigatorHandler
{
public:
    KexiRecordNavigatorHandler();
    virtual ~KexiRecordNavigatorHandler();
    
    //! Moving to record \a r is requested. Records are counted from 0.
    virtual void moveToRecordRequested(uint r) = 0;
    virtual void moveToLastRecordRequested() = 0;
    virtual void moveToPreviousRecordRequested() = 0;
    virtual void moveToNextRecordRequested() = 0;
    virtual void moveToFirstRecordRequested() = 0;
    virtual void addNewRecordRequested() = 0;
    
    //! Allow the handler to tell the navigator about the records
    virtual int recordCount() const { return 0; }
    virtual int currentRecord() const { return 0; }
};

#endif
