/*
$Header: /var/lib/cvsd/var/lib/cvsd/smaSHeM/src/smaSHeM.h,v 1.3 2013-11-06 15:18:13 timb Exp $

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

(c) Tim Brown, 2012
<mailto:timb@nth-dimension.org.uk>
<http://www.nth-dimension.org.uk/> / <http://www.machine.org.uk/>
*/

#ifndef SMASHEM_H
#define SMASHEM_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <ctype.h>

#ifdef WITH_QTGUI

#include <Qt/qimage.h>
#include <sys/wait.h>

#else

// Qt comes with true and false
#define TRUE 1
#define FALSE 0

#endif

#define PRETTYLINELENGTH 8

int main(int argc, char **argv);

#endif
