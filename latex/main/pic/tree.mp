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
  simpleState(0,0,5,3, btex Дата etex);
  simpleState(6,0,11,3, btex Время etex);
  simpleState(12,0,17,3, btex Выключен etex);
  simpleState(18,0,23,3, btex Включен etex);
  simpleState(3,5,8,8, btex Изображение etex);
  simpleState(13,5,22,8, btex СостояниеБудильника etex);
  simpleState(9,10,14,13, btex Включены etex);
  draw((2.5,3)*un--(4,5)*un);
  draw((8.5,3)*un--(7,5)*un);
  draw((14.5,3)*un--(16,5)*un);
  draw((20.5,3)*un--(19,5)*un);
  draw((5.5,8)*un--(10,10)*un);
  draw((17.5,8)*un--(13,10)*un);
endfig;


beginfig(3)
  un := 0.5cm;
  ANDStateHor(0, 0, 20, 10, btex Включены etex, 10, btex Изображение etex, btex СостояниеБудильника etex);
  simpleState(0.5, 0.5, 9.5, 3.5, btex Время etex);
  simpleState(10.5, 0.5, 19.5, 3.5, btex Включен etex);
  simpleState(0.5, 4.5, 9.5, 7.5, btex Дата etex);
  simpleState(10.5, 4.5, 19.5, 7.5, btex Выключен etex);
endfig;




btex
  }
etex

end.