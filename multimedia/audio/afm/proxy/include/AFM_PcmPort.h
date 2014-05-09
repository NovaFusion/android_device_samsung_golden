/*****************************************************************************/
/*
 * Copyright (C) ST-Ericsson SA 2009,2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file   AFM_PcmPort.h
 * \brief  
 * \author ST-Ericsson
 */
/*****************************************************************************/
#ifndef _AFM_PCMPORT_H_
#define _AFM_PCMPORT_H_

#include "AFM_Port.h"

#define     DEFAULT_MINBUFFERSIZE_PCM 4096

class AFM_PcmPort: public AFM_Port {
    public:

#ifndef BC_BREAK
        AFM_API_IMPORT AFM_PcmPort(
                OMX_U32 nIndex,
                OMX_DIRTYPE eDir,
                OMX_U32 nBufferSizeMin,
                const OMX_AUDIO_PARAM_PCMMODETYPE &defaultPcmSettings,
                ENS_Component &enscomp);
#endif

        AFM_API_IMPORT AFM_PcmPort(
                OMX_U32         nIndex,
                OMX_DIRTYPE     eDir,
                OMX_U32         nChannels,
                OMX_U32         nSampleRate,
                OMX_U32         nBitPerSample,
                ENS_Component & enscomp);

        AFM_API_IMPORT virtual OMX_ERRORTYPE setParameter(
                OMX_INDEXTYPE nParamIndex,
                OMX_PTR pComponentParameterStructure);

        AFM_API_IMPORT virtual OMX_ERRORTYPE getParameter(
                OMX_INDEXTYPE nParamIndex,
                OMX_PTR pComponentParameterStructure) const;

        AFM_API_IMPORT virtual OMX_ERRORTYPE checkFormatCompatibility(
                OMX_HANDLETYPE hTunneledComp, OMX_U32 nTunneledPort) const;

        AFM_API_IMPORT const AFM_PARAM_PCMLAYOUTTYPE &     getPcmLayout() const;
        
        const OMX_AUDIO_PARAM_PCMMODETYPE & getPcmSettings() const 
            { return mPcmSettings; }

        OMX_AUDIO_PARAM_PCMMODETYPE &       getPcmSettings()
            { return mPcmSettings; }

        AFM_API_IMPORT OMX_ERRORTYPE updatePcmSettings(OMX_U32 nChannels, OMX_U32 nSampleRate);

        OMX_BOOL isSynchronized() {return mIsSynchronized;}
#ifdef HOST_ONLY
protected:
        OMX_BOOL                                mIsSynchronized;
#else
private:
	OMX_BOOL                                mIsSynchronized;
#endif

    private:
         // PCM settings 	 
        mutable AFM_PARAM_PCMLAYOUTTYPE         mPcmLayout; //FIXME: should be moved to ProcessingComp? 
        mutable OMX_AUDIO_PARAM_PCMMODETYPE     mPcmSettings;
        // Private methods
        OMX_ERRORTYPE           fillPcmLayout(void) const;
#ifndef BC_BREAK
        virtual OMX_ERRORTYPE   checkPcmSettings(
                                const OMX_AUDIO_PARAM_PCMMODETYPE &pcmSettings) const { return OMX_ErrorNone; }
#else 
        virtual OMX_ERRORTYPE   checkPcmSettings(
                                const OMX_AUDIO_PARAM_PCMMODETYPE &pcmSettings) const = 0;
#endif
        static OMX_ERRORTYPE    checkFWSupportedPcmSettings(
                                    const OMX_AUDIO_PARAM_PCMMODETYPE &pcmSettings);

        static OMX_ERRORTYPE    checkChannelMapping(
                                    const OMX_AUDIO_PARAM_PCMMODETYPE &pcmSettings);

        void                    fillDefaultPcmSettings(OMX_U32 nChannels, OMX_U32 nSampleRate, OMX_U32 nBitPerSample);
};

/// Pointer to a ENS_Port
typedef AFM_PcmPort * AFM_PcmPort_p;

//FIXME : to be removed!!!!
bool operator!=(const AFM_PARAM_PCMLAYOUTTYPE &pcmlayout1, 
                const AFM_PARAM_PCMLAYOUTTYPE &pcmlayout2);

#endif // _AFM_PCMPORT_H_
