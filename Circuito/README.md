# Circuito 

## Descrizione Generale
Il programma `Circuito` permette al robot Zumo di percorrere un circuito di gara fino a quando non ha completato il numero di giri richiesti oppure è uscito di pista a causa dell'eccessiva velocità.

## Preparazione del circuito
Il circuito di gara deve essere realizzato sovrapponendo una linea nera (larghezza pari a circa due centimetri) su uno sfondo bianco. La linea del traguardo deve essere invece realizzata tracciando una linea nera, di lunghezza pari ad almeno la larghezza del robot, ortogonalmente ad un tratto di circuito rettilineo. 

## Preparazione Robot Zumo
1. Dopo aver compilato e caricato `Circuito.ino` sulla scheda Arduino, scollegare il cavo usb e alimentare il robot agendo sull'interrutore ON/OFF montato sulla parte posteriore. Si accenderanno quattro led: due rossi e due blu.
2. Posizionare il robot sul circuito, preferibilmente nel tratto di rettilineo antecedente il traguardo e premere il pulsante `PLAY`. Attendere che il robot finisca la calibrazione: ciò avviene quando lo Zumo si ferma e il led arancione si spegne.
3. Dopo aver posizionato nuovamente il robot in posizione corretta, premere il pulsante `PLAY`: la gara ha inizio e il robot prosegue fino a quando ha completato i giri richiesti (a tal fine non viene conteggiato il primo passaggio dal traguardo in quanto si effettua una partenza in velocità). 
4. Nel caso il robot esca dal circuito (vedere di seguito per capire in quali circostanze ciò possa accadere) riposizionare lo Zumo nel punto in cui era uscito e premere il pulsante `PLAY` per farlo ripartire.
5. Quando il robot riesce a completare correttamente i giri previsti, se si desidera ripetere la gara, posizionare nuovamente lo Zumo nel tratto di circuito antecedente il traguardo e ripetere il punto 3.

## Parametri modificabili
1. `MAX_LAPS`: numero di giri che il robot deve effettuare.
2. `SPEED_INCREASE`: valore che determina di quanto aumentare la velocità del robot rispetto ad un valore predefinito di sicurezza. Tale costante, che può variare nel range 0 - 100, permette quindi di rendere il proprio robot più veloce di quello degli altri: ovviamente tale scelta è un rischio in quanto ad una maggiore velocità corrisponde un maggior rischio di non riuscire ad affrontare le curve più impegnative. Si noti che se il valore viene lasciato pari a 0 si è in una condizione in cui, se il circuito è stato realizzato correttamente, il robot non dovrebbe mai uscire di pista.

## Risoluzione dei problemi
La probabilità con cui il robot esce di pista è controllata principalmente dalla costante `SPEED_INCREASE`. Naturalmente, però, grante importanza è rivestita da come è stato progettato il circuito e, in particolare, dal raggio delle curve e dalla loro dislocazione. Nel caso quindi non si riesca ad ottenere l'effetto voluto (il robot esce di pista anche se `SPEED_INCREASE` è pari a 0, oppure non esce mai anche nel caso di valore massimo), è opportuno modificare il circuito oppure effettuare alcuni cambiamenti nel codice del programma.

## Licenza
Questo programma è una versione modificata presso il Politecnico di Torino dell'esempio `line_following` fornito dalla Polulu insieme alle librerie per il robot Zumo. Le modifiche si concrentrano principalmente in:

1. Aggiunta della linea del traguardo e conteggio del numero di giri;
2. Introduzione della possibilità, nel caso venga impostata una velocità troppo elevata, che il robot non riesca a percorrere le curve più impegnative ed esca di pista. 

Vedere il file `LICENSE` per maggiori informazioni.
