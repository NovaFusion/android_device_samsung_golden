/**
  @file src/getline.h

  ST Ericsson specific getline file.

  Copyright (C) 2009 STEricsson

  This library is free software; you can redistribute it and/or modify it under
  the terms of the GNU Lesser General Public License as published by the Free
  Software Foundation; either version 2.1 of the License, or (at your option)
  any later version.

  This library is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
  FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
  details.

  You should have received a copy of the GNU Lesser General Public License
  along with this library; if not, write to the Free Software Foundation, Inc.,
  51 Franklin St, Fifth Floor, Boston, MA
  02110-1301  USA

*/

#include <stdlib.h>
#include <stdio.h>
#include "getline.h"
int getdelim1(char **lineptr, size_t *n, int delim, FILE *stream)
{
        char *p;                    // reads stored here
        size_t const rchunk = 512;  // number of bytes to read
        size_t const mchunk = 512;  // number of extra bytes to malloc
        size_t m = rchunk + 1;      // initial buffer size

        if (*lineptr) {
                if (*n < m) {
                        *lineptr = (char*)realloc(*lineptr, m);
                        if (!*lineptr) return -1;
                        *n = m;
                }
        } else {
                *lineptr = (char*)malloc(m);
                if (!*lineptr) return -1;
                *n = m;
        }

        m = 0; // record length including seperator

        do {
                size_t i;     // number of bytes read etc
                size_t j = 0; // number of bytes searched

                p = *lineptr + m;

                i = fread(p, 1, rchunk, stream);
                if (i < rchunk && ferror(stream))
                        return -1;
		while (j < i) {
                        ++j;
                        if (*p++ == (char)delim) {
                                *p = '\0';
                                if (j != i) {
                                        if (fseek(stream, j - i, SEEK_CUR))
                                                return -1;
                                        if (feof(stream))
                                                clearerr(stream);
                                }
                                m += j;
                                return m;
                        }
                }

                m += j;
                if (feof(stream)) {
                        if (m) return m;
                        if (!i) return -1;
                }

                // allocate space for next read plus possible null terminator
                i = ((m + (rchunk + 1 > mchunk ? rchunk + 1 : mchunk) +
                      mchunk - 1) / mchunk) * mchunk;
                if (i != *n) {
                        *lineptr = (char*)realloc(*lineptr, i);
                        if (!*lineptr) return -1;
                        *n = i;
                }
        } while (1);
}

int getline(char **lineptr, size_t *n, FILE *stream)
{
        return getdelim1(lineptr,n,'\n',stream);
}
