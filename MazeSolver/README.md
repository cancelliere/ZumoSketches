MAZE SOLVER
===========

Descrizione Generale 
--------------------
Lo sketch guida il polizumo attraverso un labirinto fino a trovare l'uscita/punto d'arrivo.

# Preparazione Polizumo
1. Controllare che l'array dei sensori di riflessione sia montato ed eventualmente montarlo e rimuovere lo
shield delle fotoresistenze. 
2. Dopo aver compilato e caricato "MazeSolver.ino" sulla scheda Arduino alimentare il robot agendo sull'interrutore ON/OFF montato sulla parte posteriore.
3. Si accenderanno quattro led: due rossi e due blu.
4. Posizionare il robot su una delle possibili entrate del labirinto e premere il pulsante 12 (PLAY) sul lato posteriore. 
Il polizumo si autocalibrerá e entrerá nel labirinto. Esso si fermerá appena incontrerá una zona nera abbastanza ampia da contenere tutti e sei i sensori di riflessione. A questo punto bisogna riposizionare lo zumo alla stessa entrata precedente e ripremere il pulsante 12. 
Il robot ritroverá il punto d'arrivo seguendo il percoro diretto.

Durante la stesura del codice si é cercato di farlo in modo chiaro e semplice aggiungendo commenti dove neccessario. Per meglio soddisfare le vostre esigenze leggete il codice e cambiate i parametri neccessari.

# General Troubleshooting

Se il vostro robot dovesse comportarsi in modo anomalo come non svoltare nelle direzioni giuste allora bisogna agire sui seguenti parametri:
* `SENSOR_THRESHOLD` linea 35
* `SPEED` linea 49
* `LINE_THICKNESS` linea 52
	
Se invece lo zumo una volta trovata l'uscita del labirinto non riesce a percorre il percorso diretto allora bisogna cambiare la dimensione del vettore `path` (linea 81). Questo cambiamento é neccessario se il labirinto é particolarmente esteso. La lunghezza di questo vettore non puó essere lunga a piacere in quanto é vincolato dalla memoria della scheda arduino.