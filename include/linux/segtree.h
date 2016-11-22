#ifndef _LINUX_SEGTREE_H
#define _LINUX_SEGTREE_H

#include <linux/ktime.h>

struct segtree_node {
	struct hrtimer *timer;
};

extern int segtree_before(struct segtree_node first, struct segtree_node second);

extern void segtree_update(int index, struct segtree_node val, struct segtree_node *st);

extern struct hrtimer *segtree_query(int cur_prio, struct segtree_node *st);

#endif /* _LINUX_SEGTREE_H */
