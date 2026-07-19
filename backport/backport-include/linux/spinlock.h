#ifndef __BACKPORT_SPINLOCK_H
#define __BACKPORT_SPINLOCK_H
#include_next <linux/spinlock.h>


#if LINUX_VERSION_IS_LESS(4,16,0)
int alloc_bucket_spinlocks(spinlock_t **locks, unsigned int *lock_mask,
			   size_t max_size, unsigned int cpu_mult,
			   gfp_t gfp);

void free_bucket_spinlocks(spinlock_t *locks);
#endif /* LINUX_VERSION_IS_LESS(4,16,0) */

#if LINUX_VERSION_IS_LESS(4,19,0)
int __alloc_bucket_spinlocks(spinlock_t **locks, unsigned int *lock_mask,
			     size_t max_size, unsigned int cpu_mult,
			     gfp_t gfp, const char *name,
			     struct lock_class_key *key);

#define alloc_bucket_spinlocks(locks, lock_mask, max_size, cpu_mult, gfp)    \
	({								     \
		static struct lock_class_key key;			     \
		int ret;						     \
									     \
		ret = __alloc_bucket_spinlocks(locks, lock_mask, max_size,   \
					       cpu_mult, gfp, #locks, &key); \
		ret;							\
	})
#endif /* LINUX_VERSION_IS_LESS(4,19,0) */

/*
 * The _bh guard variants were added in v6.15 and backported to v6.12.37.
 * DEFINE_LOCK_GUARD_1() itself only exists since v6.5, so older kernels are
 * left alone, they cannot use guard() anyway.
 */
#if LINUX_VERSION_IS_GEQ(6,5,0) &&			\
    !LINUX_VERSION_IN_RANGE(6,12,37, 6,13,0) &&		\
    LINUX_VERSION_IS_LESS(6,15,0)
DEFINE_LOCK_GUARD_1(raw_spinlock_bh, raw_spinlock_t,
		    raw_spin_lock_bh(_T->lock),
		    raw_spin_unlock_bh(_T->lock))

DEFINE_LOCK_GUARD_1(spinlock_bh, spinlock_t,
		    spin_lock_bh(_T->lock),
		    spin_unlock_bh(_T->lock))

/* Conditional guards were only introduced in v6.8. */
#if LINUX_VERSION_IS_GEQ(6,8,0)
DEFINE_LOCK_GUARD_1_COND(raw_spinlock_bh, _try, raw_spin_trylock_bh(_T->lock))

DEFINE_LOCK_GUARD_1_COND(spinlock_bh, _try,
			 spin_trylock_bh(_T->lock))
#endif /* LINUX_VERSION_IS_GEQ(6,8,0) */
#endif /* _bh guard variants */

#endif /* __BACKPORT_SPINLOCK_H */
