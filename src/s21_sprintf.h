#ifndef __S21_SPRINTF__
#define __S21_SPRINTF__

#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "s21_string.h"

#define ACCSES_FLAGS "-+ #0"
#define ACCSES_LENGTH "hlL"
#define ACCSES_SPECIFICATOR "diouxXfFeEgGcspn"
#define MAX_LEN_ST11 4095

typedef struct {
  bool end_width;
  bool end_precision;
  bool end_length;
  bool end_format;
  bool error_format;
  bool fl_minus;
  bool fl_plus;
  bool fl_space;
  bool fl_sharp;
  bool fl_zero;
  int width;
  int precision;
  bool fl_h;
  bool fl_l;
  bool fl_L;
} ActiveOpts;

int s21_sprintf(char* str, const char* format, ...);
const char* get_format(ActiveOpts* state, va_list* args, const char* format);
const char* set_flags(ActiveOpts* state, const char* format);
const char* set_width(ActiveOpts* state, va_list* args, const char* format);
const char* set_precision(ActiveOpts* state, va_list* args, const char* format);
const char* set_length(ActiveOpts* state, const char* format);
char* apply_format_for_arg(char* cur_buf, va_list* args,
                           const ActiveOpts* state, const char* start_buf,
                           char specificator);
char* apply_d_i(char* cur_buf, va_list* args, const ActiveOpts* state);
char* apply_o_u_x_X(char* cur_buf, va_list* args, const ActiveOpts* state,
                    char spe);
char* apply_f_F_e_E_g_G(char* cur_buf, va_list* args, const ActiveOpts* state,
                        char spe);
char* apply_s(char* cur_buf, va_list* args, const ActiveOpts* state);
char* apply_ls(char* cur_buf, va_list* args, const ActiveOpts* state);
char* apply_c(char* cur_buf, va_list* args, const ActiveOpts* state);
char* apply_lc(char* cur_buf, va_list* args, const ActiveOpts* state);
char* apply_p(char* cur_buf, va_list* args, const ActiveOpts* state);
char* apply_n(const char* start_buf, char* cur_buf, va_list* args);
int s21_itoa(char* buf, long value, int precision);
int s21_uitoa(char* buf, unsigned long value, int base, int precision);
int get_res_f_F(char* buf, long double value, int precision, bool fl_sharp,
                char spe);
int get_res_e_E(char* buf, int* e10, long double value, int first_precision,
                bool fl_sharp, char spe);
int make_buf_integer_part(char* buf, long double integer_part, bool isNegative);
bool make_buf_fractional_part(char* buf, int* indx, long double fractional_part,
                              int precision);
int make_round_up(char* buf, int* indx, bool round_up, int delta_indx,
                  bool value_isNegative, int e10);
int insert_chr(char* buf, int cur_indx, int insert_indx, char chr);
int process_integer_part_for_e_E(char* buf, int* cur_indx, int* e10,
                                 long double* fractional_part,
                                 long double integer_part, int first_precision,
                                 bool val_isNeg, bool fl_sharp);
int delet_0_for_g_G(char* buf, int indx);
char* do_opts_for_d_i(char* cur_buf, const ActiveOpts* state, int len,
                      const char* temp_buf);
char* do_opts_for_o_u_x_X(char* cur_buf, const ActiveOpts* state, int len,
                          const char* temp_buf, char spe);
char* do_opts_for_f_F_e_E_g_G(char* cur_buf, const ActiveOpts* state, int len,
                              const char* temp_buf);
char* do_opts_for_s_c(char* cur_buf, const char* str, int str_len,
                      const ActiveOpts* state);
char* do_opts_for_p(char* cur_buf, const ActiveOpts* state, int len,
                    const char* temp_buf);
#endif