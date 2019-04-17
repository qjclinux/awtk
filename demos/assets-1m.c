#include "awtk.h"
#include "base/assets_manager.h"

#include "assets/inc/styles/default.data"
#include "assets/inc/images/ball.res"
#include "assets/inc/ui/svg_image.data"
#include "assets/inc/images/progress_circle.res"
#include "assets/inc/images/pointer_4.bsvg"
#include "assets/inc/images/china.bsvg"
#include "assets/inc/images/pointer_1.bsvg"
#include "assets/inc/images/pointer.bsvg"
#include "assets/inc/images/girl.bsvg"

ret_t assets_init(void) {
  assets_manager_t* rm = assets_manager();

  assets_manager_add(rm, style_default);
  assets_manager_add(rm, image_ball);
  assets_manager_add(rm, ui_svg_image);
  assets_manager_add(rm, image_progress_circle);
  
  assets_manager_add(rm, image_pointer_4);
  assets_manager_add(rm, image_china);
  assets_manager_add(rm, image_pointer_1);
  assets_manager_add(rm, image_pointer);
  assets_manager_add(rm, image_girl);

  tk_init_assets();

  return RET_OK;
}
