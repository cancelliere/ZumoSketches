CIRCUITO
========

Descrizione Generale
--------------------
	Lo sketch guida il polizumo nel seguire una linea nera su uno sfondo bianco fino a quando non trova uno
	spazio nero tale da coprire contemporaneamente i due sensori di riflessione posizionati sui due estremi della
	batteria. Il robot si ferma anche quando perde il suo riferimento o meglio quando la linea nera non si
	trova pi� sotto i sensori.

Preparazione Polizumo
=====================
1. Dopo aver compilato e caricato LineFollowerWithFinishLine_new.ino sulla scheda Arduino alimentare il robot agendo sull'interrutore ON/OFF montato sulla parte posteriore. Si accenderanno quattro led: due rossi e due blu.
2. Successivamente posizionare il robot sulla linea nera e premere il pulsante 12 (PLAY).
3. Attendere che il polizumo finisca la calibrazione. Ci� avviene quando lo zumo si ferma e il led 13 si spegne.
4. Premendo nuovamente il pulsante il robot parte e segue la linea fino al traguardo(da posizionare su una tratta retta del circuito) dove si fermer�. Il robot si fermer� anche se dovesse uscire fuori dal circuito dovuto a delle curve impegnative.
5. Quando il robot si ferma � perch� ha raggiunto il traguardo oppure perch� � uscito fuori dalla pista; basta riposizionarlo sul circuito e premere di nuovo il pulsante 12 per farlo ripartire.

Durante la stesura del codice si � cercato di farlo in modo chiaro e semplice aggiungendo commenti dove neccessario. Per meglio soddisfare le vostre esigenze leggete il codice e cambiate i parametri neccessari.

General Troubleshooting
=======================
Il parametro principale � `MAX_SPEED` (linea 23). In base al valore che sar� assegnato ad esso ci si
presenteranno 3 casi:
* `MAX_SPEED <= 300` : il valore di `error_divider` * (linea 47) viene impostato uguale a 4. Con queste impostazioni il robot non uscir� mai fuori pista.

* `MAX_SPEED > 300 && MAX_SPEED <= 340` il valore di `error_divider` * (linea 53) viene impostato uguale a 7. In queto caso il robot potrebbe perdere il riferimento della linea nera e uscire dal tracciato. Dunque vi sono pi� probabili� che il robot esca se il valore di `MAX_SPEED` � piu vicino a 340 che a 300 e qundo le pile sono cariche/nuove. Se invece le pile sono scariche/usate allora vi � pi� probabilit� che il polizumo completi il giro ed arrivi al traguardo. Ho pensato di mettere questo range in modo tale che la fortuna possa assistere i pi� intraprendenti.

* `MAX_SPEED > 340` : il valore di "error_divider" * (linea 57) viene impostato uguale a 9. In questo modo il robot uscir� sicuramente dal circuito.

* (*) `error_divider` indica il valore derivativo nella formula che si trova alla linea 125. Pi� questo valore � alto meno il robot riesce a seguire le curve impegnative. Dunque se nel vostro circuito le curve non sono abbastanza strette da fare uscire il robot dal circuito allora agite su questo valore.
