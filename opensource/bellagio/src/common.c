/**
  src/common.c

  This file implements some useful common functionalities for handling the register files used in Bellagio

  Copyright (C) 2007-2009 STMicroelectronics
  Copyright (C) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).

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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

#include "common.h"

#define REGISTRY_FILENAME ".omxregister"

/** @brief Get registry filename
 * This function returns the name of the registry file for the components loaded with the default component loader.
 */
char* componentsRegistryGetFilename() {
  char *omxregistryfile = NULL;
  char *buffer;

  buffer=getenv("OMX_BELLAGIO_REGISTRY");
  if(buffer!=NULL&&*buffer!='\0') {
    omxregistryfile = strdup(buffer);
    return omxregistryfile;
  }

  buffer=getenv("XDG_DATA_HOME");
  if(buffer!=NULL&&*buffer!='\0') {
    omxregistryfile = malloc(strlen(buffer) + strlen(REGISTRY_FILENAME) + 2);
    strcpy(omxregistryfile, buffer);
    strcat(omxregistryfile, "/");
    strcat(omxregistryfile, REGISTRY_FILENAME);
    return omxregistryfile;
  }

  buffer=getenv("HOME");
  if(buffer!=NULL&&*buffer!='\0') {
    omxregistryfile  = malloc(strlen(buffer) + strlen(REGISTRY_FILENAME) + 3);
    strcpy(omxregistryfile, buffer);
    strcat(omxregistryfile, "/");
    strcat(omxregistryfile, REGISTRY_FILENAME);
  } else {
    omxregistryfile  = malloc(strlen("/tmp/") + strlen(REGISTRY_FILENAME) + 2);
    strcpy(omxregistryfile, "/tmp/");
    strcat(omxregistryfile, REGISTRY_FILENAME);
  }
  return omxregistryfile;
}

char* loadersRegistryGetFilename(char* registry_name) {
	char *omxregistryfile = NULL;
	char *buffer;

  buffer=getenv("OMX_BELLAGIO_LOADER_REGISTRY");
  if(buffer!=NULL&&*buffer!='\0') {
    omxregistryfile = strdup(buffer);
    return omxregistryfile;
  }

	buffer=getenv("XDG_DATA_HOME");
	if(buffer!=NULL&&*buffer!='\0') {
		omxregistryfile = malloc(strlen(buffer) + strlen(registry_name) + 2);
		strcpy(omxregistryfile, buffer);
		strcat(omxregistryfile, "/");
		strcat(omxregistryfile, registry_name);
		return omxregistryfile;
	}

	buffer=getenv("HOME");
	if(buffer!=NULL&&*buffer!='\0') {
		omxregistryfile  = malloc(strlen(buffer) + strlen(registry_name) + 3);
		strcpy(omxregistryfile, buffer);
		strcat(omxregistryfile, "/");
		strcat(omxregistryfile, registry_name);
	} else {
		omxregistryfile  = malloc(strlen("/tmp/") + strlen(registry_name) + 2);
		strcpy(omxregistryfile, "/tmp/");
		strcat(omxregistryfile, registry_name);
	}
	return omxregistryfile;
}

/** This function creates the directory specified by newdir
 * It returns 1 if is not possible to create it
 * It returns 0 if the directory is created or it already exists
 */
int makedir (const char *newdir) {
  char *buffer;
  char *p;
  int err;
  size_t len;

  buffer = strdup(newdir);
  len = strlen(buffer);

  if (len == 0) {
    free(buffer);
    return 1;
  }
  if (buffer[len-1] == '/') {
    buffer[len-1] = '\0';
  }

	err = mkdir(buffer, 0755);
	if (err == 0 || (( err == -1) && (errno == EEXIST))) {
		free(buffer);
		return 0;
	}

  p = buffer+1;
  while (1) {
		char hold;

		while(*p && *p != '\\' && *p != '/')
			p++;
		hold = *p;
		*p = 0;
		if ((mkdir(buffer, 0755) == -1) && (errno == ENOENT)) {
			free(buffer);
			return 1;
		}
		if (hold == 0)
			break;
		*p++ = hold;
	}
  free(buffer);
  return 0;
}


/** This function adds an new element to a given list
 * It returns 0 if component is added with no error
 * It returns -1 otherwise
 */
int addComponentToList(HandleList** list, void* handle) {
  HandleList *componentTemp;
  HandleList *componentNext;

  if (!*list) {
    *list = malloc(sizeof(HandleList));
    if (!*list) {
      return -1;
    }
    (*list)->handle = handle;
    (*list)->next = NULL;
    return 0;
  }
  componentTemp = *list;
  while(componentTemp->next) {
    componentTemp = componentTemp->next;
  }
  componentNext = malloc(sizeof(HandleList));
  if (!componentNext) {
    return -1;
  }
  componentTemp->next = componentNext;
  componentNext->next = NULL;
  componentNext->handle = handle;
  return 0;
}

/** This function removes the given element from the list
 * It returns 0 if the component was removed
 * It returns -1 if component was not found in the list
 */
int removeComponentFromList(HandleList** list, void* handle) {
  HandleList* componentTemp;
  HandleList* componentPrev;
  int bFound = 0;

  if (!*list) {
    return -1;
  }
  componentTemp = *list;
  componentPrev = *list;
  while(componentTemp) {
    if (componentTemp->handle == handle) {
      if (componentTemp == *list) {
        *list = (*list)->next;
        free(componentTemp);
      } else {
        componentPrev->next = componentTemp->next;
        free(componentTemp);
      }
      bFound = 1;
      break;
    } else {
      if (componentTemp != *list) {
        componentPrev = componentPrev->next;
      }
      componentTemp = componentTemp->next;
    }
  }
  if(!bFound) {
      return -1;
  }
  return 0;
}




#ifdef COMMON_TEST
int main (int argc, char*argv[]) {
  printf (componentsRegistryGetFilename ());
}
#endif

