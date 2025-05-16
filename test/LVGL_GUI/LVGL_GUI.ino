#include <lvgl.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include <floatToString.h>

// Screen settings
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define DRAW_BUF_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT / 10 * (LV_COLOR_DEPTH / 8))

// Touchscreen calibration numbers
#define TS_MIN_X 200
#define TS_MAX_X 3700
#define TS_MIN_Y 300
#define TS_MAX_Y 3800

// touchscreen and LVGL setup
#define XPT2046_IRQ 36
#define XPT2046_MOSI 32
#define XPT2046_MISO 39
#define XPT2046_CLK 25
#define XPT2046_CS 33

// Custom UART pins on CN1 connector
#define UART_RX 22
#define UART_TX 27
#define SERIAL_BAUD 115200
uint32_t TIMEOUT_TIME = 500;
constexpr uint16_t RESPONSE_ERROR = 0xFFFF;


SPIClass ts_SPI = SPIClass(VSPI); // SPI bus for touch input
XPT2046_Touchscreen ts(XPT2046_CS, XPT2046_IRQ); // touchscreen input handling
HardwareSerial SensorSerial(1); // UART coms to meter


// Meter specific variables
constexpr uint32_t READING_INTERVAL = 200; // ms delay before reading from the meter
uint32_t lastRead = 0; 
uint8_t channel = 0; // Used to select meter channel
uint16_t adcValue = 0;
bool AUTO_RANGE_R = false; // Used to switch between auto range and manual range modes for resistance
bool AUTO_RANGE_VAL = true; // Used to switch between auto range and manual range for value display
uint8_t value_range[4] = {0, 0, 0, 0}; // Used to store reading range per channel
const float resistor_select[6] = {100.0, 1000.0, 10000.0, 100000.0, 470000.0, 1000000.0};

int resistor_index = 1; // WIP - hard coded for now so that the simulator readings work with the code

// LVGL specific variables
uint32_t lastTick = 0; // Used for lv_timer()
uint32_t now = 0; // Also used for meter reading interval

const char * channel_labels[4] = {"Current", "Voltage", "Resistance", "Capacitance"};
const char * btn_labels[5] = {"I", "V", "R", "C", ""};

const char * value_labels[4][3] = {
  {"A",      "mA",     "uA"}, 
  {"V",      "mV",     "uV"},
  {"M Ohms", "K Ohms", "Ohms",},
  {"uF",     "nF",     "pF"}
};

const float value_scale[3] = {1, 1000, 1000000};

// LVGL CALLBACKS ---------------------------------------------------------------------------

// Tell LVGL where the user is touching
void read_ts(lv_indev_t * indev, lv_indev_data_t * data) {
    if (ts.touched()) {
        TS_Point p = ts.getPoint();
        data->point.x = map(p.x, TS_MIN_X, TS_MAX_X, 0, SCREEN_WIDTH);
        data->point.y = map(p.y, TS_MIN_Y, TS_MAX_Y, 0, SCREEN_HEIGHT);
        data->state = LV_INDEV_STATE_PRESSED;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

// Manual reading channel selection
static void channel_change_callback(lv_event_t * e) {
  lv_obj_t * obj = (lv_obj_t*) lv_event_get_target(e);
  uint8_t button = lv_buttonmatrix_get_selected_button(obj);

  if (button >= 0 && button <= 3) {
    channel = button;
  }
}

// Manual reading auto-range selection
static void auto_range_select_callback(lv_event_t * e) {
  // WIP - will use another button matrix or a dropdown for range selection
}
// END LVGL CALLBACKS ---------------------------------------------------------------------------


// LVGL STYLE DEFINITIONS -----------------------------------------------------------------------

static lv_style_t style_btnm;

void setup_styles() {

  // style for the button matrix
  lv_style_init(&style_btnm);
  lv_style_set_border_width(&style_btnm, 2);
  lv_style_set_border_opa(&style_btnm, LV_OPA_50);
  lv_style_set_border_color(&style_btnm, lv_palette_main(LV_PALETTE_GREY));
  lv_style_set_text_font(&style_btnm, &lv_font_montserrat_26);
}
// END LVGL STYLE DEFINITIONS --------------------------------------------------------------------


// LVGL screens ---------------------------------------------------------------------------

// LVGL object definitions
lv_obj_t * channel_lbl;
lv_obj_t * reading_lbl;
lv_obj_t * value_lbl;
lv_obj_t * mbox_no_meter;

// Draw the manual testing screen
void manual_test_screen(lv_obj_t * parent) {

  lv_obj_t * btnm = lv_buttonmatrix_create(parent);
  lv_buttonmatrix_set_map(btnm, btn_labels);
  lv_obj_add_style(btnm, &style_btnm, LV_PART_ITEMS);
  lv_obj_add_event_cb(btnm, channel_change_callback, LV_EVENT_CLICKED, NULL);
  lv_obj_set_size(btnm, SCREEN_WIDTH, 80);

  // Allow selecting only one channel at a time
  lv_buttonmatrix_set_button_ctrl_all(btnm, LV_BUTTONMATRIX_CTRL_CHECKABLE);
  lv_buttonmatrix_set_one_checked(btnm, true);
  lv_buttonmatrix_set_button_ctrl(btnm, 0, LV_BUTTONMATRIX_CTRL_CHECKED);
  lv_obj_align(btnm, LV_ALIGN_BOTTOM_MID, 0, 0);

  // Top center Channel label
  channel_lbl = lv_label_create(parent);
  lv_obj_set_width(channel_lbl, 250);
  lv_obj_set_style_text_align(channel_lbl, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_set_style_text_font(channel_lbl, &lv_font_montserrat_26, 0);
  lv_obj_align(channel_lbl, LV_ALIGN_TOP_MID, 0, 15);

  // Display the reading with associated units
  reading_lbl = lv_label_create(parent);
  lv_obj_set_width(reading_lbl, 100);
  lv_obj_set_style_text_align(reading_lbl, LV_TEXT_ALIGN_RIGHT, 0);
  lv_obj_align(reading_lbl, LV_ALIGN_CENTER, -50, -40);

  value_lbl = lv_label_create(parent);
  lv_obj_set_width(value_lbl, 100);
  lv_obj_align(value_lbl, LV_ALIGN_CENTER, 55, -40);

}

// Meter not detected message box
void mbox_meter_not_found() {
  mbox_no_meter = lv_msgbox_create(NULL);
  lv_msgbox_add_title(mbox_no_meter, LV_SYMBOL_WARNING);
  lv_msgbox_add_text(mbox_no_meter, "Meter not detected!");
}
// END LVGL screens ---------------------------------------------------------------------------


// SERIAL1 FUNCTIONS ---------------------------------------------------------------------------

// Request a channel, return value or RESPONSE_ERROR on timeout/parse error
uint16_t requestChannel(uint8_t channel) {
  // 1) Clear any stale data
  while (SensorSerial.available()) SensorSerial.read();

  // 2) Send command
  SensorSerial.print("CMD ");
  SensorSerial.println(channel);

  // 3) Wait for line (up to TIMEOUT_TIME)
  uint32_t t0 = millis();
  while (!SensorSerial.available()) {
    if (millis() - t0 >= TIMEOUT_TIME) {
      return RESPONSE_ERROR;
    }
  }

  // 4) Read and parse
  String resp = SensorSerial.readStringUntil('\n');
  resp.trim();
  if (!resp.startsWith("VAL ")) {
    return RESPONSE_ERROR;
  }
  return (uint16_t)resp.substring(4).toInt();
}
// END SERIAL FUNCTIONS


// METER FUNCTIONS ---------------------------------------------------------------------------

// Write output to screen
void display_reading(float reading) {
  int range_index = value_range[channel];

  // Write output to Serial
  Serial.print(channel_labels[channel]);
  Serial.print(" : ");
  Serial.print(reading, 3);
  Serial.print(" ");
  Serial.println(value_labels[channel][range_index]);
  Serial.println();

  // Draw measurement type at top of screen
  lv_label_set_text(channel_lbl, channel_labels[channel]);

  // Draw calculated value with units
  char readingString[10];
  floatToString(reading, readingString, 10, 2);
  lv_label_set_text(reading_lbl, readingString);
  lv_label_set_text(value_lbl, value_labels[channel][range_index]);
}


// Convert the ADC value to the measurement value based on channel and scale
float calculate_reading(uint16_t adcValue) {

  float adc = (float)adcValue;
  float value = 0;
  float scaled_value = 0;
  int range_index = value_range[channel];

  // Current reading ------------------------------------
  switch (channel) {
    case 0:
      value = adc / 200.0;
      scaled_value = value * value_scale[range_index];

      if (AUTO_RANGE_VAL) {
        if (scaled_value < 1 && range_index < 2) {
          range_index++;
          scaled_value = value * value_scale[range_index];
        }
        else if (scaled_value > 1000 && range_index > 0) {
          range_index--;
          scaled_value = value * value_scale[range_index];
        }
        value_range[channel] = range_index;
      }
    return scaled_value;

    // Voltage reading ----------------------------------
    case 1:
      value = (adc / 4095) * 17;
      scaled_value = value * value_scale[range_index];

      if (AUTO_RANGE_VAL) {
        if (scaled_value < 1 && range_index < 2) {
          range_index++;
          scaled_value = value * value_scale[range_index];
        }
        else if (scaled_value > 1000 && range_index > 0) {
          range_index--;
          scaled_value = value * value_scale[range_index];
        }
        value_range[channel] = range_index;
      }
    return scaled_value;

    // Resistance reading -------------------------------
    case 2:
      value = calculate_resistance(adcValue);
      scaled_value = value / value_scale[range_index];

      if (AUTO_RANGE_VAL) {
        if (scaled_value > 1000 && range_index < 2) {
          range_index++;
          scaled_value = value / value_scale[range_index];
        }
        else if (scaled_value < 1 && range_index > 0) {
          range_index--;
          scaled_value = value / value_scale[range_index];
        }
        value_range[channel] = range_index;
      }
    return scaled_value;

    // Capacitance reading ------------------------------
    case 3:
      value = adc * 2;
      value = (64285 / value) * 2;
      scaled_value = value / value_scale[range_index];

      if (AUTO_RANGE_VAL) {
        if (scaled_value > 1000 && range_index < 2) {
          range_index++;
          scaled_value = value / value_scale[range_index];
        }
        else if (scaled_value < 1 && range_index > 0) {
          range_index--;
          scaled_value = value / value_scale[range_index];
        }
        value_range[channel] = range_index;
      }
    return scaled_value;
  }
}

// calculate resistance and set resistors
float calculate_resistance(uint16_t adcValue) {
  if (AUTO_RANGE_R) {
    if (adcValue > 4090 && resistor_index < 5) {
      resistor_index++;
    }
    else if (adcValue < 100 && resistor_index > 0) {
      resistor_index--;
    }

  }

  float adc = (float)adcValue;
  float resistor_value = resistor_select[resistor_index];
  float resistance = ((resistor_value * 4095) / adc) - resistor_value;
  return resistance;
}
// END METER FUNCTIONS

// ESP32-2432S028R RGB LED pins
#define LED_RED 4
#define LED_GREEN 16
#define LED_BLUE 17

void setup() {
  Serial.begin(SERIAL_BAUD);

  // Configure UART1 on custom pins
  SensorSerial.begin(SERIAL_BAUD, SERIAL_8N1, UART_RX, UART_TX);
  Serial.setTimeout(TIMEOUT_TIME);

  lv_init(); // initialize LVGL

  ts_SPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS); // start second SPI bus for touchscreen
  ts.begin(ts_SPI); // touchscreen init
  
  uint8_t * draw_buf = new uint8_t[DRAW_BUF_SIZE];
  lv_display_t * disp = lv_tft_espi_create(SCREEN_WIDTH, SCREEN_HEIGHT, draw_buf, DRAW_BUF_SIZE);

  lv_obj_t * scr = lv_scr_act(); // get the active screen object
  lv_obj_set_style_bg_color(scr, lv_color_black(), 0); // set the background of the active screen object to black
  setup_styles(); // Load any custom LVGL styles

  lv_indev_t * indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev, read_ts); // set the touchpad read function

  manual_test_screen(scr); // pass the active screen object to the button matrix setup function

  // Turn off the LEDs (they are inverted -> 0 = ON, 1 = OFF)
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);

  digitalWrite(LED_RED, 1);
  digitalWrite(LED_GREEN, 1);
  digitalWrite(LED_BLUE, 1);

  delay(1000);

  // check if the meter is connected on startup
  adcValue = requestChannel(channel);
  if (adcValue == RESPONSE_ERROR) {
    // Display error on screen
    mbox_meter_not_found();
    now = millis();
    lv_tick_inc(now - lastTick);
    lastTick = now;
    lv_timer_handler();
    Serial.println("Meter not detected");

    // Keep retrying until we get data
    do {
      adcValue = requestChannel(channel);
      delay(50);
    } while (adcValue == RESPONSE_ERROR);

    // clear error after coms restored
    lv_msgbox_close(mbox_no_meter);
  }

  Serial.println("Setup complete");
  delay(1000);
}


void loop() {
  now = millis();
  lv_tick_inc(now - lastTick); // update the tick timer
  lastTick = now;
  lv_timer_handler(); // update the LVGL UI

  if (now - lastRead >= READING_INTERVAL) {
    lastRead = now;
    adcValue = requestChannel(channel);

    if (adcValue == RESPONSE_ERROR) {
      // Display error on screen
      mbox_meter_not_found();
      lv_timer_handler();
      Serial.println("Meter not detected");

      // Keep retrying until we get data
      do {
        adcValue = requestChannel(channel);
        delay(50);
      } while (adcValue == RESPONSE_ERROR);

      // clear error after coms restored
      lv_msgbox_close(mbox_no_meter);
    }

    float reading = calculate_reading(adcValue);
    display_reading(reading);
  }

  delay(5);
}
