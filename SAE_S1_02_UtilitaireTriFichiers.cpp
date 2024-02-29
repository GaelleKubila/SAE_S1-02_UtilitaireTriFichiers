//SAE S1.02 - Gaëlle KUBILA, Chaïma HANNOU, Laïla ALHOUSSEINI

//Programme permettant de ranger la liste demandée et de produire un fichier out (niveau 2)

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define TAILLE_MAX 30 // Max 30 caractères dans une chaine

///// structure d'étudiant

typedef struct {

    char *prenom;
    char *nom;
    int numetud;

}etudiant;

////////// FONCTIONS BASIQUES

// fonction utilisée pour simplifier l'échange des deux valeurs

void swap(etudiant *a, etudiant *b) {

    etudiant temp;

    temp = *b;
    *b = *a;
    *a = temp;

}

// note : cette fonction nous permet de contourner les problèmes de gestion de la mémoire causant des doublons de strcpy, grâce à l'usage de pointeurs

//////// FONCTIONS DE TRI

char *minimum(etudiant *tab, int deb, int fin) { //donne la première chaine dans l'ordre alphabétique d'un tableau de chaine de caracteres (fonction necessaire pour le tri simple)


    char *min; // on stockera ici la chaîne de charactère la plus petite, càd qui vient en premier dans l'alphabet
    int i;

    min=tab[deb].nom; // on initialise le minimum pour la première valeur du tableau

    for(i=deb; i<fin; i++) { // on itère pour chaque chaine du tableau

        if (strcmp(min,tab[i].nom)>0) { // si la chaine sélectionnée vient avant le minimum dans l'alphabet...
            min=tab[i].nom; // elle devient le minimum
        }
    }

    return min;

}

void unpassage(etudiant *tab, int deb, int fin) { // effectue un passage d'échanges à effectuer sur les valeurs du tableau lors du tri à bulles.

    int k;

    for(k=deb; k<(fin-1); k++) { // on itère pour chaque chaine du tableau

        if(strcmp(tab[k].nom,tab[k+1].nom)>0) { //si la chaine a vient après la chaine a+1...

            // ...on échange la place des deux

            swap(&tab[k],&tab[k+1]); // note : ne pas utiliser strcpy!!! elle cause des doublons.

        }

    }

}

void trisimple(etudiant *tab, int deb, int fin) { // effectue un tri sur tab selon la méthode "simple", entre deb et fin

    int i, j, val;
    char *min;

    val=fin-deb; // on en a besoin pour déterminer combien de fois boucler (meme si on ne commence pas à l'élément 0)

    for (j=0; j<val; j++) {

        min=minimum(tab, deb, fin); // on détermine le minimum du tableau grâce à la fonction précédente

        i=deb;

        while (strcmp(tab[i].nom,min)!=0) { // on recherche le rang du minimum du tableau

            i=i+1;

        }

        // trouvé!

        // enfin, on procède à l'échange : le minimum prend la première place, et inversement

        swap(&tab[deb],&tab[i]);

        deb=deb+1; // car on n'a plus besoin première valeur, désormais triée!

    }

}

void tribulle(etudiant *tab, int deb, int fin) { //effectue un tri sur tab selon la méthode "à bulles", entre deb et fin

    int i;

    while (fin>deb) { // tant que l'espace de tri n'est pas réduit à une seule chaine de caractres...

        for (i=deb; i<fin; i++) {

            unpassage(tab, deb, fin); // ...on effectue un passage...
        }

        fin=fin-1; //...et on n'oublie pas d'exclure le dernier élément de poids fort de l'espace de tri.

    }

}

///////// FONCTIONS SYSTEME

int extractionTab(etudiant *tab, FILE* fic) { //extrait du fichier in les lignes pour les ranger dans tableau

    int i, j, longChain;
    char nm[TAILLE_MAX] = ""; // et une chaîne vide de taille TAILLE_MAX - déclarée au début à 30 caractres
    char prenm[TAILLE_MAX] = ""; // idem
    int numet;


    i=0;

        while(fscanf(fic, "%s %s %d", &prenm, &nm, &numet)!=EOF) { // tant qu'on est pas arrivé à la fin de la liste :

            //on alloue de la place mémoire dans le tableau pour stocker les noms du fichier in, un à un.

            printf("%s %s %d\n", nm, prenm, numet);
            longChain=strlen(nm);
            tab[i].nom=(char *)malloc(sizeof(char)*(longChain+1));
            strcpy(tab[i].nom,nm);
            longChain=strlen(prenm);
            tab[i].prenom=(char *)malloc(sizeof(char)*(longChain+1));
            strcpy(tab[i].prenom,prenm);
            tab[i].numetud=numet;
            i=i+1;

        }



    // on vérifie que la liste est correcte

    for (j=0; j<i; j++) {

        printf("\n nom n.%d : %s ", j , tab[j].nom);

    }

    printf("\n");

    //on ferme le fichier in : on n'en a plus besoin

        fclose(fic);

    return i;
}

void demandeTri(etudiant *tab, int nbelem) { // alternative demandant à l'utilisateur le tri préféré et l'exécute

    int choix, j;

    choix=0;

        // saisie controlée : on demande à l'utilisateur le tri voulu

    do {

        printf("Quel tri voulez-vous choisir?\n");
        printf("1 - tri simple | 2 - tri à bulles\n");

        scanf("%d", &choix);
    }

    while((choix!=1) && (choix!=2));

    printf("\n");
    printf("Tri en cours\n");

    //on effectue le tri en conséquence de ce choix

    if (choix==1) {

        trisimple(tab, 0, nbelem);
    }

    else {

        tribulle(tab, 0, nbelem);
    }

    printf("Tri terminé\n");

    for (j=0; j<nbelem; j++) {

        printf("etudiant n.%d : %s %s %d\n", j , tab[j].nom, tab[j].prenom, tab[j].numetud);

    }


}

void ecritureFich(etudiant *tab, int nbelem, FILE* fic) { //écrit le tout trié dans le fichier out et libère la mémoire occupée dans tab

    int j;

    // une fois le tableau trié, on écrit un à un, chaque bom dans le fichier out.

    for (j=0; j<nbelem; j++) {

        fprintf(fic, "%s %s %d\n", tab[j].prenom, tab[j].nom, tab[j].numetud);

    }

    printf("Données copiées dans le fichier out\n");

    // on ferme le fichier out, on n'en a plus besoin

    fclose(fic);

    // on libère la mémoire pour chaque chaine de caractere

    for (j=0; j<nbelem; j++) {

        free(tab[j].nom);
        free(tab[j].prenom);

    }

    printf("Mémoire libérée!\n");

}

void finProgramme() { // esthétique juste pour la fin du code

    printf("Fin du programme...\n");
    printf("SAE S1.02 - Gaelle KUBILA, Chaima HANNOU, Laila ALHOUSSEINI\n");
}

////// PROCEDURE PRINCIPALE

int main() {

    int nbchar;
    etudiant liste[32767]; // on declare le tableau des noms des etudiants (maximum 32767, valeur max d'un int en C)
                        // donc maximum 32767 etudiants, mais avec un nom infiniment long?

    FILE* fichier = NULL;
    FILE* fichier2 = NULL;
    fichier = fopen("FileIN.txt", "r"); // le fichier in, accessible en lecture
    fichier2 = fopen("FileOUT.txt", "w"); // le fichier out, accessible en ecriture

    if (fichier == NULL) { // si le fichier in n'existe pas :
        printf("Erreur : FileIN inexistant!!!\n");
    }
    else {
        nbchar=extractionTab(liste, fichier);
        demandeTri(liste,nbchar);
        ecritureFich(liste,nbchar,fichier2);
    }

    finProgramme();


    return 0;
}