/**
  src/utils.c

  Set of utility functions for debugging purposes

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


#include "utils.h"

/*
 * sizeof(word) MUST BE A POWER OF TWO
 * SO THAT wmask BELOW IS ALL ONES
 */
typedef	long word;		/* "word" used for optimal copy speed */

#define	wsize	sizeof(word)
#define	wmask	(wsize - 1)

char *stateName(OMX_STATETYPE state) {
	char *nameString;
	switch(state) {
	case 0:
		nameString = "OMX_StateInvalid";
		break;
	case 1:
		nameString = "OMX_StateLoaded";
		break;
	case 2:
		nameString = "OMX_StateIdle";
		break;
	case 3:
		nameString = "OMX_StateExecuting";
		break;
	case 4:
		nameString = "OMX_StatePause";
		break;
	case 5:
		nameString = "OMX_StateWaitForResources";
		break;
	default: nameString = '\0';
	}
	return nameString;
}

char *transientStateName(int state) {
	char *nameString;
	switch(state) {
	case 0:
		nameString = "OMX_StateInvalid";
		break;
	case 1:
		nameString = "OMX_TransStateLoadedToIdle";
		break;
	case 2:
		nameString = "OMX_TransStateIdleToPause";
		break;
	case 3:
		nameString = "OMX_TransStatePauseToExecuting";
		break;
	case 4:
		nameString = "OMX_TransStateIdleToExecuting";
		break;
	case 5:
		nameString = "OMX_TransStateExecutingToIdle";
		break;
	case 6:
		nameString = "OMX_TransStateExecutingToPause";
		break;
	case 7:
		nameString = "OMX_TransStatePauseToIdle";
		break;
	case 8:
		nameString = "OMX_TransStateIdleToLoaded";
		break;
	default: nameString = '\0';
	}
	return nameString;
}

char *errorName(OMX_ERRORTYPE error) {
	char *nameString;
	switch(error) {
	case 0:
		nameString = "OMX_ErrorNone";
		break;
	case 0x80001000:
		nameString = "OMX_ErrorInsufficientResources";
		break;
	case 0x80001001:
		nameString = "OMX_ErrorUndefined";
		break;
	case 0x80001002:
		nameString = "OMX_ErrorInvalidComponentName";
		break;
	case 0x80001003:
		nameString = "OMX_ErrorComponentNotFound";
		break;
	case 0x80001004:
		nameString = "OMX_ErrorInvalidComponent";
		break;
	case 0x80001005:
		nameString = "OMX_ErrorBadParameter";
		break;
	case 0x80001006:
		nameString = "OMX_ErrorNotImplemented";
		break;
	case 0x80001007:
		nameString = "OMX_ErrorUnderflow";
		break;
	case 0x80001008:
		nameString = "OMX_ErrorOverflow";
		break;
	case 0x80001009:
		nameString = "OMX_ErrorHardware";
		break;
	case 0x8000100A:
		nameString = "OMX_ErrorInvalidState";
		break;
	case 0x8000100B:
		nameString = "OMX_ErrorStreamCorrupt";
		break;
	case 0x8000100C:
		nameString = "OMX_ErrorPortsNotCompatible";
		break;
	case 0x8000100D:
		nameString = "OMX_ErrorResourcesLost";
		break;
	case 0x8000100E:
		nameString = "OMX_ErrorNoMore";
		break;
	case 0x8000100F:
		nameString = "OMX_ErrorVersionMismatch";
		break;
	case 0x80001010:
		nameString = "OMX_ErrorNotReady";
		break;
	case 0x80001011:
		nameString = "OMX_ErrorTimeout";
		break;
	case 0x80001012:
		nameString = "OMX_ErrorSameState";
		break;
	case 0x80001013:
		nameString = "OMX_ErrorResourcesPreempted";
		break;
	case 0x80001014:
		nameString = "OMX_ErrorPortUnresponsiveDuringAllocation";
		break;
	case 0x80001015:
		nameString = "OMX_ErrorPortUnresponsiveDuringDeallocation";
		break;
	case 0x80001016:
		nameString = "OMX_ErrorPortUnresponsiveDuringStop";
		break;
	case 0x80001017:
		nameString = "OMX_ErrorIncorrectStateTransition";
		break;
	case 0x80001018:
		nameString = "OMX_ErrorIncorrectStateOperation";
		break;
	case 0x80001019:
		nameString = "OMX_ErrorUnsupportedSetting";
		break;
	case 0x8000101A:
		nameString = "OMX_ErrorUnsupportedIndex";
		break;
	case 0x8000101B:
		nameString = "OMX_ErrorBadPortIndex";
		break;
	case 0x8000101C:
		nameString = "OMX_ErrorPortUnpopulated";
		break;
	case 0x8000101D:
		nameString = "OMX_ErrorComponentSuspended";
		break;
	case 0x8000101E:
		nameString = "OMX_ErrorDynamicResourcesUnavailable";
		break;
	case 0x8000101F:
		nameString = "OMX_ErrorMbErrorsInFrame";
		break;
	case 0x80001020:
		nameString = "OMX_ErrorFormatNotDetected";
		break;
	case 0x80001021:
		nameString = "OMX_ErrorContentPipeOpenFailed";
		break;
	case 0x80001022:
		nameString = "OMX_ErrorContentPipeCreationFailed";
		break;
	case 0x80001023:
		nameString = "OMX_ErrorSeperateTablesUsed";
		break;
	case 0x80001024:
		nameString = "OMX_ErrorTunnelingUnsupported";
		break;
	default: nameString = '\0';
	}
	return nameString;
}

/*
 * Copy a block of memory, handling overlap.
 * Method taken from bionic to prevent size check.
 */
void *
memcpy_unsafe(void *dst0, const void *src0, size_t length)
{
	char *dst = dst0;
	const char *src = src0;
	size_t t;

	if (length == 0 || dst == src)		/* nothing to do */
		goto done;

	/*
	 * Macros: loop-t-times; and loop-t-times, t>0
	 */
#define	TLOOP(s) if (t) TLOOP1(s)
#define	TLOOP1(s) do { s; } while (--t)

	if ((unsigned long)dst < (unsigned long)src) {
		/*
		 * Copy forward.
		 */
		t = (long)src;	/* only need low bits */
		if ((t | (long)dst) & wmask) {
			/*
			 * Try to align operands.  This cannot be done
			 * unless the low bits match.
			 */
			if ((t ^ (long)dst) & wmask || length < wsize)
				t = length;
			else
				t = wsize - (t & wmask);
			length -= t;
			TLOOP1(*dst++ = *src++);
		}
		/*
		 * Copy whole words, then mop up any trailing bytes.
		 */
		t = length / wsize;
		TLOOP(*(word *)dst = *(word *)src; src += wsize; dst += wsize);
		t = length & wmask;
		TLOOP(*dst++ = *src++);
	} else {
		/*
		 * Copy backwards.  Otherwise essentially the same.
		 * Alignment works as before, except that it takes
		 * (t&wmask) bytes to align, not wsize-(t&wmask).
		 */
		src += length;
		dst += length;
		t = (long)src;
		if ((t | (long)dst) & wmask) {
			if ((t ^ (long)dst) & wmask || length <= wsize)
				t = length;
			else
				t &= wmask;
			length -= t;
			TLOOP1(*--dst = *--src);
		}
		t = length / wsize;
		TLOOP(src -= wsize; dst -= wsize; *(word *)dst = *(word *)src);
		t = length & wmask;
		TLOOP(*--dst = *--src);
	}
done:
	return (dst0);
}
