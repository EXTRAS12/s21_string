#include <check.h>
#include <stdlib.h>

Suite *s21_suite(void);
Suite *test_s21_sscanf(void);
Suite *test_s21_printf(void);

int main() {
  SRunner *sr = srunner_create(NULL);

  srunner_add_suite(sr, s21_suite());
  srunner_add_suite(sr, test_s21_sscanf());
  srunner_add_suite(sr, test_s21_printf());

  srunner_run_all(sr, CK_NORMAL);
  int failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}