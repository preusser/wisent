# Standard Header
include ../../Makefile.def

registrar := registrar.cpp
library   := w
sources   := WFrontend.cpp WBuilder.cpp WScanner.cpp WParser.cpp

include ../../Makefile.lib

# Flex Adaptation
ifneq ($(MAKECMDGOALS), clean)
ifneq ($(MAKECMDGOALS), clobber)
-include Makefile.conf
Makefile: Makefile.conf

Makefile.conf:
	@set -e; \
	echo Configuring ...; \
        if which flex >/dev/null; then \
	  echo "FLEX := $$(which flex)" > Makefile.conf; \
	else \
	  ln -s FlexLexer.0 FlexLexer.h; \
	  ln -s WScanner.cpp.0 WScanner.cpp; \
	  echo "CPPFLAGS += -I." > Makefile.conf; \
	fi

ifdef FLEX
WScanner.cpp: WScanner.lpp
	$(FLEX) $<
endif

endif
endif

# Clean Up
clobber::
	rm -f Makefile.conf FlexLexer.h WScanner.cpp
