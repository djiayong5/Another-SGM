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
  simpleState(0,0,5,3, btex Ожидание etex);
endfig;

beginfig(2)
  un := 0.5cm;
  ORState(0, 0, 10, 5, btex Включены etex);
  simpleState(0.5, 0.5, 4.5, 3.5, btex Время etex);
  simpleState(5.5, 0.5, 9.5, 3.5, btex Дата etex);
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