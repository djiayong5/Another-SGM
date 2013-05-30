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


def mainPart(expr b) =
  simpleState(9,22,13,26, "");
  simpleState(5,16,9,20, b);
  simpleState(13,16,17,20, "");
  simpleState(13,10,17,14, "");
  draw((8,20)*un--(10,22)*un);
  draw((14,20)*un--(12,22)*un);
  draw((15,14)*un--(15,16)*un);
enddef;

def oldBranch(expr a,b,c,d,e) =
  simpleState(5,10,9,14, a);
  simpleState(2,4,6,8, b);
  simpleState(2,-2,6,2, c);
  simpleState(8,4,12,8, d);
  simpleState(8,-2,12,2, e);
  draw((7,14)*un--(7,16)*un);
  draw((5,8)*un--(6,10)*un);
  draw((9,8)*un--(8,10)*un);
  draw((4,2)*un--(4,4)*un);
  draw((10,2)*un--(10,4)*un);
enddef;

def newState =
  dashedState(-4,0,0,4, btex A etex);
enddef;

def newLine = 
  dashedState(-4,-2,0,2, btex A etex);
  dashedState(-4,4,0,8, "");
  dashedState(-2,10,2,14, "");
  draw((-2,2)*un--(-2,4)*un) dashed evenly;
  draw((-2,8)*un--(-1,10)*un) dashed evenly;
  draw((1,14)*un--(6,16)*un) dashed evenly;
enddef;

def newBranch = 
  simpleState(-4,-2,0,2, "3");
  simpleState(-4,4,0,8, "2");
  simpleState(-2,10,2,14, "1");
  simpleState(2,4,6,8, "4");
  simpleState(2,-2,6,2, "5");
  draw((-2,2)*un--(-2,4)*un);
  draw((-2,8)*un--(-1,10)*un);
  draw((1,14)*un--(6,16)*un);
  draw((4,2)*un--(4,4)*un);
  draw((3,8)*un--(1,10)*un);
enddef;


beginfig(1)
  un := 0.2cm;
  mainPart("");
  oldBranch("","","","","");
  newState;
endfig;

beginfig(2)
  un := 0.2cm;
  mainPart(btex B etex);
  oldBranch("","","","","");
  newLine;
endfig;

beginfig(3)
  un := 0.2cm;
  mainPart(btex B etex);
  oldBranch("5","2","1","4","3");
  newLine;
endfig;

beginfig(4)
  un := 0.2cm;
  mainPart(btex B etex);
  newBranch;
endfig;


btex
  }
etex

end.