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
  ANDStateHor(0, 0, 20, 10, btex A etex, 10, btex B etex, btex C etex);
  simpleState(1, 1, 9, 3, btex E etex);
  simpleState(11, 1, 19, 3, btex G etex);
  simpleState(1, 5, 9, 7, btex D etex);
  simpleState(11, 5, 19, 7, btex F etex);
  transitionAuto((5,5), (5,3), btex e etex);
  transitionAuto((15,5), (15,3), btex e etex);
  initTransition((5, 9), (5, 7), "");
  initTransition((15, 9), (15, 7), "");
endfig;

beginfig(2)
  un := 0.5cm;
  ANDStateHor(0, 0, 20, 10, btex A etex, 10, btex B etex, btex C etex);
  simpleState(1, 1, 9, 3, btex E etex);
  simpleState(11, 1, 19, 3, btex G etex);
  simpleState(1, 5, 9, 7, btex D etex);
  simpleState(11, 5, 19, 7, btex F etex);
  transitionAuto((5,5), (5,3), btex e / z$_1$ etex);
  transitionAuto((15,5), (15,3), btex e / z$_2$ etex);
  initTransition((5, 9), (5, 7), "");
  initTransition((15, 9), (15, 7), "");
endfig;


beginfig(3)
  un := 0.5cm;
  simpleState(0, 0, 5, 3, btex ¬зрыв etex);
  simpleState(0, 5, 5, 8, btex ƒетонаци€ etex);
  simpleState(0, 10, 5, 13, btex Ќеактивна etex);
  transitionAuto((4,10), (4,8), btex t[нажата нопка¬зрыв] etex);
  transitionAuto((1,8), (1,10), btex t[нажата нопкаќтмена] etex);
  transitionAuto((2.5,5), (2.5,3), btex t[врем€ == 0] etex);
  initTransition((2.5, 14), (2.5, 13), "");
endfig;

beginfig(4)
  un := 0.5cm;
  simpleState(0, 0, 5, 3, btex ¬зрыв etex);
  simpleState(0, 5, 5, 8, btex ƒетонаци€ etex);
  simpleState(0, 10, 5, 13, btex Ќеактивна etex);
  transitionAuto((4,10), (4,8), btex t[нажата нопка¬зрыв] etex);
  transitionAuto((1,8), (1,10), btex t[нажата нопкаќтмена \&\& врем€ != 0] etex);
  transitionAuto((2.5,5), (2.5,3), btex t[врем€ == 0] etex);
  initTransition((2.5, 14), (2.5, 13), "");
endfig;




btex
  }
etex

end.