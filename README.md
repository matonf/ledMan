# ledMan

Par Matthieu ONFRAY (http://www.onfray.info)  
Licence : CC by sa

ledMan est un projet arduino de contrôle de LED, au choix mono ou RGB.
2 effets aléatoire pour la mono, 4 pour la RGB.

En mode proto, les effets sont rapprochés et s'exécutent rapidement, sinon ils sont plus distants en minutes et plus longs à s'afficher.

La particularité est que la puce s'endort entre ces cycles d'effets lumineux, ne consommant plus d'énergie (utilisation de watchdog). Les modules inutiles sont désactivés aussi (ADC, compteur numéro 1, etc).

Le fichier ledMan624 effectue globalement les même effets lumineux (ajout d'un nouvel effet pour la led monochome), sa particularité est surtout de faire fonctionner la LED 6 heures, puis rien pendant 18h, puis reprise. Ainsi elle ne peut s'allumer que pour des cycles de 6 heures, ce qui économise la pile de 3V. A démarrer le soir pour profiter de l'illumination...
