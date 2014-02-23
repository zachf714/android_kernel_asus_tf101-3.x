#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include "board-ventana.h"

static struct kobject *ventana_kobj;

static ssize_t hw_show(struct kobject *kobj,
	struct kobj_attribute *attr, char *buf)
{
	char *s = buf;
	s += sprintf(s, "%02x\n", ventana_hw);
	return s - buf;
}

static struct kobj_attribute hw_attr =
	__ATTR(hw, 0440, hw_show, NULL);

#define CHK_ERR(x) if (x) { \
	pr_err("%s: sysfs_create_file fail(%d)!", __func__, x); \
	return x; \
}

static int __init ventana_sysfs_init(void)
{
	ventana_kobj = kobject_create_and_add("ventana", firmware_kobj);
	if (!ventana_kobj) {
		pr_err("%s: ventana_kobj create fail\n", __func__);
		return -ENODEV;
	}

	CHK_ERR(sysfs_create_file(ventana_kobj, &hw_attr.attr));

	return 0;
}

static void __exit ventana_sysfs_exit(void)
{
	sysfs_remove_file(ventana_kobj, &hw_attr.attr);
	kobject_del(ventana_kobj);
}

late_initcall(ventana_sysfs_init);
module_exit(ventana_sysfs_exit);
