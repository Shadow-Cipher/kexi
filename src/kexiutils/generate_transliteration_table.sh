#!/bin/bash
#
#   Copyright (C) 2006-2007 Jaroslaw Staniek <js@iidea.pl>
#
#   Based on the original script by Michal Svec <rebel@atrey.karlin.mff.cuni.cz>
#
#   This program is free software; you can redistribute it and/or
#   modify it under the terms of the GNU General Public
#   License as published by the Free Software Foundation; either
#   version 2 of the License, or (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#    General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program; see the file COPYING.  If not, write to
#   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
#   Boston, MA 02111-1307, USA.

#
# Generates a transliteration_table.{h|cpp} files using recode's "flat" character set
#

out_cpp="transliteration_table.cpp"
out_h="transliteration_table.h"
max=65534

decl="const char *const transliteration_table[TRANSLITERATION_TABLE_SIZE + 1]"

header=\
"/* Transliteration table of `expr $max + 1` unicode characters
   Do not edit this file, it is generated
   by $0 script. */

"
echo "$header
#define TRANSLITERATION_TABLE_SIZE `expr $max + 1`
extern $decl;
" > $out_h

echo "$header
#include \"$out_h\"
$decl = {
" > $out_cpp

for i in `seq 0 $max` ; do
	f=`printf "%04x" $i`
	if [ "$i" -lt 16 -o "$i" -eq 92 ] ; then
		printf "$i\n/*$f*/\n_\n" $i
	elif [ "$i" -lt 128 ] ; then
		ch=`printf "%03o" $i`
		printf "$i\n/*$f*/\n\\"$ch"\n"
	else
		{ /usr/bin/printf "${i}\n/*${f}*/\n\u${f}\n" 2>&- || echo "_"; }
	fi
done | \
while read i && read f && read ch; do
	if ! expr "$i" % 8 > /dev/null ; then
		expr "$i" % 320 > /dev/null || echo -n ..`expr "$i" \* 100 / $max `% >&2 #progress
		echo
	else
		f= # <-- comment to add /*numbers*/ everywhere
	fi
	r=`echo -n "$ch" | recode -f utf-8..flat | \
		sed -r -e 's/[^[:alnum:]]//g;s/_+/_/g'`
	if [ -z "$r" -o "$r" == "_" ] ; then
		echo -n "${f}0/*${ch}*/,"
	else
		echo -n "${f}\"$r\"/*${ch}*/,"
	fi
done >> $out_cpp

echo "0};" >> $out_cpp;
