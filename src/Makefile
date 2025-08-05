CC = gcc
CFLAGS = -std=c11 -Wall -Werror -Wextra

ifeq ($(shell uname),Linux)
	CF=  -lm
	TEST_LIBS = -lcheck -lsubunit -lm
else
	TEST_LIBS = -lcheck
	CF =
endif
GCOV_FLAGS = --coverage

LIB_SRCS = s21_string.c s21_sscanf.c s21_sprintf.c
LIB_OBJ = $(LIB_SRCS:.c=.o)

TEST_SRCS = test_main.c test_s21_string.c test_s21_sscanf.c test_s21_sprintf.c 
TEST_OBJ = $(TEST_SRCS:.c=.o)

TARGET = s21_tests



all: s21_string.a

s21_string.a: $(LIB_OBJ)
	ar rcs $@ $^

$(TARGET): $(TEST_OBJ) s21_string.a
	$(CC) $(CFLAGS) $^ -o $@ $(TEST_LIBS) 

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(CF)

test: $(TARGET)
	./$(TARGET)

gcov_report: 
	$(CC) $(CFLAGS) $(GCOV_FLAGS) s21_string.c s21_sscanf.c s21_sprintf.c test_s21_string.c test_s21_sscanf.c test_s21_sprintf.c test_main.c -o s21_tests $(TEST_LIBS)
	./s21_tests
	lcov -t "$(TARGET)" -c -d . --no-external -o report.info
	genhtml -o html_report report.info

check_code:
	clang-format -n *.c *.h
	cppcheck --enable=all --check-level=exhaustive --suppress=missingIncludeSystem *.c *.h

clean:
	rm -rf *.o *.a $(TARGET) *.gc* *.info html_report

rebuild: clean all
