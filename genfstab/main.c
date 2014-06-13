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

#define WRITE_STATIC(data) write(fd, data, sizeof(data) - 1)
#define WRITE_DYNAMIC write_entry

int fd;

void write_entry(const char *device, const char *if_f2fs, const char *if_ext4) {
    short f2fs = is_f2fs(device);
    if (f2fs == 1) write(fd, if_f2fs, strlen(if_f2fs));
    if (f2fs == 0) write(fd, if_ext4, strlen(if_ext4));
}

void write_fstab() {
    WRITE_STATIC(fstab_original_a);
    WRITE_DYNAMIC(BLOCK_DATA, fstab_data_f2fs, fstab_data_ext4);
    WRITE_STATIC(fstab_original_b);
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
