.PHONY: all clean clobber

wisent: all
	ln -sf build/$@ .
clean clobber all:
	make -C build/ $@
