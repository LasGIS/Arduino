enum RobotCommandType {       //    команда               | параметр 
  MOTOR_FORWARD          = 0, // движение вперед          | расстояние в см
  MOTOR_BACKWARD         = 1, // движение назад           | расстояние в см 
  MOTOR_LEFT             = 2, // разворот на месте влево  | угол в градусах 
  MOTOR_RIGHT            = 3, // разворот на месте вправо | угол в градусах 
  MOTOR_FORWARD_LEFT     = 4, // движение вперед и влево  | угол в градусах 
  MOTOR_FORWARD_RIGHT    = 5, // движение вперед и вправо | угол в градусах 
  MOTOR_BACKWARD_LEFT    = 6, // движение назад и влево   | угол в градусах 
  MOTOR_BACKWARD_RIGHT   = 7, // движение назад и вправо  | угол в градусах 
  ROBOT_SCANING          = 8,  // движение головой
  ROBOT_ANALYSE          = 9  // анализ ситуации и принятие решений
};

class RobotCommand {
public:
  RobotCommandType type;
  int param;
  bool isExecuted = false;
};

