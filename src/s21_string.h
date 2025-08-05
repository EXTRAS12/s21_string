#ifndef SRC_S21_STRING_H_
#define SRC_S21_STRING_H_
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <wctype.h>

#define MAX_WIDTH 4095
#define S21_NULL (void *)0
typedef long unsigned int s21_size_t;
#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define FINDBRACKET(frm, i) while (++i && frm[i] && frm[i] != ']');

#define ANYNUM(S, base, num, width) \
  while (conv(S) < (base) && width--) (num) *= (base), (num) += conv(S), (S)++;
// Numerical shift using ASCII to represent ANY number with base up to 36
// 87 = 'a' -10 (shifts 'a' to 10 etc.), 55 ='A'-10 (shifts 'C' to 12 etc.)
// 0x7F the highest number in ASCII without locale - error code
#define conv(A)                    \
  (u)(*(A) - (isdigit(*(A))   ? 48 \
              : isupper(*(A)) ? 55 \
              : islower(*(A)) ? 87 \
                              : -0x7f))

#define SKIP(P) \
  while (isspace(*(P))) (P)++

#define LENGTH(P) \
  ((*P == 'L' || *P == 'l' || *P == 'h' || *P == '*') ? *P++ : 0)

#define SIGN(P) ((*P == '-' || *P == '+') ? *P++ == '-' : 0)
// Assign integer or double to variable pointer from elipsis using proper length
// modifier from form
#define ASINT(FORM, LVAL, AP)         \
  if (*str) {                         \
    if ((FORM)->length == 'l')        \
      *va_arg(*(AP), l *) = (LVAL);   \
    else if ((FORM)->length == 'h')   \
      *va_arg(*(AP), h *) = (LVAL);   \
    else if (!(FORM)->length)         \
      *va_arg(*(AP), int *) = (LVAL); \
  }

#define ASDOUBLE(FORM, STR, AP)                         \
  if ((FORM).length == 'L')                             \
    *va_arg((AP), Lf *) = strtold1(&(STR), FORM.width), \
                     FORM.assigned += str != S21_NULL;  \
  else if (!(FORM).length || (FORM).length == 'l')      \
    *va_arg((AP), f *) = strtod1(&(STR), FORM.width),   \
                    FORM.assigned += str != S21_NULL;   \
  else                                                  \
    strtod1(&(STR), FORM.width);

typedef long l;
typedef short h;
typedef unsigned u;
typedef long double Lf;
typedef unsigned long lu;
typedef unsigned short hu;
typedef double f;
typedef char *s;
typedef struct {
  int exc;
  unsigned width;
  char length;
  char spec;
  char set[257];
  unsigned count;
  unsigned assigned;
} fstr;

int s21_sscanf(const char *str, const char *frm, ...);
int s21_sprintf(char *str, const char *frm, ...);
lu strtoul1(const char **number, u base, u width);
l strtol1(const char **number, u base, u width);
f strtod1(const char **num, u width);
Lf strtold1(const char **num, u width);
s21_size_t s21_strspn(const char *str1, const char *str2);

int _sscanf(const char *str, const char *frm, ...);
int parse_format(const char *frm, fstr *f);
int process_form(fstr *form, const char **str, va_list *ap);
int apply_spec(char **str, fstr f, va_list *ap);

void *s21_memchr(const void *str, int c, s21_size_t n);
int s21_memcmp(const void *str1, const void *str2, s21_size_t n);
void *s21_memcpy(void *dest, const void *src, s21_size_t n);
s21_size_t s21_strlen(const char *str);
void *s21_memset(void *str, int c, s21_size_t n);
char *s21_strncat(char *dest, const char *src, s21_size_t n);
char *s21_strchr(const char *str, int c);
int s21_strncmp(const char *str1, const char *str2, size_t n);
char *s21_strncpy(char *dest, const char *src, s21_size_t n);
s21_size_t s21_strcspn(const char *str1, const char *str2);
char *s21_strpbrk(const char *str1, const char *str2);
char *s21_strrchr(const char *str, int c);
char *s21_strstr(const char *haystack, const char *needle);
char *s21_strtok(char *str, const char *delim);
char *s21_strerror(int errnum);
wchar_t *s21_wcsncpy(wchar_t *dest, const wchar_t *src, s21_size_t n);
s21_size_t s21_wstrlen(const wchar_t *str);

void *s21_insert(const char *src, const char *str, size_t start_index);
void *s21_to_lower(const char *str);
void *s21_to_upper(const char *str);
void *s21_trim(const char *src, const char *trim_chars);

#endif
