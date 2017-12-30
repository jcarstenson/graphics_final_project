all:	paper.tex bibliography.bib
	latex paper
	bibtex paper
	latex paper
	pdflatex paper

clean: 
	${RM} *.dvi *.log *.aux *.blg *.bbl paper.ps paper.ps paper.pdf
