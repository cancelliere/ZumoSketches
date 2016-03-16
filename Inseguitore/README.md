# Inseguitore

## Descrizione Generale
Lo sketch permette di controllare il robot Zumo con un fascio di luce: puntando la luce su una delle quattro fotoresistenze presenti sulla board aggiuntiva, esso si muove nella direzione relativa. Le direzioni ammesse sono: avanti, indietro, destra e sinistra.

## Preparazione robot Zumo
1. Dopo aver montato la board aggiuntiva con le fotoresistenze sul robot, rimosso l'array dei sensori di riflessione e compilato e caricato `Inseguitore.ino` sulla scheda Arduino, alimentare il robot agendo sull'interrutore ON/OFF montato sulla parte posteriore: si accenderanno quattro led: due rossi e due blu.
2. Posizionare lo Zumo sul tavolo su cui esso verrà utilizzato e procedere con la calibrazione dei sensori: premere il pulsante `PLAY` e allontanare subito la mano dalla board in modo tale da non fare ombra. Si sentirà un beep che indica l'inizio della calibrazione, e un secondo beep e lo spegnimento del led arancione indicheranno la fine del procedimento. Nel caso si renda necessario spostare il robot su un altro piano di lavoro, oppure vi sia un cambiamento di illuminazione, è necessario effettuare nuovamente la calibrazione dopo aver spento e riacceso il robot.
3. Utilizzare una torcia con fascio spot per illuminare una delle fotoresistenze e controllare il robot a distanza.

## Risoluzione dei problemi
Nel caso in cui più di una resistenza si trovi sotto il fascio di luce, il robot rimarrà fermo e non risponderà ai comandi: in tal caso è necessario porre la massima attenzione nell'orientare in modo corretto la torcia. Se il problema continua a manifestarsi, verificare che la calibrazione sia stata effettuata correttamente e provare a ripeterla dopo aver spento e riacceso il robot.
