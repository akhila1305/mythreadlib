#ifndef _MLIST_H
#define _MLIST_H

struct list_head {
  struct list_head *next,*prev;
};

#define DECLARE_MLIST_HEAD(head)  \
struct list_head head = { &(head),&(head) };


#define MLIST_EMPTY(head)   ( (head) == (head)->next )

#define INIT_MLIST_HEAD(ptr) \
do { \
(ptr)->next = ptr ; \
(ptr)->prev = ptr ; \
}while(0)


/*Add an element between a and b */

#define __list_add(e,a,b) \
do { \
(e)->next = b;\
(e)->prev = a;\
(a)->next = e;\
(b)->prev = e;\
}while(0)

/*Arrange after element deletion */

#define __list_del(a,b) \
do { \
(a)->next = b;\
(b)->prev = a;\
}while(0)

/*Add an element to the head */

static __inline__ void list_add(struct list_head *e,struct list_head *h) {
  struct list_head *n = h->next;
  __list_add(e,h,n);
}

static __inline__ void list_add_tail(struct list_head *e,struct list_head *h) {
  struct list_head *p = h->prev;
  __list_add(e,p,h); 
}

static __inline__ void list_del(struct list_head *e) {
  struct list_head *a = e->prev;
  struct list_head *b = e->next;
  __list_del(a,b); //rearrange 
}

static __inline__ void list_del_init(struct list_head *e) {
  list_del(e); 
  INIT_LIST_HEAD(e); //reinitialise the list head
  return ;
}

/*join the contents of the 2nd list after the end of the 1st list into the 1st list.*/

static __inline__ void list_splice(struct list_head *a,struct list_head *b) {
  if(! MLIST_EMPTY(b) && ! MLIST_EMPTY(a) ) {
    struct list_head *a_next = a->next; 
    struct list_head *b_first =b->next;
    struct list_head *b_last = b->prev;
  
    a->next = b_first; 
    b_first->prev = a;
    b_last->next = a_next;
    a_next->prev = b_last;

  }
  return ;
}

#define list_for_each(pos,head) \
for(pos = (head)->next; pos != (head); pos = pos->next)

#define list_entry(list_head,typecast,member) \
(typecast *) ( (unsigned char *)(list_head) -  ( (unsigned long)&((typecast *)0)->member) )

 
#endif
