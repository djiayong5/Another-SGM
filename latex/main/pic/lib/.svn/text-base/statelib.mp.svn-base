un := 0.2cm;
stateWidth := 2pt; 
transitionWidth := 1pt;
initPointWidth := 4pt;

color color_main, color_one, color_two, color_three;
color_main := red + green * 0.5;
color_one := 0.9*white + 0.1*color_main;
color_two := 0.8*white + 0.2*color_main;
color_three := 0.7*white + 0.3*color_main;

def stateBox(expr l, b, r, t, c) =
  pickup pencircle scaled stateWidth;
  fill((l*un+un,b*un)
       --(r-1,b)*un{right}
       ..(r,b+1)*un
       --(r,t-1)*un{up}
       ..(r-1,t)*un
       --(l+1,t)*un{left}
       ..(l,t-1)*un
       --(l,b+1)*un{down}
       ..cycle) withcolor c;
  draw((l*un+un,b*un)
       --(r-1,b)*un{right}
       ..(r,b+1)*un
       --(r,t-1)*un{up}
       ..(r-1,t)*un
       --(l+1,t)*un{left}
       ..(l,t-1)*un
       --(l,b+1)*un{down}
       ..cycle);
enddef;

def simpleStateColor(expr l, b, r, t, s, c) =
  stateBox(l, b, r, t, c);
  label(s, ((l+r)/2,(b+t)/2)*un);
enddef;

def simpleState(expr l, b, r, t, s) =
  simpleStateColor(l, b, r, t, s, color_two);
enddef;

def simpleStateHColor(expr l, b, r, t, s, h, c) =
  stateBox(l, b, r, t, c);
  picture ll;
  ll := thelabel.urt(h, (l + 1, b)*un);
  draw ll;
  y := (ypart (urcorner ll)) / un;
  label(s, ((l+r)/2,(y+t)/2)*un);
enddef;

def simpleStateH(expr l, b, r, t, s, h) =
  simpleStateHColor(l, b, r, t, s, h, color_two);
enddef;

def dashedState(expr l, b, r, t, s) =
  pickup pencircle scaled (stateWidth / 2);
  draw((l*un+un,b*un)
       --(r-1,b)*un{right}
       ..(r,b+1)*un
       --(r,t-1)*un{up}
       ..(r-1,t)*un
       --(l+1,t)*un{left}
       ..(l,t-1)*un
       --(l,b+1)*un{down}
       ..cycle) dashed evenly;
  label(s, ((l+r)/2,(b+t)/2)*un);
enddef;

def ORStateColor(expr l, b, r, t, s, c) =
  stateBox(l, b, r, t, c);
  label.lrt(s, (l+1, t-0.2) * un);
enddef;

def ORState(expr l, b, r, t, s) =
  ORStateColor(l, b, r, t, s, color_one);
enddef;

def ANDStateHorColor(expr l, b, r, t, s, x, sa, sb, c) =
  stateBox(l, b, r, t, c);
  label.urt(s, (l+1, t+0.2) * un);
  label.lrt(sa, (l+1, t-0.2) * un);
  label.lrt(sb, (x+0.2, t-0.2) * un);
  draw((x,b)*un--(x,t)*un) dashed evenly;
enddef;

def ANDStateHor(expr l, b, r, t, s, x, sa, sb) =
  ANDStateHorColor(l, b, r, t, s, x, sa, sb, color_one);
enddef;

def condition(expr x, y) =
  r := 1;
  pickup pencircle scaled (stateWidth / 2);
  draw((x-r,y)*un..(x,y-r)*un..(x+r,y)*un..(x,y+r)*un..cycle);
  label(btex C etex, (x, y)*un);
enddef;

def transitionArrow(expr p) =
  pickup pencircle scaled transitionWidth; 
  ahlength := un / 2;
  drawarrow(p);
enddef;

def transitionLine(expr p) =
  pickup pencircle scaled transitionWidth; 
  draw(p);
enddef;

def transitionAuto(expr a, b, s) =
  pair c, q;
  q := (b - a) rotated 90;
  c := (a + b) / 2 + q * 0.15;
  transitionArrow(a*un..c*un..b*un);
  picture l;
  l := thelabel(s, (0,0));
  r := length ((llcorner l - urcorner l) / 2) / un;
  c := (a + b) / 2 + q * (0.2 + r / length q);
  label(s, c * un);
enddef;

def transitionAutoR(expr a, b, s) =
  pair c, q;
  q := (b - a) rotated 90;
  c := (a + b) / 2 - q * 0.15;
  transitionArrow(a*un..c*un..b*un);
  picture l;
  l := thelabel(s, (0,0));
  r := length ((llcorner l - urcorner l) / 2) / un;
  c := (a + b) / 2 - q * (0.2 + r / length q);
  label(s, c * un);
enddef;

def initTransition(expr a, b, s) =
  transitionAuto(a, b, s);
  pickup pencircle scaled initPointWidth; 
  drawdot(a*un);
enddef;