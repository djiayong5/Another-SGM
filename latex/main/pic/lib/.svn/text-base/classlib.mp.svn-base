un := 0.2cm;
classWidth := 2pt; 
lineWidth := 1pt;
offset := 1;
istep := 1.5*un;

def addtolist(expr a, b) =
  addto a also b; %(b shifted (llcorner a - ulcorner b));
enddef;

def drawClass(expr l, b, r, t, name, args, ops) =
  pickup pencircle scaled classWidth;
  draw((l,b)*un--(r,b)*un--(r,t)*un--(l,t)*un--cycle);

  picture ll;
  ll := thelabel.bot(name, ((l+r)/2,t)*un);
  draw ll;
  y := ypart (llcorner ll) / un - offset;
  draw((l,y)*un--(r,y)*un);

  ll := thelabel.lrt(args, (l,y)*un);
  draw ll;
  y := ypart (llcorner ll) / un - offset;

  draw((l,y)*un--(r,y)*un);

  ll := thelabel.lrt(ops, (l,y)*un);
  draw ll;

enddef;



