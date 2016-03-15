LIGHT CONTROL
=============

Descrizione generale 
--------------------
	Lo sketch permette di controllare il polizumo robot con un fascio di luce. 
	Puntando la luce su una delle quattro fotoresistenze montate sul robot esso si muove. 
	Puó andare avanti, indietro, destra e sinistra.
	
# Preparazione Polizumo
Dopo aver montato lo shield con le fotoresistenze sul robot, rimosso l'array dei sensori di riflessione e compilato e caricato "LightControl.ino" sulla scheda Arduino alimentare il robot agendo sull'interrutore ON/OFF montato sulla parte posteriore.Si accenderanno quattro led: due rossi e due blu. A questo punto bisogna procedere con la calibrazione delle fotoresistenze:

1. Posizionare lo zumo sul piano di utilizzo(se farete correre il vostro robot su un tavolo allora posizionarlo sul tavolo, altrimenti sul pavimento, e cosi via).
2. Premere il pulsante di sinistra e allontanare subito la mano dalla scheda in modo tale da non fare `ombra(avrete due secondi per fare ció). Sentirete un beep che indica l'inizio della calibrazione, e un secondo beep e lo spegnimento del led 13 indicheranno la fine del procedimento.
3. Se doveste spostare il vostro zumo dal tavolo al pavimento, oppure vi é un cambiamento di illuminazione, allora dovrete andare al passo 1.

Durante la stesura del codice si é cercato di farlo in modo chiaro e semplice aggiungendo commenti dove 
neccessario. Per meglio soddisfare le vostre esigenze leggete il codice e cambiate i parametri neccessari.

# General Troubleshooting

Se piú di una resistenza si trova sotto il fascio di luce o per qualche motivo(calibrazione non corretta delle fotoresistenze o cambiamento della luminositá del ambiente dove vi trovate) il polizumo rimane fermo e non risponderá ai vostri comandi. Se nessuno dei casi descritti sopra si verifica ma il robot presenta comunque un funzionamento anomalo controllare che la scheda con le fotoresistenze sia montata bene e che i pin siano correttamente inizializati nello sketch "LightControl.ino".
	
	
	
	