// relations.c: définit le point d'entrée pour l'application .
//

typedef int bool;
#define false 0
#define true -1

#include "stdlib.h"
#include "memory.h"
#include "stdio.h"
#include "string.h"

////////////////////////////////////////
// Exercice 1: Classement des Relations

typedef enum {
	FRERE = 2, COUSIN, PARENT, ONCLE, EPOUX, AMI, VIT, CONNAIT,
	CHEF, COLLEGUE, LOCATAIRE, TRAVAILLE, PROPRIETAIRE, SITUE, DECOUVERT
} rtype;


typedef struct s_KeyValue {
    rtype key;
    char* value;
} KeyValue;

KeyValue table[] = {
    { FRERE, "frère ou soeur de" },
    { COUSIN, "cousin(e) de" },
    { PARENT, "parent de" },
    { ONCLE, "oncle/tante de" },
    { EPOUX, "epoux(se) de" },
    { AMI, "ami(e) de" },
    { VIT, "vit avec" },
    { CONNAIT, "connait" },
    { CHEF, "chef(fe) de" },
    { COLLEGUE, "collegue de" },
    { LOCATAIRE, "locataire de" },
    { TRAVAILLE, "travaille " },
    { PROPRIETAIRE, "proprietaire de" },
    { SITUE, "situe(e) à" },
    { DECOUVERT, "decouvert" }
};


#define TABLE_SIZE (sizeof(table) / sizeof(table[0]))


bool est_lien_parente(rtype id) {
    return (id >= 2 && id <= 6);
}

bool est_lien_professionel(rtype id) {
    return (id == CHEF || id == COLLEGUE);
}

bool est_lien_connaissance(rtype id) {
    return (id >= 7 && id <= 9);
}

char* toStringRelation(rtype id) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (table[i].key == id) {
            return table[i].value;
        }
    }
    return "";
}

////////////////////////////////////////
// Exercice 2: Liste de pointeurs

typedef struct s_node {
	void *val;  // pointeur vers objet quelconque
	struct s_node *suiv;
} *listeg;

listeg listegnouv() {
	return NULL;
}

listeg adjtete(listeg lst, void *x) {
	listeg ajoute = malloc(sizeof(struct s_node));
	ajoute->val = x;
	ajoute->suiv = lst;
	return ajoute;
}

listeg adjqueue(listeg lst, void *x) {
	listeg parcours = lst;
	while (parcours != NULL) {
		parcours = parcours->suiv;
	}
	parcours = malloc(sizeof(struct s_node));
	parcours->val = x;
	parcours->suiv = NULL;
	return lst;
}

bool estvide(listeg lst) {
	return lst == NULL;
}

listeg suptete(listeg lst) {
	if (estvide(lst)) {
		return lst;
	}
	listeg deuxieme = lst->suiv;
	free(lst);
	return deuxieme;
}

listeg rech(listeg lst, void *x, int(*comp)(void *, void *)) {
	listeg retour = listegnouv();
	while (!estvide(lst)) {
		if (comp(x, lst->val) == 0) {
			adjtete(retour, lst->val);
		}
		lst = lst->suiv;
	}
	return retour;
}

void *tete(listeg lst) {
	return lst->val;
}

int longueur(listeg lst) {
	int longueur=0;
	while (lst != NULL) {
		longueur++;
		lst = lst->suiv;
	}
	return longueur;
}

void detruire(listeg lst) {
	while (!estvide(lst)) {
		lst = suptete(lst);
	}
}

bool contiens(listeg lst, void *x) {
	listeg parcours = lst;
	for (int i=0; i<longueur(lst); i++) {
		if (parcours->val == x) {
			return true;
		}
		parcours = parcours->suiv;
	}
	return false;
}

////////////////////////////////////////
// Exercice 3: Construction du graphe

#define LONG_NOM_MAX 64
typedef enum { PERSONNE=1, OBJET, ADRESSE, VILLE } etype;
typedef struct s_entite {
	char nom[LONG_NOM_MAX]; // le nom de l'entité p.ex à Peugeot 106 
	etype ident; // l'identifiant associé, p.ex OBJET
} *Entite;

//3.1 les structures de données
typedef struct s_sommet {
	listeg larcs;
	Entite x;
} *Sommet;

typedef struct s_arc {
	rtype t;
	Entite x;
} *Arc;

typedef struct s_relations {
	listeg l;
} *Relations;

//3.2 les constructeurs
Entite creerEntite(char *s, etype e) {
	Entite retour = malloc(sizeof(struct s_entite));
	strcpy(retour->nom, s);
	retour->ident = e;
	return retour;
}

Sommet nouvSommet(Entite e) {
	Sommet retour = malloc(sizeof(struct s_sommet));
	retour->larcs = NULL;
	retour->x = e;
	return retour;
}

Arc nouvArc(Entite e, rtype type) {
	Arc retour = malloc(sizeof(struct s_arc));
	retour->t = type;
	retour->x = e;
	return retour;
}

void relationInit(Relations *g) {
	*g = malloc(sizeof(struct s_relations));
	(*g)->l = listegnouv();
}

void relationFree(Relations *g) {
	listeg parcours_sommets = (*g)->l;
	for (int i=0; i<longueur(parcours_sommets); i++) {
		Sommet s = parcours_sommets->val;
		free(s->x);
		listeg parcours_arcs = s->larcs;
		for (int j=0; j<longueur(parcours_arcs); j++) {
			Arc a = parcours_arcs->val;
			free(a->x);
			free(a);
			parcours_arcs = parcours_arcs->suiv;
		}
		detruire(s->larcs);
		free(s);
		parcours_sommets = parcours_sommets->suiv;
	}
	detruire((*g)->l);
}

//3.3 les comparaisons
int compEntite(void *e, void *string) {
	Entite entite = (Entite) e;
	char* cmp = (char*) string;
	return strcmp(entite->nom, cmp);
}

int compSommet(void *s, void *string) {
	Sommet sommet = (Sommet) s;
	return compEntite(sommet->x, string);
}

int compArc(void *a, void *string) {
	Arc arc = (Arc) a;
	return compEntite(arc->x, string);
}

int compArcRtype(void* arc, void* type) {
	Arc a = (Arc) arc;
	rtype r = (rtype) type;
	if (a->t == r) {
		return 0;
	}
	return -1;
}

//3.4 ajout d'entites et de relations

void adjEntite(Relations g, char *nom, etype t) {
	listeg recherche = rech(g->l, nom, compSommet);
	if (!estvide(recherche)) {
		detruire(recherche);
		return;
	}
	detruire(recherche);
	Sommet s = nouvSommet(creerEntite(nom, t));
	adjqueue(g->l, s);
}

// PRE CONDITION: id doit être cohérent avec les types des sommets correspondants à x et y
//                p.ex si x est de type OBJET, id ne peut pas etre une relation de parente
// PRE CONDITION: strcmp(nom1,nom2)!=0
void adjRelation(Relations g, char *nom1, char *nom2, rtype id) {
	listeg l_nom1 = rech(g->l, nom1, compSommet);
	listeg l_nom2 = rech(g->l, nom2, compSommet);
	if (estvide(l_nom1) || estvide(l_nom2)) {
		detruire(l_nom1);
		detruire(l_nom2);
		return;
	}
	Sommet s_nom1 = l_nom1->val;
	Sommet s_nom2 = l_nom2->val;
	etype type_nom1 = s_nom1->x->ident;
	etype type_nom2 = s_nom2->x->ident;
	listeg parcours = g->l;
	for (int i=0; i<longueur(g->l); i++) {
		Sommet s_act = parcours->val;
		if (strcmp(nom1, s_act->x->nom)) {
			listeg l_arc = rech(s_act->larcs, nom1, compArcRtype);
			if (l_arc == NULL)
				adjqueue(s_act->larcs, nouvArc(creerEntite(nom2, type_nom2), id));
			else {
				Arc a = (Arc) l_arc->val;
				a->t = id;
			}
		}
		else if (strcmp(nom2, s_act->x->nom)) {
			listeg l_arc = rech(s_act->larcs, nom2, compArcRtype);
			if (l_arc == NULL)
				adjqueue(s_act->larcs, nouvArc(creerEntite(nom1, type_nom1), id));
			else {
				Arc a = (Arc) l_arc->val;
				a->t = id;
			}
		}
		parcours = parcours->suiv;
	}
	detruire(l_nom1);
	detruire(l_nom2);
}

////////////////////////////////////////
// Exercice 4: Explorer les relations entre personnes

// 4.1 listes de relations
listeg en_relation(Relations g, char *x) {
	listeg parcours = g->l;
	for (int i=0; i<longueur(g->l); i++) {
		listeg recherche = rech(parcours->val, x, compSommet);
		if (!estvide(recherche)) {
			detruire(recherche);
			return ((Sommet)parcours->val)->larcs;
		}
		detruire(recherche);
		parcours = parcours->suiv;
	}
	return NULL;
}

bool chemin(Relations g, char *x, char *y) {
	listeg parcours = g->l;
	for (int i=0; i<longueur(g->l); i++) {
		listeg toutes_relations_x = en_relation(g, x);
		listeg parcours_relations = toutes_relations_x;
		for (int i=0; i<longueur(toutes_relations_x); i++) {
			if (strcmp(((Sommet)parcours_relations->val)->x->nom, y) == 0) {
				detruire(toutes_relations_x);
				return true;
			}
			parcours_relations = parcours_relations->suiv;
		}
		detruire(toutes_relations_x);
		parcours = parcours->suiv;
	}
	return false;
}

listeg chemin2(Relations g, char *x, char *y) {
	listeg retour = listegnouv();
	listeg x_relations = en_relation(g, x);
	listeg y_relations = en_relation(g, y);
	listeg parcours_x = x_relations;
	for (int i=0; i<longueur(x_relations); i++) {
		listeg parcours_y = y_relations;
		for (int j=0; j<longueur(y_relations); y++) {
			if (strcmp(((Arc)parcours_x->val)->x->nom, ((Arc)parcours_y->val)->x->nom) == 0) {
				retour = adjtete(retour, ((Arc)parcours_x->val)->x);
			}
			parcours_y = parcours_y->suiv;
		}
		parcours_x = parcours_x->suiv;
	}
	detruire(x_relations);
	detruire(y_relations);
	return retour;
}

// 4.2 verifier un lien de parente
// PRE CONDITION: strcmp(x,y)!=0
bool ont_lien_parente(Relations g, char *x, char *y) {
	listeg x_relation = en_relation(g, x);
	listeg parcours_x = x_relation;
	for (int i=0; i<longueur(x_relation); i++) {
		if ((strcmp(((Arc)parcours_x->val)->x->nom, y) == 0) && (((Arc)parcours_x->val)->t) <= 6) {
			return true;
		}
	}
	return false;
}

// 4.3 tester connaissances
// PRE CONDITION: les sommets correspondants à x et y sont de type PERSONNE
// PRE CONDITION: strcmp(x,y)!=0
bool se_connaissent(Relations g, char *x, char *y) {
	return false;
}
// PRE CONDITION: les sommets correspondants à x et y sont de type PERSONNE
// PRE CONDITION: strcmp(x,y)!=0
bool se_connaissent_proba(Relations g, char *x, char *y) {
	return false;
}
// PRE CONDITION: les sommets correspondants à x et y sont de type PERSONNE
// PRE CONDITION: strcmp(x,y)!=0
bool se_connaissent_peutetre(Relations g, char *x, char *y) {
	return false;
}

////////////////////////////////////////
// Exercice 5: Affichages

void affichelg(listeg l, void(*aff)(void *)) {

}

void afficheEntite(void *x) {

}
void afficheArc(void *x) {

}

////////////////////////////////////////
// Exercice 6: Parcours
void affiche_degre_relations(Relations r, char *x) {
    
}


int main()
{
    

    


	int i,j;
	Relations r; relationInit(&r);
	// ajouter les entites de l'exemple
	char *tabe[] = {"KARL","LUDOVIC","CELINE","CHLOE","GILDAS","CEDRIC","SEVERINE",
		"PEUGEOT 106" ,"1, RUE DE LA RUE","STRASBOURG" };
	for (i = 0; i < 7; i++) adjEntite(r, tabe[i], PERSONNE);
	adjEntite(r, tabe[7], OBJET);
	adjEntite(r, tabe[8], ADRESSE);
	adjEntite(r, tabe[9], VILLE);
	// ajouter les relations de l'exemple
	adjRelation(r, tabe[0], tabe[1], FRERE);
	adjRelation(r, tabe[0], tabe[2], AMI);
	adjRelation(r, tabe[0], tabe[3], CONNAIT);
	adjRelation(r, tabe[0], tabe[5], COUSIN);
	adjRelation(r, tabe[0], tabe[7], PROPRIETAIRE);
	adjRelation(r, tabe[0], tabe[8], PROPRIETAIRE);
	adjRelation(r, tabe[3], tabe[4], VIT);
	adjRelation(r, tabe[5], tabe[6], EPOUX);
	adjRelation(r, tabe[5], tabe[8], LOCATAIRE);
	adjRelation(r, tabe[7], tabe[8], DECOUVERT);
	adjRelation(r, tabe[8], tabe[9], SITUE);

	// explorer les relations
	printf("%s est en relation avec:\n", tabe[0]);
	affichelg(en_relation(r, tabe[0]),afficheArc);
	printf("\n");

	for (i = 0; i < 7; i++) for (j = i + 1; j < 10; j++) {
		printf("<%s> et <%s> ont les relations communes:\n", tabe[i], tabe[j]);
		affichelg(chemin2(r, tabe[i], tabe[j]), afficheEntite);
		printf("\n");
	}
	printf("\n\n");

	for (i = 0; i < 10; i++) for (j = i + 1; j < 10; j++) {
		printf("<%s> et <%s> ont lien de parente: %s\n",
			tabe[i], tabe[j], ont_lien_parente(r, tabe[i], tabe[j]) ? "vrai" : "faux");
	}
	printf("\n");
	for (i = 0; i < 7; i++) {
		for (j = i + 1; j < 7; j++) {
			printf("<%s> et <%s> se connaissent: %s\n",
				tabe[i], tabe[j], se_connaissent(r, tabe[i], tabe[j]) ? "vrai" : "faux");
			printf("<%s> et <%s> se connaissent tres probablement: %s\n",
				tabe[i], tabe[j], se_connaissent_proba(r, tabe[i], tabe[j]) ? "vrai" : "faux");
			printf("<%s> et <%s> se connaissent peut etre: %s\n",
				tabe[i], tabe[j], se_connaissent_peutetre(r, tabe[i], tabe[j]) ? "vrai" : "faux");
		}
		printf("\n");
	}

	affiche_degre_relations(r, tabe[3]);

	relationFree(&r);

	printf("\nPRESS RETURN\n");
	char buff[64]; fscanf(stdin, "%s", buff);
    return 0;
}
