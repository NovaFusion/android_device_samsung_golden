/*****************************************************************************/
/*
 * Copyright (C) ST-Ericsson SA 2010. All rights reserved.
 * This code is ST-Ericsson proprietary and confidential.
 * Any use of the code for whatever purpose is subject to
 * specific written permission of ST-Ericsson SA.
 *
 */

/**
 * \file   AudioEffect.h
 * \brief
 * \author ST-Ericsson
 */
/*****************************************************************************/
#ifndef _AUDIO_EFFECT_H_
#define _AUDIO_EFFECT_H_

#include "OMX_Types.h"
#include "OMX_Core.h"
#include "TraceObject.h"
#include "AudioEffectMpcConfigs.h"
#include "AFM_Types.h"
#include "ENS_Component.h"

#define MAX_EFFECTS_REGISTERED 16

#define AUDIO_EFFECT_MMPROBE_INDEX 1

class AudioEffectCoreSpecific;
class AudioEffectParams;

class AudioEffect : public TraceObject {
	public:
        /// Constructor
        AudioEffect(const AudioEffectParams &sParams, ENS_Component &enscomp);

        /// Destructor
        virtual ~AudioEffect(void);

        virtual OMX_ERRORTYPE setConfig(OMX_U32 nConfigIndex, OMX_PTR pComponentConfigStructure) { return OMX_ErrorUnsupportedSetting; }
        virtual OMX_ERRORTYPE getConfig(OMX_U32 nConfigIndex, OMX_PTR pComponentConfigStructure) const { return OMX_ErrorUnsupportedSetting; }

        OMX_U32 getPosition(void) const { return mPosition; }
        OMX_U32 getPortIdx(void) const { return mPortIdx; }
        const char * getName(void) const { return mName; }

        OMX_U32 getNbChannels(void) const;
        OMX_U32 getSamplingRate(void) const;

        AFM_AUDIO_CONFIG_PCM_PROBE * getProbeConfig(void) const { return (AFM_AUDIO_CONFIG_PCM_PROBE *)&mProbeConfig; }
        OMX_ERRORTYPE setProbeConfig(AFM_AUDIO_CONFIG_PCM_PROBE * pConfig);

        const AudioEffectCoreSpecific & getCoreSpecificPart(void) const { return *mCoreSpecificImpl; }
        AudioEffectCoreSpecific & getCoreSpecificPart(void) { return *mCoreSpecificImpl; }
        void setCoreSpecificPart(AudioEffectCoreSpecific *coreSpecificImpl) { mCoreSpecificImpl = coreSpecificImpl; }

        inline void fillConfigIndexesTable(OMX_U32 nbConfigs, OMX_INDEXTYPE *configIndexesTable) {
            DBC_ASSERT(mConfigIndexTable == NULL);
            mNbConfigs = nbConfigs;
            mConfigIndexTable = new OMX_INDEXTYPE [mNbConfigs];
            for(OMX_U32 i=0; i<mNbConfigs; i++) {
                mConfigIndexTable[i] = configIndexesTable[i];
            }
        }

        OMX_U32 getNbConfigs(void) const { return mNbConfigs; }
        OMX_INDEXTYPE getConfigIndex(OMX_U32 configTableIndex) const {
            DBC_ASSERT(configTableIndex < mNbConfigs);
            return mConfigIndexTable[configTableIndex];
        }

    protected:
        AFM_AUDIO_CONFIG_PCM_PROBE      mProbeConfig; //1 config is sufficient : only effect output datas can be probed
        ENS_Component                   &mENSComponent;

	private:
        AudioEffectCoreSpecific *       mCoreSpecificImpl;
        OMX_U32                         mPortIdx;
        OMX_U32                         mPosition;
        char                            mName[OMX_MAX_STRINGNAME_SIZE];

        //To be filled by derived classes in createMethod using fillConfigIndexesTable method
        OMX_U32                         mNbConfigs; //Number of configuration indexes supported by effect
        OMX_INDEXTYPE                   *mConfigIndexTable; //list of configuration indexes supported by effect
};

/// Typedef for an AudioEffect' create methods
/// @param [in] ppENSComponent
///    Pointer to an ENS_Component pointer which must be filled by
///    the factory method
/// @return OMX_ERRORTYPE
typedef OMX_ERRORTYPE (*AUDIO_EFFECT_CREATE_METHODTYPE) (AudioEffect **ppAudioEffect, ENS_Component &enscomp, const AudioEffectParams &sParams);

/// Structure to store a registry effect entry
struct AudioEffect_RegistryEntry {
    /// Name of the registered effect.
    char * mName;
    /// Create method
    AUDIO_EFFECT_CREATE_METHODTYPE mCreateMethod;
};

class AudioEffectsRegister {
    public:

        /// Create a pointer to the AudioEffectsRegister class.
        /// Ensures there is a unique instance of this class.
        static AudioEffectsRegister * createInstance(void);

        /// Destroy pointer to the AudioEffectsRegister class if no more users.
        static void destroyInstance(void);

        /// Get a pointer to the AudioEffectsRegister class.
        static AudioEffectsRegister * getInstance(void);

        /// Register an effect
        /// @param [in] cEffectName
        ///    Name of the effect. Shall be less than 128 characters long
        ///    including the trailing null byte. No effect with the same name
        ///    must have been already registered.
        /// @param [in] pCreateMethod
        ///    Function pointer to the provided create method which
        ///    can be used to create instances of the registered effect
        OMX_ERRORTYPE registerEffect(
                const OMX_STRING cEffectName,
                AUDIO_EFFECT_CREATE_METHODTYPE pCreateMethod);

        /// Search the effect registry for an effect with the given name
        /// @param [in] cEffectName
        ///    Name of the effect to look for.
        /// @param [out] ppCreateMethod
        ///    Pointer to the create method pointer to be filled by this method
        /// @return OMX_ERRORTYPE
        ///    - OMX_ErrorNone
        ///    - OMX_ErrorBadParameter
        ///    - OMX_ErrorInvalidComponentName
        ///    - OMX_ErrorComponentNotFound
        OMX_ERRORTYPE lookupEffect(OMX_STRING cEffectName, AUDIO_EFFECT_CREATE_METHODTYPE *ppCreateMethod);

        /// Create an instance of the effect identified by the given name.
        /// This method will search registered effects with
        /// the given name. If one is found, it will create an instance of it by
        /// calling the create method with which it was registered.
        ///
        /// @param [out] ppAudioEffect
        /// @param [in] cEffectName
        ///    Name of the component. Shall be less than 128 characters long
        ///    including the trailing null byte.
        /// @return OMX_ERRORTYPE
        ///    - OMX_ErrorNone
        ///    - OMX_ComponentNotFound
        OMX_ERRORTYPE createEffect(AudioEffect **ppAudioEffect, ENS_Component &enscomp, const AudioEffectParams &sParams);

        /// Free memory allocated by createEffect
        OMX_ERRORTYPE destroyEffect(void *pEffect);

    private:
        /// Constructor
        /// Private to ensure the ENS Core is instantiated at most once
        AudioEffectsRegister(void);

        /// Destructor
        virtual ~AudioEffectsRegister(void);

        static AudioEffectsRegister *   instance;
        static int                      nbInstance;

        /// The effect registry
        int                             mRegisteredCount;
        AudioEffect_RegistryEntry       mEntries[MAX_EFFECTS_REGISTERED];
};

/// Structure used to pass the common information that an AudioEffect needs
/// at instantiation time.
class  AudioEffectParams{
    public:
        OMX_U32 mPortIndex;
        OMX_U32 mPosition;
        OMX_STRING cEffectName;

        // Constructor
        AudioEffectParams(
                OMX_U32 nPortIndex,
                OMX_U32 nPosition,
                OMX_STRING cEffectName);
};

#endif // _AUDIO_EFFECT_H_
