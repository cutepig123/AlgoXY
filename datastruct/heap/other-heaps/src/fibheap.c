/*
 * fibheap.c, Fibonacci heap
 * Copyright (C) 2011, Liu Xinyu (liuxinyu95@gmail.com)

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * Based on Thomas H. Cormen, Charles E. Leiserson, Ronald L. Rivest 
 * and Clifford Stein. ``Introduction to Algorithms, Second Edition.''
 *  Chapter 20, Fibonacci heaps. The MIT Press 2001, ISBN:0262032937
 */

#include <stdlib.h>
#include <stdio.h>

#define BIG_RAND()  (rand() % 10000) /* for verification purpose only. */

typedef int Key;

/* Definition of tree */
struct node{
  Key key;
  struct node *next, *prev, *parent, *children;
  int degree; /* As known as rank */
  int mark;
};

struct FibHeap{
  struct node *roots;
  struct node *minTr; /* Point to tree contains the minimum element in root */
  int n; /* number of nodes */
};

/* Auxiliary functions */
struct node* singleton(Key x){
  struct node* t = (struct node*)malloc(sizeof(struct node));
  t->key = x;
  t->parent = t->children = 0;
  t->prev = t->next = t;
  t->degree = 0;
  t->mark = 0;
  return t;
}

/* Create an empty heap */
struct FibHeap* empty(){
  struct FibHeap* h = (struct FibHeap*)malloc(sizeof(struct FibHeap));
  h->roots = 0;
  h->minTr = 0;
  h->n = 0;
  return h;
}

/* Helper function to release a tree */
void destroy_tr(struct node* x){
  struct node *y, *z;
  if(x->children){
    y = x->children;
    do{
      z = y;
      y = y->next;
      destroy_tr(z);
    }while(y != x->children);
  }
  free(x);
}

/* Helper function to release a heap */
void destroy_heap(struct FibHeap* h){
  struct node *x, *y;
  if(h && h->roots){
    x = h->roots;
    do{
      y = x;
      x = x->next;
      destroy_tr(y);
    }while(x != h->roots);
  }
  free(h);
}

/* swap two nodes */
void swap(struct node** x, struct node** y){
  struct node* p = *x;
  *x = *y;
  *y = p;
}

/* Auxiliary functions for verification */

void print_tr(struct node* t){
  struct node* x;
  if(t){
    printf("%d", t->key);
    if(t->mark)
      printf("*");
    if(t->children){
      printf("-(");
      x=t->children;
      do{
	print_tr(x);
	printf(", ");
	x = x->next;
      }while(x!=t->children);
      printf(")");
    }
  }
}

void print_heap(struct FibHeap* h){
  struct node* x;
  if(h->roots){
    x=h->roots;
    do{
      if(x==h->minTr)
	printf("[min]");
      print_tr(x);
      printf("-->");
      x = x->next;
    }while(x != h->roots);
  }
  printf(".\n");
}

/* Concatenate 2 doubly linked lists */
struct node* concat(struct node* first1, struct node* first2){
  struct node* last1 = first1->prev;
  struct node* last2 = first2->prev;
  last1->next = first2;
  first2->prev = last1;
  last2->next = first1;
  first1->prev = last2;
  return first1;
}

/* Append a new node to the tail of a doubly linked list */
struct node* append(struct node* first, struct node* x){
  if(!first)
    return x;
  struct node* last = first->prev;
  last->next = x;
  x->prev = last;
  x->next = first;
  first->prev = x;
  return first;
}

/* Remove a node from a doubly linkied list */
struct node* remove_node(struct node* first, struct node* x){
  struct node *p, *n;
  if(x->next == x && first == x)
    first = 0; /* empty */
  else{
    p = x->prev;
    n = x->next;
    p->next = n;
    n->prev = p;
    x->next = x;
    x->prev = x;
    if(x == first)
      first = n;
  }
  return first;
}

struct FibHeap* add_tree(struct FibHeap* h, struct node* t){
  if(!h)
    h = empty();
  h->roots = append(h->roots, t);
  return h;
}

/* Insertion. O(1) */
struct FibHeap* insert(struct FibHeap* h, Key x){
  struct node* t = singleton(x);
  h = add_tree(h, t);
  if(h->minTr==0 || x < h->minTr->key)
    h->minTr = t;
  h->n++;
  return h;
}

/* top, finding the minimum element. O(1) */
Key top(struct FibHeap* h){
  return h->minTr->key;
}

/* 
 * merge. O(1)
 * note that the two heaps passed in are destroyed after merging.
 */
struct FibHeap* merge(struct FibHeap* h1, struct FibHeap* h2){
  struct FibHeap* h;
  if(!h1)
    return h2;
  if(!h2)
    return h1;
  h = empty();
  h->roots = concat(h1->roots, h2->roots);
  if(h1->minTr->key < h2->minTr->key)
    h->minTr = h1->minTr;
  else
    h->minTr = h2->minTr;
  h->n = h1->n + h2->n;
  free(h1);
  free(h2);
  return h;
}

/*
 * Link 2 trees to one bigger tree.
 */

struct node* link(struct node* x, struct node* y){
  if(y->key < x->key)
    swap(&x, &y);
  x->children = append(x->children, y);
  y->parent = x;
  x->degree++;
  y->mark = 0;
  return x;
}

/* 
 * calculate the upper limit of maximum degree
 *
 * Corollary 20.4. in CLRS,
 * For any node x in a N-node Fibonacci heap, 
 *  N >= size(x) >= F_{k+2} >= \Phi^k
 * Since N is an integer, we have k <= \lfloor \log_{\Phi} N \rfloor
 *
 * We can use Fibonacci sequence to calculate k.
 */
int max_degree(int n){
  int k, F;
  int F2 = 0; 
  int F1 = 1;
  for(F=F1+F2, k=2; F<n; ++k){
    F2 = F1;
    F1 = F;
    F = F1 + F2;
  }
  return k-2;
}

/*
 * Consolidate 
 */

void consolidate(struct FibHeap* h){
  if(!h->roots)
    return;
  int D = max_degree(h->n)+1;
  struct node *x, *y;
  struct node** a = (struct node**)malloc(sizeof(struct node*)*(D+1));
  int i, d;
  for(i=0; i<=D; ++i)
    a[i] = 0;
  while(h->roots){
    x = h->roots;
    h->roots = remove_node(h->roots, x);
    d= x->degree;
    while(a[d]){ 
      y = a[d];  /* Another node has the same degree as x */
      x = link(x, y);
      a[d++] = 0;
    }
    a[d] = x;
  }
  h->minTr = 0;
  h->roots = 0;
  for(i=0; i<=D; ++i)
    if(a[i]){
      h->roots = append(h->roots, a[i]);
      if(h->minTr == 0 || a[i]->key < h->minTr->key)
	h->minTr = a[i];
    }
  free(a);
}

/* 
 * Extract the minimum element. (pop). 
 * O(D(N)) = O(\lg N) amortized. 
 */
void pop(struct FibHeap* h){
  struct node* x = h->minTr;
  struct node *y, *child;
  if(x){
    child = x->children;
    if(child){
      do{
	y = child;
	child = child->next;
	append(h->roots, y);
	y->parent = 0;
      }while(child != x->children);
    }
    h->roots = remove_node(h->roots, x);
    h->minTr = 0;
    h->n--;
    consolidate(h);
    free(x);
  }
}

void heap_sort(int* xs, int n){
  int i;
  struct FibHeap* h = 0;
  for(i=0; i<n; ++i)
    h = insert(h, xs[i]);
  for(i=0; i<n; ++i){
    xs[i] = top(h);
    pop(h);
  }
  destroy_heap(h);
}

/* testing */

int sorted(const int* xs, int n){
  int i;
  for(i=0; i<n-1; ++i)
    if(xs[i+1] < xs[i])
      return 0;
  return 1;
}

void print_lst(const int* xs, int n){
  int i;
  for(i=0; i<n; ++i)
    printf("%d, ", xs[i]);
  printf("\n");
}

int test_heap_sort(){
  int m = 100;
  int i, n, *xs, res;
  do{
    n = 1 + BIG_RAND();
    xs = (int*)malloc(sizeof(int)*n);
    for(i=0; i<n; ++i)
      xs[i] = BIG_RAND();
    heap_sort(xs, n);
    res = sorted(xs, n);
    free(xs);
  }while(res && m--);
  return res;
}

int test_merge(){
  /* TODO: test merge operation*/
  return 1;
}

int main(){
  int r = test_heap_sort() && test_merge(); 
  return r;
}