**Nume: Daraban Timotei, Mihaicuta Iulia;**
**Grupa: 314CA**

## Tema 3 - Simple Linux File System

### Descriere:

Programul are ca scop implementarea unui set de comenzi, asemanatoare cu cele
folosite de terminalul de tip bash, pentru interactiunea cu un sistem de
fisiere; avem implementate urmatoarele comenzi:

> ls: comanda listeaza fisierele din directorul curent, sau un altul,
    specificat
>> * daca comanda este insotita de un argument, verificam daca
    fisierul cerut exista cu ajutorul functei findNode()
>> * se va afisa continutul fisierului sau al directorului, daca exista
>> * in caz contrar, afisam mesajul de eroare

> pwd: afiseaza calea absoluta catre directorul curent
>> * cu ajutorul functiei printPath(), obtinem calea absoluta, pe care o
    afisam

> cd: schimba directorul curent, dupa o cale relativa data
>> * parsam stringul ce contine calea relativa cu parsePath()
>> * obtinem fisierul final cu findFile() si verificam daca acesta este un
    director sau daca exista

> tree: afisam ierarhia de directoare
>> * daca comanda este insotita de un argument, salvam in variabila Folder
    nodul final din calea primita
>> * in caz contrar, variabila Folder va contine nodul curent
>> * afisam ierarhia de fisiere cu functia recursiva treePrint()

> mkdir: adauga un nou director in ierarhia de fisiere
>> * folosim findNode() pentru a verifica daca exista un director cu numele dat
>> * cream folderul cu functia createFolder() si il adaugam ierarhiei de fisiere
    folosind addNode()

> rmrec: sterge un director si tot continutul acestuia
>> * eliminam directorul din sistemul de fisiere folosind removeNode()
>> * eliberam memoria folosita cu macro-ul FREE_FILE_OR_DIR()

> rm: sterge un fisier
>> * verifica daca numele introdus caracterizeaza un fisier sau un director
>> * folosim removeNode si FREE_FILE_OR_DIR() pentru a sterge fisierul si a
    elibera memoria folosita de acesta

> rmdir: sterge un folder in memorie daca acesta este gol
>> * obtinem directorul cu functia findNode() si verificam daca acesta este cu
    adevarat un folder si daca continutul acestuia este gol
>> * folosim removeNode si FREE_FILE_OR_DIR() pentru a sterge directorul si a
    elibera memoria folosita de acesta

> touch: adaugam un nou fisier in ierarhia acestora
>> * folosim findNode() pentru a verifica daca fiserul dat exista in ierarhie
>> * in caz contrar, cream noul fisier cu functia createFile() si il adaugam
    sistemului de fisiere cu addNode()

> cp: copiaza un fisier la o cale data
>> * parsam caile primite
>> * se verifica daca argumenetele oferite sunt valide
>> * daca destinatia este un director, folosim touch() pentru a crea copia
    fisierului dat
>> * daca destinatia este un fisier, eliberam memoria alocata de continutul
    initial si salvam noua valoare a acestuia

> mv: mutam un fisier/director la o cale data
>> * parsam caile primite
>> * se verifica daca argumenetele oferite sunt valide
>> * daca comanda este aplicata unui fisier, putem intalni doua cazuri:
>>> * daca la adresa data, nu exista un fisier cu acelasi nume, eliminam fiserul
    initial - removeNode(), si cream unul nou, cu acelasi continut si nume - touch(),
    la calea data
>>> * in caz contrat, eliminam fisierul initial, urmand sa inlocuim continutul
    fisierului destinatie
>> * in cazul directoarelor, le eliminam din memorie, urmand sa le adaugam la
    locatia dorita folosind addNode()

### Comentarii asupra temei

O imbunatatire ce poate fi adusa programului, din punct de vedere al manipularii
memoriei, consta in implementarea unei solutii ce, la folosirea comenzii mv nu
sterge si creaza din nou fisierele in cauza, ci doar schimba pointerii ce li se
adreseaza.