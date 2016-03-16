# Tartaruga

## Descrizione Generale
Lo sketch permette all'utente di programmare fisicamente il comportamento della macchina, coprendo con un dito i fotoresistori corrispondenti all'azione voluta e premendo il tasto `PLAY`.

## Preparazione del robot Zumo
1. Montare lo shield con i fotoresistori.
2. Dopo aver compilato e caricato lo sketch sulla scheda Arduino, posizionare la macchina sul piano dove la si vuole utilizzare e portare su ON l'interruttore posteriore.
3. Si accenderanno quattro led: due rossi e due blu.
4. Il robot si muoverà in avanti autonomamente per circa tre secondi per tarare i coefficienti di correzione della velocità.
5. Dopo il primo suono del buzzer, verranno effettuate delle letture della luminosità ambientale.
6. Dopo quattro rapidi beep, sarà necessario coprire tutti i fotoresistori per tarare la soglia di copertura dei sensori.
7. A questo punto si può iniziare a programmare la sequenza coprendo un singolo fotoresistore alla volta.
8. Al raggiungimento del dodicesimo comando, la sequenza viene eseguita in automatico; alternativamente, in caso di una sequenza più corta, bisogna premere il tasto `PLAY`.

## Parametri modificabili
Di seguito una spiegazione dei principali parametri modificabili dall'utente:

* `COMMANDS_NUM`: massimo numero di comandi programmabili;
* `STRAIGHT_DURATION`: durata temporale di un singolo passo;
* `STRAIGHT_SPEED`: velocità nei tratti rettilinei;
* `TURN_SPEED`: velocità minima raggiunta nei tratti finali delle rotazioni; in curva la velocità raggiunge quattro volte questo valore;
* `TURN_ANGLE`: angolo di rotazione del robot.
