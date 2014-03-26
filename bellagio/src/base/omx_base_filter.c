/**
  src/omx_base_filter.c

  OpenMAX Base Filter component. This component does not perform any multimedia
  processing. It derives from base component and contains two ports. It can be used
  as a base class for codec and filter components.

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

#include <unistd.h>
#include <asm/unistd.h>
#include <omxcore.h>

#include "omx_base_filter.h"

OMX_ERRORTYPE omx_base_filter_Constructor(OMX_COMPONENTTYPE *openmaxStandComp,OMX_STRING cComponentName) {
  OMX_ERRORTYPE err;
  omx_base_filter_PrivateType* omx_base_filter_Private;

  DEBUG(DEB_LEV_FUNCTION_NAME, "In %s of component %x\n", __func__, (int)openmaxStandComp);
  if (openmaxStandComp->pComponentPrivate) {
    omx_base_filter_Private = (omx_base_filter_PrivateType*)openmaxStandComp->pComponentPrivate;
  } else {
    omx_base_filter_Private = calloc(1,sizeof(omx_base_filter_PrivateType));
    if (!omx_base_filter_Private) {
    	DEBUG(DEB_LEV_ERR, "Insufficient memory in %s\n", __func__);
    	return OMX_ErrorInsufficientResources;
    }
    openmaxStandComp->pComponentPrivate=omx_base_filter_Private;
  }

  /* Call the base class constructor */
  err = omx_base_component_Constructor(openmaxStandComp,cComponentName);
  if (err != OMX_ErrorNone) {
	  DEBUG(DEB_LEV_ERR, "The base constructor failed in %s\n", __func__);
	  return err;
  }
  /* here we can override whatever defaults the base_component constructor set
  * e.g. we can override the function pointers in the private struct */
  omx_base_filter_Private = openmaxStandComp->pComponentPrivate;

  omx_base_filter_Private->BufferMgmtFunction = omx_base_filter_BufferMgmtFunction;

  DEBUG(DEB_LEV_FUNCTION_NAME, "Out of %s of component %x\n", __func__, (int)openmaxStandComp);
  return OMX_ErrorNone;
}

OMX_ERRORTYPE omx_base_filter_Destructor(OMX_COMPONENTTYPE *openmaxStandComp) {
	OMX_ERRORTYPE err;
	  DEBUG(DEB_LEV_FUNCTION_NAME, "In %s of component %x\n", __func__, (int)openmaxStandComp);
	  err = omx_base_component_Destructor(openmaxStandComp);
	  if (err != OMX_ErrorNone) {
		  DEBUG(DEB_LEV_ERR, "The base component destructor failed\n");
		  return err;
	  }
	  DEBUG(DEB_LEV_FUNCTION_NAME, "Out of %s of component %x\n", __func__, (int)openmaxStandComp);
	  return OMX_ErrorNone;
}

/** This is the central function for component processing. It
  * is executed in a separate thread, is synchronized with
  * semaphores at each port, those are released each time a new buffer
  * is available on the given port.
  */
void* omx_base_filter_BufferMgmtFunction (void* param) {
  OMX_COMPONENTTYPE* openmaxStandComp = (OMX_COMPONENTTYPE*)param;
  omx_base_filter_PrivateType* omx_base_filter_Private = (omx_base_filter_PrivateType*)openmaxStandComp->pComponentPrivate;
  omx_base_PortType *pInPort=(omx_base_PortType *)omx_base_filter_Private->ports[OMX_BASE_FILTER_INPUTPORT_INDEX];
  omx_base_PortType *pOutPort=(omx_base_PortType *)omx_base_filter_Private->ports[OMX_BASE_FILTER_OUTPUTPORT_INDEX];
  tsem_t* pInputSem = pInPort->pBufferSem;
  tsem_t* pOutputSem = pOutPort->pBufferSem;
  queue_t* pInputQueue = pInPort->pBufferQueue;
  queue_t* pOutputQueue = pOutPort->pBufferQueue;
  OMX_BUFFERHEADERTYPE* pOutputBuffer=NULL;
  OMX_BUFFERHEADERTYPE* pInputBuffer=NULL;
  OMX_BOOL isInputBufferNeeded=OMX_TRUE,isOutputBufferNeeded=OMX_TRUE;
  int inBufExchanged=0,outBufExchanged=0;

  omx_base_filter_Private->bellagioThreads->nThreadBufferMngtID = (long int)syscall(__NR_gettid);
  DEBUG(DEB_LEV_FUNCTION_NAME, "In %s of component %x\n", __func__, (int)openmaxStandComp);
  DEBUG(DEB_LEV_SIMPLE_SEQ, "In %s the thread ID is %i\n", __func__, (int)omx_base_filter_Private->bellagioThreads->nThreadBufferMngtID);

  DEBUG(DEB_LEV_FUNCTION_NAME, "In %s\n", __func__);
  while(omx_base_filter_Private->state == OMX_StateIdle || omx_base_filter_Private->state == OMX_StateExecuting ||  omx_base_filter_Private->state == OMX_StatePause ||
    omx_base_filter_Private->transientState == OMX_TransStateLoadedToIdle){

    /*Wait till the ports are being flushed*/
    pthread_mutex_lock(&omx_base_filter_Private->flush_mutex);
    while( PORT_IS_BEING_FLUSHED(pInPort) ||
           PORT_IS_BEING_FLUSHED(pOutPort)) {
      pthread_mutex_unlock(&omx_base_filter_Private->flush_mutex);

      DEBUG(DEB_LEV_FULL_SEQ, "In %s 1 signalling flush all cond iE=%d,iF=%d,oE=%d,oF=%d iSemVal=%d,oSemval=%d\n",
        __func__,inBufExchanged,isInputBufferNeeded,outBufExchanged,isOutputBufferNeeded,pInputSem->semval,pOutputSem->semval);

      if(isOutputBufferNeeded==OMX_FALSE && PORT_IS_BEING_FLUSHED(pOutPort)) {
        pOutPort->ReturnBufferFunction(pOutPort,pOutputBuffer);
        outBufExchanged--;
        pOutputBuffer=NULL;
        isOutputBufferNeeded=OMX_TRUE;
        DEBUG(DEB_LEV_FULL_SEQ, "Ports are flushing,so returning output buffer\n");
      }

      if(isInputBufferNeeded==OMX_FALSE && PORT_IS_BEING_FLUSHED(pInPort)) {
        pInPort->ReturnBufferFunction(pInPort,pInputBuffer);
        inBufExchanged--;
        pInputBuffer=NULL;
        isInputBufferNeeded=OMX_TRUE;
        DEBUG(DEB_LEV_FULL_SEQ, "Ports are flushing,so returning input buffer\n");
      }

      DEBUG(DEB_LEV_FULL_SEQ, "In %s 2 signalling flush all cond iE=%d,iF=%d,oE=%d,oF=%d iSemVal=%d,oSemval=%d\n",
        __func__,inBufExchanged,isInputBufferNeeded,outBufExchanged,isOutputBufferNeeded,pInputSem->semval,pOutputSem->semval);

      tsem_up(omx_base_filter_Private->flush_all_condition);
      tsem_down(omx_base_filter_Private->flush_condition);
      pthread_mutex_lock(&omx_base_filter_Private->flush_mutex);
    }
    pthread_mutex_unlock(&omx_base_filter_Private->flush_mutex);

    /*No buffer to process. So wait here*/
    if((isInputBufferNeeded==OMX_TRUE && pInputSem->semval==0) &&
      (omx_base_filter_Private->state != OMX_StateLoaded && omx_base_filter_Private->state != OMX_StateInvalid)) {
      //Signalled from EmptyThisBuffer or FillThisBuffer or some thing else
      DEBUG(DEB_LEV_FULL_SEQ, "Waiting for next input/output buffer\n");
      tsem_down(omx_base_filter_Private->bMgmtSem);

    }
    if(omx_base_filter_Private->state == OMX_StateLoaded || omx_base_filter_Private->state == OMX_StateInvalid) {
      DEBUG(DEB_LEV_SIMPLE_SEQ, "In %s Buffer Management Thread is exiting\n",__func__);
      break;
    }
    if((isOutputBufferNeeded==OMX_TRUE && pOutputSem->semval==0) &&
      (omx_base_filter_Private->state != OMX_StateLoaded && omx_base_filter_Private->state != OMX_StateInvalid) &&
       !(PORT_IS_BEING_FLUSHED(pInPort) || PORT_IS_BEING_FLUSHED(pOutPort))) {
      //Signalled from EmptyThisBuffer or FillThisBuffer or some thing else
      DEBUG(DEB_LEV_FULL_SEQ, "Waiting for next input/output buffer\n");
      tsem_down(omx_base_filter_Private->bMgmtSem);

    }
    if(omx_base_filter_Private->state == OMX_StateLoaded || omx_base_filter_Private->state == OMX_StateInvalid) {
      DEBUG(DEB_LEV_SIMPLE_SEQ, "In %s Buffer Management Thread is exiting\n",__func__);
      break;
    }

    DEBUG(DEB_LEV_FULL_SEQ, "Waiting for input buffer semval=%d \n",pInputSem->semval);
    if(pInputSem->semval>0 && isInputBufferNeeded==OMX_TRUE ) {
      tsem_down(pInputSem);
      if(pInputQueue->nelem>0){
        inBufExchanged++;
        isInputBufferNeeded=OMX_FALSE;
        pInputBuffer = dequeue(pInputQueue);
        if(pInputBuffer == NULL){
          DEBUG(DEB_LEV_ERR, "Had NULL input buffer!!\n");
          break;
        }
      }
    }
    /*When we have input buffer to process then get one output buffer*/
    if(pOutputSem->semval>0 && isOutputBufferNeeded==OMX_TRUE) {
      tsem_down(pOutputSem);
      if(pOutputQueue->nelem>0){
        outBufExchanged++;
        isOutputBufferNeeded=OMX_FALSE;
        pOutputBuffer = dequeue(pOutputQueue);
        if(pOutputBuffer == NULL){
          DEBUG(DEB_LEV_ERR, "Had NULL output buffer!! op is=%d,iq=%d\n",pOutputSem->semval,pOutputQueue->nelem);
          break;
        }
      }
    }

    if(isInputBufferNeeded==OMX_FALSE) {
      if(pInputBuffer->hMarkTargetComponent != NULL){
        if((OMX_COMPONENTTYPE*)pInputBuffer->hMarkTargetComponent ==(OMX_COMPONENTTYPE *)openmaxStandComp) {
          /*Clear the mark and generate an event*/
          (*(omx_base_filter_Private->callbacks->EventHandler))
            (openmaxStandComp,
            omx_base_filter_Private->callbackData,
            OMX_EventMark, /* The command was completed */
            1, /* The commands was a OMX_CommandStateSet */
            0, /* The state has been changed in message->messageParam2 */
            pInputBuffer->pMarkData);
        } else {
          /*If this is not the target component then pass the mark*/
          omx_base_filter_Private->pMark.hMarkTargetComponent = pInputBuffer->hMarkTargetComponent;
          omx_base_filter_Private->pMark.pMarkData            = pInputBuffer->pMarkData;
        }
        pInputBuffer->hMarkTargetComponent = NULL;
      }
    }

    if(isInputBufferNeeded==OMX_FALSE && isOutputBufferNeeded==OMX_FALSE) {

      if(omx_base_filter_Private->pMark.hMarkTargetComponent != NULL){
        pOutputBuffer->hMarkTargetComponent = omx_base_filter_Private->pMark.hMarkTargetComponent;
        pOutputBuffer->pMarkData            = omx_base_filter_Private->pMark.pMarkData;
        omx_base_filter_Private->pMark.hMarkTargetComponent = NULL;
        omx_base_filter_Private->pMark.pMarkData            = NULL;
      }

      pOutputBuffer->nTimeStamp = pInputBuffer->nTimeStamp;
      if((pInputBuffer->nFlags & OMX_BUFFERFLAG_STARTTIME) == OMX_BUFFERFLAG_STARTTIME) {
         DEBUG(DEB_LEV_FULL_SEQ, "Detected  START TIME flag in the input buffer filled len=%d\n", (int)pInputBuffer->nFilledLen);
         pOutputBuffer->nFlags = pInputBuffer->nFlags;
         pInputBuffer->nFlags = 0;
      }

      if(omx_base_filter_Private->state == OMX_StateExecuting)  {
        if (omx_base_filter_Private->BufferMgmtCallback && pInputBuffer->nFilledLen > 0) {
          (*(omx_base_filter_Private->BufferMgmtCallback))(openmaxStandComp, pInputBuffer, pOutputBuffer);
        } else {
          /*It no buffer management call back the explicitly consume input buffer*/
          pInputBuffer->nFilledLen = 0;
        }
      } else if(!(PORT_IS_BEING_FLUSHED(pInPort) || PORT_IS_BEING_FLUSHED(pOutPort))) {
        DEBUG(DEB_LEV_ERR, "In %s Received Buffer in non-Executing State(%x)\n", __func__, (int)omx_base_filter_Private->state);
      } else {
          pInputBuffer->nFilledLen = 0;
      }

      if((pInputBuffer->nFlags & OMX_BUFFERFLAG_EOS) == OMX_BUFFERFLAG_EOS && pInputBuffer->nFilledLen==0) {
        DEBUG(DEB_LEV_FULL_SEQ, "Detected EOS flags in input buffer filled len=%d\n", (int)pInputBuffer->nFilledLen);
        pOutputBuffer->nFlags=pInputBuffer->nFlags;
        pInputBuffer->nFlags=0;
        (*(omx_base_filter_Private->callbacks->EventHandler))
          (openmaxStandComp,
          omx_base_filter_Private->callbackData,
          OMX_EventBufferFlag, /* The command was completed */
          1, /* The commands was a OMX_CommandStateSet */
          pOutputBuffer->nFlags, /* The state has been changed in message->messageParam2 */
          NULL);
        omx_base_filter_Private->bIsEOSReached = OMX_TRUE;
      }
      if(omx_base_filter_Private->state==OMX_StatePause && !(PORT_IS_BEING_FLUSHED(pInPort) || PORT_IS_BEING_FLUSHED(pOutPort))) {
        /*Waiting at paused state*/
        tsem_wait(omx_base_filter_Private->bStateSem);
      }

      /*If EOS and Input buffer Filled Len Zero then Return output buffer immediately*/
      if((pOutputBuffer->nFilledLen != 0) || ((pOutputBuffer->nFlags & OMX_BUFFERFLAG_EOS) == OMX_BUFFERFLAG_EOS) || (omx_base_filter_Private->bIsEOSReached == OMX_TRUE)) {
        pOutPort->ReturnBufferFunction(pOutPort,pOutputBuffer);
        outBufExchanged--;
        pOutputBuffer=NULL;
        isOutputBufferNeeded=OMX_TRUE;
      }
    }

    if(omx_base_filter_Private->state==OMX_StatePause && !(PORT_IS_BEING_FLUSHED(pInPort) || PORT_IS_BEING_FLUSHED(pOutPort))) {
      /*Waiting at paused state*/
      tsem_wait(omx_base_filter_Private->bStateSem);
    }

    /*Input Buffer has been completely consumed. So, return input buffer*/
    if((isInputBufferNeeded == OMX_FALSE) && (pInputBuffer->nFilledLen==0)) {
      pInPort->ReturnBufferFunction(pInPort,pInputBuffer);
      inBufExchanged--;
      pInputBuffer=NULL;
      isInputBufferNeeded=OMX_TRUE;
    }
  }
  DEBUG(DEB_LEV_FUNCTION_NAME, "Out of %s of component %x\n", __func__, (int)openmaxStandComp);
  return NULL;
}
