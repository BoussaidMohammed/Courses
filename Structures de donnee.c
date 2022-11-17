#include <stdio.h>
#include <stdlib.h>
#define COUNT 10
typedef enum
{
    false = 0,
    true = 1,
}bool;

typedef int Element;
typedef struct noeud
{
    Element val;
    struct noeud *fg;
    struct noeud *fd;
}noeud,*Pnoeud,*ABR;

void print2DUtil(ABR A, int space) 
{ 
    int i;
    // Base case 
    if (A == NULL) 
        return; 
  
    // Increase distance between levels 
    space += COUNT; 
  
    // Process right child first 
    print2DUtil(A->fd, space); 
  
    // Print current node after space 
    // count 
    printf("\n"); 
    for (i = COUNT; i < space; i++) 
        printf(" "); 
    printf("%d\n", A->val); 
  
    // Process left child 
    print2DUtil(A->fg, space); 
} 
  
// Wrapper over print2DUtil() 
void afficher_ABR(ABR A) 
{ 
   // Pass initial space count as 0 
   print2DUtil(A, 0); 
}

ABR cree_ABR()
{
    return NULL;
}

bool est_vide(ABR a)
{
    return (a == NULL);
}

Pnoeud cree_noeud(Element e)
{
    Pnoeud p = (Pnoeud)malloc(sizeof(noeud));
    p->val = e;
    p->fg = p->fd = NULL;
    return p;
}

ABR construire(Pnoeud p,ABR g,ABR d)
{
    if(!est_vide(p)) 
    {
        p->fg = g;
        p->fd = d;
        return p;
    }
    return NULL;
}

ABR inserer(ABR a,Element e)
{
    if(est_vide(a))
    {
        Pnoeud p = cree_noeud(e);
        return p;
    }
    if(e < a->val) return construire(a,inserer(a->fg,e),a->fd);
    if(e > a->val) return construire(a,a->fg,inserer(a->fd,e));
    else return a;
}

ABR detruire(ABR a)
{
    if(est_vide(a)) return a;
    a->fg = detruire(a->fg);
    a->fd = detruire(a->fd);
    free(a); a = NULL;
    return a;
}

void afficher_croissant(ABR a)
{
    if(est_vide(a)) return;
    afficher_croissant(a->fg);
    printf("%d ",a->val);
    afficher_croissant(a->fd);
}

void afficher2(ABR a)
{
    if(est_vide(a))
    { 
        printf("_");
        return;
    }
    printf("{");
    afficher2(a->fg);
    printf(",%d,",a->val);
    afficher2(a->fd);
    printf("}");
}

void ABR2tab(ABR a,int *tab,int *n)//la convertion d'une arbre a un tableau
{
    if(est_vide(a)) return;
    ABR2tab(a->fg,tab,n);
    tab[*n] = a->val;
    (*n)++;
    ABR2tab(a->fd,tab,n);
}

bool verifie(ABR a)
{
    int tab[100];//tableau qui va contenir les noeud a l'ordre croissant,a l'aide de parcours infixe;
    int n = 0;
    ABR2tab(a,tab,&n);
    for(int i = 0;i < n-1;i++) if(tab[i+1] < tab[i]) return false;//si on trouve une une seul valeur inf a sa precedent,pas une ABR
    return true;
}

int taille(ABR a)
{
    if(est_vide(a)) return 0;
    return 1 + taille(a->fd) + taille(a->fg);
}

int max(int a,int b)
{
    return ( a > b ? a:b);
}

int hauteur(ABR a)
{
    if(est_vide(a)) return 0;
    return 1 + max(hauteur(a->fg),hauteur(a->fd));// on addition 1 sur le max hauteur des 2 arbres;
}

int somme(ABR a)
{
    if(est_vide(a)) return 0;
    return a->val + somme(a->fg) + somme(a->fd);
}

Pnoeud rechercher(ABR a,Element e)
{
    if(est_vide(a)) return NULL;
    if(e < a->val) return rechercher(a->fg,e);
    if(e > a->val) return rechercher(a->fd,e);
    return a;
}

Pnoeud maximum(ABR a)
{
    if(est_vide(a->fd)) return a;
    return maximum(a->fd);
}

ABR supp_max(ABR a)
{
    if(est_vide(a->fd)) return a->fg;
    return construire(a,a->fg,supp_max(a->fd));
}

ABR supprimer(ABR a,Element e)
{
    if(est_vide(a)) return a;
    if(e < a->val) return construire(a,supprimer(a->fg,e),a->fd);
    if(e > a->val) return construire(a,a->fg,supprimer(a->fd,e));
    else
    {
        if(est_vide(a->fg)) return a->fd;
        if(est_vide(a->fd)) return a->fg;
        else
        {
            Pnoeud p = maximum(a->fg);
            return construire(p,supp_max(a->fg),a->fd);
        }
    }
}

Pnoeud predecesseur(ABR a,Element e)
{
    static Pnoeud p ;//variable modifiable par fct predecesseur
    if(est_vide(a)) return NULL;//on n'a pas trouve notre valeur
    if(e < a->val) return predecesseur(a->fg,e);
    if(e > a->val)
    {
        p = a;//on sauvgarde le dernier element inferieur a notre element
        return predecesseur(a->fd,e);
    } 
    else
    {
        if(est_vide(a->fg)) return p;//pas de fils gauche, donc pas des valeur inferieur a notre valeur,donc predecesseur et on haut
        else return maximum(a->fg);//si non on renvoie le plus grand des fils gauche
    }
}

bool equivalents(ABR a,ABR b)
{
    if(est_vide(a)) return est_vide(b);//a est vide, si b vide renvoie true, sinon false
    else if(est_vide(b)) return false;//a n'est pas vide et b est vide --> renvoie false
    else if(a->val == b->val) return equivalents(a->fg,b->fg) && equivalents(a->fd,b->fd);
    return false;//a et b n'ont pas la meme valeur -->vaut false;
}

int main()
{
    ABR a = cree_ABR();
    a = inserer(a,5);
    a = inserer(a,3);
    a = inserer(a,9);
    a = inserer(a,4);
    a = inserer(a,6);
    a = inserer(a,1);

    afficher_ABR(a);
    Pnoeud p = predecesseur(a,5);
    if(!p) printf("NULL\n");
    printf("predecesseur = %d \n",p->val);
    system("pause");
    return 0;
}
