
all:
	make -C client all
	make -C server all

clean:
	-rm *~
	-rm *.log
	-rm core*
	-make -C client clean
	-make -C server clean

update:
	-make -C client update
	-make -C server update
