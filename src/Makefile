.PHONY: all clean build format test gcov_report

CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
COMLFAGS=-Wall -Werror -Wextra -std=c11 -pedantic 
TFLAGS=-o testing -lcheck --coverage 
MODEL_FILES = back/viewer_back.c back/affine.c

all:
	$(MAKE) clean
	$(MAKE) uninstall
	$(MAKE) install

build:
	$(CC) $(COMLFAGS) -c back/*.c
	ar -rc 3DViewer_lib.a *.o

install:
	@echo "Installing 3DViewer..."
	@cd front && qmake && make && make clean && rm Makefile 
	@cp -R front/3DViewer.app ~/Desktop/3dViewerV1.app
	@echo "The installation was successful"
	@echo "The project is located in the"
	@echo "release folder and"
	@echo "on the desktop"

	

uninstall:
	@echo "Removing 3DViewer..."
	@rm -rf ~/Desktop/3dViewerV1.app front/.qmake.stash
	@echo "Uninstall completed successfully"

dvi:
	@open documentation.md

dist:
	 tar -cf 3DViewer.tar front

test: build
	checkmk clean_mode=1  tests/test.check > testing.c
	$(CC) *.c 3DViewer_lib.a $(TFLAGS)
	./testing


gcov_report:
	@echo "Generating your html report ..."
	@gcov -b *.c 
	@lcov -d . -c -o coverage.info
	@genhtml -o html_report coverage.info
	@open html_report/index.html
format:
	@clang-format -style=Google -i back/*.c back/*.h front/*.cpp front/*.h

format_check:
	clang-format -style=Google -n back/*.c back/*.h front/*.cpp front/*.h

clean:
	@rm -rf *.o *.a report *.gcno *.gcda *.info *.tar *.html test gcovreport
	@rm -rf *.gcov *.gcov_report html_report
	@rm -rf testing testing.c
	@rm -f testing/testing.c