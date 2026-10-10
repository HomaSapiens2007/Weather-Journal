#include "WeatherJournal.h"

WeatherStorage::WeatherStorage() : data(nullptr), capacity(0), size(0) {}

WeatherStorage::WeatherStorage(const WeatherStorage& other) : data(nullptr), capacity(0), size(0) {
      if (other.size > 0) {
            capacity = other.size;
            data = new WeatherRecord[capacity];
            size = other.size;
            for (size_t i = 0; i < size; ++i) {
                  data[i] = other.data[i];
            }
      }
}

WeatherStorage::~WeatherStorage() {
      delete[] data;
}

WeatherStorage& WeatherStorage::operator=(const WeatherStorage& other) {
      if (this != &other) {
            delete[] data;
            data = nullptr;
            capacity = 0;
            size = 0;
            if (other.size > 0) {
                  capacity = other.size;
                  data = new WeatherRecord[capacity];
                  size = other.size;
                  for (size_t i = 0; i < size; ++i) {
                        data[i] = other.data[i];
                  }
            }
      }
      return *this;
}

void WeatherStorage::resize() {
      size_t newCapacity = (capacity == 0) ? 1 : capacity * 2;
      WeatherRecord* newData = new WeatherRecord[newCapacity];
      for (size_t i = 0; i < size; ++i) {
            newData[i] = data[i];
      }
      delete[] data;
      data = newData;
      capacity = newCapacity;
}

void WeatherStorage::add(const WeatherRecord& record) {
      if (size >= capacity) {
            resize();
      }
      data[size++] = record;
}

void WeatherStorage::removeAt(size_t index) {
      if (index >= size) return;
      for (size_t i = index; i < size - 1; ++i) {
            data[i] = data[i + 1];
      }
      size--;
}

void WeatherStorage::clear() {
      delete[] data;
      data = nullptr;
      size = 0;
      capacity = 0;
}

static bool compareDatesStatic(int y1, int m1, int d1, int y2, int m2, int d2){
    if (y1!=y2) return y1<y2;
    if (m1!=m2) return m1<m2;
    return (d1<d2);
}

static void bubleSortByDate(WeatherStorage& storage){
    size_t n = storage.getSize();
    if (n<2) return;
    for (size_t i = 0; i<n-1;++i){
        bool flag = false;
        for (size_t j = 0; j<n-i-1;++j){
            WeatherRecord& r1 = storage.getAt(j);
            WeatherRecord& r2 = storage.getAt(j+1);
            if (compareDatesStatic(r1.year, r1.month, r1.day, r2.year, r2.month, r2.day)){
                WeatherRecord temp = r1;
                r1 = r2;
                r2 = temp;
                flag = true;
            }
        }
        if (!flag) break;
    }
}


WeatherJournal::WeatherJournal() {}

WeatherJournal::WeatherJournal(const WeatherJournal& other) : records(other.records) {}

WeatherJournal::~WeatherJournal() {}

WeatherJournal& WeatherJournal::operator=(const WeatherJournal& other) {
      if (this != &other) {
            records = other.records;
      }
      return *this;
}

int WeatherJournal::compareDates(int y1, int m1, int d1, int y2, int m2, int d2) const {
      if (y1 != y2) return (y1 < y2) ? -1 : 1;
      if (m1 != m2) return (m1 < m2) ? -1 : 1;
      if (d1 != d2) return (d1 < d2) ? -1 : 1;
      return 0;
}

bool WeatherJournal::operator==(const WeatherJournal& other) const {
      if (records.getSize() != other.records.getSize()) return false;
      for (size_t i = 0; i < records.getSize(); ++i) {
            const WeatherRecord& r1 = records.getAt(i);
            const WeatherRecord& r2 = other.records.getAt(i);
            if (r1.year < r2.year || r1.month < r2.month ||
                  r1.day < r2.day || r1.temperature < r2.temperature ||
                    r1.year > r2.year || r1.month > r2.month ||
                                      r1.day > r2.day || r1.temperature > r2.temperature) {
                  return false;
            }
      }
      return true;
}

bool WeatherJournal::operator!=(const WeatherJournal& other) const {
      return !(*this == other);
}

std::ostream& operator<<(std::ostream& os, const WeatherJournal& journal) {
      for (size_t i = 0; i < journal.records.getSize(); ++i) {
            const WeatherRecord& r = journal.records.getAt(i);
            os << r.year << "-" << r.month << "-" << r.day << ": " << r.temperature << " C\n";
      }
      return os;
}

WeatherJournal WeatherJournal::operator+(const WeatherJournal& other) const {
      WeatherJournal result(*this);
      result += other;
      return result;
}

WeatherJournal& WeatherJournal::operator+=(const WeatherJournal& other) {
      for (size_t i = 0; i < other.records.getSize(); ++i) {
            records.add(other.records.getAt(i));
      }
      return *this;
}

bool WeatherJournal::operator()(int year, int month, int day, double temperature) {
      if (temperature < -273.15) {
            return false;  
      }
      records.add(WeatherRecord(year, month, day, temperature));
      return true;
}

void WeatherJournal::removeObservation(int year, int month, int day) {
      for (size_t i = 0; i < records.getSize(); ++i) {
            const WeatherRecord& r = records.getAt(i);
            if (r.year == year && r.month == month && r.day == day) {
                  records.removeAt(i);
                  return;  
            }
      }
}

void WeatherJournal::printRange(int startYear, int startMonth, int startDay,  
                                                int endYear, int endMonth, int endDay) const {
      WeatherStorage tempStorage;
       
      for (size_t i = 0; i < records.getSize(); ++i) {
            const WeatherRecord& r = records.getAt(i);
             
            bool afterStart = compareDates(r.year, r.month, r.day, startYear, startMonth, startDay) >= 0;
            bool beforeEnd = compareDates(r.year, r.month, r.day, endYear, endMonth, endDay) <= 0;
             
            if (afterStart && beforeEnd) {
                  tempStorage.add(r);
            }
      }

      bubleSortByDate(tempStorage);

      for (size_t i = 0; i < tempStorage.getSize(); ++i) {
            const WeatherRecord& r = tempStorage.getAt(i);
            std::cout << r.year << "-" << r.month << "-" << r.day << ": " << r.temperature << " C\n";
      }
}
