# Standard Header
include Makefile.def

library := wisent
sources := exceptions.cpp \
	   IDKeyedPtrMap.cpp Properties.cpp \
           Klass.cpp Blocks.cpp \
           BitSet.cpp BitMatrix.cpp \
           Symbol.cpp SymbolSet.cpp SymbolMatrix.cpp \
           IdentPool.cpp Terminal.cpp TermPool.cpp Variable.cpp VarPool.cpp \
           Rule.cpp RulePool.cpp ActionPool.cpp \
           Item.cpp ItemInfo.cpp Core.cpp CoreInfo.cpp Collection.cpp \
           ActionTable.cpp \
           Grammar.cpp

# Include Configuration File
-include frontend/Makefile.conf
-include backend/Makefile.conf

subdirs := $(sort $(dir $(frontend_regs) $(backend_regs) \
                        $(frontend_libs) $(backend_libs)))

.PHONY: all clean clobber
all:: wisent

%/Makefile.conf: %
	@set -e; \
	echo Collecting $<s ...; \
	./collect $<

# Make executable
wisent: wisent.o $(frontend_regs) $(backend_regs) \
                 $(frontend_libs) $(backend_libs) lib$(library).a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o$@ $+
-include wisent.dep

# Library
include Makefile.lib

# Build dependencies in subdirectories if necessary
.PHONY: FORCE
$(frontend_regs) $(backend_regs) $(frontend_libs) $(backend_libs): FORCE
	$(MAKE) -C $(dir $@) $(notdir $@)

# Recurse
.PHONY: $(subdirs)
all clean clobber:: $(subdirs)
$(subdirs): FORCE
	$(MAKE) -C $@ $(MAKECMDGOALS)

# Clean up
clean::
	rm -f wisent
clobber::
	rm -f frontend/Makefile.conf backend/Makefile.conf
