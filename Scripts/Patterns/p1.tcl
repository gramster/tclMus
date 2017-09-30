set auto_path "../Utils"

set keyboard [makesound sample ../Samples/syncomp.wav] 

set x [MakeSequence $keyboard piano 1000 1000 1000 {
		a2 c1 c1 f0 c1 c1 f0 e2
		c2 c2 d1 d2 g1 f2 e2 f1
		b2 d1 a2 a2 c1 a2 b1 f2
		d2 f2 d2 c2 a2 g2 e2 f2
		a2 c1 c1 b0 c1 c1 c0 e2
		c2 c2 d1 d2 e1 f2 e2 f1
		b2 f1 a2 a2 f1 a2 f1 g2
		d2 g2 g2 d2 f2 e2 f2 c2
	     } ]

set xx [expandsound $x]

destroysound $keyboard $x

return $xx





