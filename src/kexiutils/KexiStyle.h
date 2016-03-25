/* This file is part of the KDE project
   Copyright (C) 2016 Jarosław Staniek <staniek@kde.org>

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

#ifndef KEXISTYLE_H
#define KEXISTYLE_H

#include <kexiutils_export.h>

#include <KIconLoader>

#include <QColor>

class QFont;
class QFrame;
class QIcon;
class QModelIndex;
class QPainter;
class QPalette;
class QRect;
class QStyleOptionViewItem;
class QWidget;

//! Styled icon parameters
class KEXIUTILS_EXPORT KexiStyledIconParameters {
public:
    explicit KexiStyledIconParameters(KIconLoader::Context c = KIconLoader::Action) : context(c)
    {
    }
    //! Icon name
    QString name;
    //! Icon color, when used for normal mode (QIcon::Normal).
    //! If the value is valid, default color will be replaced with it.
    QColor color;
    //! Icon color, when used for selected mode (QIcon::Selected).
    //! If the value is valid, default color will be replaced with it.
    QColor selectedColor;
    //! Icon color, when used for disabled palette group (QPalette::Disabled).
    //! If the value is valid, default color will be replaced with it.
    QColor disabledColor;
    //! Icon context such as "actions"
    KIconLoader::Context context;
};

//! Application style.
//! @todo make it configurable?
namespace KexiStyle
{
    //! Setup style for @a frame. By default flat style is set (QFrame::NoFrame).
    KEXIUTILS_EXPORT void setupFrame(QFrame *frame);

    //! Setup style for the global view mode selector widget (KexiGlobalViewModeSelector).
    //! By default setupFrame() is called to set flat style, minimal fonts are set
    //! and a bit darker version of alternativePalette().
    KEXIUTILS_EXPORT void setupGlobalViewModeSelector(QFrame *selector);

    //! Overpaints entire global view mode selector. By default it paints a dark shadow an arrow
    //! for selected item. If @a selectedRect is not null, provides geometry of selected item.
    KEXIUTILS_EXPORT void overpaintGlobalViewModeSelector(QWidget *widget, QPainter *painter,
                                                          const QRect &selectedRect);

    //! Overpaints global view mode selector's item. By default does nothing.
    KEXIUTILS_EXPORT void overpaintModeSelectorItem(QPainter *painter,
                                                    const QStyleOptionViewItem &option,
                                                    const QModelIndex &index);

    //! @return alternative palette based on @a palette.
    //! By default it is dark one based on Breeze colors.
    KEXIUTILS_EXPORT QPalette alternativePalette(const QPalette &palette);

    //! @return palette dedicated for side bars, based on @a palette.
    //! By default it equal to alternativePalette().
    KEXIUTILS_EXPORT QPalette sidebarsPalette(const QPalette &palette);

    //! Sets sidebarsPalette(). If it's nonstandard palette,
    //! QWidget::setAutoFillBackground(true) is also called for the widget.
    KEXIUTILS_EXPORT void setSidebarsPalette(QWidget *widget);

    //! @return font @a font adjusted to make it a title font.
    //! By default capitalization is set for the font.
    KEXIUTILS_EXPORT QFont titleFont(const QFont &font);

    //! @return styled dark icon @a iconName of context @a iconContext. Group can be "actions", etc.
    //! The same styled icon can be used with light and dark background. Filename for dark
    //! variants should have "@dark" suffix.
    //! KIconLoader::Any means KIconLoader::Action.
    KEXIUTILS_EXPORT QIcon darkIcon(const QString &iconName,
                                    KIconLoader::Context iconContext = KIconLoader::Action);

    KEXIUTILS_EXPORT QIcon icon(const KexiStyledIconParameters &parameters);
}

#endif // KEXISTYLE_H
