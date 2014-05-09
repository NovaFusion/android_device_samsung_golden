/**
  src/omx_base_source.c

  OpenMAX base source component. This component does not perform any multimedia
  processing. It derives from base component and contains a single output port.
  This class can also be used for a two port source component
  It can be used as base class for source components.

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

#include <omxcore.h>
#include <omx_base_source.h>

OMX_ERRORTYPE omx_base_source_Constructor(OMX_COMPONENTTYPE *openmaxStandComp, OMX_STRING cComponentName) {

  OMX_ERRORTYPE err = OMX_ErrorNone;
  omx_base_source_PrivateType* omx_base_source_Private;

  if (openmaxStandComp->pComponentPrivate) {
    omx_base_source_Private = (omx_base_source_PrivateType*)openmaxStandComp->pComponentPrivate;
  } else {
    omx_base_source_Private = calloc(1,sizeof(omx_base_source_PrivateType));
    if (!omx_base_source_Private) {
      return OMX_ErrorInsufficientResources;
    }
  }

  // we could create our own port structures here
  // fixme maybe the base class could use a "port factory" function pointer?
  err = omx_base_component_Constructor(openmaxStandComp, cComponentName);

  /* here we can override whatever defaults the base_component constructor set
  * e.g. we can override the function pointers in the private struct  */
  omx_base_source_Private = openmaxStandComp->pComponentPrivate;
  omx_base_source_Private->BufferMgmtFunction = omx_base_source_BufferMgmtFunction;

  return err;
}

OMX_ERRORTYPE omx_base_source_Destructor(OMX_COMPONENTTYPE *openmaxStandComp) {

  return omx_base_component_Destructor(openmaxStandComp);
}

/** This is the central function for component processing. It
  * is executed in a separate thread, is synchronized with
  * semaphores at each port, those are released each time a new buffer
  * is available on the given port.
  */
void* omx_base_source_BufferMgmtFunction (void* param) {

  OMX_COMPONENTTYPE* openmaxStandComp = (OMX_COMPONENTTYPE*)param;
  omx_base_component_PrivateType* omx_base_component_Private = (omx_base_component_PrivateType*)openmaxStandComp->pComponentPrivate;
  omx_base_source_PrivateType* omx_base_source_Private = (omx_base_source_PrivateType*)omx_base_component_Private;
  omx_base_PortType *pOutPort = (omx_base_PortType *)omx_base_source_Private->ports[OMX_BASE_SOURCE_OUTPUTPORT_INDEX];
  tsem_t* pOutputSem = pOutPort->pBufferSem;
  queue_t* pOutputQueue = pOutPort->pBufferQueue;
  OMX_BUFFERHEADERTYPE* pOutputBuffer = NULL;
  OMX_COMPONENTTYPE* target_component;
  OMX_BOOL isOutputBufferNeeded = OMX_TRUE;
  int outBufExchanged = 0;

  omx_base_source_Private->bellagioThreads->nThreadBufferMngtID = (long int)syscall(__NR_gettid);
  DEBUG(DEB_LEV_SIMPLE_SEQ, "In %s the thread ID is %i\n", __func__, (int)omx_base_source_Private->bellagioThreads->nThreadBufferMngtID);

  DEBUG(DEB_LEV_FUNCTION_NAME, "In %s \n", __func__);
  while(omx_base_component_Private->state == OMX_StateIdle || omx_base_component_Private->state == OMX_StateExecuting ||
    omx_base_component_Private->state == OMX_StatePause || omx_base_component_Private->transientState == OMX_TransStateLoadedToIdle){

    /*Wait till the ports are being flushed*/
    pthread_mutex_lock(&omx_base_source_Private->flush_mutex);
    while( PORT_IS_BEING_FLUSHED(pOutPort)) {
      pthread_mutex_unlock(&omx_base_source_Private->flush_mutex);

      if(isOutputBufferNeeded == OMX_FALSE) {
        pOutPort->ReturnBufferFunction(pOutPort, pOutputBuffer);
        outBufExchanged--;
        pOutputBuffer = NULL;
        isOutputBufferNeeded = OMX_TRUE;
        DEBUG(DEB_LEV_FULL_SEQ, "Ports are flushing,so returning output buffer\n");
      }
      DEBUG(DEB_LEV_FULL_SEQ, "In %s signalling flush all condition \n", __func__);

      tsem_up(omx_base_source_Private->flush_all_condition);
      tsem_down(omx_base_source_Private->flush_condition);
      pthread_mutex_lock(&omx_base_source_Private->flush_mutex);
    }
    pthread_mutex_unlock(&omx_base_source_Private->flush_mutex);

    /*No buffer to process. So wait here*/
    if((isOutputBufferNeeded==OMX_TRUE && pOutputSem->semval==0) &&
      (omx_base_source_Private->state != OMX_StateLoaded && omx_base_source_Private->state != OMX_StateInvalid)) {
      DEBUG(DEB_LEV_SIMPLE_SEQ, "Waiting for output buffer \n");
      tsem_down(omx_base_source_Private->bMgmtSem);
    }

    if(omx_base_source_Private->state == OMX_StateLoaded || omx_base_source_Private->state == OMX_StateInvalid) {
      DEBUG(DEB_LEV_FULL_SEQ, "In %s Buffer Management Thread is exiting\n",__func__);
      break;
    }

    DEBUG(DEB_LEV_SIMPLE_SEQ, "Waiting for output buffer semval=%d \n",pOutputSem->semval);
    if(pOutputSem->semval > 0 && isOutputBufferNeeded == OMX_TRUE ) {
      tsem_down(pOutputSem);
      if(pOutputQueue->nelem>0){
        outBufExchanged++;
        isOutputBufferNeeded = OMX_FALSE;
        pOutputBuffer = dequeue(pOutputQueue);
        if(pOutputBuffer == NULL){
          DEBUG(DEB_LEV_ERR, "In %s Had NULL output buffer!!\n",__func__);
          break;
        }
      }
    }

    if(isOutputBufferNeeded == OMX_FALSE) {
      if((pOutputBuffer->nFlags & OMX_BUFFERFLAG_EOS) == OMX_BUFFERFLAG_EOS) {
        pOutputBuffer->nFlags = 0;
      }

      if(omx_base_source_Private->pMark.hMarkTargetComponent != NULL){
        pOutputBuffer->hMarkTargetComponent = omx_base_source_Private->pMark.hMarkTargetComponent;
        pOutputBuffer->pMarkData            = omx_base_source_Private->pMark.pMarkData;
        omx_base_source_Private->pMark.hMarkTargetComponent = NULL;
        omx_base_source_Private->pMark.pMarkData            = NULL;
      }

      target_component = (OMX_COMPONENTTYPE*)pOutputBuffer->hMarkTargetComponent;
      if(target_component == (OMX_COMPONENTTYPE *)openmaxStandComp) {
        /*Clear the mark and generate an event*/
        (*(omx_base_component_Private->callbacks->EventHandler))
          (openmaxStandComp,
          omx_base_component_Private->callbackData,
          OMX_EventMark, /* The command was completed */
          1, /* The commands was a OMX_CommandStateSet */
          0, /* The state has been changed in message->messageParam2 */
          pOutputBuffer->pMarkData);
      } else if(pOutputBuffer->hMarkTargetComponent != NULL) {
        /*If this is not the target component then pass the mark*/
        DEBUG(DEB_LEV_FULL_SEQ, "Pass Mark. This is a Source!!\n");
      }

      if(omx_base_source_Private->state == OMX_StateExecuting)  {
        if (omx_base_source_Private->BufferMgmtCallback && pOutputBuffer->nFilledLen == 0) {
          (*(omx_base_source_Private->BufferMgmtCallback))(openmaxStandComp, pOutputBuffer);
        } else {
          /*It no buffer management call back then don't produce any output buffer*/
          pOutputBuffer->nFilledLen = 0;
        }
      } else {
        DEBUG(DEB_LEV_ERR, "In %s Received Buffer in non-Executing State(%x)\n", __func__, (int)omx_base_source_Private->state);
      }
      if(omx_base_source_Private->state == OMX_StatePause && !PORT_IS_BEING_FLUSHED(pOutPort)) {
        /*Waiting at paused state*/
        tsem_wait(omx_base_source_Private->bStateSem);
      }

      if((pOutputBuffer->nFlags & OMX_BUFFERFLAG_EOS) == OMX_BUFFERFLAG_EOS) {
        DEBUG(DEB_LEV_SIMPLE_SEQ, "Detected EOS flags in output buffer\n");

        (*(omx_base_component_Private->callbacks->EventHandler))
          (openmaxStandComp,
          omx_base_component_Private->callbackData,
          OMX_EventBufferFlag, /* The command was completed */
          0, /* The commands was a OMX_CommandStateSet */
          pOutputBuffer->nFlags, /* The state has been changed in message->messageParam2 */
          NULL);
        //pOutputBuffer->nFlags = 0;
        omx_base_source_Private->bIsEOSReached = OMX_TRUE;
      }

      /*Output Buffer has been produced or EOS. So, return output buffer and get new buffer*/
      if((pOutputBuffer->nFilledLen != 0) || ((pOutputBuffer->nFlags & OMX_BUFFERFLAG_EOS) ==OMX_BUFFERFLAG_EOS) || (omx_base_source_Private->bIsEOSReached == OMX_TRUE)) {
    	  if((pOutputBuffer->nFlags & OMX_BUFFERFLAG_EOS) == OMX_BUFFERFLAG_EOS)
    		  DEBUG(DEB_LEV_SIMPLE_SEQ, "In %s nFlags=%x Name=%s \n", __func__, (int)pOutputBuffer->nFlags, omx_base_source_Private->name);
    	  pOutPort->ReturnBufferFunction(pOutPort, pOutputBuffer);
    	  outBufExchanged--;
    	  pOutputBuffer = NULL;
    	  isOutputBufferNeeded = OMX_TRUE;
      }
    }
  }
  DEBUG(DEB_LEV_SIMPLE_SEQ, "Exiting Buffer Management Thread\n");
  return NULL;
}

/** This is the central function for buffer processing of a two port source component.
  * It is executed in a separate thread, is synchronized with
  * semaphores at each port, those are released each time a new buffer
  * is available on the given port.
  */

void* omx_base_source_twoport_BufferMgmtFunction (void* param) {
  OMX_COMPONENTTYPE* openmaxStandComp = (OMX_COMPONENTTYPE*)param;
  omx_base_component_PrivateType* omx_base_component_Private=(omx_base_component_PrivateType*)openmaxStandComp->pComponentPrivate;
  omx_base_source_PrivateType* omx_base_source_Private = (omx_base_source_PrivateType*)omx_base_component_Private;
  omx_base_PortType *pOutPort[2];
  tsem_t* pOutputSem[2];
  queue_t* pOutputQueue[2];
  OMX_BUFFERHEADERTYPE* pOutputBuffer[2];
  OMX_COMPONENTTYPE* target_component;
  OMX_BOOL isOutputBufferNeeded[2];
  int i,outBufExchanged[2];

  pOutPort[0]=(omx_base_PortType *)omx_base_source_Private->ports[OMX_BASE_SOURCE_OUTPUTPORT_INDEX];
  pOutPort[1]=(omx_base_PortType *)omx_base_source_Private->ports[OMX_BASE_SOURCE_OUTPUTPORT_INDEX_1];
  pOutputSem[0] = pOutPort[0]->pBufferSem;
  pOutputSem[1] = pOutPort[1]->pBufferSem;
  pOutputQueue[0] = pOutPort[0]->pBufferQueue;
  pOutputQueue[1] = pOutPort[1]->pBufferQueue;
  pOutputBuffer[1]= pOutputBuffer[0]=NULL;
  isOutputBufferNeeded[0]=isOutputBufferNeeded[1]=OMX_TRUE;
  outBufExchanged[0]=outBufExchanged[1]=0;

  DEBUG(DEB_LEV_FUNCTION_NAME, "In %s\n", __func__);
  while(omx_base_source_Private->state == OMX_StateIdle || omx_base_source_Private->state == OMX_StateExecuting ||  omx_base_source_Private->state == OMX_StatePause ||
    omx_base_source_Private->transientState == OMX_TransStateLoadedToIdle){

    /*Wait till the ports are being flushed*/
    pthread_mutex_lock(&omx_base_source_Private->flush_mutex);
    while( PORT_IS_BEING_FLUSHED(pOutPort[0]) ||
           PORT_IS_BEING_FLUSHED(pOutPort[1])) {
      pthread_mutex_unlock(&omx_base_source_Private->flush_mutex);

      DEBUG(DEB_LEV_FULL_SEQ, "In %s 1 signalling flush all cond iE=%d,iF=%d,oE=%d,oF=%d iSemVal=%d,oSemval=%d\n",
        __func__,outBufExchanged[0],isOutputBufferNeeded[0],outBufExchanged[1],isOutputBufferNeeded[1],pOutputSem[0]->semval,pOutputSem[1]->semval);

      if(isOutputBufferNeeded[1]==OMX_FALSE && PORT_IS_BEING_FLUSHED(pOutPort[1])) {
        pOutPort[1]->ReturnBufferFunction(pOutPort[1],pOutputBuffer[1]);
        outBufExchanged[1]--;
        pOutputBuffer[1]=NULL;
        isOutputBufferNeeded[1]=OMX_TRUE;
        DEBUG(DEB_LEV_FULL_SEQ, "Ports are flushing,so returning output 1 buffer\n");
      }

      if(isOutputBufferNeeded[0]==OMX_FALSE && PORT_IS_BEING_FLUSHED(pOutPort[0])) {
        pOutPort[0]->ReturnBufferFunction(pOutPort[0],pOutputBuffer[0]);
        outBufExchanged[0]--;
        pOutputBuffer[0]=NULL;
        isOutputBufferNeeded[0]=OMX_TRUE;
        DEBUG(DEB_LEV_FULL_SEQ, "Ports are flushing,so returning output 0 buffer\n");
      }

      DEBUG(DEB_LEV_FULL_SEQ, "In %s 2 signalling flush all cond iE=%d,iF=%d,oE=%d,oF=%d iSemVal=%d,oSemval=%d\n",
        __func__,outBufExchanged[0],isOutputBufferNeeded[0],outBufExchanged[1],isOutputBufferNeeded[1],pOutputSem[0]->semval,pOutputSem[1]->semval);

      tsem_up(omx_base_source_Private->flush_all_condition);
      tsem_down(omx_base_source_Private->flush_condition);
      pthread_mutex_lock(&omx_base_source_Private->flush_mutex);
    }
    pthread_mutex_unlock(&omx_base_source_Private->flush_mutex);

    /*No buffer to process. So wait here*/
    if((isOutputBufferNeeded[0]==OMX_TRUE && pOutputSem[0]->semval==0) &&
      (omx_base_source_Private->state != OMX_StateLoaded && omx_base_source_Private->state != OMX_StateInvalid)) {
      //Signalled from EmptyThisBuffer or FillThisBuffer or some thing else
      DEBUG(DEB_LEV_FULL_SEQ, "Waiting for next output buffer 0\n");
      tsem_down(omx_base_source_Private->bMgmtSem);

    }
    if(omx_base_source_Private->state == OMX_StateLoaded || omx_base_source_Private->state == OMX_StateInvalid) {
      DEBUG(DEB_LEV_SIMPLE_SEQ, "In %s Buffer Management Thread is exiting\n",__func__);
      break;
    }
    if((isOutputBufferNeeded[1]==OMX_TRUE && pOutputSem[1]->semval==0) &&
      (omx_base_source_Private->state != OMX_StateLoaded && omx_base_source_Private->state != OMX_StateInvalid) &&
       !(PORT_IS_BEING_FLUSHED(pOutPort[0]) || PORT_IS_BEING_FLUSHED(pOutPort[1]))) {
      //Signalled from EmptyThisBuffer or FillThisBuffer or some thing else
      DEBUG(DEB_LEV_FULL_SEQ, "Waiting for next output buffer 1\n");
      tsem_down(omx_base_source_Private->bMgmtSem);

    }
    if(omx_base_source_Private->state == OMX_StateLoaded || omx_base_source_Private->state == OMX_StateInvalid) {
      DEBUG(DEB_LEV_SIMPLE_SEQ, "In %s Buffer Management Thread is exiting\n",__func__);
      break;
    }

    DEBUG(DEB_LEV_SIMPLE_SEQ, "Waiting for output buffer 0 semval=%d \n",pOutputSem[0]->semval);
    if(pOutputSem[0]->semval>0 && isOutputBufferNeeded[0]==OMX_TRUE ) {
      tsem_down(pOutputSem[0]);
      if(pOutputQueue[0]->nelem>0){
        outBufExchanged[0]++;
        isOutputBufferNeeded[0]=OMX_FALSE;
        pOutputBuffer[0] = dequeue(pOutputQueue[0]);
        if(pOutputBuffer[0] == NULL){
          DEBUG(DEB_LEV_ERR, "Had NULL output buffer!!\n");
          break;
        }
      }
    }
    /*When we have input buffer to process then get one output buffer*/
    if(pOutputSem[1]->semval>0 && isOutputBufferNeeded[1]==OMX_TRUE) {
      tsem_down(pOutputSem[1]);
      if(pOutputQueue[1]->nelem>0){
        outBufExchanged[1]++;
        isOutputBufferNeeded[1]=OMX_FALSE;
        pOutputBuffer[1] = dequeue(pOutputQueue[1]);
        if(pOutputBuffer[1] == NULL){
          DEBUG(DEB_LEV_ERR, "Had NULL output buffer!! op is=%d,iq=%d\n",pOutputSem[1]->semval,pOutputQueue[1]->nelem);
          break;
        }
      }
    }

    for(i=0;i < (omx_base_component_Private->sPortTypesParam[OMX_PortDomainAudio].nPorts  +
                 omx_base_component_Private->sPortTypesParam[OMX_PortDomainVideo].nPorts +
                 omx_base_component_Private->sPortTypesParam[OMX_PortDomainImage].nPorts +
                 omx_base_component_Private->sPortTypesParam[OMX_PortDomainOther].nPorts -1);i++) {

      if(omx_base_source_Private->ports[i]->sPortParam.eDomain!=OMX_PortDomainOther){ /* clock ports are not to be processed */
        /*Process Output buffer of Port i */
        if(isOutputBufferNeeded[i]==OMX_FALSE) {

          /*Pass the Mark to all outgoing buffers*/
          if(omx_base_source_Private->pMark.hMarkTargetComponent != NULL){
            pOutputBuffer[i]->hMarkTargetComponent = omx_base_source_Private->pMark.hMarkTargetComponent;
            pOutputBuffer[i]->pMarkData            = omx_base_source_Private->pMark.pMarkData;
          }

          target_component=(OMX_COMPONENTTYPE*)pOutputBuffer[i]->hMarkTargetComponent;
          if(target_component==(OMX_COMPONENTTYPE *)openmaxStandComp) {
            /*Clear the mark and generate an event*/
            (*(omx_base_source_Private->callbacks->EventHandler))
              (openmaxStandComp,
              omx_base_source_Private->callbackData,
              OMX_EventMark, /* The command was completed */
              1, /* The commands was a OMX_CommandStateSet */
              i, /* The state has been changed in message->messageParam2 */
              pOutputBuffer[i]->pMarkData);
          } else if(pOutputBuffer[i]->hMarkTargetComponent!=NULL){
            /*If this is not the target component then pass the mark*/
            DEBUG(DEB_LEV_FULL_SEQ, "Pass Mark. This is a Source!!\n");
          }

          if(omx_base_source_Private->state == OMX_StateExecuting)  {
            if (omx_base_source_Private->BufferMgmtCallback && pOutputBuffer[i]->nFilledLen == 0) {
              (*(omx_base_source_Private->BufferMgmtCallback))(openmaxStandComp, pOutputBuffer[i]);
            } else {
              /*If no buffer management call back then don't produce any output buffer*/
              pOutputBuffer[i]->nFilledLen = 0;
            }
          } else {
            DEBUG(DEB_LEV_ERR, "In %s Received Buffer in non-Executing State(%x)\n", __func__, (int)omx_base_source_Private->state);
          }

          if((pOutputBuffer[i]->nFlags & OMX_BUFFERFLAG_EOS) == OMX_BUFFERFLAG_EOS && pOutputBuffer[i]->nFilledLen==0) {
            DEBUG(DEB_LEV_FULL_SEQ, "Detected EOS flags in input buffer filled len=%d\n", (int)pOutputBuffer[i]->nFilledLen);
            (*(omx_base_source_Private->callbacks->EventHandler))
              (openmaxStandComp,
              omx_base_source_Private->callbackData,
              OMX_EventBufferFlag, /* The command was completed */
              i, /* The commands was a OMX_CommandStateSet */
              pOutputBuffer[i]->nFlags, /* The state has been changed in message->messageParam2 */
              NULL);
          }
          if(omx_base_source_Private->state==OMX_StatePause && !(PORT_IS_BEING_FLUSHED(pOutPort[0]) || PORT_IS_BEING_FLUSHED(pOutPort[1]))) {
            /*Waiting at paused state*/
            tsem_wait(omx_base_component_Private->bStateSem);
          }

           /*Output Buffer has been produced or EOS. So, return output buffer and get new buffer*/
          if(pOutputBuffer[i]->nFilledLen!=0 || (pOutputBuffer[i]->nFlags & OMX_BUFFERFLAG_EOS) == OMX_BUFFERFLAG_EOS){
            pOutPort[i]->ReturnBufferFunction(pOutPort[i],pOutputBuffer[i]);
            outBufExchanged[i]--;
            pOutputBuffer[i]=NULL;
            isOutputBufferNeeded[i]=OMX_TRUE;
          }
        }
      }
    }

    /*Clear the Mark*/
    if(omx_base_source_Private->pMark.hMarkTargetComponent != NULL){
      omx_base_source_Private->pMark.hMarkTargetComponent = NULL;
      omx_base_source_Private->pMark.pMarkData            = NULL;
    }
  }
  DEBUG(DEB_LEV_SIMPLE_SEQ,"Exiting Buffer Management Thread\n");
  return NULL;
}
