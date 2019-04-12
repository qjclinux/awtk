/**
 * File:   main.c
 * Author: AWTK Develop Team
 * Brief:  bitmap font generator
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2018-01-21 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "common/utils.h"
#include "base/image_manager.h"
#include "image_loader/image_loader_stb.h"

int main(int argc, char** argv) {
  bitmap_t image;
  uint32_t size = 0;
  uint8_t* buff = NULL;
  const char* in_filename = NULL;

  TKMEM_INIT(4 * 1024 * 1024);

  if (argc < 2) {
    printf("Usage: %s filename \n", argv[0]);

    return 0;
  }

  in_filename = argv[1];
  buff = (uint8_t*)read_file(in_filename, &size);
  if (buff != NULL) {
    if (stb_load_image(0, buff, size, &image, FALSE, FALSE) == RET_OK) {
      bitmap_dump(&image);
    } else {
      printf("load %s failed\n", in_filename);
    }
    TKMEM_FREE(buff);
  }

  return 0;
}
