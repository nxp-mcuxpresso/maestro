
all: clean rf

rf: clean
	if test -d "../docs/src"; then rm -rf ../docs/src; fi
	mkdir generated
	mkdir ../docs/src
	doxygen Doxyfile_RM
	cp -a generated/refMan/html/. ../docs/src/

pdf: clean
	make -C generated/refMan/latex pdf
	cp generated/refMan/latex/refman.pdf MaestroRM.pdf

clean:
	rm -rf generated/