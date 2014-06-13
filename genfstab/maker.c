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

#include "fstab.h"
#include "block.h"

#define UID_SYSTEM 1000
#define GID_SYSTEM 1000

int main() {
    int fd = open(FSTAB, O_WRONLY | O_CREAT | O_TRUNC, 0640);
    if (fd != -1) {
        write(fd, fstab_top, sizeof(fstab_top) - 1);

        short f2fs = is_f2fs(BLOCK_DATA);
        if (f2fs == 1) write(fd, fstab_f2fs, sizeof(fstab_f2fs) - 1);
        if (f2fs == 0) write(fd, fstab_ext4, sizeof(fstab_ext4) - 1);

        write(fd, fstab_bottom, sizeof(fstab_bottom) - 1);
        close(fd);

        chown(FSTAB, UID_SYSTEM, GID_SYSTEM);
    }

    return 0;
}
