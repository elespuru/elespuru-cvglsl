#
# Peter R. Elespuru
#
# This makefile _should_ call another one specific to your currently
# executing platform, and supports Linux and OS X (Darwin) at the moment.
#
# IMPORTANT: If this automatic feature doesn't work, make -f Makefile.<platform>,
# where platform is 'Linux' or 'Darwin' can also be run manually to build on those
# platforms explicitly.
#
APPROOT=.

#
##  Trigger the correct makefile for the current platform
#
all:
	$(MAKE) -f $(APPROOT)/Makefile.`uname`;

#
###  Clean up
#
clean:
	rm -f cvglsl
