# Prova finale - Progetto di Algoritmi e Strutture dati - A.A. 2019/2020

L'obiettivo del progetto è quello di implementare un semplice **editor di testo** eseguibile da terminale, con le classiche funzionalità di aggiunta/rimozione di righe dal documento, la visualizzazione dell'intero documento (o parte di esso) e l'esecuzione di altri comandi, come _undo_ e _redo_.
Il codice sorgente è stato sottoposto da un verificatore online a numerosi *test*, ciascuno focalizzato su specifiche funzionalità del programma. Obiettivo di questi test è quello di valutare la complessità dell'algoritmo sviluppato, in termini di **utilizzo della memoria** e di **tempo di esecuzione**.
A seguito di questi test è stato attribuito al progetto la valutazione di **30 e Lode**.

## Comandi

I comandi, necessari per poter interagire con il programma, sono forniti al terminale tramite **tastiera** (_stdin_). Ciascun comando, per poter essere interpretato correttamente dall'editor di testo, deve rispettare la sintassi di seguito mostrata:

#### *(ind1,ind2)c*
Il comando **_change_** cambia il contenuto delle righe di testo con indice compreso tra _ind1_ e _ind2_. Le _(ind2-ind1)+1_ righe devono essere inserite assicurandosi prima di essere andati a capo nel terminale. Se gli indici delle righe indicate non esistono ancora nel documento, il programma provvederà a inserire queste nuove righe in coda.
Per terminare il comando è necessario inserire un punto '.' dopo aver scritto su terminale il contenuto di tutte le righe da modificare/aggiungere.

_Esempio di utilizzo_

1,3c
Prima riga del file di testo
Seconda riga del file di testo
Terza riga del file di testo
.

Il documento contiene ora le righe appena inserite.

#### *(ind1,ind2)d*
Il comando **_delete_** cancella tutte le righe comprese tra _ind1_ e _ind2_ (estremi inclusi), modificando e portando a _ind1_ l'indice delle righe successive.

_Esempio di utilizzo_

2,3d

Il documento contiene ora una sola riga (_Prima riga del file di testo_).

#### *(ind1,ind2)p

Il comando **_print_** stampa a video (attraverso _stdout_) il contenuto delle righe comprese tra _ind1_ e _ind2_ (estremi inclusi). Se le righe non esistono queste vengono visualizzate con un punto '.'

_Esempio di utilizzo_

1,3p

Supponendo di aver già eseguito i comandi degli esempi precedenti, l'output sarà il seguente:

Prima riga del file di testo
.
.

_(I punti visualizzati indicano che le righe 2 e 3 non esistono nel documento, poichè eliminate)



