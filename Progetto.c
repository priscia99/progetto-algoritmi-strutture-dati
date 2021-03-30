/*
 * PROGETTO DI ALGORITMI E STRUTTURE DATI
 * Andrea Prisciantelli
 * Anno Accademico: 2019/2020
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STR 1050    // Dimensione massima della stringa in ingresso (riga o comando)

// Struttura dati per le righe del documento
typedef struct R{
    char *contenuto;
    struct R *next;
    struct R *prev;
}Row;

// Struttura dati per la conservazione di comandi nelle pile di Undo/Redo
typedef struct C{
    char commandType;
    int posInizio;
    int posFine;
    int posChange;
    Row *toAddStart;
    Row *toAddEnd;
    Row *toChangeStart;
    Row *toChangeEnd;
    Row *toDeleteStart;
    Row *toDeleteEnd;
    struct C *next;
}Command;

// Funzioni del sistema
int readCommand(char *command);                     // Funzione per la lettura e analisi del comando in input
void changeRows(int inizio, int fine);              // Funzione per il comando di CHANGE
void deleteRows(int inizio, int fine);              // Funzione per il comando di DELETE
void printRows(int inizio, int fine);               // Funzione per il comando di PRINT
Row* getRow(int position);                          // Funzione per la restituzione della riga nella posizione cercata
Row* allocaRiga(char *newStr);                      // Funzione per l'allocazione di una generica riga
Row* inserisciRigaCoda(char *newStr);               // Funzione appoggio di changeRows per l'inserimento in coda
int accorpaUndoRedo(int number, char commandType);  // Funzione di accorpamento di comandi consecutivi di undo/redo
Command* newUndoCommand(char commandType);          // Funzione per la creazione di un elemento comando nella pila undo
void performUndo(int number);                       // Funzione per lo svolgimento di UNDO
void performRedo(int number);                       // Funzione per lo svolgimento di REDO
void resetRedoList();                               // Funzione che svuota il contenuto della pila delle Redo
void svuotaDocumento();                             // Svuota il documento nel caso di esecuzione del max numero undo

// Variabili globali del sistema
char inputCommand[MAX_STR];     // Per input dei comandi
char inputString[MAX_STR];      // Per input di stringhe in ingresso

Row *document = NULL;           // Puntatore alla root del documento
Row *lastRow = NULL;            // Puntatore all'ultima riga del documento
int numRows = 0;

Command *pilaUndo = NULL;       // Puntatore alla pila delle undo
Command *pilaRedo = NULL;       // Puntatore alla pila delle redo
int undoListLength = 0;         // Counter della dimensione di elementi nella pila undo
int redoListLength = 0;         // Counter della dimensione di elementi nella pila redo

int main(){
    do{
        fgets(inputCommand, MAX_STR, stdin);    // Lettura del comando in input
    }while(readCommand(inputCommand));          // Elaborazione del comando
    return 0;
}

int readCommand(char *command){
    int inizio, fine;
    char *tok_inizio;

    if(command[0] == 'q'){
        return 0;   // E' stato inserito il comando di terminazione
    }
    if(strstr(command, ",") != NULL){
        // Comando di change, delete, print
        char type = command[strlen(command)-2];         // Ricavo il tipo di comando
        tok_inizio = strtok(command, ",");
        inizio = atoi(tok_inizio);                      // Leggo gli estremi da usare nel comando
        fine = atoi(strtok(NULL, ","));

        switch(type){
            case 'c':
                changeRows(inizio, fine);
                break;
            case 'd':
                deleteRows(inizio, fine);
                break;
            case 'p':
                printRows(inizio, fine);
                break;
            default:
                break;
        }
    }
    else{
        // Devo eseguire dei comandi di UNDO/REDO
        char type = command[strlen(command)-2];
        int number = atoi(command);
        // Richiamo la funzione di accorpamento e ritorno il valore di uscita della prossima istruzione
        return accorpaUndoRedo(number, type);
    }
    return 1;
}

void changeRows(int inizio, int fine){
    resetRedoList();
    int rowCounter;
    Row *leftRow = NULL;
    Row *rightRow = NULL;
    Row *firstRow = NULL;
    Row *newRow = NULL;
    Row *prevRow = NULL;
    if(fine < inizio || inizio == 0){
        return;
    }
    Command *com = newUndoCommand('c');
    com->posInizio = inizio;
    com->posFine = fine;
    Row *startChangeRow = getRow(inizio);
    if(startChangeRow)
        leftRow = startChangeRow->prev;
    Row *endChangeRow = getRow(fine);
    if(endChangeRow)
        rightRow = endChangeRow->next;

    // Sicuramente ho delle righe da modificare
    if(inizio <= numRows) {
        com->toChangeStart = startChangeRow;
        com->toChangeEnd = startChangeRow;
        com->posChange = inizio;
    }

    for(rowCounter=inizio; rowCounter<=numRows && rowCounter<=fine; rowCounter++){
        fgets(inputString, MAX_STR, stdin);
        newRow = allocaRiga(inputString);
        if(firstRow==NULL){
            firstRow = newRow;
        }
        else{
            prevRow->next = newRow;
            newRow->prev = prevRow;
        }
        prevRow = newRow;
        com->toChangeEnd = startChangeRow;
        startChangeRow = startChangeRow->next;
    }
    com->posChange = rowCounter;
    // Se ho modificato qualche riga già presente nel documento allora devo sistemare i puntatori
    if(firstRow != NULL){
        // Ho modificato qualche riga già esistente
        if(leftRow != NULL){
            leftRow->next = firstRow;
            firstRow->prev = leftRow;
        }
        else{
            document = firstRow;
        }
    }

    // Inserisco eventuali nuove stringhe nel documento
    if(rowCounter == fine+1){
        // Non ho alcuna nuova riga da allocare
        newRow->next = rightRow;
        if(rightRow)
            rightRow->prev = newRow;
        else
            lastRow = newRow;
    }
    else {
        if(firstRow)
            lastRow = newRow;
        // Ho delle nuove righe da allocare nel documento
        for (rowCounter = rowCounter; rowCounter <= fine; rowCounter++) {
            fgets(inputString, MAX_STR, stdin);
            inserisciRigaCoda(inputString);
            if(com->toDeleteStart == NULL)
                com->toDeleteStart = lastRow;
            com->toDeleteEnd = lastRow;
        }
    }

    do{
        fgets(inputCommand, MAX_STR, stdin);
    }while(inputCommand[0] != '.');
    return;
}

void deleteRows(int inizio, int fine) {
    resetRedoList();
    if(fine > numRows)
        fine = numRows;

    Row *startRow = getRow(inizio);
    Row *endRow = getRow(fine);
    Row *leftRow = NULL;
    Row *rightRow = NULL;

    if(startRow)
        leftRow = startRow->prev;
    if(endRow)
        rightRow = endRow->next;

    if(inizio == 0 || inizio > numRows){
        newUndoCommand('v');    // Comando non valido ma da considerare -> 'v' = vuoto
        return;
    }

    Command *com = newUndoCommand('d');
    com->posInizio = inizio;
    com->posFine = fine;
    com->toAddStart = startRow;
    com->toAddEnd = endRow;

    if(leftRow && rightRow){
        leftRow->next = rightRow;
        rightRow->prev = leftRow;
    }
    else if(!leftRow && rightRow){
        document = rightRow;
        rightRow->prev = NULL;
    }
    else if(leftRow && !rightRow){
        lastRow = leftRow;
        leftRow->next = NULL;
    }
    else if(!leftRow && !rightRow){
        document = NULL;
        lastRow = NULL;
    }

    numRows = numRows - (fine-inizio+1);    // Decremento il numero di righe
    return;
}

void printRows(int inizio, int fine){
    int rowCounter;
    if(inizio == 0) {
        printf(".\n");  // Comando 'limite' che può capitare nel test
        return;
    }
    Row *currRow = getRow(inizio);  // L'idea è di usare la funzione di ricerca per sapere da dover partire
    for(rowCounter=inizio; rowCounter<=fine; rowCounter++){
        if(currRow != NULL){
            printf("%s", currRow->contenuto);
            currRow = currRow->next;
        }
        else{
            printf(".\n");
        }
    }
    return;
}

Row *getRow(int position){
    // Funzione per il ritorno del riferimento alla riga grazie alla posizione
    // O(n/2)
    if(position > numRows || position < 0)
        return NULL;    // Parametri non corretti -> torno NULL
    else if(position == 1)
        return document;    // Primo elemento della lista documento
    else if(position == numRows) {
        return lastRow;     // Ultimo elemento del documento
    }
    int rowCounter;
    Row* currRow = document;

    if(position>numRows/2){
        // Se la posizione è verso la fine inizio la lettura dal basso
        currRow = lastRow;
        for(rowCounter=numRows; rowCounter>=1; rowCounter--){
            if(rowCounter == position)
                return currRow;
            currRow = currRow->prev;
        }
        return NULL;
    }else {
        // Se la posizione è verso l'inizio inizio la lettura dall'alto
        for (rowCounter = 1; rowCounter <= numRows; rowCounter++) {
            if (position == rowCounter) {
                return currRow;
            }
            currRow = currRow->next;
        }
    }
    return NULL;
}

Row* allocaRiga(char *newStr){
    Row *row = (Row*) malloc(sizeof(Row));
    row->next = NULL;
    row->prev = NULL;
    row->contenuto = (char*) malloc(sizeof(char)*(strlen(newStr)+2));
    strcpy(row->contenuto, newStr);
    return row;
}

Row* inserisciRigaCoda(char *newStr){
    Row *row = allocaRiga(newStr);
    if(document == NULL){
        // Il documento inizialmente è vuoto
        document = row;
        lastRow = row;
    }
    else{
        lastRow->next = row;
        row->prev = lastRow;
    }
    lastRow = row;
    numRows++;
    return row;
}

int accorpaUndoRedo(int number, char commandType){
    int undoLetti = 0;
    int redoLetti = 0;
    char comandoProcessato;
    int numeroLetto;
    int diff;

    if(commandType == 'u'){
        if(number > undoListLength){
            undoLetti = undoListLength;
            undoListLength = 0;
            redoListLength += undoLetti;
        }
        else {
            undoLetti = number;
            undoListLength -= undoLetti;
            redoListLength += undoLetti;
        }
    }
    else if(commandType == 'r'){
        if(number > redoListLength){
            redoLetti = redoListLength;
            redoListLength = 0;
            undoListLength += redoLetti;
        }
        else{
            redoLetti = number;
            redoListLength -= redoLetti;
            undoListLength += redoLetti;
        }
    }

    fgets(inputCommand, MAX_STR, stdin);
    comandoProcessato = inputCommand[strlen(inputCommand)-2];
    while(comandoProcessato == 'u' || comandoProcessato == 'r'){
        numeroLetto = atoi(inputCommand);
        if(comandoProcessato == 'u'){
            if(numeroLetto > undoListLength){
                numeroLetto = undoListLength;
            }
            undoListLength -= numeroLetto;
            redoListLength += numeroLetto;
            undoLetti += numeroLetto;
        }
        else if(comandoProcessato == 'r'){
            if(numeroLetto > redoListLength){
                numeroLetto = redoListLength;
            }
            redoListLength -= numeroLetto;
            undoListLength += numeroLetto;
            redoLetti += numeroLetto;
        }
        fgets(inputCommand, MAX_STR, stdin);
        comandoProcessato = inputCommand[strlen(inputCommand)-2];
    }
    if(undoLetti > redoLetti){
        if(undoListLength == 0 && ( inputCommand[strlen(inputCommand)-2] == 'c' || inputCommand[strlen(inputCommand-2)] == 'd'))
            svuotaDocumento();
        else{
            diff = undoLetti - redoLetti;
            performUndo(diff);
        }
    }
    else if(redoLetti > undoLetti){
        diff = redoLetti - undoLetti;
        performRedo(diff);
    }
    // Eseguo il comando successivo
    if(inputCommand[0] == 'q')
        return 0;
    return readCommand(inputCommand);
}

void svuotaDocumento(){
    document = NULL;
    lastRow = NULL;
    numRows = 0;
    pilaUndo = NULL;
    undoListLength = 0;
}

void performUndo(int number){
    int counter;
    for(counter=1; counter<=number; counter++){
        Command *com = pilaUndo;

        if(com->commandType == 'c'){
            // Sto eseguendo UNDO di un comando di CHANGE
            Row *leftRow = NULL;
            Row *rightRow = NULL;
            Row *newStart;
            Row *newEnd;
            if(com->toChangeStart != NULL){
                leftRow = com->toChangeStart->prev;
                rightRow = com->toChangeEnd->next;
                // Devo re-inserire delle stringhe
                if(leftRow && rightRow){
                    newStart = leftRow->next;
                    newEnd = rightRow->prev;
                    leftRow->next = com->toChangeStart;
                    rightRow->prev = com->toChangeEnd;
                }
                else if(!leftRow && rightRow){
                    newStart = document;
                    newEnd = rightRow->prev;
                    document = com->toChangeStart;
                    rightRow->prev = com->toChangeEnd;
                }else if(leftRow && !rightRow){
                    newStart = leftRow->next;
                    newEnd = lastRow;
                    leftRow->next = com->toChangeStart;
                    lastRow = com->toChangeEnd;
                }else if(!leftRow && !rightRow){
                    newStart = document;
                    newEnd = lastRow;
                    document = com->toChangeStart;
                    lastRow = com->toChangeEnd;
                }
                com->toChangeStart = newStart;
                com->toChangeEnd = newEnd;
                if(com->toDeleteStart != NULL){
                    // Dovrebbe già essere eliminato - sistemo i puntatori
                    numRows = numRows - (com->posFine - com->posChange + 1);
                    com->toAddStart = com->toDeleteStart;
                    com->toAddEnd = com->toDeleteEnd;
                    com->toDeleteStart = NULL;
                    com->toDeleteEnd = NULL;
                }
            }
            else{
                // Devo semplicemente eliminare le stringhe appena allocate nel comando di change
                Row *leftRow = com->toDeleteStart->prev;
                if(leftRow){
                    lastRow = leftRow;
                    leftRow->next = NULL;
                }
                else{
                    document = NULL;
                    lastRow = NULL;
                }
                numRows = numRows - (com->posFine - com->posInizio + 1);
                com->toAddStart = com->toDeleteStart;
                com->toAddEnd = com->toDeleteEnd;
                com->toDeleteStart = NULL;
                com->toDeleteEnd = NULL;
            }


        }
        else if(com->commandType == 'd'){
            // Sto eseguendo REDO di un comando di DELETE
            Row *leftRow = com->toAddStart->prev;
            Row *rightRow = com->toAddEnd->next;
            if(leftRow && rightRow){
                leftRow->next = com->toAddStart;
                rightRow->prev = com->toAddEnd;
            }
            else if(!leftRow && rightRow){
                document = com->toAddStart;
                rightRow->prev = com->toAddEnd;
            }else if(leftRow && !rightRow){
                lastRow = com->toAddEnd;
                leftRow->next = com->toAddStart;
            }else if(!leftRow && !rightRow){
                document = com->toAddStart;
                lastRow = com->toAddEnd;
            }
            numRows = numRows + (com->posFine - com->posInizio + 1);
            com->toDeleteStart = com->toAddStart;
            com->toDeleteEnd = com->toAddEnd;
            com->toAddStart = NULL;
            com->toAddEnd = NULL;
        }
        // Sposto il comando nella pila delle redo
        pilaUndo = pilaUndo->next;
        com->next = pilaRedo;
        pilaRedo = com;
    }
    return;
}

void performRedo(int number){
    int counter;
    for(counter=1; counter<=number; counter++){
        Command *com = pilaRedo;

        if(com->commandType == 'c'){
            if(com->toChangeStart == NULL && com->toAddStart!=NULL){
                // Devo solo ri-aggiungere elementi
                if(lastRow) {
                    lastRow->next = com->toAddStart;
                    com->toAddStart->prev = lastRow;
                    lastRow = com->toAddEnd;
                }else{
                    document = com->toAddStart;
                    lastRow = com->toAddEnd;
                }
                com->toDeleteStart = com->toAddStart;
                com->toDeleteEnd = com->toAddEnd;
                com->toAddStart = NULL;
                com->toAddEnd = NULL;
                numRows = numRows + (com->posFine - com->posInizio +1);
            }
            else{
                // Ci sono anche degli elementi da modificare
                Row *leftRow = com->toChangeStart->prev;
                Row *rightRow = com->toChangeEnd->next;
                Row *newStart;
                Row *newEnd;
                if(leftRow != NULL){
                    newStart = leftRow->next;
                    leftRow->next = com->toChangeStart;
                }else{
                    newStart = document;
                    document = com->toChangeStart;
                }
                if(com->toAddStart == NULL){
                    if(rightRow != NULL){
                        newEnd = rightRow->prev;
                        rightRow->prev = com->toChangeEnd;
                    }else{
                        newEnd = lastRow;
                        lastRow = com->toChangeEnd;
                    }
                }else{
                    newEnd = lastRow;
                    lastRow = com->toAddEnd;
                    numRows = numRows + (com->posFine - com->posChange+1);
                }
                com->toChangeStart = newStart;
                com->toChangeEnd = newEnd;
                com->toDeleteStart = com->toAddStart;
                com->toDeleteEnd = com->toAddEnd;
                com->toAddStart = NULL;
                com->toAddEnd = NULL;

            }
        }else if(com->commandType == 'd'){
            Row *leftRow = com->toDeleteStart->prev;
            Row *rightRow = com->toDeleteEnd->next;

            if(leftRow && rightRow){
                leftRow->next = rightRow;
                rightRow->prev = leftRow;
            }else if(!leftRow && rightRow){
                document = rightRow;
                rightRow->prev = NULL;
            }else if(leftRow && !rightRow){
                leftRow->next = NULL;
                lastRow = leftRow;
            }else if(!leftRow && !rightRow){
                document = NULL;
                lastRow = NULL;
            }
            numRows = numRows - (com->posFine - com->posInizio +1);
            com->toAddStart = com->toDeleteStart;
            com->toAddEnd = com->toDeleteEnd;
            com->toDeleteStart = NULL;
            com->toDeleteEnd = NULL;
        }

        // Sposto il comando nella pila delle undo
        pilaRedo = pilaRedo->next;
        com->next = pilaUndo;
        pilaUndo = com;
    }
}

Command* newUndoCommand(char type){
    // Alloco un nuovo comando e copio il tipo nella variabile corrispondente
    Command *new = (Command*) calloc(1, sizeof(Command));
    new->commandType = type;
    if(pilaUndo != NULL){
        new->next = pilaUndo;   // Esiste già un comando all'interno della pila undo
    }
    pilaUndo = new;
    undoListLength++;   // Incremento il numero di elementi in pila
    return new;
}

void resetRedoList(){
    redoListLength = 0;
    pilaRedo = NULL;
    return ;
}
