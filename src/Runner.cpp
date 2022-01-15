#include "Runner.hpp"

Runner runner = {};

void Runner::run() {
    auto const pos = location.pos();

    // полностью останавливаем робота при доезде до конца
    if(pos.y > fieldEndY)
    {
        driver.speedL(0);
        driver.speedR(0);
        driver.rotate(0);
        finish = true;
    }

    // больше не запускаем цикл
    if(finish)
    {
        return;
    }

    int dir = 0;
    float distance = 0.0f;
    scanObstacle(0, dir, distance);

    float rotation;
    float moveDistance;
    if(distance <= 0.0f || distance >= 350)
    {
        // если препятствия перед роботом нет, едем в сторону центра поля
        rotation = -10;
        moveDistance = 300;
    } else if(distance < 150)
    {
        // если препятствие очень близко, поворачиваем на 90
        rotation = 90;
        moveDistance = 250;
    } else if(distance < 270)
    {
        // если есть достаточно места для манёвра, объезжаем препятствие под углом
        rotation = 50;
        moveDistance = 450;
    } else
    {
        // если места совсем много, то под небольшим углом
        rotation = 40;
        moveDistance = 550;
    }

    // если препятствие было сбоку от робота, то добавляем дополнительное расстояние
    // робот проедет меньше, если он собирался поворачивать в противоположную сторону от препятствия
    // или больше, если смотрел на препятствие
    float const additionalMovement = 45 * dir;
    float const additionalRotation = 15 * dir;
    float const movement = moveDistance + additionalMovement;
    //rotation += additionalRotation;

    // если позиция робота левее центра, то он будет поворачиваться направо, чтобы
    // стать ближе к центру поля и наооборот
    if (pos.x < 0) {
        rotation *= -1;
    }

    driver.rotate(rotation);
    driver.move(movement);

    // возвращаем в исходную позицию
    driver.rotate(0);

}

void Runner::scanObstacle(float rotation, int& side, float& distance) {
    // поворачиваем робот в каждую из сторон на угол checkAngle,
    // совершая при этом checkCount проверок на сторону
    for (int i = -checkCount; i <= checkCount; ++i) {
        driver.rotate(rotation - (i * checkAngle / checkCount));
        float const scannedDistance = distanceSensor.distanceMm();

        // игнорируем все препятствия находящиеся за пределами видимости
        if (scannedDistance < sensorLen) {
            // записываем на какой проверке робот увидел препятствие
            side = i;
            distance = scannedDistance;
            return;
        }
    }
}

void Runner::enable() {
    driver.enable();
    location.enable();
    distanceSensor.enable();
}

