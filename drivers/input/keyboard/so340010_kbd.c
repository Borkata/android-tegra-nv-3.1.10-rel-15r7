/*
 * Filename    : android/kernel/driver/input/keyboard/so340010_gpio.c
 * Description : Button driver for SMBA1102. Four button is supported: HOME, 
 *					MENU, CANCEL, VOLUME_UP and VOLUME_DOWN
 * Athor       : LiuZheng <xmlz@malata.com>
 * Date        : 2010/06/28
 *
 * Ported to newer standard kernel interface 
 * Author:     : Jens Andersen <jens.andersen@gmail.com>
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/input.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/earlysuspend.h>
#include <linux/i2c.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <linux/slab.h>

#define DRIVER_NAME	"so340010_kbd"

#define SO340010_I2C_TRY_COUNT			3

#define SO340010_TIMER_INTERVAL			2000

#define __SO340010_GENERIC_DEBUG__        1
#define __I2C_SNAG_DETECTED__           0

#define TAG             "SO340010: "

#if (__SO340010_GENERIC_DEBUG__)
#define logd(x...)      do { printk(x); } while(0)
#else
#define logd(x...)      do {} while(0)
#endif


/*
 * TODO irq gpio number should be modify in SMBA1102 
 */
// TODO: Settings from platform_info
/*#define SO340010_IRQ_PORT				('v'-'a')
#define SO340010_IRQ_PIN				6
#define SO340010_GPIO_DEBOUNCE_TIME		10*/

#define SO340010_REG_GENERAL_CONFIG		0x0001
#define SO340010_REG_GPIO_STATE			0x0108
#define SO340010_REG_BUTTON_STATE		0x0109
#define SO340010_REG_NUM				74

#if (__SO340010_GENERIC_DEBUG__)
static ssize_t so340010_read_sysfs_debug(struct device *device, struct device_attribute *attr, char *buffer);
static ssize_t so340010_read_sysfs_intr(struct device *device, struct device_attribute *attr, char *buffer);
static ssize_t so340010_read_sysfs_i2c_snag(struct device *device, struct device_attribute *attr, char *buffer);
static ssize_t so340010_read_sysfs_reset(struct device *device, struct device_attribute *attr, char *buffer);
static ssize_t so340010_write_sysfs_reset(struct device *device, struct device_attribute *attr, const char *buffer, size_t count);
static ssize_t so340010_read_sysfs_pending_mask(struct device *device, struct device_attribute *attr, char *buffer);
#endif


struct so340010_kbd_dev {
	/* i2c client for convinience */
	struct i2c_client	*client;
	
	/* input dev */
	struct input_dev *input_dev;

	/* pending button */
	unsigned short pending_keys;


	/* early suspend */
	struct early_suspend	early_suspend; 

	/* work */
	struct work_struct work;

	/* workqueue */
	struct workqueue_struct *workqueue;

	/* timer */
	struct timer_list timer;

	int last_read;

#if (__I2C_SNAG_DETECTED__)
	unsigned int i2c_snag_no_slave;
	unsigned int i2c_snag_read_failed;
#endif

#if (__SO340010_GENERIC_DEBUG__)
	int last_i2c_error;
	int last_reset_error;
#endif

};

struct so340010_kbd_info {
	unsigned int 	key_mask;
	int 			key_code; 
};

static struct so340010_kbd_info key_table[] = {
	{ 0x0008, KEY_BACK },
	{ 0x0004, KEY_MENU },
	{ 0x0002, KEY_HOMEPAGE },
	{ 0x0001, KEY_SEARCH },
};

static int key_num = sizeof(key_table)/sizeof(key_table[0]);

struct so340010_register {
	unsigned short address;
	unsigned char value[2];
};

static struct so340010_register so340010_register_init_table[] = {

	{ 0x0000, { 0x00, 0x07 } },
	{ 0x0001, { 0x00, 0x20 } },
	{ 0x0004, { 0x00, 0x0F } },
//	{ 0x000E, { 0x01, 0x00 } },
	{ 0x0010, { 0xA0, 0xA0 } },
	{ 0x0011, { 0xA0, 0xA0 } },
};

#if (__SO340010_GENERIC_DEBUG__)
static DEVICE_ATTR(debug, 0777, so340010_read_sysfs_debug, NULL);
static DEVICE_ATTR(intr, 0777, so340010_read_sysfs_intr, NULL);
static DEVICE_ATTR(i2c_snag, 0777, so340010_read_sysfs_i2c_snag, NULL);
static DEVICE_ATTR(reset, 0777, so340010_read_sysfs_reset, so340010_write_sysfs_reset);
static DEVICE_ATTR(pending_mask, 0777, so340010_read_sysfs_pending_mask, NULL);
#endif

static int so340010_i2c_write(struct so340010_kbd_dev *dev, unsigned short reg_start, unsigned char *buffer, unsigned int write_num)
{
	int i,ret;
	unsigned char *write_buffer;
	struct i2c_msg msg;
	
	write_buffer = kmalloc(write_num + 2, GFP_KERNEL);
	if (write_buffer == NULL) {
		logd(TAG "so340010_i2c_write kmalloc nomem");
		return -ENOMEM;
	}

	write_buffer[0] = reg_start >> 8;
	write_buffer[1] = reg_start & 0xFF;
	for (i = 0; i < write_num; i++) {
		write_buffer[i+2] = buffer[i];
	}


	msg.addr = dev->client->addr;
	msg.len = write_num + 2;
	msg.buf = write_buffer;
	msg.flags = I2C_M_IGNORE_NAK;

	ret = i2c_transfer(dev->client->adapter, &msg, 1);
	if(ret != 1)
	{
		logd(TAG "so340010_i2c_write failed(%d)\r\n", ret);
		kfree(write_buffer);
		return -EINVAL;
	}
	kfree(write_buffer);
	return 0;
}

static int so340010_i2c_read(struct so340010_kbd_dev *dev, unsigned short reg_start, unsigned char *buffer, unsigned int read_num)
{
	int i;
	unsigned char reg_buffer[2];
	int ret;
	struct i2c_msg msgs[2];
	
	reg_buffer[0] = reg_start >> 8;
	reg_buffer[1] = reg_start & 0xFF;


	msgs[0].addr = dev->client->addr;
	msgs[0].len = 2;
	msgs[0].buf = &reg_buffer;
	msgs[0].flags = 0;
	
	msgs[1].addr = dev->client->addr;
	msgs[1].len= read_num;
	msgs[1].buf = buffer;
	msgs[1].flags = I2C_M_RD;

	for (i = 0; i < SO340010_I2C_TRY_COUNT; i++) {
		ret = i2c_transfer(dev->client->adapter, msgs, 2);
	}

	if(ret != 2)
	{
		logd(TAG "i2c_read failed(%d)\r\n", ret);
		return -EINVAL;
	}
	
	return 0;
	/*TransactionInfo[0].Flags = NVODM_I2C_IS_WRITE | NVODM_I2C_USE_REPEATED_START;
	TransactionInfo[0].Address = dev->i2c_address;
	TransactionInfo[0].Buf = reg_buffer;
	TransactionInfo[0].NumBytes = 2;
	
	TransactionInfo[1].Flags = 0;
	TransactionInfo[1].Address = dev->i2c_address;
	TransactionInfo[1].Buf = buffer;
	TransactionInfo[1].NumBytes = read_num;

	for (i = 0; i < SO340010_I2C_TRY_COUNT; i++) {
		I2cStatus = NvOdmI2cTransaction(dev->i2c, TransactionInfo,
				2, dev->i2c_speed, dev->i2c_timeout);
	}

	if (I2cStatus != NvOdmI2cStatus_Success) {
		logd(TAG "i2c_read failed(%d)\r\n", I2cStatus);
		return I2cStatus;
	}*/
}

static int so340010_i2c_write_word(struct so340010_kbd_dev *dev, unsigned short reg, unsigned short data) 
{
	unsigned char data_buffer[2];

	data_buffer[0] = data >> 8;
	data_buffer[1] = data & 0xFF;
	return so340010_i2c_write(dev, reg, data_buffer, 2);
}

static int so340010_i2c_read_word(struct so340010_kbd_dev *dev, unsigned short reg, unsigned short *data) 
{
	int ret;
	unsigned char buffer[2];
	ret = so340010_i2c_read(dev, reg, buffer, 2);
	if (ret) return ret;
	*data = (buffer[0] << 8) | buffer[1];
	return 0;
}

#if (__SO340010_GENERIC_DEBUG__)
static void dump(struct so340010_kbd_dev *dev)
{
	int i;
	unsigned char buffer[SO340010_REG_NUM];

	if (so340010_i2c_read(dev, 0, buffer, SO340010_REG_NUM)) {
		logd( TAG "dump() failed\r\n");
		return ;
	}

	for (i = 0; i < SO340010_REG_NUM/2; i++) {
		logd(TAG "0x%08x = 0x%04x  0x%04x\r\n", i, buffer[i*2], buffer[i*2+1]);
	}
}
#endif

#if (__SO340010_GENERIC_DEBUG__)
static ssize_t so340010_read_sysfs_debug(struct device *device, struct device_attribute *attr, char *buffer)
{
	int i, ret;
	unsigned char read_buffer[SO340010_REG_NUM] = {0};
	char *cursor;
	struct so340010_kbd_dev *dev;

	dev = (struct so340010_kbd_dev *)dev_get_drvdata(device);

	ret = so340010_i2c_read(dev, 0, read_buffer, SO340010_REG_NUM);
	if (ret) {
		return sprintf(buffer, "i2c read failed(%d)", ret);
	}

	cursor = buffer;
	for (i = 0; i < SO340010_REG_NUM; i++) {
		cursor += sprintf(cursor, "R0x%04x=0x%04x \r\n", i, read_buffer[i]);
	}

	return cursor - buffer;
}

static ssize_t so340010_read_sysfs_intr(struct device *device, struct device_attribute *attr, char *buffer)
{
	int pin_state;
	struct so340010_kbd_dev *dev;
	int gpio;
	
	dev = (struct so340010_kbd_dev *)dev_get_drvdata(device);
	gpio = irq_to_gpio(dev->client->irq);
	pin_state = gpio_get_value(gpio);

	return sprintf(buffer, pin_state ? "1" : "0");
}

static ssize_t so340010_read_sysfs_i2c_snag(struct device *device, struct device_attribute *attr, char *buffer)
{
	struct so340010_kbd_dev *dev;
	dev = (struct so340010_kbd_dev *)dev_get_drvdata(device);
	return sprintf(buffer, "%d", dev->last_i2c_error);
}

static int so340010_reset(struct so340010_kbd_dev *dev);
static ssize_t so340010_write_sysfs_reset(struct device *device, struct device_attribute *attr, const char *buffer, size_t count) 
{
	struct so340010_kbd_dev *dev;
	dev = (struct so340010_kbd_dev *)dev_get_drvdata(device);
	dev->last_reset_error = so340010_reset(dev);
	return count;
}

static ssize_t so340010_read_sysfs_reset(struct device *device, struct device_attribute *attr, char *buffer)
{
	struct so340010_kbd_dev *dev;
	dev = (struct so340010_kbd_dev *)dev_get_drvdata(device);
	return sprintf(buffer, "%s", dev->last_reset_error ? "failed" : "success");
}

static ssize_t so340010_read_sysfs_pending_mask(struct device *device, struct device_attribute *attr, char *buffer)
{
	struct so340010_kbd_dev *dev;
	dev = (struct so340010_kbd_dev *)dev_get_drvdata(device);
	return sprintf(buffer, "0x%04d", dev->pending_keys);
}

#endif

static int so340010_reset(struct so340010_kbd_dev *dev)
{
	int i;
	unsigned short reg_val;

	for (i = 0; i < sizeof(so340010_register_init_table)/sizeof(so340010_register_init_table[0]); i++) {
		if (so340010_i2c_write(dev, so340010_register_init_table[i].address, 
				so340010_register_init_table[i].value, 2)) {
			goto failed;
		}
	}
	if (so340010_i2c_read_word(dev, SO340010_REG_GPIO_STATE, &reg_val)
		|| so340010_i2c_read_word(dev, SO340010_REG_BUTTON_STATE, &reg_val)) {
		goto failed;
	}
	dev->pending_keys = 0;
	dev->last_read = jiffies_to_msecs(jiffies);
	return 0;
failed:
	return -EINVAL;
}

static int so340010_sleep(struct so340010_kbd_dev *dev, bool sleep) 
{
#define SO340010_SLEEP		((unsigned short)(0x0020))
#define SO340010_AWAKE		((unsigned short)(0x00A0))
	if (sleep) {
		return so340010_i2c_write_word(dev, SO340010_REG_GENERAL_CONFIG, SO340010_SLEEP);
	} else {
		return so340010_i2c_write_word(dev, SO340010_REG_GENERAL_CONFIG, SO340010_AWAKE);
	}
}

static void so340010_timer_func(unsigned long __dev) 
{
	int pin_state;
	struct so340010_kbd_dev *dev;
	int gpio;
	dev = (struct so340010_kbd_dev *)__dev;

	gpio = irq_to_gpio(dev->client->irq);
	pin_state = gpio_get_value(gpio);
	//NvOdmGpioGetState(dev->gpio_handle, dev->irq_pin_handle, &pin_state);

	if (pin_state == 0) { 
		if ((jiffies_to_msecs(jiffies) - dev->last_read) >= 2000) {
			queue_work(dev->workqueue, &dev->work);
		}
	}
	mod_timer(&dev->timer, jiffies + msecs_to_jiffies(SO340010_TIMER_INTERVAL));
}

static void so340010_work_func(struct work_struct *work)
{
	int i, ret;
	unsigned int gpio_val, button_val;
	struct so340010_kbd_dev *dev;

	dev = (struct so340010_kbd_dev *)container_of(work, struct so340010_kbd_dev, work);
	
	if ((ret = so340010_i2c_read_word(dev, SO340010_REG_GPIO_STATE, &gpio_val) != 0)
		|| (ret = so340010_i2c_read_word(dev, SO340010_REG_BUTTON_STATE, &button_val) != 0)) {	
		goto i2c_snag;
	}
	
	logd(TAG "gpio_val=0x%04x, button_val = 0x%04x\r\n", gpio_val, button_val);
	
	for (i = 0; i < key_num; i++) {
		if (button_val & key_table[i].key_mask) {
			dev->pending_keys |= key_table[i].key_mask;
			input_report_key(dev->input_dev, key_table[i].key_code, 1);
		} else {
			if (dev->pending_keys & key_table[i].key_mask) {
				input_report_key(dev->input_dev, key_table[i].key_code, 0);
				dev->pending_keys &= ~(key_table[i].key_mask);
			}
		}
	}

	dev->last_read = jiffies_to_msecs(jiffies);
	return;

i2c_snag:
#if (__SO340010_GENERIC_DEBUG__)
	dev->last_i2c_error = ret;
#endif
	if(ret == ETIMEDOUT)
	{
		so340010_reset(dev);
	}
	/*switch (ret) {
		case NvOdmI2cStatus_SlaveNotFound:
		case NvOdmI2cStatus_Timeout:
		case NvOdmI2cStatus_ReadFailed:
			so340010_reset(dev);
			break;
	}*/
}

static irqreturn_t  so340010_irq_callback(int irq, void *args)
{
	struct so340010_kbd_dev *dev;
	dev = (struct so340010_kbd_dev*)args;
	queue_work(dev->workqueue, &dev->work);
	return IRQ_HANDLED;
}

static int so340010_kbd_suspend(struct i2c_client *client, pm_message_t state)
{
	struct so340010_kbd_dev *dev = i2c_get_clientdata(client);
	int ret;

	if (WARN_ON(!dev))
		return -EINVAL;

	disable_irq(client->irq);

	return 0;
}

static int so340010_kbd_resume(struct i2c_client *client)
{
	struct so340010_kbd_dev *dev = i2c_get_clientdata(client);
	int ret = 0;

	if (WARN_ON(!dev))
		return -EINVAL;

	so340010_reset(dev);

	enable_irq(client->irq);

	return 0;
}

#ifdef CONFIG_HAS_EARLYSUSPEND
static void so340010_kbd_early_suspend(struct early_suspend *es)
{
	struct so340010_kbd_dev *dev;

	logd(TAG "so340010_kbd_early_suspend() IN\r\n");
	
	dev = (struct so340010_kbd_dev *)container_of(es, struct so340010_kbd_dev, early_suspend);
	disable_irq(dev->client->irq);
	//NvOdmGpioInterruptMask(dev->irq_handle, NV_TRUE);
	cancel_work_sync(&dev->work);
	so340010_sleep(dev, true);
	
	logd(TAG "so340010_kbd_early_suspend() OUT\r\n");
}

static void so340010_kbd_late_resume(struct early_suspend *es)
{
	struct so340010_kbd_dev *dev;

	dev = (struct so340010_kbd_dev *)container_of(es, struct so340010_kbd_dev, early_suspend);
	dev->pending_keys = 0;
	so340010_sleep(dev, false);
	if (so340010_reset(dev)) {
		logd(TAG "so340010_reset_failed\r\n");
	}
	enable_irq(dev->client->irq);
	//NvOdmGpioInterruptMask(dev->irq_handle, NV_FALSE);
}
#endif

static int so340010_kbd_probe(struct i2c_client *client,
							  const struct i2c_device_id *id) 
{
	int i;
	struct so340010_kbd_dev *dev;

	logd(TAG "so340010_kbd_probe\r\n");

	dev = kzalloc(sizeof(struct so340010_kbd_dev), GFP_KERNEL);
	if (!dev) {
		logd(TAG "so340010_kbd_probe kmalloc fail \r\n");
		goto failed_alloc_dev;
	}
	i2c_set_clientdata(client, dev);
	// For convinience
	dev->client = client;
	
	/* register input device */
	dev->input_dev = input_allocate_device();
	if (!dev->input_dev) {
		logd(TAG "so340010_kbd_probe input_allocate_device fail \r\n");
		goto failed_alloc_input;
	}
	dev->input_dev->name = "so340010_kbd";
	set_bit(EV_KEY, dev->input_dev->evbit);
	for (i = 0; i < key_num; i++) {
		set_bit(key_table[i].key_code, dev->input_dev->keybit);
	}
	if (input_register_device(dev->input_dev)) {
		goto failed_register_input;
	}

#ifdef CONFIG_HAS_EARLYSUSPEND
	dev->early_suspend.level = EARLY_SUSPEND_LEVEL_STOP_DRAWING;
	dev->early_suspend.suspend = so340010_kbd_early_suspend;
	dev->early_suspend.resume = so340010_kbd_late_resume;
	register_early_suspend(&dev->early_suspend);
#endif

	dev->workqueue = create_singlethread_workqueue("so340010_kbc");
	if (!dev->workqueue) {
		goto failed_create_workqueue;
	}

	INIT_WORK(&dev->work, so340010_work_func);
	if (so340010_reset(dev)) {
		logd(TAG "so340010_kbd_probe so340010_reset fail \r\n");
		goto failed_reset_hardware;
	}

	if(request_threaded_irq(client->irq, NULL, so340010_irq_callback, IRQF_TRIGGER_FALLING, 
			"so340010_kbd", dev))
		goto failed_enable_irq;


#if (__SO340010_GENERIC_DEBUG__)
	if (device_create_file(&client->dev, &dev_attr_debug)
		|| device_create_file(&client->dev, &dev_attr_intr)
		|| device_create_file(&client->dev, &dev_attr_i2c_snag)
		|| device_create_file(&client->dev, &dev_attr_reset)
		|| device_create_file(&client->dev, &dev_attr_pending_mask)) {
		goto failed_add_sysfs;
	}
#endif

	init_timer(&dev->timer);
	dev->timer.function = so340010_timer_func;
	dev->timer.data = dev;
	dev->last_read = jiffies_to_msecs(jiffies);
	mod_timer(&dev->timer, jiffies + msecs_to_jiffies(SO340010_TIMER_INTERVAL));

	return 0;
#if (__SO340010_GENERIC_DEBUG__)
failed_add_sysfs:
#endif
failed_reset_hardware:
	free_irq(client->irq, dev);
failed_enable_irq:
	input_unregister_device(dev->input_dev);
#ifdef CONFIG_HAS_EARLYSUSPEND
	unregister_early_suspend(&dev->early_suspend);
#endif
failed_register_input:
	destroy_workqueue(dev->workqueue);
failed_create_workqueue:
	input_free_device(dev->input_dev);	
failed_alloc_input:
	kfree(dev);
failed_alloc_dev:
	logd(TAG "so34001_kbd_probe failed\r\n");
	return -1;
}

static int so340010_kbd_remove(struct i2c_client *client)
{
	struct so340010_kbd_dev *dev;

	dev = (struct so340010_kbd_dev *)i2c_get_clientdata(client);
	
#if (__SO340010_GENERIC_DEBUG__)
	device_remove_file(&client->dev, &dev_attr_debug);
	device_remove_file(&client->dev, &dev_attr_intr);
	device_remove_file(&client->dev, &dev_attr_i2c_snag);
	device_remove_file(&client->dev, &dev_attr_reset);
	device_remove_file(&client->dev, &dev_attr_pending_mask);
#endif
	free_irq(client->irq, dev);
	input_unregister_device(dev->input_dev);
	unregister_early_suspend(&dev->early_suspend);
	destroy_workqueue(dev->workqueue);
	input_free_device(dev->input_dev);
	kfree(dev);
	return 0;
}

static const struct i2c_device_id so340010_kbd_id[] = {
	{ DRIVER_NAME, 0 },
	{ }
};

static struct i2c_driver so340010_kbd_driver = {
	.probe		= so340010_kbd_probe,
	.remove		= so340010_kbd_remove,
#ifndef CONFIG_HAS_EARLYSUSPEND
	.suspend	= so340010_kbd_suspend,
	.resume		= so340010_kbd_resume,
#endif
	.id_table	= so340010_kbd_id,
	.driver		= {
		.name	= DRIVER_NAME,
	},
};

static int __init so340010_kbd_init(void)
{
	logd(TAG "so340010_kbd_init\r\n");
	int e;

	e = i2c_add_driver(&so340010_kbd_driver);
	if (e != 0) {
		pr_err("%s: failed to register with I2C bus with "
		       "error: 0x%x\n", __func__, e);
	}
	return e;
}

static void __exit so340010_kbd_exit(void) 
{
	i2c_del_driver(&so340010_kbd_driver);
}

module_init(so340010_kbd_init);
module_exit(so340010_kbd_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("SO340010 I2C keypad driver");
