/**
  src/omx_base_sink.c

  OpenMAX base sink component. This component does not perform any multimedia
  processing. It derives from base component and contains a single input port.
  It can be used as base class for sink components.

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
#include <omx_base_sink.h>

OMX_ERRORTYPE omx_base_sink_Constructor(OMX_COMPONENTTYPE *openmaxStandComp,OMX_STRING cComponentName) {
  OMX_ERRORTYPE err = OMX_ErrorNone;
  omx_base_sink_PrivateType* omx_base_sink_Private;

  if (openmaxStandComp->pComponentPrivate) {
    omx_base_sink_Private = (omx_base_sink_PrivateType*)openmaxStandComp->pComponentPrivate;
  } else {
    omx_base_sink_Private = calloc(1,sizeof(omx_base_sink_PrivateType));
    if (!omx_base_sink_Private) {
      return OMX_ErrorInsufficientResources;
    }
  }

  // we could create our own port structures here
  // fixme maybe the base class could use a "port factory" function pointer?
  err = omx_base_component_Constructor(openmaxStandComp,cComponentName);

  /* here we can override whatever defaults the base_component constructor set
  * e.g. we can override the function pointers in the private struct  */
  omx_base_sink_Private = openmaxStandComp->pComponentPrivate;

  omx_base_sink_Private->BufferMgmtFunction = omx_base_sink_BufferMgmtFunction;

  return err;
}

OMX_ERRORTYPE omx_base_sink_Destructor(OMX_COMPONENTTYPE *openmaxStandComp)
{
  return omx_base_component_Destructor(openmaxStandComp);
}

/** This is the central function for component processing. It
  * is executed in a separate thread, is synchronized with
  * semaphores at each port, those are released each time a new buffer
  * is available on the given port.
  */
void* omx_base_sink_BufferMgmtFunction (void* param) {
  OMX_COMPONENTTYPE* openmaxStandComp = (OMX_COMPONENTTYPE*)param;
  omx_base_component_PrivateType* omx_base_component_Private  = (omx_base_component_PrivateType*)openmaxStandComp->pComponentPrivate;
  omx_base_sink_PrivateType*      omx_base_sink_Private       = (omx_base_sink_PrivateType*)omx_base_component_Private;
  omx_base_PortType               *pInPort                    = (omx_base_PortType *)omx_base_sink_Private->ports[OMX_BASE_SINK_INPUTPORT_INDEX];
  tsem_t*                         pInputSem                   = pInPort->pBufferSem;
  queue_t*                        pInputQueue                 = pInPort->pBufferQueue;
  OMX_BUFFERHEADERTYPE*           pInputBuffer                = NULL;
  OMX_COMPONENTTYPE*              target_component;
  OMX_BOOL                        isInputBufferNeeded         = OMX_TRUE;
  int                             inBufExchanged              = 0;

  omx_base_sink_Private->bellagioThreads->nThreadBufferMngtID = (long int)syscall(__NR_gettid);
  DEBUG(DEB_LEV_SIMPLE_SEQ, "In %s the thread ID is %i\n", __func__, (int)omx_base_sink_Private->bellagioThreads->nThreadBufferMngtID);

  DEBUG(DEB_LEV_FUNCTION_NAME, "In %s \n", __func__);
  while(omx_base_component_Private->state == OMX_StateIdle || omx_base_component_Private->state == OMX_StateExecuting ||  omx_base_component_Private->state == OMX_StatePause ||
    omx_base_component_Private->transientState == OMX_TransStateLoadedToIdle){

    /*Wait till the ports are being flushed*/
    pthread_mutex_lock(&omx_base_sink_Private->flush_mutex);
    while( PORT_IS_BEING_FLUSHED(pInPort)) {
      pthread_mutex_unlock(&omx_base_sink_Private->flush_mutex);

      if(isInputBufferNeeded==OMX_FALSE) {
        pInPort->ReturnBufferFunction(pInPort,pInputBuffer);
        inBufExchanged--;
        pInputBuffer=NULL;
        isInputBufferNeeded=OMX_TRUE;
        DEBUG(DEB_LEV_FULL_SEQ, "Ports are flushing,so returning input buffer\n");
      }
      DEBUG(DEB_LEV_FULL_SEQ, "In %s signalling flush all condition \n", __func__);

      tsem_up(omx_base_sink_Private->flush_all_condition);
      tsem_down(omx_base_sink_Private->flush_condition);
      pthread_mutex_lock(&omx_base_sink_Private->flush_mutex);
    }
    pthread_mutex_unlock(&omx_base_sink_Private->flush_mutex);

    /*No buffer to process. So wait here*/
    if((pInputSem->semval==0 && isInputBufferNeeded==OMX_TRUE ) &&
      (omx_base_sink_Private->state != OMX_StateLoaded && omx_base_sink_Private->state != OMX_StateInvalid)) {
      DEBUG(DEB_LEV_SIMPLE_SEQ, "Waiting for input buffer \n");
      tsem_down(omx_base_sink_Private->bMgmtSem);
    }

    if(omx_base_sink_Private->state == OMX_StateLoaded || omx_base_sink_Private->state == OMX_StateInvalid) {
      DEBUG(DEB_LEV_FULL_SEQ, "In %s Buffer Management Thread is exiting\n",__func__);
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

    if(isInputBufferNeeded==OMX_FALSE) {
    	if((pInputBuffer->nFlags & OMX_BUFFERFLAG_EOS) ==OMX_BUFFERFLAG_EOS) {
        DEBUG(DEB_LEV_SIMPLE_SEQ, "Detected EOS flags in input buffer\n");

        (*(omx_base_component_Private->callbacks->EventHandler))
          (openmaxStandComp,
          omx_base_component_Private->callbackData,
          OMX_EventBufferFlag, /* The command was completed */
          0, /* The commands was a OMX_CommandStateSet */
          pInputBuffer->nFlags, /* The state has been changed in message->messageParam2 */
          NULL);
        pInputBuffer->nFlags=0;
      }

      target_component=(OMX_COMPONENTTYPE*)pInputBuffer->hMarkTargetComponent;
      if(target_component==(OMX_COMPONENTTYPE *)openmaxStandComp) {
        /*Clear the mark and generate an event*/
        (*(omx_base_component_Private->callbacks->EventHandler))
          (openmaxStandComp,
          omx_base_component_Private->callbackData,
          OMX_EventMark, /* The command was completed */
          1, /* The commands was a OMX_CommandStateSet */
          0, /* The state has been changed in message->messageParam2 */
          pInputBuffer->pMarkData);
      } else if(pInputBuffer->hMarkTargetComponent!=NULL){
        /*If this is not the target component then pass the mark*/
        DEBUG(DEB_LEV_FULL_SEQ, "Can't Pass Mark. This is a Sink!!\n");
      }

      if((omx_base_sink_Private->state == OMX_StateExecuting) || (omx_base_sink_Private->state == OMX_StateIdle)) {
        if ((omx_base_sink_Private->BufferMgmtCallback && pInputBuffer->nFilledLen > 0)
        		|| (pInputBuffer->nFlags)){
          (*(omx_base_sink_Private->BufferMgmtCallback))(openmaxStandComp, pInputBuffer);
        }
        else {
          /*If no buffer management call back the explicitly consume input buffer*/
          pInputBuffer->nFilledLen = 0;
        }
      } else {
        DEBUG(DEB_LEV_ERR, "In %s Received Buffer in non-Executing State(%s) TrState (%s)\n",
          __func__, stateName(omx_base_sink_Private->state),
          transientStateName(omx_base_component_Private->transientState));
        if(OMX_TransStateExecutingToIdle == omx_base_component_Private->transientState ||
           OMX_TransStatePauseToIdle == omx_base_component_Private->transientState) {
          pInputBuffer->nFilledLen = 0;
        }
      }
      /*Input Buffer has been completely consumed. So, get new input buffer*/

      if(omx_base_sink_Private->state==OMX_StatePause && !PORT_IS_BEING_FLUSHED(pInPort)) {
        /*Waiting at paused state*/
        tsem_wait(omx_base_sink_Private->bStateSem);
      }

      /*Input Buffer has been completely consumed. So, return input buffer*/
      if(pInputBuffer->nFilledLen==0) {
        pInPort->ReturnBufferFunction(pInPort,pInputBuffer);
        inBufExchanged--;
        pInputBuffer=NULL;
        isInputBufferNeeded = OMX_TRUE;
      }

    }
  }
  DEBUG(DEB_LEV_SIMPLE_SEQ,"Exiting Buffer Management Thread\n");
  return NULL;
}

/** This is the central function for buffer processing of a two port sink component.
  * It is executed in a separate thread, is synchronized with
  * semaphores at each port, those are released each time a new buffer
  * is available on the given port.
  */

void* omx_base_sink_twoport_BufferMgmtFunction (void* param) {
  OMX_COMPONENTTYPE* openmaxStandComp = (OMX_COMPONENTTYPE*)param;
  omx_base_component_PrivateType* omx_base_component_Private=(omx_base_component_PrivateType*)openmaxStandComp->pComponentPrivate;
  omx_base_sink_PrivateType* omx_base_sink_Private = (omx_base_sink_PrivateType*)omx_base_component_Private;
  omx_base_PortType *pInPort[2];
  tsem_t* pInputSem[2];
  queue_t* pInputQueue[2];
  OMX_BUFFERHEADERTYPE* pInputBuffer[2];
  OMX_COMPONENTTYPE* target_component;
  OMX_BOOL isInputBufferNeeded[2];
  int i,outBufExchanged[2];

  pInPort[0]=(omx_base_PortType *)omx_base_sink_Private->ports[OMX_BASE_SINK_INPUTPORT_INDEX];
  pInPort[1]=(omx_base_PortType *)omx_base_sink_Private->ports[OMX_BASE_SINK_INPUTPORT_INDEX_1];
  pInputSem[0] = pInPort[0]->pBufferSem;
  pInputSem[1] = pInPort[1]->pBufferSem;
  pInputQueue[0] = pInPort[0]->pBufferQueue;
  pInputQueue[1] = pInPort[1]->pBufferQueue;
  pInputBuffer[1]= pInputBuffer[0]=NULL;
  isInputBufferNeeded[0]=isInputBufferNeeded[1]=OMX_TRUE;
  outBufExchanged[0]=outBufExchanged[1]=0;

  DEBUG(DEB_LEV_FUNCTION_NAME, "In %s\n", __func__);
  while(omx_base_sink_Private->state == OMX_StateIdle || omx_base_sink_Private->state == OMX_StateExecuting ||  omx_base_sink_Private->state == OMX_StatePause ||
    omx_base_sink_Private->transientState == OMX_TransStateLoadedToIdle){

    /*Wait till the ports are being flushed*/
    pthread_mutex_lock(&omx_base_sink_Private->flush_mutex);
    while( PORT_IS_BEING_FLUSHED(pInPort[0]) ||
           PORT_IS_BEING_FLUSHED(pInPort[1])) {
      pthread_mutex_unlock(&omx_base_sink_Private->flush_mutex);

      DEBUG(DEB_LEV_FULL_SEQ, "In %s 1 signalling flush all cond iE=%d,iF=%d,oE=%d,oF=%d iSemVal=%d,oSemval=%d\n",
        __func__,outBufExchanged[0],isInputBufferNeeded[0],outBufExchanged[1],isInputBufferNeeded[1],pInputSem[0]->semval,pInputSem[1]->semval);

      if(isInputBufferNeeded[1]==OMX_FALSE && PORT_IS_BEING_FLUSHED(pInPort[1])) {
        pInPort[1]->ReturnBufferFunction(pInPort[1],pInputBuffer[1]);
        outBufExchanged[1]--;
        pInputBuffer[1]=NULL;
        isInputBufferNeeded[1]=OMX_TRUE;
        DEBUG(DEB_LEV_FULL_SEQ, "Ports are flushing,so returning Input 1 buffer\n");
      }

      if(isInputBufferNeeded[0]==OMX_FALSE && PORT_IS_BEING_FLUSHED(pInPort[0])) {
        pInPort[0]->ReturnBufferFunction(pInPort[0],pInputBuffer[0]);
        outBufExchanged[0]--;
        pInputBuffer[0]=NULL;
        isInputBufferNeeded[0]=OMX_TRUE;
        DEBUG(DEB_LEV_FULL_SEQ, "Ports are flushing,so returning Input 0 buffer\n");
      }

      DEBUG(DEB_LEV_FULL_SEQ, "In %s 2 signalling flush all cond iE=%d,iF=%d,oE=%d,oF=%d iSemVal=%d,oSemval=%d\n",
        __func__,outBufExchanged[0],isInputBufferNeeded[0],outBufExchanged[1],isInputBufferNeeded[1],pInputSem[0]->semval,pInputSem[1]->semval);

      tsem_up(omx_base_sink_Private->flush_all_condition);
      tsem_down(omx_base_sink_Private->flush_condition);
      pthread_mutex_lock(&omx_base_sink_Private->flush_mutex);
    }
    pthread_mutex_unlock(&omx_base_sink_Private->flush_mutex);

    /*No buffer to process. So wait here*/
    if((isInputBufferNeeded[0]==OMX_TRUE && pInputSem[0]->semval==0) &&
      (omx_base_sink_Private->state != OMX_StateLoaded && omx_base_sink_Private->state != OMX_StateInvalid)) {
      //Signalled from EmptyThisBuffer or FillThisBuffer or some thing else
      DEBUG(DEB_LEV_FULL_SEQ, "Waiting for next Input buffer 0\n");
      tsem_down(omx_base_sink_Private->bMgmtSem);

    }
    if(omx_base_sink_Private->state == OMX_StateLoaded || omx_base_sink_Private->state == OMX_StateInvalid) {
      DEBUG(DEB_LEV_SIMPLE_SEQ, "In %s Buffer Management Thread is exiting\n",__func__);
      break;
    }
    if((isInputBufferNeeded[1]==OMX_TRUE && pInputSem[1]->semval==0) &&
      (omx_base_sink_Private->state != OMX_StateLoaded && omx_base_sink_Private->state != OMX_StateInvalid) &&
       !(PORT_IS_BEING_FLUSHED(pInPort[0]) || PORT_IS_BEING_FLUSHED(pInPort[1]))) {
      //Signalled from EmptyThisBuffer or FillThisBuffer or some thing else
      DEBUG(DEB_LEV_FULL_SEQ, "Waiting for next Input buffer 1\n");
      tsem_down(omx_base_sink_Private->bMgmtSem);

    }
    if(omx_base_sink_Private->state == OMX_StateLoaded || omx_base_sink_Private->state == OMX_StateInvalid) {
      DEBUG(DEB_LEV_SIMPLE_SEQ, "In %s Buffer Management Thread is exiting\n",__func__);
      break;
    }

    DEBUG(DEB_LEV_SIMPLE_SEQ, "Waiting for Input buffer 0 semval=%d \n",pInputSem[0]->semval);
    if(pInputSem[0]->semval>0 && isInputBufferNeeded[0]==OMX_TRUE ) {
      tsem_down(pInputSem[0]);
      if(pInputQueue[0]->nelem>0){
        outBufExchanged[0]++;
        isInputBufferNeeded[0]=OMX_FALSE;
        pInputBuffer[0] = dequeue(pInputQueue[0]);
        if(pInputBuffer[0] == NULL){
          DEBUG(DEB_LEV_ERR, "Had NULL Input buffer!!\n");
          break;
        }
      }
    }
    /*When we have input buffer to process then get one Input buffer*/
    if(pInputSem[1]->semval>0 && isInputBufferNeeded[1]==OMX_TRUE) {
      tsem_down(pInputSem[1]);
      DEBUG(DEB_LEV_FULL_SEQ, "Wait over for Input buffer 1 semval=%d \n",pInputSem[1]->semval);
      if(pInputQueue[1]->nelem>0){
        outBufExchanged[1]++;
        isInputBufferNeeded[1]=OMX_FALSE;
        pInputBuffer[1] = dequeue(pInputQueue[1]);
        if(pInputBuffer[1] == NULL){
          DEBUG(DEB_LEV_ERR, "Had NULL Input buffer!! op is=%d,iq=%d\n",pInputSem[1]->semval,pInputQueue[1]->nelem);
          break;
        }
      }
    }

    for(i=0;i < (omx_base_component_Private->sPortTypesParam[OMX_PortDomainAudio].nPorts  +
                 omx_base_component_Private->sPortTypesParam[OMX_PortDomainVideo].nPorts +
                 omx_base_component_Private->sPortTypesParam[OMX_PortDomainImage].nPorts +
                 omx_base_component_Private->sPortTypesParam[OMX_PortDomainOther].nPorts);i++) {

      if(omx_base_sink_Private->ports[i]->sPortParam.eDomain != OMX_PortDomainOther){ /* clock ports are not to be processed */
        /*Process Input buffer of Port i */
        if(isInputBufferNeeded[i]==OMX_FALSE) {

          /*Pass the Mark to all outgoing buffers*/
          if(omx_base_sink_Private->pMark.hMarkTargetComponent != NULL){
            pInputBuffer[i]->hMarkTargetComponent = omx_base_sink_Private->pMark.hMarkTargetComponent;
            pInputBuffer[i]->pMarkData            = omx_base_sink_Private->pMark.pMarkData;
          }

          target_component=(OMX_COMPONENTTYPE*)pInputBuffer[i]->hMarkTargetComponent;
          if(target_component==(OMX_COMPONENTTYPE *)openmaxStandComp) {
            /*Clear the mark and generate an event*/
            (*(omx_base_sink_Private->callbacks->EventHandler))
              (openmaxStandComp,
              omx_base_sink_Private->callbackData,
              OMX_EventMark, /* The command was completed */
              1, /* The commands was a OMX_CommandStateSet */
              i, /* The state has been changed in message->messageParam2 */
              pInputBuffer[i]->pMarkData);
          } else if(pInputBuffer[i]->hMarkTargetComponent!=NULL){
            /*If this is not the target component then pass the mark*/
            //pInputBuffer[i]->pMarkData=NULL;
            DEBUG(DEB_LEV_FULL_SEQ, "Pass Mark. This is a Source!!\n");
          }

          if(omx_base_sink_Private->state == OMX_StateExecuting)  {
            if (omx_base_sink_Private->BufferMgmtCallback && pInputBuffer[i]->nFilledLen > 0) {
              (*(omx_base_sink_Private->BufferMgmtCallback))(openmaxStandComp, pInputBuffer[i]);
            } else {
              /*If no buffer management call back then don't produce any Input buffer*/
              pInputBuffer[i]->nFilledLen = 0;
            }
          } else {
            DEBUG(DEB_LEV_ERR, "In %s Received Buffer in non-Executing State(%x)\n", __func__, (int)omx_base_sink_Private->state);

            if(OMX_TransStateExecutingToIdle == omx_base_component_Private->transientState ||
               OMX_TransStatePauseToIdle == omx_base_component_Private->transientState) {
              pInputBuffer[i]->nFilledLen = 0;
            }
          }

          if((pInputBuffer[i]->nFlags & OMX_BUFFERFLAG_EOS) == OMX_BUFFERFLAG_EOS && pInputBuffer[i]->nFilledLen==0) {
            DEBUG(DEB_LEV_FULL_SEQ, "Detected EOS flags in input buffer filled len=%d\n", (int)pInputBuffer[i]->nFilledLen);
            (*(omx_base_sink_Private->callbacks->EventHandler))
              (openmaxStandComp,
              omx_base_sink_Private->callbackData,
              OMX_EventBufferFlag, /* The command was completed */
              i, /* The commands was a OMX_CommandStateSet */
              pInputBuffer[i]->nFlags, /* The state has been changed in message->messageParam2 */
              NULL);
          }
          if(omx_base_sink_Private->state==OMX_StatePause && !(PORT_IS_BEING_FLUSHED(pInPort[0]) || PORT_IS_BEING_FLUSHED(pInPort[1]))) {
            /*Waiting at paused state*/
            tsem_wait(omx_base_component_Private->bStateSem);
          }

           /*Input Buffer has been produced or EOS. So, return Input buffer and get new buffer*/
          if(pInputBuffer[i]->nFilledLen ==0 || ((pInputBuffer[i]->nFlags & OMX_BUFFERFLAG_EOS) == OMX_BUFFERFLAG_EOS)){
            pInPort[i]->ReturnBufferFunction(pInPort[i],pInputBuffer[i]);
            outBufExchanged[i]--;
            pInputBuffer[i]=NULL;
            isInputBufferNeeded[i]=OMX_TRUE;
          }
        }
      }
    }

    /*Clear the Mark*/
    if(omx_base_sink_Private->pMark.hMarkTargetComponent != NULL){
      omx_base_sink_Private->pMark.hMarkTargetComponent = NULL;
      omx_base_sink_Private->pMark.pMarkData            = NULL;
    }
  }
  DEBUG(DEB_LEV_SIMPLE_SEQ,"Exiting Buffer Management Thread\n");
  return NULL;
}
