/* This file is part of MVTH - the Machine Vision Test Harness.
 * 
 * Copyright Sam Bromley (sam@sambromley.com) 2005.
 *
 * The function in this file is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * You may do with it as your wish.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <assert.h>

void *load_symbol(char *libname,char *symbname, void *handle)
{
	void *h=NULL;
	void *symbol;
	assert(handle!=NULL);
	if (*(char**)handle==NULL)
	{
		dlerror(); /* clear any old errors */
		h=dlopen(libname,RTLD_NOW);
		if (h==NULL)
		{
			fprintf(stderr,"Error opening library '%s'\n",libname);
			fprintf(stderr,"%s\n",dlerror());
			return NULL;
		}
		*(char**)handle=(char*)h; /* cast to char * only to avoid compiler warnings*/
	} else {
		h=*(char**)handle;
	}
	dlerror();
	/* try to load the requested symbol */
	symbol=dlsym(h,symbname);
	if (symbol==NULL)
	{
		fprintf(stderr,"Error getting symbol '%s'\n",symbname);
		fprintf(stderr,"%s\n",dlerror());
		return NULL;
	}
	return symbol;
}

void release_handle(void *handle)
{
	void *h=*(char**)handle;
	dlclose(h);
	*(char**)handle=NULL;
}
