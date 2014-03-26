/**
  src/omx_create_loaders_linux.c

  In this file is implemented the entry point for the construction
  of every component loader in linux. In the current implementation
  only the ST static loader is called.

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

#define _GNU_SOURCE
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>
#include "component_loader.h"
#include "omx_create_loaders.h"
#include "st_static_component_loader.h"
#include "common.h"

#define OMX_LOADERS_FILENAME ".omxloaders"

#ifndef INSTALL_PATH_STR
#define INSTALL_PATH_STR "/usr/local/lib/bellagio"
#endif
#define DEFAULT_LOADER_LIBRARY_NAME "/libstbaseloader.so"

/**
 * This function allocates all the structures needed by the component loaders
 * available in the system and initialize the function pointers of the loader.
 * Finally it searches for more loaders that can be pointed by the .omxloaders
 * file. If this file exists it contains a list of library containing custom
 * component loaders. Each library should contain a function named 'setup_component_loader'
 * that will initialize the custom loader. This mechanism is similar to the
 * default loading of the components.
 *
 */
int createComponentLoaders() {
	// load component loaders
	BOSA_COMPONENTLOADER *loader;
	void *handle;
	void *functionPointer;
	void (*fptr)(BOSA_COMPONENTLOADER *loader);
	char *libraryFileName = NULL;
	FILE *loaderFP;
	size_t len = 0;
	int read;
	char *omxloader_registry_filename;
	char *dir, *dirp;
	int onlyDefault = 0;
	int oneAtLeast = 0;
	OMX_ERRORTYPE eError;

	omxloader_registry_filename = loadersRegistryGetFilename(OMX_LOADERS_FILENAME);

#if 0	/* !! No need to check if directory exist + below code is wrong in case file is under / !! */

	/* make sure the registry directory exists */
	dir = strdup(omxloader_registry_filename);
	if (dir == NULL) {
		DEBUG(DEB_LEV_ERR, "The directory has not been specified\n");
		onlyDefault = 1;
	}
	dirp = strrchr(dir, '/');
	if (dirp != NULL) {
		*dirp = '\0';
		if (makedir(dir)) {
			DEBUG(DEB_LEV_ERR, "Cannot create OpenMAX registry directory %s\n", dir);
			onlyDefault = 1;
		}
	}
	free(dir);
#endif
	/* test the existence of the file */
	loaderFP = fopen(omxloader_registry_filename, "r");
	if (loaderFP == NULL){
		onlyDefault = 1;
	}
	free(omxloader_registry_filename);
	if (onlyDefault) {
		loader = calloc(1, sizeof(BOSA_COMPONENTLOADER));
		if (loader == NULL) {
				DEBUG(DEB_LEV_ERR, "not enough memory for this loader\n");
				return OMX_ErrorInsufficientResources;
		}
		st_static_setup_component_loader(loader);
		eError = BOSA_AddComponentLoader(loader);
		if (eError != OMX_ErrorNone) {
			free(loader);
		}
		return 0;
	}
	// dlopen all loaders defined in .omxloaders file
	while((read = getline(&libraryFileName, &len, loaderFP)) != -1) {

		// strip delimeter, the dlopen doesn't like it
		if(libraryFileName[read-1] == '\n') {
			libraryFileName[read-1] = 0;
		}
		handle = dlopen(libraryFileName, RTLD_NOW);

		if (!handle)
		{
			DEBUG(DEB_LEV_ERR, "library %s dlopen error: %s\n", libraryFileName, dlerror());
			continue;
		}

		if ((functionPointer = dlsym(handle, "setup_component_loader")) == NULL)
		{
			DEBUG(DEB_LEV_ERR, "the library %s is not compatible - %s\n", libraryFileName, dlerror());
			continue;
		}
		fptr = functionPointer;

		loader = calloc(1, sizeof(BOSA_COMPONENTLOADER));

		if (loader == NULL)
		{
			DEBUG(DEB_LEV_ERR, "not enough memory for this loader\n");
			fclose(loaderFP);
			return OMX_ErrorInsufficientResources;
		}

		/* setup the function pointers */
		(*fptr)(loader);

		/* add loader to core */
		eError = BOSA_AddComponentLoader(loader);
		if (eError != OMX_ErrorNone) {
			free(loader);
			continue;
		}
		oneAtLeast = 1;
	}
	if (!oneAtLeast) {
		/* add the ST static component loader */
		loader = calloc(1, sizeof(BOSA_COMPONENTLOADER));
		if (loader == NULL) {
			DEBUG(DEB_LEV_ERR, "not enough memory for this loader\n");
			fclose(loaderFP);
			return OMX_ErrorInsufficientResources;
		}
		st_static_setup_component_loader(loader);
		eError = BOSA_AddComponentLoader(loader);
		if (eError != OMX_ErrorNone) {
			free(loader);
			return OMX_ErrorInsufficientResources;
		}
	} else {
		/* Always register ST static component loader even if another one found */
		loader = calloc(1, sizeof(BOSA_COMPONENTLOADER));
		st_static_setup_component_loader(loader);
		eError = BOSA_AddComponentLoader(loader);
		if (eError != OMX_ErrorNone) {
			free(loader);
			return OMX_ErrorInsufficientResources;
		}
	}
	if (libraryFileName)
	{
		free(libraryFileName);
	}

	fclose(loaderFP);

	return 0;
}
