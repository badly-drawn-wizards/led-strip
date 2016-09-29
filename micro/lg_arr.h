#ifndef _LG_ARR_H_
#define _LG_ARR_H_

#define LG_ARR_PARTS 5
#define LG_ARR_TAIL_SIZE(SIZE,STEP) (SIZE/LG_ARR_PARTS/STEP*STEP)
#define LG_ARR_HEAD_SIZE(SIZE,STEP) (SIZE - (LG_ARR_PARTS-1)*LG_ARR_TAIL_SIZE(SIZE,STEP))
#define LG_ARR_DECL_HELPER(NAME, SIZE, IDX) uint8_t NAME ## _ ## IDX [SIZE];
#define LG_ARR_DECL(NAME, SIZE, STEP) \
  LG_ARR_DECL_HELPER(NAME, LG_ARR_HEAD_SIZE(SIZE,STEP), 0); \
  LG_ARR_DECL_HELPER(NAME, LG_ARR_TAIL_SIZE(SIZE,STEP), 1); \
  LG_ARR_DECL_HELPER(NAME, LG_ARR_TAIL_SIZE(SIZE,STEP), 2); \
  LG_ARR_DECL_HELPER(NAME, LG_ARR_TAIL_SIZE(SIZE,STEP), 3); \
  LG_ARR_DECL_HELPER(NAME, LG_ARR_TAIL_SIZE(SIZE,STEP), 4);

#define LG_ARR_PART_HELPER(NAME, PT, SIZE, BODY, IDX) \
  {                                                   \
    uint8_t *PT = NAME ## _ ## IDX;             \
    uint8_t SIZE = sizeof(NAME ## _ ## IDX);    \
    BODY                                              \
  }

#define LG_ARR_PART(NAME, PT, SIZE, BODY)       \
  {                                             \
    LG_ARR_PART_HELPER(NAME, PT, SIZE, BODY, 0) \
    LG_ARR_PART_HELPER(NAME, PT, SIZE, BODY, 1) \
    LG_ARR_PART_HELPER(NAME, PT, SIZE, BODY, 2) \
    LG_ARR_PART_HELPER(NAME, PT, SIZE, BODY, 3) \
    LG_ARR_PART_HELPER(NAME, PT, SIZE, BODY, 4) \
  }


#define LG_ARR_LOOP(NAME, PT, STEP, BODY)       \
  LG_ARR_PART(NAME, _arr_pt, _arr_size, {       \
      for(uint8_t i=0; i<_arr_size; i++) {      \
        uint8_t *PT = _arr_pt + i;              \
        BODY                                    \
      }                                         \
    })

#endif
