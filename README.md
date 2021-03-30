# Prova finale - Progetto di Algoritmi e Strutture dati - A.A. 2019/2020

### Indice dei contenuti

* [Introduzione](#introduzione)
* [Comandi](#comandi)
* [Implementazione](#implementazione)
* [Compilazione ed esecuzione](#compilazione)
* [Note e considerazioni finali](#note-finali)

<a name="introduzione"></a>
## Introduzione

La documentazione qui prodotta presenta il progetto dell'esame "Prova Finale - Progetto di Algoritmi e Strutture dati" del Politecnico di Milano, Anno Accademico 2019/2020.<br/>

L'obiettivo del progetto è quello di implementare un semplice **editor di testo** eseguibile da terminale, con le classiche funzionalità di aggiunta/rimozione di righe, la visualizzazione dell'intero documento (o parte di esso) e l'esecuzione di altri comandi, come _undo_ e _redo_.<br/>
Il codice sorgente è stato sottoposto da un verificatore online a numerosi **test**, ciascuno focalizzato su specifiche funzionalità del programma. Obiettivo di questi test è quello di valutare la complessità dell'algoritmo sviluppato, in termini di **utilizzo della memoria** e di **tempo di esecuzione**.<br/>
A seguito di questi test è stato attribuito al progetto la valutazione di **30 e Lode**.

<a name="comandi"></a>
## Comandi

I comandi, necessari per poter interagire con il programma, sono forniti al terminale tramite **tastiera** (_stdin_). Ciascun comando, per poter essere interpretato correttamente dall'editor di testo, deve rispettare la sintassi di seguito mostrata:

### *(ind1,ind2)c*
Il comando **_change_** cambia il contenuto delle righe di testo con indice compreso tra _ind1_ e _ind2_. Le _(ind2-ind1)+1_ righe devono essere inserite assicurandosi prima di essere andati a capo nel terminale. Se gli indici delle righe indicate non esistono ancora nel documento, il programma provvederà a inserire queste nuove righe in coda.
Per terminare il comando è necessario inserire un punto '.' dopo aver scritto su terminale il contenuto di tutte le righe da modificare/aggiungere.

_Esempio di utilizzo_
```
1,3c
Prima riga del file di testo
Seconda riga del file di testo
Terza riga del file di testo
.
```

Il documento contiene ora le righe appena inserite.

### *(ind1,ind2)d*
Il comando **_delete_** cancella tutte le righe comprese tra _ind1_ e _ind2_ (estremi inclusi), modificando e portando a _ind1_ l'indice delle righe successive.

_Esempio di utilizzo_

```
2,3d
```

Il documento contiene ora una sola riga (_Prima riga del file di testo_).

### *(ind1,ind2)p*

Il comando **_print_** stampa a video (attraverso _stdout_) il contenuto delle righe comprese tra _ind1_ e _ind2_ (estremi inclusi). Se le righe non esistono queste vengono visualizzate con un punto '.'

_Esempio di utilizzo_

```
1,3p
```

Supponendo di aver già eseguito i comandi degli esempi precedenti, l'output sarà il seguente:

```
Prima riga del file di testo
.
.
```

_(I punti visualizzati indicano che le righe 2 e 3 non esistono nel documento, poichè eliminate)_

### *(number)u*
Il comando **_undo_** annulla l'effetto di un certo numero, indicato da _number_,  di comandi di tipo _change_ o _delete_ precedentemente eseguiti. Se il numero indicato di comandi da annullare è superiore a quelli precedenemente eseguiti, vengono allora annullati tutti i passi ed il documento diventa vuoto.

_Esempio di utilizzo_

```
1u
```

Il documento, per effetto di una _undo_, contiene anche le due righe precedentemente eliminate.

### *(number)r*
Il comando **_redo_** annulla l'effetto di un certo numero di _undo_ precedentemente eseguiti, indicato da _number_. Non è possibile eseguire la redo di comandi precedenti all'inserimento, modifica o cancellazione di righe nel file di testo.

_Esempio di utilizzo_

```
1r
```

Il documento, per effetto della _redo_, rettifica l'effetto della _undo_ precedente ed elimina di nuovo le due righe che erano state ripristinate.

### *q*
Il comando **_quit_** termina l'esecuzione dell'editor.

<a name="implementazione"></a>
## Implementazione

Durante la fase di progettazione e stesura del codice sono state considerate numerose strategie implementative, ciascuna di esse con diversi punti di forza e criticità.<br/>
E' stata individuata quindi una soluzione che ha consentito un buon bilanciamento tra complessità temporale e spaziale, attraverso l'utilizzo di una **lista doppiamente concatenata** (lista bidirezionale) e di due **pile**:

- Una prima lista concatenata **_Row_** serve per implementare la struttura logica delle righe di un documento;
- Due pile **_pilaUndo_** e **_pilaRedo_** di tipo _**Command**_ per implementare le rispettive funzionalità di _undo_ e _redo_ di comandi.

L'algoritmo, in maniera molto semplificata, segue questi passi:

- Viene ricevuto da terminale il comando desiderato dall'utente, che viene decodificato individuandone il tipo di comando richiesto (_c,d,p,u,r,q_) ed eventuali parametri(_ind1, ind2, number_);
- Nel caso di un comando di tipo _change_ o _delete_ viene istanziato un nodo di tipo _Command_ nella pila _pilaUndo_, che contiene al suo interno riferimenti ad eventuali righe aggiunte, modificate o eliminate. In questo modo risulta estremamente semplice effetuare una operazione di _undo_, nel caso in cui questa venga richiesta in futuro;
- Nel caso di un comando di tipo _undo_, viene preso dalla pila un determinato numero di nodi di tipo _Command_ dalla pila _pilaUndo_, vengono processate le informazioni al loro interno e viene ripristinata la situazione desiderata dall'utente. I comandi precedentemente estratti da _pilaUndo_ vengono poi inseriti, in ordine inverso nella seconda pila _pilaRedo_;
- Analogamente, viene eseguito il medesimo processo nel caso di un comando di tipo _redo_;
- Se il comando richiesto è di tipo _print_, infine, vengono stampate le righe richieste dall'utente procedendo con uno scorrimento sequenziale partendo dagli estremi del documento, arrivando all'indice di inizio desiderato.

L'utilizzo di una lista doppiamente concatenata permette l'accesso alle righe del documento partendo da due possibili estremi: un primo, che corrisponde all'inizio del documento; un secondo, che corrisponde invece all'ultima riga.
In base alla posizione (indice) della riga desiderata è possibile decidere da dove iniziare lo scorrimento della lista, permettendo un notevole risparmio dei tempi di esecuzione nel caso in cui righe selezionate siano vicine agli estremi del documento.

<a name="compilazione"></a>
## Compilazione ed esecuzione
Per compilare il programma è sufficiente scaricare il file <a href="https://github.com/priscia99/progetto-algoritmi-strutture-dati/blob/main/Progetto.c">Progetto.c</a> nel proprio PC, raggiungere da terminale la _directory_ in cui il file è salvato ed eseguire il seguente comando:

```
/usr/bin/gcc -DEVAL -std=gnu11 -O2 -pipe -static -s -o progetto progetto.c -lm
```

Infine, è possibile eseguire il programma digitando su linea di comando:

```
./progetto
```

<a name="note-finali"></a>
## Note e considerazioni finali

Il progetto presenta tutte le funzionalità richieste dal documento di specifica. Non è stato possibile implementare funzionalità aggiuntive in quanto queste avrebbero potuto alterare l'output del programma, con conseguente fallimento dei test generati del verificatore.<br/>
Data la natura del verificatore non si è reso necessario il caricamento/salvataggio dei documenti da/in memoria di massa.<br/><br/>
Il verificatore online, implementato dai docenti, ha effettuato test utilizzando input di **~250 MB** e richiedendo un tempo massimo di esecuzione di **2,100s**. Il progetto, nei _worst case scenario_, ha impegato al massimo **1,575 secondi** per eseguire i comandi richiesti.<br/><br/>
Per informazioni dettagliate su come è stata richiesta l'implementazione delle funzionalità del programma (ed il comportamento da seguire nei numerosi _corner case_) si veda il <a href ="https://github.com/priscia99/progetto-algoritmi-strutture-dati/blob/main/Specifica.pdf">documento di specifica</a>.

