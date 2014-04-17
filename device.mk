#
# Copyright (C) 2014 NovaFusion http://novafusion.pl
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# Add overlay
DEVICE_PACKAGE_OVERLAYS += device/samsung/golden/overlay

# Define kind of DPI
PRODUCT_AAPT_CONFIG := normal hdpi
PRODUCT_AAPT_PREF_CONFIG := hdpi

# We have enough storage space to hold precise GC data
PRODUCT_TAGS += dalvik.gc.type-precise

# Open-source HALs
PRODUCT_PACKAGES += \
    audio.a2dp.default \
    libomxil-bellagio \
    libblt_hw \
    lights.montblanc

# Misc packages
PRODUCT_PACKAGES += \
    make_ext4fs \
    setup_fs \
    Stk

##########################################
## Kernel stuff
##########################################

# Prebuilt kernel
PRODUCT_COPY_FILES := \
    device/samsung/golden/rootfs/kernel:kernel

# InitFS
PRODUCT_COPY_FILES += \
    device/samsung/golden/rootfs/init.samsunggolden.rc:root/init.samsunggolden.rc \
    device/samsung/golden/rootfs/init.samsunggolden.usb.rc:root/init.samsunggolden.usb.rc \
    device/samsung/golden/rootfs/fstab.samsunggolden:root/fstab.samsunggolden \
    device/samsung/golden/rootfs/ueventd.samsunggolden.rc:root/ueventd.samsunggolden.rc \
    device/samsung/golden/rootfs/lpm.rc:root/lpm.rc

# Kernel modules
PRODUCT_COPY_FILES += \
    device/samsung/golden/prebuilt/lib/modules/dhd.ko:system/lib/modules/dhd.ko \
    device/samsung/golden/prebuilt/lib/modules/param.ko:system/lib/modules/param.ko \
    device/samsung/golden/prebuilt/lib/modules/j4fs.ko:system/lib/modules/j4fs.ko \
    device/samsung/golden/prebuilt/lib/modules/bthid.ko:system/lib/modules/bthid.ko

##########################################
## STE blobs
##########################################

# System STE Libs
PRODUCT_COPY_FILES += \
    device/samsung/golden/prebuilt/lib/libbassapp.so:system/lib/libbassapp.so \
    device/samsung/golden/prebuilt/lib/libcn.so:system/lib/libcn.so \
    device/samsung/golden/prebuilt/lib/libcspsa.so:system/lib/libcspsa.so \
    device/samsung/golden/prebuilt/lib/libfactoryutil.so:system/lib/libfactoryutil.so \
    device/samsung/golden/prebuilt/lib/libisimessage.so:system/lib/libisimessage.so \
    device/samsung/golden/prebuilt/lib/liblos.so:system/lib/liblos.so \
    device/samsung/golden/prebuilt/lib/libmalmon.so:system/lib/libmalmon.so \
    device/samsung/golden/prebuilt/lib/libmalrf.so:system/lib/libmalrf.so \
    device/samsung/golden/prebuilt/lib/libmalutil.so:system/lib/libmalutil.so \
    device/samsung/golden/prebuilt/lib/libmmprobe.so:system/lib/libmmprobe.so \
    device/samsung/golden/prebuilt/lib/libmpl.so:system/lib/libmpl.so \
    device/samsung/golden/prebuilt/lib/libnmf.so:system/lib/libnmf.so \
    device/samsung/golden/prebuilt/lib/libnmfee.so:system/lib/libnmfee.so \
    device/samsung/golden/prebuilt/lib/libomission_avoidance.so:system/lib/libomission_avoidance.so \
    device/samsung/golden/prebuilt/lib/libphonet.so:system/lib/libphonet.so \
    device/samsung/golden/prebuilt/lib/libpscc.so:system/lib/libpscc.so \
    device/samsung/golden/prebuilt/lib/libsecnativefeature.so:system/lib/libsecnativefeature.so \
    device/samsung/golden/prebuilt/lib/libshmnetlnk.so:system/lib/libshmnetlnk.so \
    device/samsung/golden/prebuilt/lib/libsms_server.so:system/lib/libsms_server.so \
    device/samsung/golden/prebuilt/lib/libspeech_proc_customer_plugin.so:system/lib/libspeech_proc_customer_plugin.so \
    device/samsung/golden/prebuilt/lib/libste_adm.so:system/lib/libste_adm.so \
    device/samsung/golden/prebuilt/lib/libste_audio_hwctrl.so:system/lib/libste_audio_hwctrl.so \
    device/samsung/golden/prebuilt/lib/libste_audio_mixer.so:system/lib/libste_audio_mixer.so \
    device/samsung/golden/prebuilt/lib/libste_ens_audio_common.so:system/lib/libste_ens_audio_common.so \
    device/samsung/golden/prebuilt/lib/libste_ens_audio_samplerateconv.so:system/lib/libste_ens_audio_samplerateconv.so \
    device/samsung/golden/prebuilt/lib/libste_ensloader.so:system/lib/libste_ensloader.so \
    device/samsung/golden/prebuilt/lib/libstecom.so:system/lib/libstecom.so \
    device/samsung/golden/prebuilt/lib/libstelpcutils.so:system/lib/libstelpcutils.so \
    device/samsung/golden/prebuilt/lib/libsterc.so:system/lib/libsterc.so \
    device/samsung/golden/prebuilt/lib/libtee.so:system/lib/libtee.so \
    device/samsung/golden/prebuilt/lib/libtrace.so:system/lib/libtrace.so

# System STE binaries
PRODUCT_COPY_FILES += \
    device/samsung/golden/prebuilt/bin/admsrv:system/bin/admsrv \
    device/samsung/golden/prebuilt/bin/cspsa-server:system/bin/cspsa-server \
    device/samsung/golden/prebuilt/bin/msa:system/bin/msa

# System STE Configs
PRODUCT_COPY_FILES += \
    device/samsung/golden/prebuilt/etc/ste_modem.sh:system/etc/ste_modem.sh \
    device/samsung/golden/prebuilt/etc/adm.sqlite-ab8505_v2:system/etc/adm.sqlite-ab8505_v2 \
    device/samsung/golden/prebuilt/etc/plmn.latam.list:system/etc/plmn.latam.list \
    device/samsung/golden/prebuilt/etc/plmn.operator.list:system/etc/plmn.operator.list

##########################################
## Displaying/Graphic
##########################################

# Display
PRODUCT_COPY_FILES += \
    device/samsung/golden/prebuilt/lib/hw/hwcomposer.montblanc.so:system/lib/hw/hwcomposer.montblanc.so \
    device/samsung/golden/prebuilt/lib/hw/gralloc.montblanc.so:system/lib/hw/gralloc.montblanc.so \
    device/samsung/golden/prebuilt/lib/hw/copybit.montblanc.so:system/lib/hw/copybit.montblanc.so

# Mali-400
PRODUCT_COPY_FILES += \
    device/samsung/golden/prebuilt/lib/egl/libEGL_mali.so:system/lib/egl/libEGL_mali.so \
    device/samsung/golden/prebuilt/lib/egl/libGLESv1_CM_mali.so:system/lib/egl/libGLESv1_CM_mali.so \
    device/samsung/golden/prebuilt/lib/egl/libGLESv2_mali.so:system/lib/egl/libGLESv2_mali.so \
    device/samsung/golden/prebuilt/lib/libMali.so:system/lib/libMali.so \
    device/samsung/golden/prebuilt/lib/libUMP.so:system/lib/libUMP.so

##########################################
## Wireless
##########################################

# RIL
PRODUCT_COPY_FILES += \
    device/samsung/golden/prebuilt/lib/libsec-ril.so:system/lib/libsec-ril.so \
    device/samsung/golden/prebuilt/lib/libsecril-client.so:system/lib/libsecril-client.so \
    device/samsung/golden/prebuilt/lib/libril.so:system/lib/libril.so

# Wi-Fi
PRODUCT_COPY_FILES += \
    device/samsung/golden/prebuilt/etc/wifi/bcmdhd_apsta.bin:system/etc/wifi/bcmdhd_apsta.bin \
    device/samsung/golden/prebuilt/etc/wifi/bcmdhd_p2p.bin:system/etc/wifi/bcmdhd_p2p.bin \
    device/samsung/golden/prebuilt/etc/wifi/bcmdhd_sta.bin:system/etc/wifi/bcmdhd_sta.bin \
    device/samsung/golden/prebuilt/etc/wifi/nvram_net.txt:system/etc/wifi/nvram_net.txt

# GPS
PRODUCT_COPY_FILES += \
    device/samsung/golden/prebuilt/lib/hw/gps.montblanc.so:system/lib/hw/gps.montblanc.so \
    device/samsung/golden/prebuilt/etc/gps.conf:system/etc/gps.conf \
    device/samsung/golden/prebuilt/etc/sirfgps.conf:system/etc/sirfgps.conf \
    device/samsung/golden/prebuilt/etc/AGPS_CA.pem:system/etc/AGPS_CA.pem

# BT
PRODUCT_COPY_FILES += \
    device/samsung/golden/prebuilt/bin/bcm4334.hcd:system/bin/bcm4334.hcd \
    device/samsung/golden/prebuilt/etc/bluetooth/bt_vendor.conf:system/etc/bluetooth/bt_vendor.conf

#########################################
## Media
#########################################

# Media configs
PRODUCT_COPY_FILES += \
    device/samsung/golden/prebuilt/etc/media_codecs.xml:system/etc/media_codecs.xml \
    device/samsung/golden/prebuilt/etc/media_profiles.xml:system/etc/media_profiles.xml

# OMX (TODO: clean it as much as possible)
PRODUCT_COPY_FILES += \
    device/samsung/golden/prebuilt/etc/omxloaders:system/etc/omxloaders \
    device/samsung/golden/prebuilt/lib/libnmftimer.so:system/lib/libnmftimer.so \
    device/samsung/golden/prebuilt/lib/libstagefrighthw.so:system/lib/libstagefrighthw.so \
    device/samsung/golden/prebuilt/lib/libste_ens_image_tuningdatabase.so:system/lib/libste_ens_image_tuningdatabase.so \
    device/samsung/golden/prebuilt/lib/libste_ens_video_common.so:system/lib/libste_ens_video_common.so \
    device/samsung/golden/prebuilt/lib/libste_ext_ens_image_common.so:system/lib/libste_ext_ens_image_common.so \
    device/samsung/golden/prebuilt/lib/libste_omxil-interface.so:system/lib/libste_omxil-interface.so \
    device/samsung/golden/prebuilt/lib/ste_omxcomponents/libspeech_processing.so:system/lib/ste_omxcomponents/libspeech_processing.so \
    device/samsung/golden/prebuilt/lib/ste_omxcomponents/libste_alsasink.so:system/lib/ste_omxcomponents/libste_alsasink.so \
    device/samsung/golden/prebuilt/lib/ste_omxcomponents/libste_alsasource.so:system/lib/ste_omxcomponents/libste_alsasource.so \
    device/samsung/golden/prebuilt/lib/ste_omxcomponents/libste_audio_mixer.so:system/lib/ste_omxcomponents/libste_audio_mixer.so \
    device/samsung/golden/prebuilt/lib/ste_omxcomponents/libste_audio_source_sink.so:system/lib/ste_omxcomponents/libste_audio_source_sink.so \
    device/samsung/golden/prebuilt/lib/ste_omxcomponents/libste_comfort_noise.so:system/lib/ste_omxcomponents/libste_comfort_noise.so \
    device/samsung/golden/prebuilt/lib/ste_omxcomponents/libste_cscall.so:system/lib/ste_omxcomponents/libste_cscall.so \
    device/samsung/golden/prebuilt/lib/ste_omxcomponents/libste_dec_amr.so:system/lib/ste_omxcomponents/libste_dec_amr.so \
    device/samsung/golden/prebuilt/lib/ste_omxcomponents/libste_dec_amrwb.so:system/lib/ste_omxcomponents/libste_dec_amrwb.so \
    device/samsung/golden/prebuilt/lib/ste_omxcomponents/libste_dec_h264.so:system/lib/ste_omxcomponents/libste_dec_h264.so \
    device/samsung/golden/prebuilt/lib/ste_omxcomponents/libste_dec_jpeg.so:system/lib/ste_omxcomponents/libste_dec_jpeg.so \
    device/samsung/golden/prebuilt/lib/ste_omxcomponents/libste_dec_mpeg2.so:system/lib/ste_omxcomponents/libste_dec_mpeg2.so \
    device/samsung/golden/prebuilt/lib/ste_omxcomponents/libste_dec_mpeg4.so:system/lib/ste_omxcomponents/libste_dec_mpeg4.so \
    device/samsung/golden/prebuilt/lib/ste_omxcomponents/libste_dec_vc1.so:system/lib/ste_omxcomponents/libste_dec_vc1.so \
    device/samsung/golden/prebuilt/lib/ste_omxcomponents/libste_drc.so:system/lib/ste_omxcomponents/libste_drc.so \
    device/samsung/golden/prebuilt/lib/ste_omxcomponents/libste_enc_aac.so:system/lib/ste_omxcomponents/libste_enc_aac.so \
    device/samsung/golden/prebuilt/lib/ste_omxcomponents/libste_enc_amr.so:system/lib/ste_omxcomponents/libste_enc_amr.so \
    device/samsung/golden/prebuilt/lib/ste_omxcomponents/libste_enc_amrwb.so:system/lib/ste_omxcomponents/libste_enc_amrwb.so \
    device/samsung/golden/prebuilt/lib/ste_omxcomponents/libste_enc_h264.so:system/lib/ste_omxcomponents/libste_enc_h264.so \
    device/samsung/golden/prebuilt/lib/ste_omxcomponents/libste_enc_jpeg.so:system/lib/ste_omxcomponents/libste_enc_jpeg.so \
    device/samsung/golden/prebuilt/lib/ste_omxcomponents/libste_enc_mpeg4.so:system/lib/ste_omxcomponents/libste_enc_mpeg4.so \
    device/samsung/golden/prebuilt/lib/ste_omxcomponents/libste_exif_mixer.so:system/lib/ste_omxcomponents/libste_exif_mixer.so \
    device/samsung/golden/prebuilt/lib/ste_omxcomponents/libste_ext_camera.so:system/lib/ste_omxcomponents/libste_ext_camera.so \
    device/samsung/golden/prebuilt/lib/ste_omxcomponents/libste_mdrc.so:system/lib/ste_omxcomponents/libste_mdrc.so \
    device/samsung/golden/prebuilt/lib/ste_omxcomponents/libste_noise_reduction.so:system/lib/ste_omxcomponents/libste_noise_reduction.so \
    device/samsung/golden/prebuilt/lib/ste_omxcomponents/libste_pcm_splitter.so:system/lib/ste_omxcomponents/libste_pcm_splitter.so \
    device/samsung/golden/prebuilt/lib/ste_omxcomponents/libste_speech_proc.so:system/lib/ste_omxcomponents/libste_speech_proc.so \
    device/samsung/golden/prebuilt/lib/ste_omxcomponents/libste_spl.so:system/lib/ste_omxcomponents/libste_spl.so \
    device/samsung/golden/prebuilt/lib/ste_omxcomponents/libste_teq.so:system/lib/ste_omxcomponents/libste_teq.so \
    device/samsung/golden/prebuilt/lib/ste_omxcomponents/libste_tonegen.so:system/lib/ste_omxcomponents/libste_tonegen.so \
    device/samsung/golden/prebuilt/lib/ste_omxcomponents/libste_val_src_sink.so:system/lib/ste_omxcomponents/libste_val_src_sink.so \
    device/samsung/golden/prebuilt/lib/ste_omxcomponents/libste_virtual_surround.so:system/lib/ste_omxcomponents/libste_virtual_surround.so \
    device/samsung/golden/prebuilt/lib/ste_omxcomponents/libst_omxarmivproc.so:system/lib/ste_omxcomponents/libst_omxarmivproc.so \
    device/samsung/golden/prebuilt/lib/ste_omxcomponents/libst_omxface_detector.so:system/lib/ste_omxcomponents/libst_omxface_detector.so \
    device/samsung/golden/prebuilt/lib/ste_omxcomponents/libst_omxnorcos.so:system/lib/ste_omxcomponents/libst_omxnorcos.so \
    device/samsung/golden/prebuilt/lib/ste_omxcomponents/libst_omxredeye_corrector.so:system/lib/ste_omxcomponents/libst_omxredeye_corrector.so \
    device/samsung/golden/prebuilt/lib/ste_omxcomponents/libst_omxredeye_detector.so:system/lib/ste_omxcomponents/libst_omxredeye_detector \
    device/samsung/golden/prebuilt/lib/ste_omxcomponents/libst_omxsplitter.so:system/lib/ste_omxcomponents/libst_omxsplitter.so \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/memgrabctl.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/memgrabctl.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/postevent.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/postevent.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/common/portstate.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/common/portstate.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/h264enc/mpc/api/algo.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/h264enc/mpc/api/algo.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/h264enc/mpc/api/end_algo.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/h264enc/mpc/api/end_algo.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/h264enc/mpc/ddep/api:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/h264enc/mpc/ddep/api \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/h264enc/mpc/ddep/api/log_event.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/h264enc/mpc/ddep/api/log_event.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/mpeg2dec/mpc/api/algo.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/mpeg2dec/mpc/api/algo.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/mpeg2dec/mpc/api/log_event.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/mpeg2dec/mpc/api/log_event.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/mpeg2dec/mpc/api/end_algo.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/mpeg2dec/mpc/api/end_algo.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/h264dec/mpc/api/end_cabac.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/h264dec/mpc/api/end_cabac.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/h264dec/mpc/api/algo.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/h264dec/mpc/api/algo.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/h264dec/mpc/api/cabac.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/h264dec/mpc/api/cabac.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/h264dec/mpc/api/end_algo.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/h264dec/mpc/api/end_algo.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/h264dec/mpc/ddep/api/log_event.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/h264dec/mpc/ddep/api/log_event.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/ext_grabctl/api/returnbuffer.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/ext_grabctl/api/returnbuffer.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/ext_grab/api/alert.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/ext_grab/api/alert.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/vpp/mpc/api/algo.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/vpp/mpc/api/algo.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/vpp/mpc/api/end_algo.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/vpp/mpc/api/end_algo.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/jpegenc/mpc/api/algo.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/jpegenc/mpc/api/algo.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/jpegenc/mpc/api/log_event.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/jpegenc/mpc/api/log_event.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/jpegenc/mpc/api/end_algo.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/jpegenc/mpc/api/end_algo.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/mpeg4dec/mpc/api/algo.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/mpeg4dec/mpc/api/algo.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/mpeg4dec/mpc/api/log_event.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/mpeg4dec/mpc/api/log_event.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/mpeg4dec/mpc/api/end_algo.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/mpeg4dec/mpc/api/end_algo.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/sendcommand.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/sendcommand.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/interruptdfc.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/interruptdfc.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/resource_manager/api/resource.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/resource_manager/api/resource.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/resource_manager/api/buffer.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/resource_manager/api/buffer.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/resource_manager/api/mtf.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/resource_manager/api/mtf.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/resource_manager/api/sleep.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/resource_manager/api/sleep.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/resource_manager/api/tcm_mem_alloc.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/resource_manager/api/tcm_mem_alloc.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/resource_manager/api/signal.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/resource_manager/api/signal.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/resource_manager/api/inform_resource_status.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/resource_manager/api/inform_resource_status.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/resource_manager/api/interrupt.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/resource_manager/api/interrupt.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/resource_manager/api/mtf_resource.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/resource_manager/api/mtf_resource.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/grabctl/api/returnbuffer.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/grabctl/api/returnbuffer.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/grab/api/alert.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/grab/api/alert.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/mpeg4enc/mpc/api/algo.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/mpeg4enc/mpc/api/algo.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/mpeg4enc/mpc/api/log_event.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/mpeg4enc/mpc/api/log_event.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/mpeg4enc/mpc/api/end_algo.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/mpeg4enc/mpc/api/end_algo.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/fillthisbuffer.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/fillthisbuffer.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/vc1dec/mpc/api/algo.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/vc1dec/mpc/api/algo.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/vc1dec/mpc/api/mpc_trace_init.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/vc1dec/mpc/api/mpc_trace_init.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/vc1dec/mpc/api/end_algo.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/vc1dec/mpc/api/end_algo.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/vc1dec/mpc/ddep/api/log_event.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/vc1dec/mpc/ddep/api/log_event.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/jpegdec/mpc/api/algo.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/jpegdec/mpc/api/algo.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/jpegdec/mpc/api/log_event.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/jpegdec/mpc/api/log_event.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/emptythisbuffer.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/emptythisbuffer.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/pcmsettings.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/pcmsettings.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/fsminit.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/fsminit.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_ev/eventhandler.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_ev/eventhandler.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/fsm/generic.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/fsm/generic.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/fsm/component.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/fsm/component.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/ispctl.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/ispctl.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/h264enc/mpc/algo.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/h264enc/mpc/algo.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/h264enc/mpc/algo_hdtv.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/h264enc/mpc/algo_hdtv.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/h264enc/mpc/ddep.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/h264enc/mpc/ddep.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/h264enc/mpc/adapter.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/h264enc/mpc/adapter.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/preload_mpcee.txt:system/usr/share/nmf/repository/mmdsp_8500_v2/preload_mpcee.txt \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/spl/nmfil/effect.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/spl/nmfil/effect.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/spl/effect/libspl.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/spl/effect/libspl.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/grab.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/grab.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/mpeg2dec/mpc/mpeg2dec.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/mpeg2dec/mpc/mpeg2dec.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/mpeg2dec/mpc/ddep.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/mpeg2dec/mpc/ddep.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/h264enc/mpc/api/mpc_trace_init_ack.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/h264enc/mpc/api/mpc_trace_init_ack.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/h264enc/mpc/api/end_algo.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/h264enc/mpc/api/end_algo.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/h264enc/mpc/ddep/api/end_codec.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/h264enc/mpc/ddep/api/end_codec.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/mpeg2dec/mpc/api/fillthisheader.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/mpeg2dec/mpc/api/fillthisheader.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/mpeg2dec/mpc/api/emptythisheader.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/mpeg2dec/mpc/api/emptythisheader.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/mpeg2dec/mpc/api/ddep.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/mpeg2dec/mpc/api/ddep.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/mpeg2dec/mpc/api/end_algo.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/mpeg2dec/mpc/api/end_algo.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/h264dec/mpc/api/end_cabac.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/h264dec/mpc/api/end_cabac.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/h264dec/mpc/api/end_algo.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/h264dec/mpc/api/end_algo.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/h264dec/mpc/ddep/api/end_codec.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/h264dec/mpc/ddep/api/end_codec.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/shared_emptythisbuffer.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/shared_emptythisbuffer.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/ext_grabctl/api/error.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/ext_grabctl/api/error.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/ext_grabctl/api/reportcurves.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/ext_grabctl/api/reportcurves.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/ext_grabctl/api/acknowledges.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/ext_grabctl/api/acknowledges.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/ext_grab/api/alert.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/ext_grab/api/alert.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/vpp/mpc/api/end_algo.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/vpp/mpc/api/end_algo.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/speech_proc/nmf/common/pcmsettings.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/speech_proc/nmf/common/pcmsettings.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/speech_proc/nmf/common/hybrid_emptythisbuffer.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/speech_proc/nmf/common/hybrid_emptythisbuffer.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/speech_proc/nmf/common/hybrid_fillthisbuffer.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/speech_proc/nmf/common/hybrid_fillthisbuffer.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/jpegenc/mpc/api/portSettings.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/jpegenc/mpc/api/portSettings.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/shared_fillthisbuffer.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/shared_fillthisbuffer.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/dmaout/low_signal.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/dmaout/low_signal.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/mpeg4dec/mpc/api/end_algodeblock.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/mpeg4dec/mpc/api/end_algodeblock.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/mpeg4dec/mpc/api/fillthisheader.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/mpeg4dec/mpc/api/fillthisheader.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/mpeg4dec/mpc/api/emptythisheader.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/mpeg4dec/mpc/api/emptythisheader.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/mpeg4dec/mpc/api/ddep.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/mpeg4dec/mpc/api/ddep.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/mpeg4dec/mpc/api/end_algo.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/mpeg4dec/mpc/api/end_algo.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/mpeg4dec/arm_nmf/api/Copy.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/mpeg4dec/arm_nmf/api/Copy.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/requestmemory.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/requestmemory.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/ext_ispctl/api/alert.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/ext_ispctl/api/alert.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/interruptdfc.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/interruptdfc.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/ispctl/api/alert.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/ispctl/api/alert.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/resource_manager/api/inform_resource_status.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/resource_manager/api/inform_resource_status.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/grabctl/api/error.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/grabctl/api/error.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/grabctl/api/reportcurves.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/grabctl/api/reportcurves.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/grabctl/api/acknowledges.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/grabctl/api/acknowledges.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/grab/api/alert.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/grab/api/alert.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/fillthisbuffer.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/fillthisbuffer.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/vc1dec/mpc/api/end_algo.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/vc1dec/mpc/api/end_algo.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/vc1dec/mpc/ddep/api/end_codec.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/vc1dec/mpc/ddep/api/end_codec.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/jpegdec/mpc/api/fillthisheader.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/jpegdec/mpc/api/fillthisheader.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/jpegdec/mpc/api/emptythisheader.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/jpegdec/mpc/api/emptythisheader.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/jpegdec/mpc/api/ddep.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/jpegdec/mpc/api/ddep.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/jpegdec/mpc/api/end_algo.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/jpegdec/mpc/api/end_algo.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/msp_dma_ctrl_ab8500/bitclock.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/msp_dma_ctrl_ab8500/bitclock.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/emptythisbuffer.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/emptythisbuffer.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/debug/mpc/api/msg.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/debug/mpc/api/msg.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/pcmsettings.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/pcmsettings.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/eventhandler.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/eventhandler.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/encoders/streamed/wrapper.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/encoders/streamed/wrapper.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/encoders/framealigned/wrapper.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/encoders/framealigned/wrapper.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/equalizer/effect/libequalizer.elf:system/usr/share/nmf/repository/mmdsp_8500_v2/equalizer/effect/libequalizer.elf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/equalizer/effect/libequalizer.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/equalizer/effect/libequalizer.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/equalizer/nmfil/effect.elf:system/usr/share/nmf/repository/mmdsp_8500_v2/equalizer/nmfil/effect.elf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/equalizer/nmfil/effect.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/equalizer/nmfil/effect.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/h264dec/mpc/cabac.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/h264dec/mpc/cabac.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/h264dec/mpc/h264dec.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/h264dec/mpc/h264dec.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/h264dec/mpc/ddep.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/h264dec/mpc/ddep.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/h264dec/mpc/adapter.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/h264dec/mpc/adapter.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/comfortnoise/nmfil/effect.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/comfortnoise/nmfil/effect.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/comfortnoise/effect/libcomfortnoise.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/comfortnoise/effect/libcomfortnoise.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/hybrid_hsem.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/hybrid_hsem.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/volctrl/nmfil/effect.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/volctrl/nmfil/effect.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/volctrl/effect/libvolctrl.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/volctrl/effect/libvolctrl.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/speech_proc/nmf/mpc/hybrid_buffer/arm2mpc.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/speech_proc/nmf/mpc/hybrid_buffer/arm2mpc.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/speech_proc/nmf/mpc/hybrid_buffer/mpc2arm.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/speech_proc/nmf/mpc/hybrid_buffer/mpc2arm.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/speech_proc/nmf/mpc/time_align.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/speech_proc/nmf/mpc/time_align.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/speech_proc/nmf/mpc/dsp_port.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/speech_proc/nmf/mpc/dsp_port.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/jpegenc/mpc/ddep.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/jpegenc/mpc/ddep.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/decoders/streamed/wrapper.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/decoders/streamed/wrapper.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/decoders/framealigned/wrapper.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/decoders/framealigned/wrapper.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/synchronous_hsem.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/synchronous_hsem.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/hybrid_lsem.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/hybrid_lsem.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/preload_imaging.txt:system/usr/share/nmf/repository/mmdsp_8500_v2/preload_imaging.txt \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/amr/nmfil/encoder.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/amr/nmfil/encoder.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/amr/nmfil/decoder.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/amr/nmfil/decoder.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/amr/codec/libamrdec24.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/amr/codec/libamrdec24.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/amr/codec/libamrenc24.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/amr/codec/libamrenc24.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/amr/codec/libamrdec.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/amr/codec/libamrdec.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/amr/codec/libamrenc.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/amr/codec/libamrenc.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/amr/codec/libamrcommon.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/amr/codec/libamrcommon.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/mpeg4dec/mpc/ddep.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/mpeg4dec/mpc/ddep.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/mpeg4dec/mpc/algo_deblock.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/mpeg4dec/mpc/algo_deblock.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/mpeg4dec/mpc/mpeg4dec.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/mpeg4dec/mpc/mpeg4dec.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/pcm_splitter/nmfil/wrapper.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/pcm_splitter/nmfil/wrapper.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/audiocodec/mpc/msp_dma_ctrl_ab8500.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/audiocodec/mpc/msp_dma_ctrl_ab8500.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/audiocodec/mpc/host_reg.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/audiocodec/mpc/host_reg.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/audiocodec/mpc/renderercontroller.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/audiocodec/mpc/renderercontroller.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/audiocodec/mpc/dmaout_bt.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/audiocodec/mpc/dmaout_bt.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/audiocodec/mpc/dmain.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/audiocodec/mpc/dmain.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/audiocodec/mpc/capturercontroller.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/audiocodec/mpc/capturercontroller.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/audiocodec/mpc/dmaout.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/audiocodec/mpc/dmaout.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/audiocodec/mpc/dmain_bt.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/audiocodec/mpc/dmain_bt.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/audiocodec/mpc/msp_dma_ctrl_bt.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/audiocodec/mpc/msp_dma_ctrl_bt.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/audiocodec/mpc/msp_dma_controller.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/audiocodec/mpc/msp_dma_controller.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/misc/controller.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/misc/controller.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/misc/synchronisation.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/misc/synchronisation.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/ext_grabctl.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/ext_grabctl.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/mixer/nmfil/wrapper.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/mixer/nmfil/wrapper.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/mixer/effect/libmixer.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/mixer/effect/libmixer.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/audiolibs/libaudiotables.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/audiolibs/libaudiotables.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/audiolibs/libcrc.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/audiolibs/libcrc.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/audiolibs/libsbrqmf.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/audiolibs/libsbrqmf.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/audiolibs/libfft.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/audiolibs/libfft.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/audiolibs/libmalloc.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/audiolibs/libmalloc.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/audiolibs/libmp3dequan.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/audiolibs/libmp3dequan.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/audiolibs/libmp3hybrid.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/audiolibs/libmp3hybrid.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/audiolibs/libmdcttables.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/audiolibs/libmdcttables.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/audiolibs/libmdct.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/audiolibs/libmdct.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/audiolibs/libaudiowindows.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/audiolibs/libaudiowindows.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/audiolibs/libpolysyn.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/audiolibs/libpolysyn.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/audiolibs/libimdct.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/audiolibs/libimdct.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/audiolibs/libvector.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/audiolibs/libvector.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/audiolibs/libbitstream.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/audiolibs/libbitstream.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/audiolibs/libdBconv.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/audiolibs/libdBconv.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/mdrc/nmfil/effect.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/mdrc/nmfil/effect.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/mdrc/effect/libmdrc5b.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/mdrc/effect/libmdrc5b.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/malloc.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/malloc.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/bindings/shmpcm/shmin.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/bindings/shmpcm/shmin.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/bindings/shmpcm/shmout.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/bindings/shmpcm/shmout.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/bindings/shm/shmin.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/bindings/shm/shmin.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/bindings/shm/shmout.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/bindings/shm/shmout.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/bindings/pcmadapter.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/bindings/pcmadapter.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/mpeg4enc/mpc/brc.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/mpeg4enc/mpc/brc.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/mpeg4enc/mpc/ddep.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/mpeg4enc/mpc/ddep.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/vc1dec/mpc/vc1dec.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/vc1dec/mpc/vc1dec.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/vc1dec/mpc/ddep.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/vc1dec/mpc/ddep.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/vc1dec/mpc/adapter.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/vc1dec/mpc/adapter.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/jpegdec/mpc/jpegdec.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/jpegdec/mpc/jpegdec.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/jpegdec/mpc/ddep.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/jpegdec/mpc/ddep.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/transducer_equalizer/nmfil/effect.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/transducer_equalizer/nmfil/effect.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/transducer_equalizer/effect/libtransducer_equalizer.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/transducer_equalizer/effect/libtransducer_equalizer.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/pcmprocessings/wrapper.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/pcmprocessings/wrapper.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/pcmprocessings/mips.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/pcmprocessings/mips.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/mpeg4enc.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/mpeg4enc.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/preload_adm.txt:system/usr/share/nmf/repository/mmdsp_8500_v2/preload_adm.txt \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/synchronous_lsem.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/synchronous_lsem.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/debug/mpc/dsp_printf.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/debug/mpc/dsp_printf.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/vpp.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/vpp.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/osttrace/mmdsp.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/osttrace/mmdsp.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/jpegenc.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/jpegenc.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/samplerateconv/nmfil/wrapper.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/samplerateconv/nmfil/wrapper.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/samplerateconv/effect/libresampling.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/samplerateconv/effect/libresampling.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/memgrabctl/api/configure.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/memgrabctl/api/configure.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/common/configure.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/common/configure.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/h264enc/mpc/api/mpc_trace_init.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/h264enc/mpc/api/mpc_trace_init.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/h264enc/mpc/ddep/api/start_codec.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/h264enc/mpc/ddep/api/start_codec.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/spl/nmfil/effect/configure.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/spl/nmfil/effect/configure.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/mpeg2dec/mpc/api/fillthisheader.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/mpeg2dec/mpc/api/fillthisheader.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/mpeg2dec/mpc/api/emptythisheader.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/mpeg2dec/mpc/api/emptythisheader.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/mpeg2dec/mpc/api/ddep.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/mpeg2dec/mpc/api/ddep.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/setmemory.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/setmemory.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/encoders/streamed/configure.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/encoders/streamed/configure.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/equalizer/nmfil/effect/configure.elf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/equalizer/nmfil/effect/configure.elf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/equalizer/nmfil/effect/configure.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/equalizer/nmfil/effect/configure.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/api/set_debug.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/api/set_debug.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/h264dec/mpc/api/trace_init.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/h264dec/mpc/api/trace_init.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/h264dec/mpc/ddep/api/start_codec.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/h264dec/mpc/ddep/api/start_codec.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/dummy.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/dummy.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/shared_emptythisbuffer.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/shared_emptythisbuffer.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/ext_grabctl/api/configure.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/ext_grabctl/api/configure.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/comfortnoise/nmfil/effect/configure.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/comfortnoise/nmfil/effect/configure.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/ext_grab/api/cmd.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/ext_grab/api/cmd.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/volctrl/nmfil/effect/configure.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/volctrl/nmfil/effect/configure.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/speech_proc/nmf/common/configure_time_align.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/speech_proc/nmf/common/configure_time_align.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/speech_proc/nmf/common/hybrid_emptythisbuffer.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/speech_proc/nmf/common/hybrid_emptythisbuffer.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/speech_proc/nmf/common/hybrid_fillthisbuffer.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/speech_proc/nmf/common/hybrid_fillthisbuffer.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/speech_proc/nmf/mpc/dsp_port/configure.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/speech_proc/nmf/mpc/dsp_port/configure.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/jpegenc/mpc/api/ddep.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/jpegenc/mpc/api/ddep.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/shared_fillthisbuffer.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/shared_fillthisbuffer.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/decoders/streamed/configure.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/decoders/streamed/configure.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/dmaout/low_power.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/dmaout/low_power.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/amr/nmfil/decoder/configure.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/amr/nmfil/decoder/configure.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/amr/nmfil/encoder/configure.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/amr/nmfil/encoder/configure.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/mpeg4dec/mpc/api/fillthisheader.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/mpeg4dec/mpc/api/fillthisheader.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/mpeg4dec/mpc/api/emptythisheader.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/mpeg4dec/mpc/api/emptythisheader.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/mpeg4dec/mpc/api/ddep.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/mpeg4dec/mpc/api/ddep.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/mpeg4dec/mpc/api/algo_deblock.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/mpeg4dec/mpc/api/algo_deblock.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/mpeg4dec/arm_nmf/api/Copy.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/mpeg4dec/arm_nmf/api/Copy.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/pcm_splitter/nmfil/wrapper/configure.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/pcm_splitter/nmfil/wrapper/configure.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/sendcommand.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/sendcommand.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/misc/controller/configure.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/misc/controller/configure.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/misc/samplesplayed.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/misc/samplesplayed.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/mixer/nmfil/wrapper/configure.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/mixer/nmfil/wrapper/configure.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/audiolibs/common/dummy.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/audiolibs/common/dummy.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/audiolibs/libmalloc/init.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/audiolibs/libmalloc/init.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/hst/common/pcmsettings.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/hst/common/pcmsettings.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/ext_ispctl/api/cfg.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/ext_ispctl/api/cfg.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/ext_ispctl/api/cmd.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/ext_ispctl/api/cmd.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/mdrc/nmfil/effect/configure.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/mdrc/nmfil/effect/configure.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/ispctl/api/cfg.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/ispctl/api/cfg.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/ispctl/api/cmd.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/ispctl/api/cmd.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/resource_manager/api/common_interface_exports.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/resource_manager/api/common_interface_exports.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/resource_manager/api/mtf_resource.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/resource_manager/api/mtf_resource.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/grabctl/api/configure.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/grabctl/api/configure.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/grab/api/cmd.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/grab/api/cmd.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/bindings/shmpcm/configure.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/bindings/shmpcm/configure.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/bindings/shm/configure.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/bindings/shm/configure.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/bindings/pcmadapter/configure.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/bindings/pcmadapter/configure.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/mpeg4enc/mpc/api/ddep.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/mpeg4enc/mpc/api/ddep.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/fillthisbuffer.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/fillthisbuffer.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/vc1dec/mpc/api/mpc_trace_init.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/vc1dec/mpc/api/mpc_trace_init.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/vc1dec/mpc/ddep/api/start_codec.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/vc1dec/mpc/ddep/api/start_codec.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/jpegdec/mpc/api/algo.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/jpegdec/mpc/api/algo.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/jpegdec/mpc/api/fillthisheader.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/jpegdec/mpc/api/fillthisheader.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/jpegdec/mpc/api/emptythisheader.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/jpegdec/mpc/api/emptythisheader.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/jpegdec/mpc/api/ddep.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/jpegdec/mpc/api/ddep.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/transducer_equalizer/nmfil/effect/configure.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/transducer_equalizer/nmfil/effect/configure.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/pcmprocessings/wrapper/configure.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/pcmprocessings/wrapper/configure.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/pcmprocessings/mips/configure.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/pcmprocessings/mips/configure.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/msp_dma_ctrl_ab8500/configure.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/msp_dma_ctrl_ab8500/configure.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/emptythisbuffer.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/emptythisbuffer.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/pcmsettings.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/pcmsettings.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/samplerateconv/nmfil/wrapper/configure.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/samplerateconv/nmfil/wrapper/configure.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/fsminit.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/fsminit.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/malloc/setheap.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/malloc/setheap.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/grabctl.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/grabctl.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/ext_ispctl.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/ext_ispctl.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/ext_grab.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/ext_grab.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/nmf/mpc/shared_bufout.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/nmf/mpc/shared_bufout.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/nmf/mpc/shared_bufin.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/nmf/mpc/shared_bufin.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/resource_manager.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/resource_manager.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/ext/api/set_debug.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/ext/api/set_debug.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_sk/pcmdump/configure.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_sk/pcmdump/configure.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/_st/pcmdump/complete.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/_st/pcmdump/complete.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/pcmdump.elf4nmf:system/usr/share/nmf/repository/mmdsp_8500_v2/pcmdump.elf4nmf \
    device/samsung/golden/prebuilt/usr/share/nmf/repository/mmdsp_8500_v2/preload_video.txt:system/usr/share/nmf/repository/mmdsp_8500_v2/preload_video.txt

# Camera
PRODUCT_COPY_FILES += \
    device/samsung/golden/prebuilt/lib/hw/camera.montblanc.so:system/lib/hw/camera.montblanc.so \
    device/samsung/golden/prebuilt/usr/share/camera/config_file/samsung_s5k4ecgx_golden.dat:system/usr/share/camera/config_file/samsung_s5k4ecgx_golden.dat \
    device/samsung/golden/prebuilt/usr/share/camera/config_file/siliconfile_sr030pc50.dat:system/usr/share/camera/config_file/siliconfile_sr030pc50.dat

# Precompiled STE audio
PRODUCT_COPY_FILES += \
    device/samsung/golden/prebuilt/lib/libasound.so:system/lib/libasound.so \
    device/samsung/golden/prebuilt/lib/hw/audio.primary.montblanc.so:system/lib/hw/audio.primary.montblanc.so \
    device/samsung/golden/prebuilt/lib/hw/audio_policy.montblanc.so:system/lib/hw/audio_policy.montblanc.so

# Audio libraries
PRODUCT_COPY_FILES += \
    device/samsung/golden/prebuilt/lib/hw/audio.tms.default.so:system/lib/hw/audio.tms.default.so \
    device/samsung/golden/prebuilt/lib/lib_Samsung_Resampler.so:system/lib/lib_Samsung_Resampler.so \
    device/samsung/golden/prebuilt/lib/libsamsungSoundbooster.so:system/lib/libsamsungSoundbooster.so \
    device/samsung/golden/prebuilt/lib/lib_SamsungRec_V01006.so:system/lib/lib_SamsungRec_V01006.so \
    device/samsung/golden/prebuilt/lib/lib_Samsung_SB_AM_for_ICS_v03007.so:system/lib/lib_Samsung_SB_AM_for_ICS_v03007.so \
    device/samsung/golden/prebuilt/lib/libaudiopolicy_sec.so:system/lib/libaudiopolicy_sec.so \
    device/samsung/golden/prebuilt/lib/libcontrolcsc.so:system/lib/libcontrolcsc.so \
    device/samsung/golden/prebuilt/lib/libhalaudioprocessing.so:system/lib/libhalaudioprocessing.so \
    device/samsung/golden/prebuilt/lib/libsamsungRecord.so:system/lib/libsamsungRecord.so

# Audio configs
PRODUCT_COPY_FILES += \
    device/samsung/golden/prebuilt/etc/asound.conf:system/etc/asound.conf \
    device/samsung/golden/prebuilt/etc/audio_policy.conf:system/etc/audio_policy.conf

# Alsa config
PRODUCT_COPY_FILES += \
    device/samsung/golden/prebuilt/usr/share/alsa/alsa.conf:system/usr/share/alsa/alsa.conf \
    device/samsung/golden/prebuilt/usr/share/alsa/cards/aliases.conf:system/usr/share/alsa/cards/aliases.conf \
    device/samsung/golden/prebuilt/usr/share/alsa/pcm/center_lfe.conf:system/usr/share/alsa/pcm/center_lfe.conf \
    device/samsung/golden/prebuilt/usr/share/alsa/pcm/default.conf:system/usr/share/alsa/pcm/default.conf \
    device/samsung/golden/prebuilt/usr/share/alsa/pcm/dmix.conf:system/usr/share/alsa/pcm/dmix.conf \
    device/samsung/golden/prebuilt/usr/share/alsa/pcm/dpl.conf:system/usr/share/alsa/pcm/dpl.conf \
    device/samsung/golden/prebuilt/usr/share/alsa/pcm/dsnoop.conf:system/usr/share/alsa/pcm/dsnoop.conf \
    device/samsung/golden/prebuilt/usr/share/alsa/pcm/front.conf:system/usr/share/alsa/pcm/front.conf \
    device/samsung/golden/prebuilt/usr/share/alsa/pcm/iec958.conf:system/usr/share/alsa/pcm/iec958.conf \
    device/samsung/golden/prebuilt/usr/share/alsa/pcm/modem.conf:system/usr/share/alsa/pcm/modem.conf \
    device/samsung/golden/prebuilt/usr/share/alsa/pcm/rear.conf:system/usr/share/alsa/pcm/rear.conf \
    device/samsung/golden/prebuilt/usr/share/alsa/pcm/side.conf:system/usr/share/alsa/pcm/side.conf \
    device/samsung/golden/prebuilt/usr/share/alsa/pcm/surround40.conf:system/usr/share/alsa/pcm/surround40.conf \
    device/samsung/golden/prebuilt/usr/share/alsa/pcm/surround41.conf:system/usr/share/alsa/pcm/surround41.conf \
    device/samsung/golden/prebuilt/usr/share/alsa/pcm/surround50.conf:system/usr/share/alsa/pcm/surround50.conf \
    device/samsung/golden/prebuilt/usr/share/alsa/pcm/surround51.conf:system/usr/share/alsa/pcm/surround51.conf \
    device/samsung/golden/prebuilt/usr/share/alsa/pcm/surround71.conf:system/usr/share/alsa/pcm/surround71.conf

#########################################
## Sensors
#########################################

# Lights/sensors modules
PRODUCT_COPY_FILES += \
    device/samsung/golden/prebuilt/lib/hw/sensors.montblanc.so:system/lib/hw/sensors.montblanc.so \
    device/samsung/golden/prebuilt/lib/libacdapi_azi.so:system/lib/libacdapi_azi.so \
    device/samsung/golden/prebuilt/lib/libalps_sensors_hal.so:system/lib/libalps_sensors_hal.so \
    device/samsung/golden/prebuilt/etc/calib.dat:system/etc/calib.dat \
    device/samsung/golden/prebuilt/etc/param.dat:system/etc/param.dat

#########################################
## Other stuff
#########################################

# Rest of /system/etc
PRODUCT_COPY_FILES += \
    device/samsung/golden/prebuilt/etc/cspsa.conf:system/etc/cspsa.conf \
    device/samsung/golden/prebuilt/etc/vold.fstab:system/etc/vold.fstab

# Permissions
PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/handheld_core_hardware.xml:system/etc/permissions/handheld_core_hardware.xml \
    frameworks/native/data/etc/android.hardware.bluetooth_le.xml:system/etc/permissions/android.hardware.bluetooth_le.xml \
    frameworks/native/data/etc/android.hardware.camera.autofocus.xml:system/etc/permissions/android.hardware.camera.autofocus.xml \
    frameworks/native/data/etc/android.hardware.camera.flash-autofocus.xml:system/etc/permissions/android.hardware.camera.flash-autofocus.xml \
    frameworks/native/data/etc/android.hardware.camera.front.xml:system/etc/permissions/android.hardware.camera.front.xml \
    frameworks/native/data/etc/android.hardware.location.gps.xml:system/etc/permissions/android.hardware.location.gps.xml \
    frameworks/native/data/etc/android.hardware.sensor.accelerometer.xml:system/etc/permissions/android.hardware.sensor.accelerometer.xml \
    frameworks/native/data/etc/android.hardware.sensor.compass.xml:system/etc/permissions/android.hardware.sensor.compass.xml \
    frameworks/native/data/etc/android.hardware.sensor.gyroscope.xml:system/etc/permissions/android.hardware.sensor.gyroscope.xml \
    frameworks/native/data/etc/android.hardware.sensor.proximity.xml:system/etc/permissions/android.hardware.sensor.proximity.xml \
    frameworks/native/data/etc/android.hardware.telephony.gsm.xml:system/etc/permissions/android.hardware.telephony.gsm.xml \
    frameworks/native/data/etc/android.hardware.touchscreen.multitouch.jazzhand.xml:system/etc/permissions/android.hardware.touchscreen.multitouch.jazzhand.xml \
    frameworks/native/data/etc/android.hardware.wifi.direct.xml:system/etc/permissions/android.hardware.wifi.direct.xml \
    frameworks/native/data/etc/android.hardware.wifi.xml:system/etc/permissions/android.hardware.wifi.xml \
    frameworks/native/data/etc/android.software.sip.voip.xml:system/etc/permissions/android.software.sip.voip.xml \
    frameworks/native/data/etc/android.software.sip.xml:system/etc/permissions/android.software.sip.xml \
    packages/wallpapers/LivePicker/android.software.live_wallpaper.xml:system/etc/permissions/android.software.live_wallpaper.xml

# Keylayout
PRODUCT_COPY_FILES += \
    device/samsung/golden/prebuilt/usr/keylayout/gpio-keys.kl:system/usr/keylayout/gpio-keys.kl

# Security Daemon Modem
PRODUCT_COPY_FILES += \
    device/samsung/golden/prebuilt/bin/copsdaemon:system/bin/copsdaemon \
    device/samsung/golden/prebuilt/lib/libcops.so:system/lib/libcops.so \
    device/samsung/golden/prebuilt/lib/tee/8500bx_cops_ta_8500bx_secure.ssw:system/lib/tee/8500bx_cops_ta_8500bx_secure.ssw \
    device/samsung/golden/prebuilt/lib/tee/cops_ta:system/lib/tee/cops_ta \
    device/samsung/golden/prebuilt/lib/tee/custom_ta.ssw:system/lib/tee/custom_ta.ssw \
    device/samsung/golden/prebuilt/lib/tee/libbassapp_ssw:system/lib/tee/libbassapp_ssw \
    device/samsung/golden/prebuilt/lib/tee/smcl_ta_8500bx_secure.ssw:system/lib/tee/smcl_ta_8500bx_secure.ssw

# LVVEFS config
PRODUCT_COPY_FILES += \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/LVVEFS_Rx_Configuration.txt:system/etc/LVVEFS_tuning_parameters/LVVEFS_Rx_Configuration.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/LVVEFS_Tx_Configuration.txt:system/etc/LVVEFS_tuning_parameters/LVVEFS_Tx_Configuration.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Rx_ControlParams_SPEAKER_WB.txt:system/etc/LVVEFS_tuning_parameters/Rx_ControlParams_SPEAKER_WB.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Tx_ControlParams_SPEAKER_WB.txt:system/etc/LVVEFS_tuning_parameters/Tx_ControlParams_SPEAKER_WB.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Rx_ControlParams_BTNRECOFF.txt:system/etc/LVVEFS_tuning_parameters/Rx_ControlParams_BTNRECOFF.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Rx_ControlParams_BTNRECOFF_VOIP.txt:system/etc/LVVEFS_tuning_parameters/Rx_ControlParams_BTNRECOFF_VOIP.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Rx_ControlParams_BTNRECOFF_VT.txt:system/etc/LVVEFS_tuning_parameters/Rx_ControlParams_BTNRECOFF_VT.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Rx_ControlParams_BTNRECOFF_WB.txt:system/etc/LVVEFS_tuning_parameters/Rx_ControlParams_BTNRECOFF_WB.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Rx_ControlParams_BTNRECON.txt:system/etc/LVVEFS_tuning_parameters/Rx_ControlParams_BTNRECON.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Rx_ControlParams_BTNRECON_VOIP.txt:system/etc/LVVEFS_tuning_parameters/Rx_ControlParams_BTNRECON_VOIP.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Rx_ControlParams_BTNRECON_VT.txt:system/etc/LVVEFS_tuning_parameters/Rx_ControlParams_BTNRECON_VT.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Rx_ControlParams_BTNRECON_WB.txt:system/etc/LVVEFS_tuning_parameters/Rx_ControlParams_BTNRECON_WB.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Rx_ControlParams_HANDSET.txt:system/etc/LVVEFS_tuning_parameters/Rx_ControlParams_HANDSET.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Rx_ControlParams_HANDSET_EXTRA.txt:system/etc/LVVEFS_tuning_parameters/Rx_ControlParams_HANDSET_EXTRA.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Rx_ControlParams_HANDSET_EXTRA_WB.txt:system/etc/LVVEFS_tuning_parameters/Rx_ControlParams_HANDSET_EXTRA_WB.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Rx_ControlParams_HANDSET_VOIP.txt:system/etc/LVVEFS_tuning_parameters/Rx_ControlParams_HANDSET_VOIP.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Rx_ControlParams_HANDSET_VT.txt:system/etc/LVVEFS_tuning_parameters/Rx_ControlParams_HANDSET_VT.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Rx_ControlParams_HANDSET_WB.txt:system/etc/LVVEFS_tuning_parameters/Rx_ControlParams_HANDSET_WB.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Rx_ControlParams_HEADPHONE.txt:system/etc/LVVEFS_tuning_parameters/Rx_ControlParams_HEADPHONE.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Rx_ControlParams_HEADPHONE_VOIP.txt:system/etc/LVVEFS_tuning_parameters/Rx_ControlParams_HEADPHONE_VOIP.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Rx_ControlParams_HEADPHONE_VT.txt:system/etc/LVVEFS_tuning_parameters/Rx_ControlParams_HEADPHONE_VT.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Rx_ControlParams_HEADPHONE_WB.txt:system/etc/LVVEFS_tuning_parameters/Rx_ControlParams_HEADPHONE_WB.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Rx_ControlParams_HEADSET.txt:system/etc/LVVEFS_tuning_parameters/Rx_ControlParams_HEADSET.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Rx_ControlParams_HEADSET_VOIP.txt:system/etc/LVVEFS_tuning_parameters/Rx_ControlParams_HEADSET_VOIP.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Rx_ControlParams_HEADSET_VT.txt:system/etc/LVVEFS_tuning_parameters/Rx_ControlParams_HEADSET_VT.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Rx_ControlParams_HEADSET_WB.txt:system/etc/LVVEFS_tuning_parameters/Rx_ControlParams_HEADSET_WB.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Rx_ControlParams_LOOPBACK.txt:system/etc/LVVEFS_tuning_parameters/Rx_ControlParams_LOOPBACK.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Rx_ControlParams_SPEAKER.txt:system/etc/LVVEFS_tuning_parameters/Rx_ControlParams_SPEAKER.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Rx_ControlParams_SPEAKER_EXTRA.txt:system/etc/LVVEFS_tuning_parameters/Rx_ControlParams_SPEAKER_EXTRA.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Rx_ControlParams_SPEAKER_EXTRA_WB.txt:system/etc/LVVEFS_tuning_parameters/Rx_ControlParams_SPEAKER_EXTRA_WB.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Rx_ControlParams_SPEAKER_VOIP.txt:system/etc/LVVEFS_tuning_parameters/Rx_ControlParams_SPEAKER_VOIP.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Rx_ControlParams_SPEAKER_VT.txt:system/etc/LVVEFS_tuning_parameters/Rx_ControlParams_SPEAKER_VT.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Tx_ControlParams_BTNRECOFF.txt:system/etc/LVVEFS_tuning_parameters/Tx_ControlParams_BTNRECOFF.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Tx_ControlParams_BTNRECOFF_VOIP.txt:system/etc/LVVEFS_tuning_parameters/Tx_ControlParams_BTNRECOFF_VOIP.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Tx_ControlParams_BTNRECOFF_VT.txt:system/etc/LVVEFS_tuning_parameters/Tx_ControlParams_BTNRECOFF_VT.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Tx_ControlParams_BTNRECOFF_WB.txt:system/etc/LVVEFS_tuning_parameters/Tx_ControlParams_BTNRECOFF_WB.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Tx_ControlParams_BTNRECON.txt:system/etc/LVVEFS_tuning_parameters/Tx_ControlParams_BTNRECON.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Tx_ControlParams_BTNRECON_VOIP.txt:system/etc/LVVEFS_tuning_parameters/Tx_ControlParams_BTNRECON_VOIP.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Tx_ControlParams_BTNRECON_VT.txt:system/etc/LVVEFS_tuning_parameters/Tx_ControlParams_BTNRECON_VT.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Tx_ControlParams_BTNRECON_WB.txt:system/etc/LVVEFS_tuning_parameters/Tx_ControlParams_BTNRECON_WB.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Tx_ControlParams_HANDSET.txt:system/etc/LVVEFS_tuning_parameters/Tx_ControlParams_HANDSET.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Tx_ControlParams_HANDSET_EXTRA.txt:system/etc/LVVEFS_tuning_parameters/Tx_ControlParams_HANDSET_EXTRA.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Tx_ControlParams_HANDSET_EXTRA_WB.txt:system/etc/LVVEFS_tuning_parameters/Tx_ControlParams_HANDSET_EXTRA_WB.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Tx_ControlParams_HANDSET_VOIP.txt:system/etc/LVVEFS_tuning_parameters/Tx_ControlParams_HANDSET_VOIP.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Tx_ControlParams_HANDSET_VT.txt:system/etc/LVVEFS_tuning_parameters/Tx_ControlParams_HANDSET_VT.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Tx_ControlParams_HANDSET_WB.txt:system/etc/LVVEFS_tuning_parameters/Tx_ControlParams_HANDSET_WB.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Tx_ControlParams_HEADPHONE.txt:system/etc/LVVEFS_tuning_parameters/Tx_ControlParams_HEADPHONE.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Tx_ControlParams_HEADPHONE_VOIP.txt:system/etc/LVVEFS_tuning_parameters/Tx_ControlParams_HEADPHONE_VOIP.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Tx_ControlParams_HEADPHONE_VT.txt:system/etc/LVVEFS_tuning_parameters/Tx_ControlParams_HEADPHONE_VT.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Tx_ControlParams_HEADPHONE_WB.txt:system/etc/LVVEFS_tuning_parameters/Tx_ControlParams_HEADPHONE_WB.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Tx_ControlParams_HEADSET.txt:system/etc/LVVEFS_tuning_parameters/Tx_ControlParams_HEADSET.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Tx_ControlParams_HEADSET_VOIP.txt:system/etc/LVVEFS_tuning_parameters/Tx_ControlParams_HEADSET_VOIP.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Tx_ControlParams_HEADSET_VT.txt:system/etc/LVVEFS_tuning_parameters/Tx_ControlParams_HEADSET_VT.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Tx_ControlParams_HEADSET_WB.txt:system/etc/LVVEFS_tuning_parameters/Tx_ControlParams_HEADSET_WB.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Tx_ControlParams_LOOPBACK.txt:system/etc/LVVEFS_tuning_parameters/Tx_ControlParams_LOOPBACK.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Tx_ControlParams_SPEAKER.txt:system/etc/LVVEFS_tuning_parameters/Tx_ControlParams_SPEAKER.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Tx_ControlParams_SPEAKER_EXTRA.txt:system/etc/LVVEFS_tuning_parameters/Tx_ControlParams_SPEAKER_EXTRA.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Tx_ControlParams_SPEAKER_EXTRA_WB.txt:system/etc/LVVEFS_tuning_parameters/Tx_ControlParams_SPEAKER_EXTRA_WB.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Tx_ControlParams_SPEAKER_VOIP.txt:system/etc/LVVEFS_tuning_parameters/Tx_ControlParams_SPEAKER_VOIP.txt \
    device/samsung/golden/prebuilt/etc/LVVEFS_tuning_parameters/Tx_ControlParams_SPEAKER_VT.txt:system/etc/LVVEFS_tuning_parameters/Tx_ControlParams_SPEAKER_VT.txt

# LPM files
PRODUCT_COPY_FILES += \
    device/samsung/golden/prebuilt/bin/playlpm:system/bin/playlpm \
    device/samsung/golden/prebuilt/bin/lpmkey:system/bin/lpmkey \
    device/samsung/golden/prebuilt/lib/libQmageDecoder.so:system/lib/libQmageDecoder.so \
    device/samsung/golden/prebuilt/media/battery_charging_5.qmg:system/media/battery_charging_5.qmg \
    device/samsung/golden/prebuilt/media/battery_charging_10.qmg:system/media/battery_charging_10.qmg \
    device/samsung/golden/prebuilt/media/battery_charging_15.qmg:system/media/battery_charging_15.qmg \
    device/samsung/golden/prebuilt/media/battery_charging_20.qmg:system/media/battery_charging_20.qmg \
    device/samsung/golden/prebuilt/media/battery_charging_25.qmg:system/media/battery_charging_25.qmg \
    device/samsung/golden/prebuilt/media/battery_charging_30.qmg:system/media/battery_charging_30.qmg \
    device/samsung/golden/prebuilt/media/battery_charging_35.qmg:system/media/battery_charging_35.qmg \
    device/samsung/golden/prebuilt/media/battery_charging_40.qmg:system/media/battery_charging_40.qmg \
    device/samsung/golden/prebuilt/media/battery_charging_45.qmg:system/media/battery_charging_45.qmg \
    device/samsung/golden/prebuilt/media/battery_charging_50.qmg:system/media/battery_charging_50.qmg \
    device/samsung/golden/prebuilt/media/battery_charging_55.qmg:system/media/battery_charging_55.qmg \
    device/samsung/golden/prebuilt/media/battery_charging_60.qmg:system/media/battery_charging_60.qmg \
    device/samsung/golden/prebuilt/media/battery_charging_65.qmg:system/media/battery_charging_65.qmg \
    device/samsung/golden/prebuilt/media/battery_charging_70.qmg:system/media/battery_charging_70.qmg \
    device/samsung/golden/prebuilt/media/battery_charging_75.qmg:system/media/battery_charging_75.qmg \
    device/samsung/golden/prebuilt/media/battery_charging_80.qmg:system/media/battery_charging_80.qmg \
    device/samsung/golden/prebuilt/media/battery_charging_85.qmg:system/media/battery_charging_85.qmg \
    device/samsung/golden/prebuilt/media/battery_charging_90.qmg:system/media/battery_charging_90.qmg \
    device/samsung/golden/prebuilt/media/battery_charging_95.qmg:system/media/battery_charging_95.qmg \
    device/samsung/golden/prebuilt/media/battery_charging_100.qmg:system/media/battery_charging_100.qmg \
    device/samsung/golden/prebuilt/media/battery_batteryerror.qmg:system/media/battery_batteryerror.qmg \
    device/samsung/golden/prebuilt/media/battery_error.qmg:system/media/battery_error.qmg \
    device/samsung/golden/prebuilt/media/chargingwarning.qmg:system/media/chargingwarning.qmg

#init.d scripts
PRODUCT_COPY_FILES += \
    $(COMMON_PATH)/configs/98netflix:system/etc/init.d/98netflix

# Dalvik config
$(call inherit-product, frameworks/native/build/phone-xhdpi-1024-dalvik-heap.mk)
