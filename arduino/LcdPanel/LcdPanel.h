enum LPModeType : uint8_t {
  show, edit
};
enum LPShowModeType : uint8_t {
  BigTime = 0,
  DataTime = 1,
  TimeHum = 2,
  Humidity = 3,
  Battery = 4
};
enum CurrentCommandType : uint8_t {
  mainCommand = 0,
  showLCDchars = 1,
  showIRkey = 2/*,
  showDistance = 3 */
};
