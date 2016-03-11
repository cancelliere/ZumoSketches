

##########################################################################################################
###############################################MAZE_SOLVER################################################
##########################################################################################################

## Descrizione Generale e Preparazione Polizumo
	
	Lo sketch guida il polizumo attraverso un labirinto fino a trovare l'uscita/punto d'arrivo.
	Controllare che l'array dei sensori di riflessione sia montato ed eventualmente montarlo e rimuovere lo
	shield delle fotoresistenze. Dopo aver compilato e caricato "MazeSolver.ino" sulla scheda Arduino ali-
	mentare il robot agendo sull'interrutore ON/OFF montato sulla parte posteriore.Si accenderanno quattro 
	led: due rossi e due blu.
	Posizionare il robot su una delle possibili entrate del labirinto e premere il pulsante 12 sul lato po-
	steriore. Il polizumo si autocalibrerá e entrerá nel labirinto. Esso si fermerá appena incotrerá una zo
	-na nera abbastanza ampia da contenere tutti e sei sensori di riflessione. Bisogna riposizionare lo zu-
	mo alla stessa entrata precedente e ripremere il pulsante 12. Il robot ritroverá il punto d'arrivo 
	seguendo il percoro diretto.
	
	
## General Troubleshooting

	Se il vostro robot dovesse comportarsi in modo anomalo come non svoltare nelle direzioni giuste allora 
	bisogna agire sui seguenti parametri:
		- SENSOR_THRESHOLD linea 35 
		- SPEED linea 49
		- LINE_THICKNESS linea 52
	
	Se invece lo zumo una volta trovato l'uscita del labirinto non riesce a percorre il percorso diretto 
	allora bisogna cambiare la dimensione del vettore "path" (linea 81). Questo cambiamento é neccessario 
	se il labirinto é particolarmente esteso. La lunghezza di questo vettore non puó essere lunga a piacere
	in quanto é vincolato dalla memoria della scheda arduino.