/*
 * Copyright (C) 2014 Marcin Chojnacki marcinch7@gmail.com
 * Copyright (C) 2014 NovaFusion https://github.com/NovaFusion
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef FSTAB_H
#define FSTAB_H

#define FSTAB_PATH "/fstab.samsunggolden"

const char fstab_original_a[] =
"# Android fstab file.\n"
"#<src>                                       <mnt_point>          <type>  <mnt_flags>                                           <fs_mgr_flags>\n"
"# The filesystem that contains the filesystem checker binary (typically /system) cannot\n"
"# specify MF_CHECK, and must come before any filesystems that do specify MF_CHECK\n\n"
;

//more fancy flags to f2fs system coming soon... do you know what flags should we apply to read-only f2fs filesystem?
const char fstab_system_f2fs[] = "/dev/block/mmcblk0p22                       /system              f2fs    ro                                                    wait\n";
const char fstab_system_ext4[] = "/dev/block/mmcblk0p22                       /system              ext4    ro                                                    wait\n";

const char fstab_cache_f2fs[] = "/dev/block/mmcblk0p23                       /cache               f2fs    noatime,nosuid,nodev,discard,inline_xattr             wait\n";
const char fstab_cache_ext4[] = "/dev/block/mmcblk0p23                       /cache               ext4    noatime,nosuid,nodev,nomblk_io_submit,errors=panic    wait\n";

const char fstab_original_b[] =
"/dev/block/mmcblk0p12                       /modemfs             ext4    noatime,nosuid,nodev,nomblk_io_submit,errors=panic    wait\n"
"/dev/block/mmcblk0p11                       /efs                 ext4    noatime,nosuid,nodev,nomblk_io_submit,errors=panic    wait\n"
;

const char fstab_data_f2fs[] = "/dev/block/mmcblk0p25                       /data                f2fs    noatime,nosuid,nodev,discard,inline_xattr             wait,encryptable=footer\n";
const char fstab_data_ext4[] = "/dev/block/mmcblk0p25                       /data                ext4    noatime,nosuid,nodev,nomblk_io_submit,errors=panic    wait,encryptable=footer\n";

const char fstab_original_c[] =
"/dev/block/mmcblk0p20                       /boot                emmc    defaults                                              defaults\n"
"/dev/block/mmcblk0p21                       /recovery            emmc    defaults                                              defaults\n"
"/devices/sdi0/mmc_host/mmc1/mmc1            /storage/sdcard1     auto    defaults                                              voldmanaged=sdcard1:auto\n"
"/devices/platform/musb-ux500.0/musb-hdrc    /storage/usbdisk0    auto    defaults                                              voldmanaged=usbdisk0:auto\n"
;

#endif
