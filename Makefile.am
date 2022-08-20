AM_CPPFLAGS = \
    -I$(top_srcdir) \
    -I$(top_srcdir)/src \
    -I$(top_srcdir)/third-party/CMock/vendor/unity/src \
    -I$(top_srcdir)/third-party/CMock/src \
	-I$(top_builddir)/src \
    -I$(top_builddir)/tests/mocks

check_LIBRARIES = libcmock.a
libcmock_a_SOURCES = \
	$(top_srcdir)/third-party/CMock/src/cmock.c \
	$(top_srcdir)/third-party/CMock/src/cmock.h \
	$(top_srcdir)/third-party/CMock/src/cmock_internals.h \
	$(top_srcdir)/third-party/CMock/vendor/unity/src/unity.c \
	$(top_srcdir)/third-party/CMock/vendor/unity/src/unity.h \
	$(top_srcdir)/third-party/CMock/vendor/unity/src/unity_internals.h

tests/runners/runner_test_%.c: tests/test_%.c
	@test -n "$(RUBY)" || { echo "\nPlease install Ruby to run tests.\n"; exit 1; }
	mkdir -p tests/runners
	$(RUBY) $(top_srcdir)/third-party/CMock/vendor/unity/auto/generate_test_runner.rb $< $@

tests/mocks/mock_%.c tests/mocks/mock_%.h: src/%.h
	@test -n "$(RUBY)" || { echo "\nPlease install Ruby to run tests.\n"; exit 1; }
	mkdir -p tests/mocks
	CMOCK_DIR=$(top_srcdir)/third-party/CMock \
	MOCK_OUT=tests/mocks \
	$(RUBY) $(top_srcdir)/third-party/CMock/scripts/create_mock.rb $<

bin_PROGRAMS =
check_PROGRAMS =
BUILT_SOURCES =
CLEANFILES =


### myapp

bin_PROGRAMS += myapp

myapp_SOURCES = \
	$(top_srcdir)/src/myapp.c \
	$(top_srcdir)/src/cfgfile.c \
	$(top_srcdir)/src/cfgfile.h \
	$(top_srcdir)/src/executor.c \
	$(top_srcdir)/src/executor.h \
	$(top_srcdir)/src/reporter.c \
	$(top_srcdir)/src/reporter.h

### cfgfile

check_PROGRAMS += tests/runners/test_cfgfile
tests_runners_test_cfgfile_SOURCES = \
	$(top_srcdir)/tests/test_cfgfile.c \
	$(top_srcdir)/src/cfgfile.c \
	$(top_srcdir)/src/cfgfile.h
nodist_tests_runners_test_cfgfile_SOURCES = \
	tests/runners/runner_test_cfgfile.c
CLEANFILES += $(nodist_tests_runners_test_cfgfile_SOURCES)
tests_runners_test_cfgfile_LDADD = libcmock.a
tests/test_cfgfile.$(OBJEXT): \
	tests/runners/runner_test_cfgfile.c

### executor

check_PROGRAMS += tests/runners/test_executor
tests_runners_test_executor_SOURCES = \
	$(top_srcdir)/tests/test_executor.c \
	$(top_srcdir)/src/executor.c \
	$(top_srcdir)/src/executor.h
nodist_tests_runners_test_executor_SOURCES = \
	tests/runners/runner_test_executor.c \
	tests/mocks/mock_cfgfile.c \
	tests/mocks/mock_cfgfile.h
CLEANFILES += $(nodist_tests_runners_test_executor_SOURCES)
tests_runners_test_executor_LDADD = libcmock.a
tests/test_executor.$(OBJEXT): \
	tests/runners/runner_test_executor.c \
	tests/mocks/mock_cfgfile.c

### reporter

check_PROGRAMS += tests/runners/test_reporter
tests_runners_test_reporter_SOURCES = \
	$(top_srcdir)/tests/test_reporter.c \
	$(top_srcdir)/src/reporter.c \
	$(top_srcdir)/src/reporter.h
nodist_tests_runners_test_reporter_SOURCES = \
	tests/runners/runner_test_reporter.c \
	tests/mocks/mock_cfgfile.c \
	tests/mocks/mock_cfgfile.h
CLEANFILES += $(nodist_tests_runners_test_reporter_SOURCES)
tests_runners_test_reporter_LDADD = libcmock.a
tests/test_reporter.$(OBJEXT): \
	tests/runners/runner_test_reporter.c \
	tests/mocks/mock_cfgfile.c


TESTS = $(check_PROGRAMS)

EXTRA_DIST = \
    README.md \
    third-party/CMock/LICENSE.txt \
    third-party/CMock/README.md \
    third-party/CMock/config \
    third-party/CMock/lib \
    third-party/CMock/scripts \
    third-party/CMock/vendor/unity/LICENSE.txt \
    third-party/CMock/vendor/unity/README.md \
    third-party/CMock/vendor/unity/auto
