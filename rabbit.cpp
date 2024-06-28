#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <thread>
#include <chrono> 

class Rabbit {
public:
    Rabbit(double speed, double changeDirectionInterval)
        : speed(speed), changeDirectionInterval(changeDirectionInterval), x(0), y(0), timeSinceLastChange(0) {
        std::srand(std::time(0)); // Ініціалізація генератора випадкових чисел
        changeDirection();
    }

    virtual void move(double deltaTime) {
        timeSinceLastChange += deltaTime;
        if (timeSinceLastChange >= changeDirectionInterval) {
            changeDirection();
            timeSinceLastChange = 0;
        }
        x += speed * deltaX * deltaTime;
        y += speed * deltaY * deltaTime;
    }

    void printPosition() const {
        std::cout << "Rabbit at (" << x << ", " << y << ")\n";
    }

protected:
    double speed;
    double x, y;

private:
    double deltaX, deltaY;
    double changeDirectionInterval;
    double timeSinceLastChange;

    void changeDirection() {
        deltaX = (std::rand() % 200 - 100) / 100.0;
        deltaY = (std::rand() % 200 - 100) / 100.0;
        // Нормалізація вектора напряму
        double length = std::sqrt(deltaX * deltaX + deltaY * deltaY);
        deltaX /= length;
        deltaY /= length;
    }
};

class AlbinoRabbit : public Rabbit {
public:
    AlbinoRabbit(double speed)
        : Rabbit(speed, 0), movingRight(true) {}

    void move(double deltaTime) override {
        if (movingRight) {
            x += speed * deltaTime;
            if (x >= boundary) {
                movingRight = false;
            }
        }
        else {
            x -= speed * deltaTime;
            if (x <= -boundary) {
                movingRight = true;
            }
        }
    }

    void setBoundary(double boundary) {
        this->boundary = boundary;
    }

private:
    bool movingRight;
    double boundary;
};

void simulateRabbit(Rabbit* rabbit, int steps) {
    for (int i = 0; i < steps; ++i) {
        rabbit->move(1.0);
        rabbit->printPosition();
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Додаємо затримку, щоб бачити зміну позиції
    }
}

int main() {
    Rabbit rabbit(1.0, 2.0); // Швидкість 1.0, зміна напрямку кожні 2 секунди
    AlbinoRabbit albino(1.0); // Швидкість 1.0
    albino.setBoundary(10.0); // Межі області проживання альбіносів від -10 до 10

    std::thread rabbitThread(simulateRabbit, &rabbit, 10);
    std::thread albinoThread(simulateRabbit, &albino, 10);

    rabbitThread.join();
    albinoThread.join();

    return 0;
}
