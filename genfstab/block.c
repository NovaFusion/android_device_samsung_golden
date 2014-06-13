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

#include "block.h"

#define F2FS_MAGIC 0xF2F52010

short is_f2fs(const char *device) {
    int fd = open(device, O_RDONLY);
    if (fd != -1) {
        unsigned magic;
        lseek(fd, 1024, SEEK_SET);
        read(fd, &magic, sizeof(unsigned));
        close(fd);

        if (magic == F2FS_MAGIC) return 1;
        return 0;
    }

    return -1;
}
