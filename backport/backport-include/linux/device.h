#ifndef __BACKPORT_DEVICE_H_
#define __BACKPORT_DEVICE_H_
#include_next <linux/device.h>

#ifndef DEVICE_ATTR_ADMIN_RW
#define DEVICE_ATTR_ADMIN_RW(_name) \
	struct device_attribute dev_attr_##_name = __ATTR_RW_MODE(_name, 0600)
#endif

#ifndef DEVICE_ATTR_ADMIN_RO
#define DEVICE_ATTR_ADMIN_RO(_name) \
	struct device_attribute dev_attr_##_name = __ATTR_RO_MODE(_name, 0400)
#endif

/*
 * backport of:
 * commit a103b833ac3806b816bc993cba77d0b17cf801f1
 * Author: Raag Jadav <raag.jadav@intel.com>
 * Date:   Wed Feb 12 11:55:05 2025 +0530
 *
 *     devres: Introduce devm_kmemdup_array()
 *
 * Added in v6.15 and backported to v6.12.31.
 */
#if !LINUX_VERSION_IN_RANGE(6,12,31, 6,13,0) &&		\
    LINUX_VERSION_IS_LESS(6,15,0)
static inline void *devm_kmemdup_array(struct device *dev, const void *src,
				       size_t n, size_t size, gfp_t flags)
{
	return devm_kmemdup(dev, src, size_mul(size, n), flags);
}
#endif /* devm_kmemdup_array */

#endif /* __BACKPORT_DEVICE_H_ */
