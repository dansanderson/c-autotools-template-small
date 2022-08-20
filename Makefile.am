AM_CPPFLAGS = \
    -I$(top_srcdir) \
    -I$(top_srcdir)/src

TEST_RUNNER_CPPFLAGS = \
    -I$(top_srcdir)/third-party/CMock/vendor/unity/src \
    -I$(top_srcdir)/third-party/CMock/src \
    -I$(top_srcdir)/src \
	-I./src \
    -I./tests/mocks

CMOCK_SRCS = \
	$(top_srcdir)/third-party/CMock/src/cmock.c \
	$(top_srcdir)/third-party/CMock/src/cmock.h \
	$(top_srcdir)/third-party/CMock/src/cmock_internals.h \
	$(top_srcdir)/third-party/CMock/vendor/unity/src/unity.c \
	$(top_srcdir)/third-party/CMock/vendor/unity/src/unity.h \
	$(top_srcdir)/third-party/CMock/vendor/unity/src/unity_internals.h

bin_PROGRAMS = myapp

myapp_SOURCES = \
	$(top_srcdir)/src/myapp.c \
	$(top_srcdir)/src/cfgfile.c \
	$(top_srcdir)/src/executor.c \
	$(top_srcdir)/src/reporter.c

tests/runners/runner_test_%.c: tests/test_%.c
	@test -n "$(RUBY)" || { echo "\nPlease install Ruby to run tests.\n"; exit 1; }
	$(RUBY) $(top_srcdir)/third-party/CMock/vendor/unity/auto/generate_test_runner.rb $< $@

tests/mocks/mock_%.c tests/mocks/mock_%.h: src/%.h
	@test -n "$(RUBY)" || { echo "\nPlease install Ruby to run tests.\n"; exit 1; }
	CMOCK_DIR=$(top_srcdir)/third-party/CMock \
	MOCK_OUT=tests/mocks \
	$(RUBY) $(top_srcdir)/third-party/CMock/scripts/create_mock.rb $<

check_PROGRAMS = \
	tests/runners/test_cfgfile \
	tests/runners/test_executor \
	tests/runners/test_reporter
TESTS = $(check_PROGRAMS)

tests_runners_test_cfgfile_SOURCES = \
	tests/runners/runner_test_cfgfile.c \
	$(top_srcdir)/tests/test_cfgfile.c \
	$(top_srcdir)/src/cfgfile.c \
	$(top_srcdir)/src/cfgfile.h \
	$(CMOCK_SRCS)
tests_runners_test_cfgfile_CPPFLAGS = $(TEST_RUNNER_CPPFLAGS)

tests_runners_test_executor_SOURCES = \
	tests/mocks/mock_cfgfile.c \
	tests/mocks/mock_cfgfile.h \
	tests/runners/runner_test_executor.c \
	$(top_srcdir)/tests/test_executor.c \
	$(top_srcdir)/src/executor.c \
	$(top_srcdir)/src/executor.h \
	$(CMOCK_SRCS)
tests_runners_test_executor_CPPFLAGS = $(TEST_RUNNER_CPPFLAGS)

tests_runners_test_reporter_SOURCES = \
	tests/mocks/mock_cfgfile.c \
	tests/mocks/mock_cfgfile.h \
	tests/runners/runner_test_reporter.c \
	$(top_srcdir)/tests/test_reporter.c \
	$(top_srcdir)/src/reporter.c \
	$(top_srcdir)/src/reporter.h \
	$(CMOCK_SRCS)
tests_runners_test_reporter_CPPFLAGS = $(TEST_RUNNER_CPPFLAGS)

CLEANFILES = \
	tests/mocks/mock_cfgfile.c \
	tests/mocks/mock_cfgfile.h \
	tests/mocks/mock_executor.c \
	tests/mocks/mock_executor.h \
	tests/mocks/mock_reporter.c \
	tests/mocks/mock_reporter.h \
	tests/runners/runner_test_cfgfile.c \
	tests/runners/runner_test_executor.c \
	tests/runners/runner_test_reporter.c

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
