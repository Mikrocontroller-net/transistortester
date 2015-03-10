void mark_as_uncalibrated( void) {
 uint8_t ii;
 if (!UnCalibrated) {
    // equipment has changed, zero capacity value is too high or zero resistance is too high
    // but the device is marked as Calibrated.
    ii = eeprom_read_byte((uint8_t *)&c_zero_tab[0]);	// read first zero offset
    ii++;
    // make the unused c_zero_tab[3] different to c_zero_tab[0] to mark uncalibrated 
    eeprom_write_byte((uint8_t *)&c_zero_tab[3], ii);	// mark uncalibrated permanent
    UnCalibrated = 1;		// set back to uncalibrated
 }
}
