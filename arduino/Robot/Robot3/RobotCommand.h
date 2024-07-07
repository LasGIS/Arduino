enum RobotCommandType : uint8_t { //    команда               | параметр
  MOTOR_FORWARD          = 0,     // движение вперед          | расстояние в см
  MOTOR_BACKWARD         = 1,     // движение назад           | расстояние в см
  MOTOR_LEFT             = 2,     // разворот на месте влево  | угол в градусах
  MOTOR_RIGHT            = 3,     // разворот на месте вправо | угол в градусах
  MOTOR_FORWARD_LEFT     = 4,     // движение вперед и влево  | угол в градусах
  MOTOR_FORWARD_RIGHT    = 5,     // движение вперед и вправо | угол в градусах
  MOTOR_BACKWARD_LEFT    = 6,     // движение назад и влево   | угол в градусах
  MOTOR_BACKWARD_RIGHT   = 7,     // движение назад и вправо  | угол в градусах
  MOTOR_SET_GEAR         = 8,     // устанавливаем передачу
  ROBOT_SCANNING         = 9,     // движение  головой
  ROBOT_ANALYSE          = 10,    // анализ ситуации и принятие решений
  ROBOT_STOP             = 11     // останавливаем всё
};

/** Состояние команды */
enum RobotCommandState : uint8_t {
  EMPTY   = 0,  // пустая || выполнена
  LOADED  = 1,  // записана
  RUNNING = 2   // выполняется
};

/** Состояние команды */
enum RobotAnalyseCheck : uint8_t {
  NO_BLOCK,       // нет препятствий, можно двигать
  BLOCK_FRONT,    // помеха спереди
  BLOCK_RIGHT,    // помеха справа
  BLOCK_LEFT      // помеха слева
};

class RobotCommand {
public:
  RobotCommandType type;
  RobotCommandState state = EMPTY;
  int param;
};
