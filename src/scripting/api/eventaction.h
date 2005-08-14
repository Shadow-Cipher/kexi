/***************************************************************************
 * eventaction.h
 * This file is part of the KDE project
 * copyright (C)2004-2005 by Sebastian Sauer (mail@dipe.org)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 * You should have received a copy of the GNU Library General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 ***************************************************************************/

#ifndef KROSS_API_EVENTACTION_H
#define KROSS_API_EVENTACTION_H

#include <qstring.h>
//#include <qobject.h>
#include <kaction.h>
#include <ksharedptr.h>

#include "event.h"

namespace Kross { namespace Api {

    // Forward declarations.
    class ScriptContainer;

//TODO handle KActionPtrList too.

    /**
     * The EventAction class is used to wrap KAction instances
     * into the Kross object hierachy and provide access to
     * them.
     */
    class EventAction : public Event<EventAction>
    {

        public:

            /// Shared pointer to implement reference-counting.
            typedef KSharedPtr<EventAction> Ptr;

            /**
             * Constructor.
             */
            EventAction(const QString& name, Object::Ptr parent, KAction* action);

            /**
             * Destructor.
             */
            virtual ~EventAction();

            /// \see Kross::Api::Object::getClassName()
            virtual const QString getClassName() const;

            /// \see Kross::Api::Event::call()
            //virtual Object::Ptr call(const QString& name, KSharedPtr<List> arguments);

        private:
            KAction* m_action;

            Kross::Api::Object::Ptr getText(Kross::Api::List::Ptr);
            Kross::Api::Object::Ptr setText(Kross::Api::List::Ptr);

            Kross::Api::Object::Ptr isEnabled(Kross::Api::List::Ptr);
            Kross::Api::Object::Ptr setEnabled(Kross::Api::List::Ptr);

            Kross::Api::Object::Ptr activate(Kross::Api::List::Ptr);
    };

}}

#endif

