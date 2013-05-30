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
  un := 0.4cm;
  ANDStateHor(0,10,20,25,btex A etex,10, btex B etex, btex C etex);
  simpleState(1,11,9,15,btex D etex);
  simpleState(1,17,9,21,btex E etex);
  simpleState(11,11,19,15,btex F etex);
  simpleState(11,17,19,21,btex G etex);

  initTransition((5,23),(5,21), "");
  initTransition((15,23),(15,21), "");
  
  simpleState(12,2,19,6,btex I etex);
  transitionLine((14,6)*un--(14,8)*un);
  label.rt(btex e etex, (14,7)*un);
  transitionArrow((14,8)*un{right}..(15,11)*un{up});
  transitionArrow((14,8)*un{left}..(7,11)*un{up});

  simpleState(1,2,8,6,btex H etex);
  transitionAuto((5,6),(5,10),btex e etex);

endfig;

beginfig(2)
  un := 0.4cm;
  ANDStateHor(0,10,20,25.5,btex A etex,10, btex B etex, btex C etex);
  simpleState(1,11,9,15,btex D etex);
  simpleState(1,20,9,24,btex E etex);
  simpleState(11,11,19,15,btex F etex);
  simpleState(11,20,19,24,btex G etex);

  condition(5,17.5);
  condition(15,17.5);

  initTransition((2,17.5),(4,17.5),"");
  transitionAuto((5,18.5),(5,20), btex a == 0 etex);
  transitionAuto((5,16.5),(5,15), btex a == 1 etex);

  initTransition((12,17.5),(14,17.5),"");
  transitionAuto((15,18.5),(15,20), btex b == 0 etex);
  transitionAuto((15,16.5),(15,15), btex b == 1 etex);

  simpleState(12,2,19,6,btex I etex);
  transitionAuto((15,6),(15,10),btex e / $a=1,b=1$ etex);

  simpleState(1,2,8,6,btex H etex);
  transitionAuto((5,6),(5,10),btex e / $a=0,b=0$ etex);

endfig;


btex
  }
etex

end.