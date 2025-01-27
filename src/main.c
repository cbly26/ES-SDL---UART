//Code from Nordic Developer Academy (Intel Corporation)
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/logging/log.h>

#define SLEEP_TIME_MS 250

//Define the size of the receive buffer
#define RECEIVE_BUFF_SIZE 10

//Define the receiving timeout period
#define RECEIVE_TIMEOUT 100

//Get the device pointers of the LEDs through gpio_dt_spec
static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);
static const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET(DT_ALIAS(led2), gpios);
//static const struct gpio_dt_spec led3 = GPIO_DT_SPEC_GET(DT_ALIAS(led3), gpios);

//Get the device pointer of the UART hardware 
const struct device *uart = DEVICE_DT_GET(DT_NODELABEL(uart0));

//Define the transmission buffer, which is a buffer to hold the data to be sent over UART 
static uint8_t tx_buf[] = {
	"nRF Connect SDK Fundamentals Course\r\n"
	"Press 1-3 on your keyboard to toggle LEDS 1-3 on your development kit\r\n"};

//Define the receive buffer
static uint8_t rx_buf[RECEIVE_BUFF_SIZE] = {0};

//Define the callback function for UART
static void uart_cb(const struct device *dev, struct uart_event *evt, void *user_data)
{
	switch (evt->type) {

	case UART_RX_RDY:

		if ((evt->data.rx.len) == 1) {

			if (evt->data.rx.buf[evt->data.rx.offset] == '1') {
				gpio_pin_toggle_dt(&led0);
			} else if (evt->data.rx.buf[evt->data.rx.offset] == '2') {
				gpio_pin_toggle_dt(&led1);
			} else if (evt->data.rx.buf[evt->data.rx.offset] == '3') {
				gpio_pin_toggle_dt(&led2);
			} //else if (evt->data.rx.buf[evt->data.rx.offset] == '4') {
				//gpio_pin_toggle_dt(&led3);
			//}
		}
		break;
	case UART_RX_DISABLED:
		uart_rx_enable(dev, rx_buf, sizeof rx_buf, RECEIVE_TIMEOUT);
		break;

	default:
		break;
	}
}

int main(void)
{
	int ret;
	//Verify that the UART device is ready
	if (!device_is_ready(uart)) {
		printk("UART device not ready\n");
		return 1;
	}
	//Verify that the LED devices are ready 
	if (!device_is_ready(led0.port)) {
		printk("GPIO device is not ready\n");
		return 1;
	}
//Configure the GPIOs of the LEDs 
	ret = gpio_pin_configure_dt(&led0, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 1;
	}
	ret = gpio_pin_configure_dt(&led1, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 1;
	}
	ret = gpio_pin_configure_dt(&led2, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 1;
	}
	// ret = gpio_pin_configure_dt(&led3, GPIO_OUTPUT_ACTIVE);
	// if (ret < 0) {
	// 	return 1;
	// }

	//Register the UART callback function
	ret = uart_callback_set(uart, uart_cb, NULL);
	if (ret) {
		return 1;
	}
	//Send the data over UART by calling uart_tx()
	ret = uart_tx(uart, tx_buf, sizeof(tx_buf), SYS_FOREVER_MS);
	if (ret) {
		return 1;
	}
	//Start receiving by calling uart_rx_enable() and pass it the address of the receive  buffer
	ret = uart_rx_enable(uart, rx_buf, sizeof rx_buf, RECEIVE_TIMEOUT);
	if (ret) {
		return 1;
	}
	while (1) {
		k_msleep(SLEEP_TIME_MS);
	}
}
