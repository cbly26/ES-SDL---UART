#include <zephyr/drivers/uart.h>
#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>

LOG_MODULE_REGISTER(UART, LOG_LEVEL_DBG);

const struct device *uart=DEVICE_DT_GET(DT_NODELABEL(uart0));

if (!device_is_ready(uart)) {
    printk("UART device not ready\r\n");
    return 1;
}

//NEED TO CHANGE CONTROLLER NAME
    #if defined (CONFIG_BOARD_NRF7002DK_NRF5340_CPUAPP)|| defined (CONFIG_BOARD_NRF7002DK_NRF5340_CPUAPP_NS)
    static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
    static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);
    #else
    static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
    static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);
    static const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET(DT_ALIAS(led2), gpios);
    #endif

//Verify LEDs are ready
if (!device_is_ready(led0.port)){
    printk("GPIO device is not ready\r\n");
    return 1;
}

// //NEED TO CHANGE CONTROLLER NAME
// //Configure GPIOs of the LEDs
// #if defined (CONFIG_BOARD_NRF7002DK_NRF5340_CPUAPP)|| defined (CONFIG_BOARD_NRF7002DK_NRF5340_CPUAPP_NS)
// 	ret = gpio_pin_configure_dt(&led0, GPIO_OUTPUT_ACTIVE);
// 	if (ret < 0) {
// 		return 1 ; 
// 	}
// 	ret = gpio_pin_configure_dt(&led1, GPIO_OUTPUT_ACTIVE);
// 	if (ret < 0) {
// 		return 1 ;
// 	}
// #else
// ret = gpio_pin_configure_dt(&led0, GPIO_OUTPUT_ACTIVE);
// 	if (ret < 0) {
// 		return 1 ; 
// 	}
// 	ret = gpio_pin_configure_dt(&led1, GPIO_OUTPUT_ACTIVE);
// 	if (ret < 0) {
// 		return 1 ;
// 	}
// 	ret = gpio_pin_configure_dt(&led2, GPIO_OUTPUT_ACTIVE);
// 	if (ret < 0) {
// 		return 1 ;
// 	}
// #endif

// //Define application callback function
// static void uart_cb(const struct device *dev, struct uart_event *evt, void *user_data)
// {
// 	switch (evt->type) {

// 	case UART_RX_RDY:
// #if defined(CONFIG_BOARD_NRF7002DK_NRF5340_CPUAPP) ||                                              \
// 	defined(CONFIG_BOARD_NRF7002DK_NRF5340_CPUAPP_NS)
//     if ((evt->data.rx.len) == 1) {

//         if (evt->data.rx.buf[evt->data.rx.offset] == '1') {
//             gpio_pin_toggle_dt(&led0);
//         } else if (evt->data.rx.buf[evt->data.rx.offset] == '2') {
//             gpio_pin_toggle_dt(&led1);
//         }
//     }
// #else
//     if ((evt->data.rx.len) == 1) {

//         if (evt->data.rx.buf[evt->data.rx.offset] == '1') {
//             gpio_pin_toggle_dt(&led0);
//         } else if (evt->data.rx.buf[evt->data.rx.offset] == '2') {
//             gpio_pin_toggle_dt(&led1);
//         } else if (evt->data.rx.buf[evt->data.rx.offset] == '3') {
//             gpio_pin_toggle_dt(&led2);
//         }
//     }
// #endif
//     break;
// 	case UART_RX_DISABLED:
//     uart_rx_enable(dev, rx_buf, sizeof rx_buf, RECEIVE_TIMEOUT);
//     break;

// 	default:
// 		break;
// 	}
// }

// //Register UART callback function
// ret = uart_callback_set(uart, uart_cb, NULL);
// if (ret) {
//     return 1;
// }

// //Define transmission buffer to hold data to be sent via UART
// static uint8_t tx_buf[] =   {"nRF Connect SDK Fundamentals Course\r\n"
//                              "Press 1-3 on your keyboard to toggle LEDS 1-3 on your development kit\r\n"};

// //Send data
// ret = uart_tx(uart, tx_buf, sizeof(tx_buf), SYS_FOREVER_US);
// if (ret) {
//     return 1;
// }

// //Define size of receive buffer & initialize members to zeros &receive timeout
// #define RECEIVE_BUFF_SIZE 10
// static uint8_t rx_buf[RECEIVE_BUFF_SIZE] = {0};
// #define RECEIVE_TIMEOUT 100

// //Start receiving
// ret = uart_rx_enable(uart ,rx_buf,sizeof rx_buf,RECEIVE_TIMEOUT);
// if (ret) {
//     return 1;
// }

k_msleep(500);