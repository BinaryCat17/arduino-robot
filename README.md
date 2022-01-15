# arduino-robot

В рамках проекта создан набор вспомогательных классов, алгоритм езды по линии и объезда препятствий.

# Высокоуровневые классы
Основаны на клкассах низкого уровня для взаимодействия с ATMega2560
## Класс Runner
Алгоритм объезда и сканирования препятствий на поле для соревнований
- [Runner.hpp](https://github.com/BinaryCat17/arduino-robot/blob/master/src/Runner.hpp)
- [Runner.cpp](https://github.com/BinaryCat17/arduino-robot/blob/master/src/Runner.cpp)

## Класс PositionTrack
Считывает расстояние, которое проехало каждое колесо, угол поворота робота и его координаты x, y
- [PositionTrack.hpp](https://github.com/BinaryCat17/arduino-robot/blob/master/src/PositionTrack.hpp)
- [PositionTrack.cpp](https://github.com/BinaryCat17/arduino-robot/blob/master/src/PositionTrack.cpp)

## Класс Driver
Управляет вращением двигателей, корректирует их скорость, выполняет повороты и передвижение
- [Driver.hpp](https://github.com/BinaryCat17/arduino-robot/blob/master/src/Driver.hpp)
- [Driver.cpp](https://github.com/BinaryCat17/arduino-robot/blob/master/src/Driver.cpp)

## Класс DistanceSensor
Считывает расстояние до препятствия перед роботом с помощью ультразвукового сенсора
- [DistanceSensor.hpp](https://github.com/BinaryCat17/arduino-robot/blob/master/src/DistanceSensor.hpp)
- [DistanceSensor.cpp](https://github.com/BinaryCat17/arduino-robot/blob/master/src/DistanceSensor.cpp)

## Класс LineDriver
Выполняет езду по черной линии на белом фоне
- [DistanceSensor.hpp](https://github.com/BinaryCat17/arduino-robot/blob/master/src/DistanceSensor.hpp)
- [DistanceSensor.cpp](https://github.com/BinaryCat17/arduino-robot/blob/master/src/DistanceSensor.cpp)

## Класс App
Стартовая точка программы, похоже на Setup и Loop в среде Arduino
- [App.hpp](https://github.com/BinaryCat17/arduino-robot/blob/master/src/App.hpp)
- [App.cpp](https://github.com/BinaryCat17/arduino-robot/blob/master/src/App.cpp)

## Класс DeltaTimer
Замеряет время, прошедшее между итерациями основного цикла или сенсора расстояния
с конфигурацией точности до микросекунды или миллисекунды с помощью аппаратного таймера
- [DeltaTimer.hpp](https://github.com/BinaryCat17/arduino-robot/blob/master/src/DeltaTimer.hpp)
- [DeltaTimer.cpp](https://github.com/BinaryCat17/arduino-robot/blob/master/src/DeltaTimer.cpp)

# Низкоуровневые классы
Проект использует библиотеки Arduino только для UART. Всё управление роботом реализовано через библиотеки AVR только для ATMega2560
## Класс ADCController
Высокоуровневый интерфейс для аналого-цифрового преобразователя
- [ADCController.hpp](https://github.com/BinaryCat17/arduino-robot/blob/master/src/AVRLib/ADCController.hpp)
- [ADCController.cpp](https://github.com/BinaryCat17/arduino-robot/blob/master/src/AVRLib/ADCController.cpp)

## Класс DPinMap
Высокоуровневый интерфейс для управления цифровыми пинами
- [DPinMap.hpp](https://github.com/BinaryCat17/arduino-robot/blob/master/src/AVRLib/DPinMap.hpp)

## Класс HardwareTimer
Высокоуровневый интерфейс для всех 16-битных таймеров в режимах Normal, CTC, PWM
- [HardwareTimer.hpp](https://github.com/BinaryCat17/arduino-robot/blob/master/src/AVRLib/HardwareTimer.hpp)
- [HardwareTimer.cpp](https://github.com/BinaryCat17/arduino-robot/blob/master/src/AVRLib/HardwareTimer.cpp)

## Класс PinInputHandler
Класс для считывания изменений с пинов и вызова прерываний
- [ADCController.hpp](https://github.com/BinaryCat17/arduino-robot/blob/master/src/AVRLib/PinInputHandler.hpp)
- [ADCController.cpp](https://github.com/BinaryCat17/arduino-robot/blob/master/src/AVRLib/PinInputHandler.cpp)

## Класс SerialMonitor
Класс для считывания изменений с пинов и вызова прерываний
- [SerialMonitor.hpp](https://github.com/BinaryCat17/arduino-robot/blob/master/src/AVRLib/SerialMonitor.hpp)
- [SerialMonitor.cpp](https://github.com/BinaryCat17/arduino-robot/blob/master/src/AVRLib/SerialMonitor.cpp)
