#include <pebble.h>

#define COUNTDOWNTO 1527206460 // The date/time in unix epoch time to count down to
#define TIME_ZONE_OFFSET -5 // Timezone offset (-5 == EST)
#define EVENT "Freedom" // The event you are counting down to	
	
Window *window;
TextLayer *text_time_layer;
TextLayer *text_date_layer;
TextLayer *text_days_layer;
TextLayer *text_hours_layer;
TextLayer *text_minutes_layer;

void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
	// Need to be static because they're used by the system later.
	static char time_text[] = "00:00";
	static char date_text[] = "Xxxxxxxxx 00";

	strftime(date_text, sizeof(date_text), "%B %e", tick_time);
	text_layer_set_text(text_date_layer, date_text);
	strftime(time_text, sizeof(time_text), "%R", tick_time);
	text_layer_set_text(text_time_layer, time_text);
	
	unsigned int unix_time;
	/* Convert time to seconds since epoch. */ 
	// This conversion from WhyIsThisOpen's "Unix" watch face (http://forums.getpebble.com/discussion/4324/watch-face-unix-time)
	unix_time = ((0-TIME_ZONE_OFFSET+2)*3600) + /* time zone offset */ 
		+ tick_time->tm_sec /* start with seconds */
		+ tick_time->tm_min*60 /* add minutes */
		+ tick_time->tm_hour*3600 /* add hours */
		+ tick_time->tm_yday*86400 /* add days */
		+ (tick_time->tm_year-70)*31536000 /* add years since 1970 */
		+ ((tick_time->tm_year-69)/4)*86400 /* add a day after leap years, starting in 1973 */
		- ((tick_time->tm_year-1)/100)*86400 /* remove a leap day every 100 years, starting in 2001 */
		+ ((tick_time->tm_year+299)/400)*86400; /* add a leap day back every 400 years, starting in 2001*/
	//APP_LOG(APP_LOG_LEVEL_DEBUG, "Current UNIX time: %d", (int)unix_time);
	int days;
	int hours;
	int minutes;
	static char daysStr[] = "XXXX Days";
	static char hoursStr[] = "XX Hours";
	static char minutesStr[] = "XX Minutes";
	int timeRemaining = COUNTDOWNTO - (int)unix_time;
	days = (timeRemaining/86400);
	hours = (timeRemaining/3600) - days*24;
	minutes = (timeRemaining/60) - days*1440 - hours*60;
	//APP_LOG(APP_LOG_LEVEL_DEBUG, "days: %d", days);
	//APP_LOG(APP_LOG_LEVEL_DEBUG, "hours: %d", hours);
	//APP_LOG(APP_LOG_LEVEL_DEBUG, "minutes: %d", minutes);
	if (days == 1)
		snprintf(daysStr, sizeof(daysStr), "%d Day", days);
	else
		snprintf(daysStr, sizeof(daysStr), "%d Days", days);
	if (hours == 1)
		snprintf(hoursStr, sizeof(hoursStr), "%d Hour", hours);
	else
		snprintf(hoursStr, sizeof(hoursStr), "%d Hours", hours);
	if (minutes == 1)
		snprintf(minutesStr, sizeof(minutesStr), "%d Minute", minutes);
	else
		snprintf(minutesStr, sizeof(minutesStr), "%d Minutes", minutes);
	//APP_LOG(APP_LOG_LEVEL_DEBUG, "daysStr: %s", daysStr);
	//APP_LOG(APP_LOG_LEVEL_DEBUG, "hoursStr: %s", hoursStr);
	//APP_LOG(APP_LOG_LEVEL_DEBUG, "minutesStr: %s", minutesStr);
	text_layer_set_text(text_days_layer, daysStr);
	text_layer_set_text(text_hours_layer, hoursStr);
	text_layer_set_text(text_minutes_layer, minutesStr);
	//APP_LOG(APP_LOG_LEVEL_DEBUG, "text_days_layer: %s", text_layer_get_text(text_days_layer));
	//APP_LOG(APP_LOG_LEVEL_DEBUG, "All layers set");
}

void handle_init(void) {
	window = window_create();
	window_stack_push(window, true /* Animated */);
	window_set_background_color(window, GColorBlack);
	
	Layer *window_layer = window_get_root_layer(window);
	
	// Current Time
	text_time_layer = text_layer_create(GRect(0, 0, 144, 43));
	text_layer_set_text_color(text_time_layer, GColorWhite);
	text_layer_set_background_color(text_time_layer, GColorClear);
	text_layer_set_font(text_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_MEDIUM_NUMBERS));
	text_layer_set_text_alignment(text_time_layer, GTextAlignmentCenter);
	layer_add_child(window_layer, text_layer_get_layer(text_time_layer));
	// Date
	text_date_layer = text_layer_create(GRect(0, 43, 144, 31));
	text_layer_set_text_color(text_date_layer, GColorWhite);
	text_layer_set_background_color(text_date_layer, GColorClear);
	text_layer_set_font(text_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
	text_layer_set_text_alignment(text_date_layer, GTextAlignmentCenter);
	layer_add_child(window_layer, text_layer_get_layer(text_date_layer));
	// Days
	text_days_layer = text_layer_create(GRect(0, 73, 144, 32));
	text_layer_set_text_color(text_days_layer, GColorWhite);
	text_layer_set_background_color(text_days_layer, GColorBlack);
	text_layer_set_font(text_days_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text_alignment(text_days_layer, GTextAlignmentCenter);
	layer_add_child(window_layer, text_layer_get_layer(text_days_layer));
	// Hours
	text_hours_layer = text_layer_create(GRect(0, 103, 144, 29));
	text_layer_set_text_color(text_hours_layer, GColorWhite);
	text_layer_set_background_color(text_hours_layer, GColorClear);
	text_layer_set_font(text_hours_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text_alignment(text_hours_layer, GTextAlignmentCenter);
	layer_add_child(window_layer, text_layer_get_layer(text_hours_layer));
	// Minutes
	text_minutes_layer = text_layer_create(GRect(0, 133, 144, 29));
	text_layer_set_text_color(text_minutes_layer, GColorWhite);
	text_layer_set_background_color(text_minutes_layer, GColorClear);
	text_layer_set_font(text_minutes_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text_alignment(text_minutes_layer, GTextAlignmentCenter);
	layer_add_child(window_layer, text_layer_get_layer(text_minutes_layer));
	
	tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
}

void handle_deinit(void) {
	text_layer_destroy(text_time_layer);
	text_layer_destroy(text_date_layer);
	text_layer_destroy(text_days_layer);
	text_layer_destroy(text_hours_layer);
	text_layer_destroy(text_minutes_layer);
	window_destroy(window);
}

int main(void) {
	  handle_init();
	  app_event_loop();
	  handle_deinit();
}
