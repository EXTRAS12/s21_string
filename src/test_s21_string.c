#include <check.h>

#include "s21_string.h"

Suite *s21_suite(void);

START_TEST(test_s21_memchr) {
  const char *test_string = "Good job!";

  ck_assert_ptr_eq(s21_memchr(test_string, 'j', 9),
                   memchr(test_string, 'j', 9));
  ck_assert_ptr_eq(s21_memchr(test_string, 'w', 9),
                   memchr(test_string, 'w', 9));
  ck_assert_ptr_eq(s21_memchr(test_string, '!', 9),
                   memchr(test_string, '!', 9));
  ck_assert_ptr_eq(s21_memchr("", 'a', 0), memchr("", 'a', 0));
  ck_assert_ptr_eq(s21_memchr(test_string, ' ', 5),
                   memchr(test_string, ' ', 5));
  ck_assert_ptr_eq(s21_memchr(test_string, 'j', 0),
                   memchr(test_string, 'j', 0));
  ck_assert_ptr_eq(s21_memchr(test_string, 'G', 9),
                   memchr(test_string, 'G', 9));
  ck_assert_ptr_eq(s21_memchr(test_string, 'o', 1),
                   memchr(test_string, 'o', 1));
  ck_assert_ptr_eq(s21_memchr(test_string, 'j', 4),
                   memchr(test_string, 'j', 4));
  ck_assert_ptr_eq(s21_memchr(test_string, '\0', 10),
                   memchr(test_string, '\0', 10));
  ck_assert_ptr_eq(s21_memchr(test_string, 'o', 9),
                   memchr(test_string, 'o', 9));
  ck_assert_ptr_eq(s21_memchr("A", 'A', 1), memchr("A", 'A', 1));
  ck_assert_ptr_eq(s21_memchr("A", 'B', 1), memchr("A", 'B', 1));
  ck_assert_ptr_eq(s21_memchr("aaaaa", 'a', 5), memchr("aaaaa", 'a', 5));
  ck_assert_ptr_eq(s21_memchr("abcde", 'e', 5), memchr("abcde", 'e', 5));
}
END_TEST

START_TEST(test_s21_memcmp) {
  const char *str1 = "Good job!";
  const char *str2 = "Good pob!!";
  const char *str3 = "Good bob!!";

  ck_assert_int_eq((s21_memcmp(str1, str2, 9) > 0),
                   (memcmp(str1, str2, 9) > 0));
  ck_assert_int_eq((s21_memcmp(str1, str3, 9) > 0),
                   (memcmp(str1, str3, 9) > 0));
  ck_assert_int_eq((s21_memcmp(str1, str3, 10) > 0),
                   memcmp(str1, str3, 10) > 0);
  ck_assert_int_eq((s21_memcmp(str1, str2, 0) > 0),
                   (memcmp(str1, str2, 0) > 0));
  ck_assert_int_eq((s21_memcmp(str1, str3, 0) > 0),
                   (memcmp(str1, str3, 0) > 0));

  ck_assert_int_eq((s21_memcmp(str1, str2, 4) > 0),
                   (memcmp(str1, str2, 4) > 0));
  ck_assert_int_eq((s21_memcmp(str1, str2, 6) > 0),
                   (memcmp(str1, str2, 6) > 0));
  ck_assert_int_eq((s21_memcmp(str1, "", 0) > 0), (memcmp(str1, "", 0) > 0));
  ck_assert_int_eq((s21_memcmp("", str2, 0) > 0), (memcmp("", str2, 0) > 0));
}
END_TEST

START_TEST(test_s21_memcpy) {
  const char src[] = "Good job!";
  char dest1[20] = "";
  char dest2[20] = "";
  char dest3[20] = "";
  char dest4[20] = "";
  const char src5[] = "Hello";
  char dest5[10] = "123456789";
  char std_dest5[10] = "123456789";
  const char src6[] = "abcdef";
  char dest6[10] = "XXXXXXXXX";
  char std_dest6[10] = "XXXXXXXXX";
  const char src7[] = {'a', '\0', 'b', 'c', 0};
  char dest7[5] = "zzzzz";
  char std_dest7[5] = "zzzzz";
  const char src8[] = "abc";
  char dest8[10] = {0};
  char std_dest8[10] = {0};

  s21_memcpy(dest8, src8, 3);
  memcpy(std_dest8, src8, 3);
  ck_assert_mem_eq(dest8, std_dest8, 10);

  s21_memcpy(dest7, src7, 4);
  memcpy(std_dest7, src7, 4);
  ck_assert_mem_eq(dest7, std_dest7, 5);

  s21_memcpy(dest6, src6, 3);
  memcpy(std_dest6, src6, 3);
  ck_assert_mem_eq(dest6, std_dest6, 10);

  s21_memcpy(dest5, src5, 0);
  memcpy(std_dest5, src5, 0);
  ck_assert_str_eq(dest5, std_dest5);

  s21_memcpy(dest1, src, s21_strlen(src) + 1);
  memcpy(dest2, src, s21_strlen(src) + 1);
  s21_memcpy(dest3, src, 7);
  memcpy(dest4, src, 7);

  ck_assert_str_eq(dest1, dest2);
  ck_assert_str_eq(dest3, dest4);
}
END_TEST

START_TEST(test_s21_strlen) {
  const char *str1 = "Good job!";
  const char *str2 = "Good job!!";
  const char *str3 = "Good boby!!";
  const char str4[] = {'a', 'b', '\0', 'c', 0};

  ck_assert_int_eq(s21_strlen(str1), strlen(str1));
  ck_assert_int_eq(s21_strlen(str2), strlen(str2));
  ck_assert_int_eq(s21_strlen(str3), strlen(str3));
  ck_assert_int_eq(s21_strlen(""), strlen(""));
  ck_assert_int_eq(s21_strlen("A"), strlen("A"));
  ck_assert_int_eq(s21_strlen("   "), strlen("   "));

  ck_assert_int_eq(s21_strlen(str4), strlen(str4));
  ck_assert_int_eq(s21_strlen("Привет"), strlen("Привет"));
  const char only_null[] = {'\0'};
  ck_assert_int_eq(s21_strlen(only_null), strlen(only_null));
  ck_assert_int_eq(s21_strlen("abc\n\t\r"), strlen("abc\n\t\r"));
  const char zeros[10] = {0};
  ck_assert_int_eq(s21_strlen(zeros), strlen(zeros));
}
END_TEST

START_TEST(test_s21_memset) {
  char str1[20] = "Good job!";
  char str2[20] = "Good job!";
  char str3[20] = "Good job!";
  char str4[20] = "Good job!";
  int c = 'b';
  int end = '\0';
  char dest[] = "\0";
  char s21_dest[] = "\0";
  char char_to_set = 'a';

  s21_memset(str1, c, 15);
  memset(str2, c, 15);
  s21_memset(str3, end, s21_strlen(str3));
  memset(str4, end, s21_strlen(str4));

  ck_assert_str_eq(str1, str2);
  ck_assert_str_eq(str3, str4);

  ck_assert_str_eq(memset(dest, char_to_set, 1),
                   s21_memset(s21_dest, char_to_set, 1));
}
END_TEST

START_TEST(test_s21_strncat) {
  char str1[20] = "Good job!";
  char str2[20] = "Bob!";
  char str3[20] = "Good job!";
  char str4[20] = "";
  char str5[20] = "";

  s21_strncat(str1, str2, 4);
  strncat(str3, str2, 4);

  s21_strncat(str4, str2, 0);
  strncat(str5, str2, 0);

  ck_assert_str_eq(str1, str3);
  ck_assert_str_eq(str4, str5);

  char dest2[20] = "Hi, ";
  char std2[20] = "Hi, ";
  s21_strncat(dest2, "world!", 3);
  strncat(std2, "world!", 3);
  ck_assert_str_eq(dest2, std2);
  char dest3[10] = {'a', 'b', '\0', 'x', 'x', 'x', 'x', 'x', 'x', 0};
  char std3[10] = {'a', 'b', '\0', 'x', 'x', 'x', 'x', 'x', 'x', 0};

  ck_assert_mem_eq(dest3, std3, 10);
  char dest4[20] = "A";
  char std4[20] = "A";
  s21_strncat(dest4, "BCDE", 1);
  strncat(std4, "BCDE", 1);
  ck_assert_str_eq(dest4, std4);
  char dest5[20] = "abc";
  ck_assert_ptr_eq(s21_strncat(dest5, "def", 2), dest5);
}
END_TEST

START_TEST(test_s21_strchr) {
  const char str[50] = "543643164361364";
  const char str1[50] = "12424521525256";
  const char str2[50] = "Good Job!";
  const char str3[80] = "";

  ck_assert_str_eq(strchr(str, '1'), s21_strchr(str, '1'));
  ck_assert_str_eq(strchr(str1, '2'), s21_strchr(str1, '2'));
  ck_assert_ptr_eq(strchr(str2, 'z'), s21_strchr(str2, 'z'));
  ck_assert_str_eq(strchr(str2, ' '), s21_strchr(str2, ' '));
  ck_assert_ptr_eq(strchr(str3, '1'), s21_strchr(str3, '1'));
  ck_assert_str_eq(strchr(str, '\0'), s21_strchr(str, '\0'));
}
END_TEST

START_TEST(test_s21_strncmp) {
  char str1[50] = "Good Job!";
  char str2[50] = "Good Job!";
  char str3[50] = "10";
  char str4[50] = "10";
  char str5[50] = "";
  char str6[50] = "5";
  char str7[50] = "Good boby";

  ck_assert_int_eq(strncmp(str1, str2, s21_strlen(str1)),
                   s21_strncmp(str1, str2, s21_strlen(str1)));
  ck_assert_int_eq(strncmp(str3, str4, s21_strlen(str3)),
                   s21_strncmp(str3, str4, strlen(str3)));
  ck_assert_int_eq(strncmp(str5, str6, s21_strlen(str5)),
                   s21_strncmp(str5, str6, strlen(str5)));
  ck_assert_int_eq(strncmp(str1, str7, s21_strlen(str1)),
                   s21_strncmp(str1, str7, s21_strlen(str1)));
}
END_TEST

START_TEST(test_s21_strncpy) {
  char str1[50] = "Good Job!";
  char str2[50] = "1234567890";
  char str3[50] = "1";
  char str4[50] = "";
  char str5[50] = " GOOD ";

  ck_assert_str_eq(strncpy(str1, str2, 3), s21_strncpy(str1, str2, 3));
  ck_assert_str_eq(strncpy(str1, str2, 5), s21_strncpy(str1, str2, 5));
  ck_assert_str_eq(strncpy(str1, str3, 1), s21_strncpy(str1, str3, 1));
  ck_assert_str_eq(s21_strncpy(str4, str5, 5), strncpy(str4, str5, 5));
}
END_TEST

START_TEST(test_s21_strcspn) {
  char str1[50] = "Good Job!";
  char str2[50] = "12345678988";
  char str3[50] = "1G234567o890";
  char str4[50] = "";
  char str5[50] = " ";

  ck_assert_int_eq(strcspn(str1, str2), s21_strcspn(str1, str2));
  ck_assert_int_eq(strcspn(str3, str1), s21_strcspn(str3, str1));
  ck_assert_int_eq(strcspn(str4, str1), s21_strcspn(str4, str1));
  ck_assert_int_eq(strcspn(str5, str1), s21_strcspn(str5, str1));
  ck_assert_int_eq(strcspn(str1, str5), s21_strcspn(str1, str5));
}
END_TEST

START_TEST(test_s21_strpbrk) {
  char str1[50] = "1234567890";
  char str2[50] = "69";
  char str3[50] = "96";
  char str4[50] = "goodJoob";
  char str5[50] = "";
  char str6[50] = "";

  ck_assert_str_eq(strpbrk(str1, str2), s21_strpbrk(str1, str2));
  ck_assert_ptr_eq(strpbrk(str1, str3), s21_strpbrk(str1, str3));
  ck_assert_ptr_eq(strpbrk(str1, str4), s21_strpbrk(str1, str4));
  ck_assert_ptr_eq(strpbrk(str5, str1), s21_strpbrk(str5, str1));
  ck_assert_ptr_eq(strpbrk(str5, str6), s21_strpbrk(str5, str6));
}
END_TEST

START_TEST(test_s21_strrchr) {
  char str[50] = "123453678390";
  char str1[50] = "";
  char str2[50] = "Good Joob";

  ck_assert_ptr_eq(strrchr(str, '3'), s21_strrchr(str, '3'));
  ck_assert_str_eq(strrchr(str, '0'), s21_strrchr(str, '0'));
  ck_assert_ptr_eq(strrchr(str1, '0'), s21_strrchr(str1, '0'));
  ck_assert_ptr_eq(strrchr(str1, '1'), s21_strrchr(str1, '1'));
  ck_assert_ptr_eq(strrchr(str2, 'g'), s21_strrchr(str2, 'g'));
  ck_assert_ptr_eq(strrchr(str2, 'J'), s21_strrchr(str2, 'J'));
  ck_assert_ptr_eq(strrchr(str2, ' '), s21_strrchr(str2, ' '));
}
END_TEST

START_TEST(test_s21_strstr) {
  char str1[50] = "1234567890";
  char str2[50] = "0";
  char str3[50] = "";
  char str4[50] = " ";
  char str5[50] = "123456";
  char str6[50] = "Good J\0b";
  char str7[50] = "Good Job have fun";

  ck_assert_str_eq(strstr(str1, str2), s21_strstr(str1, str2));
  ck_assert_str_eq(strstr(str1, str3), s21_strstr(str1, str3));
  ck_assert_ptr_eq(strstr(str1, str4), s21_strstr(str1, str4));
  ck_assert_ptr_eq(strstr(str3, str4), s21_strstr(str3, str4));
  ck_assert_ptr_eq(strstr(str2, str4), s21_strstr(str2, str4));
  ck_assert_ptr_eq(strstr(str4, str2), s21_strstr(str4, str2));
  ck_assert_str_eq(strstr(str1, str5), s21_strstr(str1, str5));
  ck_assert_str_eq(strstr(str7, str6), s21_strstr(str7, str6));
  ck_assert_ptr_eq(strstr(str6, str7), s21_strstr(str6, str7));
}
END_TEST

START_TEST(test_s21_strtok) {
  char str1[] = "Good job";
  char str2[] = "Good job";
  const char *delim = "";
  ck_assert_pstr_eq(s21_strtok(str1, delim), strtok(str2, delim));
  for (int i = 0; i < 20; ++i) {
    ck_assert_pstr_eq(s21_strtok(NULL, delim), strtok(NULL, delim));
  }

  char str3[] = "Good job Good job Good job";
  char str4[] = "Good job Good job Good job";
  const char *delim1 = "Good job";
  ck_assert_pstr_eq(s21_strtok(str3, delim1), strtok(str4, delim1));
  for (int i = 0; i < 20; ++i) {
    ck_assert_pstr_eq(s21_strtok(NULL, delim1), strtok(NULL, delim1));
  }

  char str5[] = "Good./job./,Good job,./Good.,/job";
  char str6[] = "Good./job./,Good job,./Good.,/job";
  const char *delim2 = "./,";
  ck_assert_pstr_eq(s21_strtok(str5, delim2), strtok(str6, delim2));
  for (int i = 0; i < 50; i++) {
    ck_assert_pstr_eq(s21_strtok(NULL, delim2), strtok(NULL, delim2));
  }

  char str7[] = "Good job Good job Good job";
  char str8[] = "Good job Good job Good job";
  const char *delim3 = " ";
  ck_assert_pstr_eq(s21_strtok(str7, delim3), strtok(str8, delim3));
  for (int i = 0; i < 20; ++i) {
    ck_assert_pstr_eq(s21_strtok(NULL, delim3), strtok(NULL, delim3));
  }

  char str9[] = "";
  char str10[] = "";
  const char *delim4 = "";
  ck_assert_pstr_eq(s21_strtok(str9, delim4), strtok(str10, delim4));
  for (int i = 0; i < 20; ++i) {
    ck_assert_pstr_eq(s21_strtok(NULL, delim4), strtok(NULL, delim4));
  }

  char *str11 = S21_NULL;
  char *str12 = S21_NULL;
  const char *delim5 = S21_NULL;
  ck_assert_pstr_eq(s21_strtok(str11, delim5), strtok(str12, delim5));
  for (int i = 0; i < 20; ++i) {
    ck_assert_pstr_eq(s21_strtok(NULL, delim5), strtok(NULL, delim5));
  }
}
END_TEST

START_TEST(test_s21_strerror) {
  for (int i = 0; i < 107; i++) {
    ck_assert_str_eq(s21_strerror(i), strerror(i));
  }
  for (int i = 0; i < 133; i++) {
    ck_assert_str_eq(s21_strerror(i), strerror(i));
  }
  for (int i = 134; i < 200; i++) {
    ck_assert_str_eq(s21_strerror(i), strerror(i));
  }
}
END_TEST

START_TEST(test_s21_to_upper) {
  char regular_str[] = "Hello my dear friend";
  char empty_str[] = "";
  char mixed_case_str[] = "HELLO my Dear Friend";
  char special_chars_str[] = "!@#$^&*()";

  char regular_res_str[] = "HELLO MY DEAR FRIEND";
  char empty_res_str[] = "";
  char mixed_case_res_str[] = "HELLO MY DEAR FRIEND";
  char special_chars_res_str[] = "!@#$^&*()";

  char *regular_conv_str = s21_to_upper(regular_str);
  char *empty_conv_str = s21_to_upper(empty_str);
  char *mixed_case_conv_str = s21_to_upper(mixed_case_str);
  char *special_chars_conv_str = s21_to_upper(special_chars_str);

  ck_assert_str_eq(regular_conv_str, regular_res_str);
  ck_assert_str_eq(empty_conv_str, empty_res_str);
  ck_assert_str_eq(mixed_case_conv_str, mixed_case_res_str);
  ck_assert_str_eq(special_chars_conv_str, special_chars_res_str);

  free(regular_conv_str);
  free(empty_conv_str);
  free(mixed_case_conv_str);
  free(special_chars_conv_str);
}
END_TEST

START_TEST(test_s21_to_lower) {
  char regular_str[] = "Hello my dear friend";
  char empty_str[] = "";
  char mixed_case_str[] = "HELLO my Dear Friend";
  char special_chars_str[] = "!@#$^&*()";

  char regular_res_str[] = "hello my dear friend";
  char empty_res_str[] = "";
  char mixed_case_res_str[] = "hello my dear friend";
  char special_chars_res_str[] = "!@#$^&*()";

  char *regular_conv_str = s21_to_lower(regular_str);
  char *empty_conv_str = s21_to_lower(empty_str);
  char *mixed_case_conv_str = s21_to_lower(mixed_case_str);
  char *special_chars_conv_str = s21_to_lower(special_chars_str);

  ck_assert_str_eq(regular_conv_str, regular_res_str);
  ck_assert_str_eq(empty_conv_str, empty_res_str);
  ck_assert_str_eq(mixed_case_conv_str, mixed_case_res_str);
  ck_assert_str_eq(special_chars_conv_str, special_chars_res_str);

  free(regular_conv_str);
  free(empty_conv_str);
  free(mixed_case_conv_str);
  free(special_chars_conv_str);
}
END_TEST

START_TEST(test_s21_insert) {
  char regular_str[] = "Hello  friend";
  char mixed_case_str[] = "HELLO my Dear Friend";
  char special_chars_str[] = "!@#$^&*()";

  char insert_regular_str[] = "my dear";
  char insert_text[] = "I am there! ";
  char insert_empty_str[] = "";
  char insert_special_chars_str[] = "!@#$^&*()";

  char desired_res_insert_regular_str[] = "Hello my dear friend";
  char desired_res_regular_str[] = "I am there! HELLO my Dear Friend";
  char desired_res_test_insert_empty_str[] = "HELLO my Dear Friend";
  char desired_res_two_special_chars_str[] = "!@#$^&*()!@#$^&*()";

  char *res_insert_regular_str = s21_insert(regular_str, insert_regular_str, 6);
  char *res_insert_mixed_case_str = s21_insert(mixed_case_str, insert_text, 0);
  char *res_insert_empty_str = s21_insert(mixed_case_str, insert_empty_str, 0);
  char *res_insert_two_special_chars_str =
      s21_insert(special_chars_str, insert_special_chars_str,
                 strlen(insert_special_chars_str));

  ck_assert_str_eq(desired_res_insert_regular_str, res_insert_regular_str);
  ck_assert_str_eq(desired_res_regular_str, res_insert_mixed_case_str);
  ck_assert_str_eq(desired_res_test_insert_empty_str, res_insert_empty_str);
  ck_assert_str_eq(desired_res_two_special_chars_str,
                   res_insert_two_special_chars_str);

  free(res_insert_regular_str);
  free(res_insert_mixed_case_str);
  free(res_insert_empty_str);
  free(res_insert_two_special_chars_str);
}
END_TEST

START_TEST(test_s21_trim) {
  char regular_str[] = "!!!^Hello!my!dear!friend^!!!";
  char regular_str_trim_left_side[] = "!!!^^!^Hello!my!dear!friend";
  char regular_str_trim_right_side[] = "Hello!my!dear!friend!^!^!^!!^";
  char full_trim_chars_string[] = "####********#####";
  char special_char_str[] = "!@#$^&*()";
  char non_printing_char_str[] = "\tStrings forever!   \t\n";

  char desired_res_regular_string[] = "Hello!my!dear!friend";
  char desired_res_full_trim_chars_string[] = "";
  char desired_res_empty_deletion[] = "!@#$^&*()";
  char desired_res_trim_non_printing_char[] = "Strings forever!";

  char *res_trim_regular_string = s21_trim(regular_str, "!^");
  char *res_trim_left_side_regular_string =
      s21_trim(regular_str_trim_left_side, "!^");
  char *res_trim_right_side_regular_string =
      s21_trim(regular_str_trim_right_side, "!^");
  char *res_full_trim_chars_string = s21_trim(full_trim_chars_string, "#*");
  char *res_empty_string_deletion = s21_trim(special_char_str, "");
  char *res_trim_all_characters = s21_trim(special_char_str, special_char_str);
  char *res_trim_non_printing_characters =
      s21_trim(non_printing_char_str, "\t \n");

  ck_assert_str_eq(res_trim_regular_string, desired_res_regular_string);
  ck_assert_str_eq(res_trim_left_side_regular_string,
                   desired_res_regular_string);
  ck_assert_str_eq(res_trim_right_side_regular_string,
                   desired_res_regular_string);
  ck_assert_str_eq(res_full_trim_chars_string,
                   desired_res_full_trim_chars_string);
  ck_assert_str_eq(res_empty_string_deletion, desired_res_empty_deletion);
  ck_assert_str_eq(res_trim_all_characters, desired_res_full_trim_chars_string);
  ck_assert_str_eq(res_trim_non_printing_characters,
                   desired_res_trim_non_printing_char);

  free(res_trim_regular_string);
  free(res_trim_left_side_regular_string);
  free(res_trim_right_side_regular_string);
  free(res_full_trim_chars_string);
  free(res_empty_string_deletion);
  free(res_trim_all_characters);
  free(res_trim_non_printing_characters);
}
END_TEST

Suite *s21_suite(void) {
  Suite *s = suite_create("String");
  TCase *core = tcase_create("Core");

  tcase_add_test(core, test_s21_memchr);
  tcase_add_test(core, test_s21_memcmp);
  tcase_add_test(core, test_s21_memcpy);
  tcase_add_test(core, test_s21_strlen);
  tcase_add_test(core, test_s21_memset);
  tcase_add_test(core, test_s21_strncat);
  tcase_add_test(core, test_s21_strchr);
  tcase_add_test(core, test_s21_strncmp);
  tcase_add_test(core, test_s21_strncpy);
  tcase_add_test(core, test_s21_strcspn);
  tcase_add_test(core, test_s21_strpbrk);
  tcase_add_test(core, test_s21_strrchr);
  tcase_add_test(core, test_s21_strstr);
  tcase_add_test(core, test_s21_strtok);
  tcase_add_test(core, test_s21_strerror);
  tcase_add_test(core, test_s21_to_upper);
  tcase_add_test(core, test_s21_to_lower);
  tcase_add_test(core, test_s21_insert);
  tcase_add_test(core, test_s21_trim);

  suite_add_tcase(s, core);

  return s;
}
