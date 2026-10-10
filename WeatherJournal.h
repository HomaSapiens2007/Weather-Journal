#ifndef WEATHER_JOURNAL_H
#define WEATHER_JOURNAL_H

#include <iostream>

// Вспомогательная структура для хранения одной записи
struct WeatherRecord {
    int year;
    int month;
    int day;
    double temperature;

    WeatherRecord() : year(0), month(0), day(0), temperature(0.0) {}
    WeatherRecord(int y, int m, int d, double t) : year(y), month(m), day(d), temperature(t) {}
};

// Класс для управления динамическим массивом (так как std::vector запрещен)
class WeatherStorage {
private:
    WeatherRecord* data;
    size_t capacity;
    size_t size;

    void resize();

public:
    WeatherStorage();
    WeatherStorage(const WeatherStorage& other);
    ~WeatherStorage();
    WeatherStorage& operator=(const WeatherStorage& other);

    void add(const WeatherRecord& record);
    void removeAt(size_t index);
    size_t getSize() const { return size; }
    WeatherRecord& getAt(size_t index) { return data[index]; }
    const WeatherRecord& getAt(size_t index) const { return data[index]; }
    void clear();
};

class WeatherJournal {
private:
    WeatherStorage records;

    // -1 если d1 < d2, 0 если равны, 1 если d1 > d2
    int compareDates(int y1, int m1, int d1, int y2, int m2, int d2) const;

public:
    WeatherJournal();
    WeatherJournal(const WeatherJournal& other);
    ~WeatherJournal();
    WeatherJournal& operator=(const WeatherJournal& other);

    bool operator==(const WeatherJournal& other) const;
    bool operator!=(const WeatherJournal& other) const;

    friend std::ostream& operator<<(std::ostream& os, const WeatherJournal& journal);

    WeatherJournal operator+(const WeatherJournal& other) const;
    WeatherJournal& operator+=(const WeatherJournal& other);

    // Возвращает true, если добавление успешно (t >= -273.15)
    bool operator()(int year, int month, int day, double temperature);

    void removeObservation(int year, int month, int day);

    void printRange(int startYear, int startMonth, int startDay, 
                    int endYear, int endMonth, int endDay) const;
};
#endif