#include "WeatherJournal.h"
#include <iostream>

int main() {
    WeatherJournal journal;

    // Добавление наблюдений
    journal(2023, 10, 1, 15.5);
    journal(2023, 10, 5, 10.0);
    journal(2023, 10, 3, 12.0);
    journal(2023, 9, 30, 20.0); 
    journal(2023, 10, 10, 5.0);

    // Попытка добавить температуру ниже абсолютного нуля
    if (!journal(2023, 10, 12, -300.0)) {
        std::cout << "Failed to add record: Temperature below absolute zero.\n";
    }

    std::cout << "All records:\n" << journal << std::endl;

    std::cout << "Records from 2023-10-02 to 2023-10-06 (sorted):\n";
    journal.printRange(2023, 10, 2, 2023, 10, 6);

    std::cout << "\nRemoving record 2023-10-05...\n";
    journal.removeObservation(2023, 10, 5);
    
    std::cout << "All records after removal:\n" << journal << std::endl;

    // Тест объединения
    WeatherJournal journal2;
    journal2(2023, 11, 1, 0.0);
    
    WeatherJournal combined = journal + journal2;
    std::cout << "Combined journal:\n" << combined << std::endl;

    return 0;
}