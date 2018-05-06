DEFINE_UNIT(Pendulum)
SET(CODE		"Sources"	)
SET(TEST    	"Tests" 	)
SET(CATEGORY	"Project"	)
SET(MODULES 
	Misc 
	Engine 
	Core 
	Components 
	SFML 
	#OSG
	ComponentVisualisers
	)
MAKE_APP()