# Requirements
* [Tclap](http://tclap.sourceforge.net)

# MPI version
There is in a [mpi](https://github.com/isalem/ppr.2-zbs/tree/mpi) branch.

# Build
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make

# Build report
	cd report
	docker build -t texlive docker
	docker run --rm -it -v $(pwd):/var/texlive texlive pdflatex zprava.tex