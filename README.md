# ledMan

Par Matthieu ONFRAY (http://www.onfray.info)  
Licence : CC by sa

ledMan est un projet arduino de contrôle de LED, au choix mono ou RGB.
2 effets aléatoire pour la mono, 4 pour la RGB.

En mode proto, les effets sont rapprochés et s'exécutent rapidement, sinon ils sont plus distants en minutes et plus longs à s'afficher.

La particularité est que la puce s'endort entre ces cycles d'effets lumineux, ne consommant plus d'énergie (utilisation de watchdog).
