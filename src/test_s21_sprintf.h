#ifndef TEST_S21_SPRINTF_H
#define TEST_S21_SPRINTF_H

#include <check.h>
#include <float.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "s21_sprintf.h"

#define FLAGS 5
#define MAX_FLAG_REPEAT 2
#define LIMIT_COMB_FLAGS 3000
#define LIMIT_STR_FORMAT 15
#define LIMIT_COMB_FORMAT 30000
#define LIMIT_STRING 4095

typedef struct {
  char str_format[LIMIT_STR_FORMAT];
  bool fl_star_width;
  bool fl_star_precision;
  char specifier;
  bool not_d_i_u;
  bool not_o_x_X;
  bool not_f_F_e_E_g_G;
  bool not_c_p_n;
  bool not_s;
} format_test_case;

typedef format_test_case test_case_t;

typedef struct {
  test_case_t test[LIMIT_COMB_FORMAT];
  size_t count;
} format_test_cases;

typedef struct {
  char str[1 << FLAGS][FLAGS + 1];
  size_t count;
} flag_combs;

extern flag_combs comb_flags;
extern format_test_cases cases;

void get_all_flags(void);
void get_all_forms(size_t f);

int is_ub_combination(const char *flag, const char *width,
                      const char *precision, const char *length,
                      const char *specifier_str);

Suite *make_suite(void);

#endif
