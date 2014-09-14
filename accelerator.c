#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>

#define ADDRESS_STM 0xFC000000
#define OFFSET_LWFPGASLAVES 0x03200000
#define OFFSET_ACCELERATOR 0x00030000

#define __MYATTR(_name, _mode, _show, _store) {                     \
	.attr = {.name = __stringify(_name),                            \
	         .mode = _mode },                                       \
	.show   = _show,                                                \
	.store  = _store,                                               \
}

static unsigned int in1 = 0;
static unsigned int in2 = 0;
static unsigned int res = 0;

static ssize_t in1_write(struct kobject        *kobj, 
	                     struct kobj_attribute *attr,
	                     const char            *buf, 
	                     size_t                count);

static ssize_t in2_write(struct kobject        *kobj, 
	                     struct kobj_attribute *attr,
	                     const char            *buf, 
	                     size_t                count);

static ssize_t res_read(struct kobject        *kobj, 
	                    struct kobj_attribute *attr,
                        char                  *buf);

static struct kobj_attribute in1_attr = __MYATTR(in1, 0666, NULL,     in1_write);
static struct kobj_attribute in2_attr = __MYATTR(in2, 0666, NULL,     in2_write);
static struct kobj_attribute res_attr = __MYATTR(res, 0666, res_read, NULL);


static struct attribute *attrs[] = {
	&in1_attr.attr,
	&in2_attr.attr,
	&res_attr.attr,
	NULL,
 };

static struct attribute_group attr_group = {
	.attrs = attrs,
};

static struct kobject *accel_kobj;

static ssize_t in1_write(struct kobject        *kobj, 
	                     struct kobj_attribute *attr,
	                     const char            *buf, 
	                     size_t                count)
{
	sscanf(buf, "%du", &in1);
	*(unsigned int *)(ADDRESS_STM + OFFSET_LWFPGASLAVES + OFFSET_ACCELERATOR + 0) = in1;
	printk(KERN_ALERT "Input 1: %d\n", in1);
	return count;
}

static ssize_t in2_write(struct kobject        *kobj, 
	                     struct kobj_attribute *attr,
	                     const char            *buf, 
	                     size_t                count)
{
	sscanf(buf, "%du", &in2);
	*(unsigned int *)(ADDRESS_STM + OFFSET_LWFPGASLAVES + OFFSET_ACCELERATOR + 1) = in2;
	printk(KERN_ALERT "Input 2: %d\n", in2);
	res = in1 + in2;
	return count;
}

static ssize_t res_read(struct kobject        *kobj, 
	                    struct kobj_attribute *attr,
                        char                  *buf)
{
	res = *(unsigned int *)(ADDRESS_STM + OFFSET_LWFPGASLAVES + OFFSET_ACCELERATOR);
	return sprintf(buf, "%d\n", res);
}

static int accelerator_init(void)
{
	int retval = 0;
	set_current_state(TASK_INTERRUPTIBLE);
	//                                             /sys/kernel/mm/
	accel_kobj = kobject_create_and_add("accelerator", mm_kobj);
	if (!accel_kobj)
		return -ENOMEM;

	retval = sysfs_create_group(accel_kobj, &attr_group);
	if (retval)
		kobject_put(accel_kobj);
	printk(KERN_ALERT "Accelerator loaded.\n");
	return retval;
}


static void accelerator_fini(void)
{
	kobject_put(accel_kobj);
	printk(KERN_ALERT "Accelerator unloaded.\n");
}  

module_init(accelerator_init);
module_exit(accelerator_fini);
MODULE_LICENSE("GPL");
