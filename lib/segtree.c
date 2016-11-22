#include <linux/segtree.h>
#include <linux/sched/prio.h>
#include <linux/hrtimer.h>
#include <linux/export.h>

int segtree_before(struct segtree_node first, struct segtree_node second)
{
	if (first.timer == NULL)
		return 0;
	if (second.timer == NULL)
		return 1;
	return ktime_before(first.timer->node.expires,
			second.timer->node.expires);
}

void segtree_update(int index, struct segtree_node val, struct segtree_node *st)
{
	index = index + MAX_PRIO;
	st[index] = val;

	for (index >>= 1; index; index >>= 1) {
		if (segtree_before(st[index << 1], st[(index << 1) + 1]))
			st[index] = st[index << 1];
		else
			st[index] = st[(index << 1) + 1];
	}
}
EXPORT_SYMBOL_GPL(segtree_update);

struct hrtimer *segtree_query(int cur_prio, struct segtree_node *st)
{
	int low_prio = MAX_PRIO;
	struct segtree_node ret = st[low_prio];

	for (cur_prio += MAX_PRIO; low_prio < cur_prio;
			cur_prio >>= 1, low_prio >>= 1) {
		if (low_prio & 1) {
			if (segtree_before(st[low_prio], ret))
				ret = st[low_prio];
			low_prio++;
		}
		if (cur_prio & 1) {
			cur_prio--;
			if (segtree_before(st[cur_prio], ret))
				ret = st[cur_prio];
		}
	}

	return ret.timer;
}
EXPORT_SYMBOL_GPL(segtree_query);
