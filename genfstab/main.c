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

#include <unistd.h>
#include <fcntl.h>

#include <string.h>

#include "fstab.h"
#include "block.h"

#define UID_SYSTEM 1000
#define GID_SYSTEM 1000

int fd;

void write_static(const char *data) {
    write(fd, data, strlen(data));
}

void write_dynamic(const char *device, const char *if_f2fs, const char *if_ext4) {
    short f2fs = is_f2fs(device);
    if (f2fs == 1) write_static(if_f2fs);
    if (f2fs == 0) write_static(if_ext4);
}

void write_fstab() {
    write_static(fstab_original_a);
    write_dynamic(BLOCK_SYSTEM, fstab_system_f2fs, fstab_system_ext4);
    write_dynamic(BLOCK_CACHE, fstab_cache_f2fs, fstab_cache_ext4);
    write_static(fstab_original_b);
    write_dynamic(BLOCK_DATA, fstab_data_f2fs, fstab_data_ext4);
    write_static(fstab_original_c);
}

int main() {
    fd = open(FSTAB_PATH, O_WRONLY | O_CREAT | O_TRUNC, 0640);
    if (fd != -1) {
        write_fstab();
        close(fd);
        chown(FSTAB_PATH, UID_SYSTEM, GID_SYSTEM);
    }

    return 0;
}
