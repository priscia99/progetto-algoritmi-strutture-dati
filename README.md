# Prova finale - Progetto di Algoritmi e Strutture dati - A.A. 2019/2020

L'obiettivo del progetto è quello di implementare un semplice **editor di testo** eseguibile da terminale, con le classiche funzionalità di aggiunta/rimozione di righe dal documento, la visualizzazione dell'intero documento (o parte di esso) e l'esecuzione di altri comandi, come _undo_ e _redo_.
Il codice sorgente è stato sottoposto da un verificatore online a numerosi *test*, ciascuno focalizzato su specifiche funzionalità del programma. Obiettivo di questi test è quello di valutare la complessità dell'algoritmo sviluppato, in termini di **utilizzo della memoria** e di **tempo di esecuzione**.
A seguito di questi test è stato attribuito al progetto la valutazione di **30 e Lode**.

## Comandi

I comandi, necessari per poter interagire con il programma, sono forniti al terminale tramite **tastiera** (_stdin_). Ciascun comando, per poter essere interpretato correttamente dall'editor di testo, deve rispettare la sintassi di seguito mostrata:

###*(ind1,ind2)*c : cambia il contenuto delle righe dei testo comprese tra _ind1_ e _ind2_. Le _(ind2-ind1)+1_ righe devono essere inserite assicurandosi prima di essere andati a capo nel terminale. Se gli indici delle righe indicate non esistono ancora nel documento, il programma provvederà a inserire queste nuove righe in coda al documento. 




