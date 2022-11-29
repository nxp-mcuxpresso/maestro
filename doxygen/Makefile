
all: clean rf

rf: clean
	if test -d "../docs/src"; then rm -rf ../docs/src; fi
	mkdir generated
	doxygen Doxyfile_RM
	cp -r generated/refman/html ../docs/src

pdf: clean
	make -C generated/refMan/latex pdf
	cp generated/refMan/latex/refman.pdf MaestroRM.pdf

clean:
	rm -rf generated/