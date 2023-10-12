bool bootkick_ign_prev = false;
bool waiting_to_boot = false;
uint32_t waiting_to_boot_count = 0;
uint8_t bootkick_harness_status_prev = HARNESS_STATUS_NC;

void bootkick_tick(bool ignition, bool recent_heartbeat) {
  const bool harness_inserted = (harness.status != bootkick_harness_status_prev) && (harness.status != HARNESS_STATUS_NC);

  // bootkick logic
  bool enabled_boot = false;
  if ((ignition && !bootkick_ign_prev) || harness_inserted) {
    enabled_boot = true;
    current_board->set_bootkick(true);
  } else if (recent_heartbeat) {
    // disable once openpilot is up
    current_board->set_bootkick(false);
  } else {

  }

  // log device boot time
  const bool som_running = current_board->read_som_gpio();
  if (enabled_boot && !som_running) {
    print("bootkick\n");
    waiting_to_boot = true;
  }
  if (waiting_to_boot) {
    if (som_running) {
      print("device booted\n");
      waiting_to_boot = false;
    } else if (waiting_to_boot_count == 45U) {
      print("not booted after 45s\n");
    } else {

    }
    waiting_to_boot_count += 1U;
  }

  // update state
  bootkick_ign_prev = ignition;
  bootkick_harness_status_prev = harness.status;
}
