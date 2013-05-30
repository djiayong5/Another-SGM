verbatimtex 
%&latex 
\documentclass[a4paper,11pt]{article} 
\usepackage[cp1251]{inputenc} 
\usepackage[russian]{babel} 
\usepackage[dvips]{epsfig} 
\pagestyle{empty} 
\begin{document} 
\textsf{
etex 
input lib/statelib



beginfig(1)
  un := 0.5cm;
  simpleState(0,0,5,3, btex «вук etex);
  simpleState(0,5,5,8, btex “ишина etex);
  transitionAuto((1,3), (1,5), btex нажата нопкаќтмены etex);
  transitionAuto((4,5), (4,3), btex пришло¬рем€[будильник¬ключен] etex);
endfig;




btex
  }
etex

end.