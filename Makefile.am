AUTOMAKE_OPTIONS = subdir-objects

bin_PROGRAMS = http_server

http_server_SOURCES = src/main.c

clean-local:
	@rm config.status config.log configure
	@rm Makefile
	@rm -r autom4te.cache/
	@rm aclocal.m4
	@rm compile install-sh missing Makefile.in
