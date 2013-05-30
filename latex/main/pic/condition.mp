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
  simpleState(0,11,5,13,btex A etex);
  simpleState(6,16,11,18,btex B etex);
  simpleState(6,6,11,8,btex C etex);
  condition(8.5,12);

  transitionAuto((5,12),(7.5,12),btex e etex);
  transitionAuto((8.5,13),(8.5,16),btex $a == 0$ etex);
  transitionAuto((8.5,11),(8.5,8),btex $a == 1$ etex);
  
endfig;

beginfig(2)
  un := 0.5cm;
  simpleState(-2,11,3,13,btex A etex);
  simpleState(6,16,11,18,btex B etex);
  simpleState(6,6,11,8,btex C etex);
  simpleStateH(5,10,12,14,btex Condition etex, btex entry / GEN(t) etex);

  transitionAuto((3,12),(5,12),btex e etex);
  transitionAuto((8.5,14),(8.5,16),btex $t[a == 0]$ etex);
  transitionAuto((8.5,10),(8.5,8),btex $t[a == 1]$ etex);
  

endfig;


btex
  }
etex

end.