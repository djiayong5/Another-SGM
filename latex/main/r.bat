echo R | latex thesis.tex
bibtex8 -B -c %~dp0sty\cp1251.csf thesis
echo R | latex thesis.tex
dvips -t portrait thesis.dvi
