# Labirinto

## Descrizione Generale
Lo sketch guida il robot Zumo attraverso un labirinto fino a quando non viene trovata l'uscita, ovvero il punto d'arrivo.

## Preparazione del robot Zumo
1. Controllare che l'array dei sensori di riflessione sia montato correttamente: in caso contrario inserirlo e rimuovere la board aggiuntiva eventualmente presente.
2. Dopo aver compilato e caricato `Labirinto.ino` sulla scheda Arduino alimentare il robot agendo sull'interrutore ON/OFF montato sulla parte posteriore. Si accenderanno quattro led: due rossi e due blu.
3. Posizionare il robot su una delle possibili entrate del labirinto e premere il pulsante `PLAY` sul lato posteriore. Il robot inizierà il processo di autocalibrazione e inizierà a percorrere il labirinto. 
4. Quando il robot raggiunge il punto d'arrivo, ovvero una linea nera sufficientemente larga da essere individuata da tutti e sei i sensori di riflessione, si fermerà. A questo punto riposizionare lo Zumo alla stessa entrata precedente e ripremere il pulsante `PLAY`. Il robot riuscirà quindi a raggiungere il punto di arrivo seguendo un percorso diretto.

## Risoluzione dei problemi 
Nel caso in cui il robot presenti comportamenti anomali, ovvero non riesca ed effettuare le svolte in modo corretto, oppure individuare correttamente l'uscita, è possibile agire sui seguenti parametri:

* `SENSOR_THRESHOLD`: soglia per i sensori;
* `SPEED`: velocità con cui il robot percorre il labirinto;
* `LINE_THICKNESS`: spessore della linea con cui è costruito il labirinto (in pollici).

Se invece lo Zumo, una volta trovata correttamente l'uscita del labirinto, non riesce a percorre il percorso diretto, si rende necessario cambiare la dimensione del vettore `path`. Questo cambiamento è necessario se il labirinto è particolarmente esteso, ma la dimensione massima di tale array è limitata dalla memoria della scheda arduino.

## Licenza
Questo programma è lo schema di esempio `MazeSolver` fornito dalla Polulu insieme alle librerie per il robot Zumo. Per maggiori informazioni relative alla licenza, vedere il file `LICENSE`.
