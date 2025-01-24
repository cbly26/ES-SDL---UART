//Code from Nordic Developer Academy (Intel Corporation)
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/logging/log.h>

LOG_REGISTER(SDL, LOG_LEVEL_DBG);
#define SLEEP_TIME_MS 1000

//Define the size of the receive buffer
#define RECEIVE_BUFF_SIZE 10

//Define the receiving timeout period
#define RECEIVE_TIMEOUT 100

//Get the device pointers of the LEDs through gpio_dt_spec
// The nRF7002dk has only 2 LEDs so this step uses a compile-time condition to reflect the DK you are building for
#if defined(CONFIG_BOARD_NRF7002DK_NRF5340_CPUAPP) ||                                              \
	defined(CONFIG_BOARD_NRF7002DK_NRF5340_CPUAPP_NS)
static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);
#else
static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);
static const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET(DT_ALIAS(led2), gpios);
//static const struct gpio_dt_spec led3 = GPIO_DT_SPEC_GET(DT_ALIAS(led3), gpios);
#endif

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
#if defined(CONFIG_BOARD_NRF7002DK_NRF5340_CPUAPP) ||                                              \
	defined(CONFIG_BOARD_NRF7002DK_NRF5340_CPUAPP_NS)
		if ((evt->data.rx.len) == 1) {

			if (evt->data.rx.buf[evt->data.rx.offset] == '1') {
				gpio_pin_toggle_dt(&led0);
			} else if (evt->data.rx.buf[evt->data.rx.offset] == '2') {
				gpio_pin_toggle_dt(&led1);
			}
		}
#else
	LOG_DBG("ENTERED DATA REGISTER");
	k_msleep(200);

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
#endif
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
#if defined(CONFIG_BOARD_NRF7002DK_NRF5340_CPUAPP) ||                                              \
	defined(CONFIG_BOARD_NRF7002DK_NRF5340_CPUAPP_NS)
	ret = gpio_pin_configure_dt(&led0, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 1;
	}
	ret = gpio_pin_configure_dt(&led1, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 1;
	}
#else
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
#endif

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

/*
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(SDL, LOG_LEVEL_DBG);

#define UART_DEVICE_NODE DT_NODELABEL(uart0)
#define LED0_NODE DT_ALIAS(led0)

const struct device *uart = DEVICE_DT_GET(UART_DEVICE_NODE);
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

#define RX_BUFF_SIZE 10
static uint8_t rx_buf[RX_BUFF_SIZE] = {0};

// UART RX Interrupt Handler
static void uart_rx_handler(const struct device *dev, struct uart_event *evt, void *user_data)
{
    switch (evt->type) {
    case UART_RX_RDY:
        LOG_DBG("Received %d bytes", evt->data.rx.len);

        // Process each byte received
        if (evt->data.rx.len >= 1) {
            char received = evt->data.rx.buf[evt->data.rx.offset];
            LOG_INF("Received: '%c' (ASCII: %d)", received, received);

            // You can trigger the LED based on the received character
            if (received == '1') {
                gpio_pin_set_dt(&led, 1);  // Turn LED on
                LOG_INF("LED turned ON by received signal");
            } else if (received == '0') {
                gpio_pin_set_dt(&led, 0);  // Turn LED off
                LOG_INF("LED turned OFF by received signal");
            }
        }
        break;

    case UART_RX_BUF_REQUEST:
        LOG_DBG("UART RX buffer request, providing new buffer");
        uart_rx_buf_rsp(dev, rx_buf, sizeof(rx_buf));
        break;

    case UART_RX_DISABLED:
        LOG_DBG("UART RX disabled, re-enabling");
        uart_rx_enable(dev, rx_buf, sizeof(rx_buf), 1000);
        break;

    default:
        LOG_WRN("Unhandled UART event: %d", evt->type);
        break;
    }
}

int main(void)
{
    int ret;

    LOG_INF("Receive Board Initializing");

    // Check if the UART device is ready
    if (!device_is_ready(uart)) {
        LOG_ERR("UART device not ready");
        return 1;
    }

    // Check if the LED GPIO is ready
    if (!device_is_ready(led.port)) {
        LOG_ERR("LED device not ready");
        return 1;
    }

    // Configure the LED GPIO pin
    ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
    if (ret < 0) {
        LOG_ERR("Error configuring LED: %d", ret);
        return 1;
    }

    // Set the UART callback to handle RX events
    ret = uart_callback_set(uart, uart_rx_handler, NULL);
    if (ret) {
        LOG_ERR("Error setting UART callback: %d", ret);
        return 1;
    }

    // Enable UART RX with a buffer and a timeout
    ret = uart_rx_enable(uart, rx_buf, sizeof(rx_buf), 1000);
    if (ret) {
        LOG_ERR("Error enabling UART RX: %d", ret);
        return 1;
    }

    LOG_INF("Receive board ready and waiting for data");

    while (1) {
        // Main loop, but everything is handled by interrupts
        k_msleep(100);  // Sleep here to avoid excessive CPU usage
    }
}



================
Transmit: 

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(transmit_board, LOG_LEVEL_DBG);

#define UART_DEVICE_NODE DT_NODELABEL(uart0)
#define BTN0_NODE DT_ALIAS(sw0)

const struct device *uart = DEVICE_DT_GET(UART_DEVICE_NODE);
static const struct gpio_dt_spec btn = GPIO_DT_SPEC_GET(BTN0_NODE, gpios);

static char tx_buf[1]; // Buffer for single-character signals

static void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    // Send '1' when button is pressed
    tx_buf[0] = '1';
    int ret = uart_tx(uart, tx_buf, sizeof(tx_buf), SYS_FOREVER_MS);
    if (ret == 0) {
        LOG_INF("Button pressed, sent: %c", tx_buf[0]);
    } else {
        LOG_ERR("Failed to send UART signal: %d", ret);
    }

    k_msleep(50); // Debounce delay
}

int main(void)
{
    int ret;

    LOG_INF("Transmit Board Initializing");

    if (!device_is_ready(uart)) {
        LOG_ERR("UART device not ready");
        return 1;
    }

    if (!device_is_ready(btn.port)) {
        LOG_ERR("Button device not ready");
        return 1;
    }

    ret = gpio_pin_configure_dt(&btn, GPIO_INPUT);
    if (ret < 0) {
        LOG_ERR("Error configuring button: %d", ret);
        return 1;
    }

    // Configure interrupts for button press and release
    struct gpio_callback button_cb_data;
    gpio_init_callback(&button_cb_data, button_pressed, BIT(btn.pin));
    gpio_add_callback(btn.port, &button_cb_data);
    gpio_pin_interrupt_configure_dt(&btn, GPIO_INT_EDGE_TO_ACTIVE);

    LOG_INF("Transmit board ready");

    while (1) {
        k_msleep(100); // Idle loop
    }
}
*/